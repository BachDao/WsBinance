#ifndef __SIGNATURE_H__
#define __SIGNATURE_H__

#include <string>

namespace WsBinance {
/*!
 * Baseclass for all CryptoService (HMAC, RSA..)
 * Basically, it provide signing capability
 */
class CryptoService {
public:
  virtual ~CryptoService() = default;
  virtual std::string sign(const std::string &) = 0;
};

} // namespace WsBinance

#endif //__SIGNATURE_H__
