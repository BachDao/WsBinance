#ifndef __HMAC_CRYPTO_SERVICE_H__
#define __HMAC_CRYPTO_SERVICE_H__

#include "crypto_service.h"
namespace WsBinance {

class HmacCryptoService : public CryptoService {
public:
  HmacCryptoService(const std::string& secretKey);
  std::string sign(const std::string &string) override;

private:
  /// binary to hex representation
  std::string toHexString(const unsigned char* data, size_t length);

private:
  const std::string secretKey_;
}; // class hmac_crypto_service
} // namespace WsBinance

#endif //__HMAC_CRYPTO_SERVICE_H__
