#ifndef __WS_SERVICE_H__
#define __WS_SERVICE_H__

#include "crypto_service.h"
#include "message.h"
#include "message_converter.h"
#include "ws_options.h"

#include <boost/asio.hpp>

#include <functional>
#include <vector>

namespace WsBinance {
class WsConnection;
/*!
 * Utilize WsConnection and MessageConverter to handle the following tasks:
 * - Request/Response serialize/deserialize, also with payload signing capabilities
 * - Sending request and register for Response handler
 * - Register for Connection reset handler
 *
 * Note: This class is somewhat complex and may benefit from future refactoring.
 */
class WsService {
  struct OngoingRequest {
    Request req;
    std::function<void(Response)> resHandler;
  };

public:
  WsService(boost::asio::io_context &ctx, WsOptions options,
            std::unique_ptr<CryptoService>);

  ~WsService() = default;
  /*!
   * Currently, we only handle Requests that have a single corresponding
   * Response. If streaming responses are needed, a separate API will be
   * required to register a handler for Stream-like responses.
   */
  void sendRequestAsync(Request, std::function<void(Response)>);

  void sendRequestAsync(std::string reqStr, std::function<void(Response)>);

  /*!
   * Notify to upper layer when connection is interrupted and reconnect happen,
   * the upper layer may need to synchronize state of information with server
   * (e.g: Order status)
   */
  void onConnectionReset(std::function<void()>);

private:
  void doSend();
  void onResponse(const std::string &);
  std::string genUUID();
  void makeRequestId(Request &request);

  /*!
   * This maybe more than we need, as currently we only sending Req one-by-one
   * @param jsonVals List of JsonValue receive from underlying connection
   */
  void processResponse(std::vector<boost::json::value>);

  /*!
   * Signing a request params with specific algorithm
   * For Binance, currently it support HMAC, RSA and Ed25519
   */
  void singedRequest(Request &);

  /*!
   * Combine Request's params into valid form:
   * firstKey=firstVal&secondKey=secondVal&....
   *
   * @return A string represent combined params
   */
  std::string prepareParamsString(Request &);

private:
  boost::asio::io_context &ioCtx_;

  std::unique_ptr<WsConnection> ptrCon_;
  std::vector<OngoingRequest> requests{};
  std::atomic<bool> sendingInProgress_ = false;
  MessageConverter msgConverter_{};
  std::string incomingBuf_{};
  std::unique_ptr<CryptoService> ptrCryptoService_;
  std::function<void()> connectionResetFn_ = nullptr;

  static const std::string IdKey;
  static const std::string ParamsKey;
  static const std::string SignatureKey;

}; // class WsService
} // namespace WsBinance

#endif //__WS_SERVICE_H__
