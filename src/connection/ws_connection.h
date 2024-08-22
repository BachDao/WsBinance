#ifndef __WS_CONNECTION_H__
#define __WS_CONNECTION_H__

#include "ws_options.h"

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>

#include <string>

namespace beast = boost::beast;
namespace asio = boost::asio;
namespace ssl = asio::ssl;

using tcp = asio::ip::tcp;
using io_context = beast::net::io_context;

namespace WsBinance {
/*!
 * WsConnection handling WebSocket specific details
 * - Establish ssl connection to Server
 * - Provide API for send and receive data
 * - Monitoring connection status
 */
class WsConnection {
public:
  WsConnection(io_context &ctx, WsOptions options, bool reconnect = true);
  ~WsConnection() = default;

  /*!
   * sync-connect to server
   * @return whether connect is success or not
   */
  bool connect();

  /*!
   * Register callback for data received handling
   */
  void onDataReceive(std::function<void(const std::string &)>);

  /*!
   * Sending data to server, it's blocking call
   */
  void sendData(const std::string &outBuf);

  /*!
   * Register callback for connection interrupted
   */
  void onDisconnect(std::function<void()>);

private:
  bool doConnect();
  void startReceiveData();
  void doRead();
  void onRead(const std::string &);

  /*!
   * Error happen on read side, notify to upper layer for correct
   * handling (e.g: re-sync state of Orders)
   */
  void teardown();

  /*!
   * Error happen on write side, we re-establish connection and re-sent
   * last fail message
   */
  void recover(const std::string &);

private:
  io_context &ioCtx_;
  asio::ssl::context sslCtx_;
  beast::websocket::stream<beast::ssl_stream<tcp::socket>> ws_;
  bool reconnect_ = false;
  WsOptions options_;

  std::function<void(const std::string &)> dataHandler_;
  std::function<void()> disconnectHandler_;

}; // class WsConnection
} // namespace WsBinance

#endif //__WS_CONNECTION_H__
