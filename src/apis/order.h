#ifndef __ORDER_H__
#define __ORDER_H__

#include <map>
#include <string>

namespace WsBinance {

class Order {
public:
  enum class Side { BUY, SELL };
  enum class Type {
    LIMIT,
    MARKET,
    STOP_LOSS,
    STOP_LOSS_LIMIT,
    TAKE_PROFIT,
    TAKE_PROFIT_LIMIT,
    LIMIT_MAKER
  };

public:
  static std::string placeNewOrder(const std::string& apiKey, const std::string &symbol, const Side, const Type,
                            double price, double quantity);

private:
  static const std::map<Type, std::string> typeMap;

  static const std::map<Side, std::string> sideMap;
};

} // namespace WsBinance

#endif //__ORDER_H__
