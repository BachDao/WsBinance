#include "ws_connection.h"

#include <iostream>

using namespace WsBinance;

WsConnection::WsConnection(io_context &ctx, WsOptions options, bool reconnect)
    : ioCtx_(ctx), sslCtx_(ssl::context::tlsv12_client), ws_(ioCtx_, sslCtx_),
      reconnect_(reconnect), options_(options) {}

/// Longggggggggg function, just throw everything related to connection
/// setup into this "bag", refactor maybe needed
bool WsConnection::doConnect() {
  // skip peer verification
  sslCtx_.set_verify_mode(boost::asio::ssl::verify_none);

  if (!SSL_set_tlsext_host_name(ws_.next_layer().native_handle(),
                                options_.host.c_str())) {
    beast::error_code ec{static_cast<int>(::ERR_get_error()),
                         asio::error::get_ssl_category()};
    throw beast::system_error{ec};
  }

  // ip resolve
  boost::system::error_code errorCode;
  tcp::resolver resolver(ioCtx_);

  // Look up the domain name
  auto const results =
      resolver.resolve(options_.host, options_.port, errorCode);
  if (errorCode) {
    std::cout << "address resolve failed on: " << options_.host << ":"
              << options_.port << errorCode.what() << std::endl;
    return false;
  }

  // Make the connection on the IP address we get from a lookup
  asio::connect(ws_.next_layer().next_layer(), results);
  if (errorCode) {
    std::cout << "connect to: " << options_.host << ":" << options_.port
              << " is failed" << std::endl;
    return false;
  }

  // Perform the SSL handshake
  ws_.next_layer().handshake(ssl::stream_base::client);
  if (errorCode) {
    std::cout << "ssl handshake is failed" << std::endl;
    return false;
  }

  // Then Websocket handshake
  ws_.handshake(options_.host, options_.path, errorCode);
  if (errorCode) {

    std::cout << "websocket handshake is failed: " << errorCode.what()
              << std::endl;
    return false;
  }
  return true;
}

bool WsConnection::connect() {
  if (!doConnect()) {
    return false;
  }
  startReceiveData();
  return true;
}

void WsConnection::startReceiveData() { doRead(); }

void WsConnection::doRead() {
  static beast::flat_buffer readBuf_;
  readBuf_.clear();
  ws_.async_read(readBuf_, [&](const boost::system::error_code &erc,
                               size_t bytesTransferred) {
    if (erc || bytesTransferred == 0) {
      teardown();
      return;
    }
    onRead(beast::buffers_to_string(readBuf_.cdata()));
    doRead();
  });
}

void WsConnection::teardown() {
  if (disconnectHandler_)
    disconnectHandler_();

  if (!reconnect_)
    return;

  auto success = doConnect();

  if (!success) {
    // maybe we need to try several times with some random timeout,
    // for now I will choose an easy path
    std::cout << "Reconnect is failed" << std::endl;
    abort();
  }
}

void WsConnection::onRead(const std::string &incomingData) {
  if (dataHandler_)
    dataHandler_(incomingData);
}

void WsConnection::onDataReceive(std::function<void(const std::string &)> fn) {
  dataHandler_ = std::move(fn);
}

void WsConnection::onDisconnect(std::function<void()> fn) {
  disconnectHandler_ = std::move(fn);
}

void WsConnection::sendData(const std::string &outBuf) {
  // blocking call
  boost::system::error_code erc;
  ws_.write(asio::buffer(outBuf), erc);

  if (erc) {
    recover(outBuf);
  }
}

void WsConnection::recover(const std::string &outgoingBuf) {
  // do we need to impose a limit on the number of retries ?
  // for now, I will say no
  if (doConnect())
    sendData(outgoingBuf);
}
