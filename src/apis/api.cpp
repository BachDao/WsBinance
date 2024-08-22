#include "api.h"
#include "connection/hmac_crypto_service.h"
#include "connection/ws_connection.h"
#include "connection/ws_service.h"

#include <iostream>

using namespace WsBinance;

Api::Api(boost::asio::io_context &ctx, const std::string &apiKey,
         const std::string &secretKey)
    : ptrService_(nullptr), ApiKey(apiKey), SecretKey(secretKey) {
  WsOptions options{.host = Host, .port = Port, .path = Path};
  auto ptrCryptoService = std::make_unique<HmacCryptoService>(SecretKey);
  ptrService_ =
      std::make_unique<WsService>(ctx, options, std::move(ptrCryptoService));
}

Api::~Api() = default;

void Api::makeOrder(const std::string &symbol, const Order::Side side,
                    const Order::Type type, double price, double quantity,
                    std::function<void(Response)> fn) {
  auto orderStr =
      Order::placeNewOrder(ApiKey, symbol, side, type, price, quantity);

  ptrService_->sendRequestAsync(orderStr, fn);
}
