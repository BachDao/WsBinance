#include "hmac_crypto_service.h"

#include <openssl/evp.h>
#include <openssl/hmac.h>

#include <sstream>
#include <iomanip>
#include <array>

using namespace WsBinance;

HmacCryptoService::HmacCryptoService(const std::string &secretKey)
    : secretKey_(secretKey) {}

std::string HmacCryptoService::toHexString(const unsigned char* data, size_t length) {
  std::ostringstream oss;
  for (size_t i = 0; i < length; ++i) {
    oss << std::hex << std::setw(2) << std::setfill('0')
        << static_cast<int>(data[i]);
  }
  return oss.str();
}

/*!
 * Compute the signature
 *
 * Ref:
 * https://developers.binance.com/docs/binance-spot-api-docs/testnet/web-socket-api#signed-request-example-hmac
 * 1. Interpret secretKey as ASCII data, using it as a key for HMAC-SHA-256.
 * 2. Sign signature payload as ASCII data.
 * 3. Encode HMAC-SHA-256 output as a hex string.
 *
 * @param payload
 * @return payload's signature
 */
std::string HmacCryptoService::sign(const std::string &payload) {
  static std::array<unsigned char, EVP_MAX_MD_SIZE> hash;
  unsigned int hashLen;
  HMAC(EVP_sha256(), secretKey_.data(), secretKey_.size(),
       reinterpret_cast<const unsigned char *>(payload.data()), payload.size(),
       hash.data(), &hashLen);
  return toHexString(hash.data(), hashLen);
}
