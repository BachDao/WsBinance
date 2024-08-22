#include "order.h"
#include "common/utils.h"
#include <sstream>

using namespace WsBinance;

const std::map<Order::Type, std::string> Order::typeMap = {
    {Type::LIMIT, "LIMIT"},
    {Type::MARKET, "MARKET"},
    {Type::STOP_LOSS, "STOP_LOSS"},
    {Type::STOP_LOSS_LIMIT, "STOP_LOSS_LIMIT"},
    {Type::TAKE_PROFIT, "TAKE_PROFIT"},
    {Type::TAKE_PROFIT_LIMIT, "TAKE_PROFIT_LIMIT"},
    {Type::LIMIT_MAKER, "LIMIT_MAKER"}};

const std::map<Order::Side, std::string> Order::sideMap = {
    {Side::BUY, "BUY"}, {Side::SELL, "SELL"}};

std::string Order::placeNewOrder(const std::string &apiKey,
                                 const std::string &symbol,
                                 const WsBinance::Order::Side side,
                                 const WsBinance::Order::Type type,
                                 double price, double quantity) {

  std::ostringstream oss;
  oss << "{\n"
      << "  \"method\": \"order.place\",\n"
      << "  \"params\": {\n"
      << "    \"symbol\": \"" << symbol << "\",\n"
      << "    \"side\": \"" << sideMap.at(side) << "\",\n"
      << "    \"type\": \"" << typeMap.at(type) << "\",\n";

  // ugly, but quick
  if (type != Type::MARKET) {
    oss << "    \"price\": \"" << price << "\",\n";
  }

  oss << "    \"quantity\": \"" << quantity << "\",\n"
      << "    \"apiKey\": \"" << apiKey << "\",\n"
      << "    \"timestamp\": " << Utils::getTimeStamp() << "\n"
      << "  }\n"
      << "}";

  return oss.str();
}
