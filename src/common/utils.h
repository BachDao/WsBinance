#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>

namespace WsBinance{
class Utils {

public:
  static std::string getTimeStamp();
  /// Remove any " exist in string
  static std::string removeQuote(const std::string&);
}; // class Utils
}

#endif //__UTILS_H__
