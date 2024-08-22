#ifndef __API_H__
#define __API_H__

#include "order.h"
#include "connection/message.h"

#include <string>
#include <memory>

namespace boost {
namespace asio {
class io_context;
}
} // namespace boost
namespace WsBinance {
class WsService;
class Api {
public:
  Api(boost::asio::io_context &, const std::string &apiKey,
      const std::string &secretKey);
  ~Api();

  void makeOrder(const std::string &symbol, const Order::Side,
                 const Order::Type, double price, double quantity, std::function<void(Response)> fn);
  std::vector<std::string> getOrder();

private:
  std::unique_ptr<WsService> ptrService_;
  const std::string ApiKey;
  const std::string SecretKey;

  const std::string Host = "testnet.binance.vision";
  const std::string Port = "443";
  const std::string Path = "/ws-api/v3";
}; // class Api

} // namespace WsBinance
#endif //__API_H__
