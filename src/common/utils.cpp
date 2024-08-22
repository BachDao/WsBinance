#include "utils.h"

#include <boost/algorithm/string.hpp>

#include <chrono>

using namespace WsBinance;

std::string Utils::getTimeStamp() {
  auto now = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
  auto duration = now.time_since_epoch().count();
  return std::to_string(duration);
}

std::string Utils::removeQuote(const std::string & input) {
  std::string result = input;
  boost::algorithm::replace_all(result, "\"", "");
  return result;
}
