#include "apis/api.h"
#include "main_app.h"
#include <iostream>

using namespace WsBinance;
using namespace std::chrono_literals;

int main() {
  MainApp mainApp(1);
  // no worriers, these keys available on document page
  const std::string ApiKey =
      "VdZVL15ONyvZmEdcT1gT17pSeVPSIEUKYlxHVXbFB4vdPDnuxiajsuDDk6JlTaVF";
  const std::string SecretKey =
      "WdubXy4w8YlhV1jMIeGQjnm10BUoMbuc65CPTfgThB37iyAuiY96nza0Dp0Wl927";

  Api api(mainApp.getContext(), ApiKey, SecretKey);
  api.makeOrder("BTCUSDT", Order::Side::BUY, Order::Type::MARKET, 0.1, 0.1,
                [](Response res) {
                  //
                  std::cout << boost::json::serialize(res) << std::endl;
                });
  // res example from my test:
  //  {
  //    "id": "65c493ac-8253-4823-a493-1569a51f795a",
  //    "status": 200,
  //    "result": {
  //      "symbol": "BTCUSDT",
  //      "orderId": 8933526,
  //      "orderListId": -1,
  //      "clientOrderId": "3a7LEBcBHbMt9W0zwUTbGq",
  //      "transactTime": 1724318427603,
  //      "price": "0.00000000",
  //      "origQty": "0.10000000",
  //      "...",
  //     }
  //  }

  mainApp.run();
  mainApp.wait();
  return 0;
}