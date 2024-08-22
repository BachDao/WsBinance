
#ifndef __WS_OPTIONS_H__
#define __WS_OPTIONS_H__

#include <string>

namespace WsBinance {
struct WsOptions {
  std::string host;
  std::string port;
  std::string path;
};
} // namespace WsBinance

#endif //__WS_OPTIONS_H__
