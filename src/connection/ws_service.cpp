#include "ws_service.h"
#include "common/utils.h"
#include "ws_connection.h"

#include <boost/json/src.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <iostream>

using namespace WsBinance;

const std::string WsService::IdKey = "id";
const std::string WsService::ParamsKey = "params";
const std::string WsService::SignatureKey = "signature";

WsService::WsService(boost::asio::io_context &ctx, WsOptions options,
                     std::unique_ptr<CryptoService> ptr)
    : ioCtx_(ctx), ptrCon_(std::make_unique<WsConnection>(ctx, options)),
      ptrCryptoService_(std::move(ptr)) {

  ptrCon_->connect();
  ptrCon_->onDataReceive(
      [this](const std::string &inData) { onResponse(inData); });

  ptrCon_->onDisconnect([this] {
    if (connectionResetFn_)
      connectionResetFn_();
  });
}

void WsService::sendRequestAsync(Request req,
                                 std::function<void(Response)> fn) {
  requests.push_back(OngoingRequest{req, fn});
  if (sendingInProgress_)
    return;

  doSend();
}

void WsService::doSend() {
  if (requests.empty()) {
    sendingInProgress_ = false;
    return;
  }

  sendingInProgress_ = true;
  auto &nextReq = requests.front().req;

  if (!nextReq.contains(IdKey)) {
    makeRequestId(nextReq);
  }
  singedRequest(nextReq);

  ptrCon_->sendData(msgConverter_.serialize(nextReq));
}

void WsService::makeRequestId(Request &request) { request[IdKey] = genUUID(); }

void WsService::onResponse(const std::string &inStr) {
  incomingBuf_.insert(incomingBuf_.end(), inStr.begin(), inStr.end());

  auto parseResult = msgConverter_.deserialize(incomingBuf_);
  if (!parseResult.bytesConsumed) {
    // Received partial JsonResponse
    return;
  }

  incomingBuf_.erase(0, parseResult.bytesConsumed);
  processResponse(std::move(parseResult.jsonVals));

  // sending next req
  doSend();
}

void WsService::processResponse(std::vector<boost::json::value> jsonVals) {
  std::vector<boost::json::string> removeList;

  for (auto &v : jsonVals) {
    if (!v.is_object())
      continue;

    auto vObj = v.get_object();
    auto id = vObj[IdKey];

    if (id.is_null())
      continue;

    removeList.push_back(id.as_string());
    auto it = std::find_if(requests.begin(), requests.end(),
                           [&](auto &req) { return req.req[IdKey] == id; });

    if (it != requests.end()) {
      it->resHandler(vObj);
    }
  }

  for (auto &id : removeList) {
    requests.erase(
        std::remove_if(requests.begin(), requests.end(),
                       [&](auto &item) { return item.req[IdKey] == id; }));
  }
}

std::string WsService::genUUID() {
  boost::uuids::random_generator uuidGen;
  return boost::uuids::to_string(uuidGen());
}

void WsService::singedRequest(Request &req) {
  if (!ptrCryptoService_) {
    // error logging
    return;
  }

  auto paramsStr = prepareParamsString(req);
  if (paramsStr.empty()) {
    // error logging
    return;
  }

  // calculate signature
  auto signature = ptrCryptoService_->sign(paramsStr);

  // then update the params
  auto &paramsObj = req[ParamsKey].get_object();
  paramsObj[SignatureKey] = signature;
}

std::string WsService::prepareParamsString(Request &req) {
  auto &paramsVal = req[ParamsKey];
  if (paramsVal.is_null())
    return {};

  auto paramsObj = paramsVal.get_object();

  // init list of key-val param
  std::vector<std::pair<std::string, boost::json::value>> paramsPairs;
  for (const auto &[key, value] : paramsObj) {
    paramsPairs.emplace_back(key, value);
  }

  // sort by name in alphabetical order
  std::sort(paramsPairs.begin(), paramsPairs.end(),
            [](const auto &a, const auto &b) { return a.first < b.first; });

  // concat these pairs using '&' separator
  std::string result;
  for (const auto &p : paramsPairs) {
    if (!result.empty()) {
      result += "&";
    }
    result +=
        p.first + "=" + Utils::removeQuote(boost::json::serialize(p.second));
  }

  return result;
}

void WsService::onConnectionReset(std::function<void()> fn) {
  connectionResetFn_ = std::move(fn);
}

void WsService::sendRequestAsync(std::string reqStr,
                                 std::function<void(Response)> fn) {
  auto req = msgConverter_.deserialize(reqStr).jsonVals.front();
  sendRequestAsync(req.get_object(), fn);
}
