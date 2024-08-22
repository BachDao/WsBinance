#ifndef __MESSAGE_CONVERTER_H__
#define __MESSAGE_CONVERTER_H__

#include <boost/json.hpp>

#include <vector>

namespace WsBinance {
struct MessageConvertResult {
  MessageConvertResult();
  std::vector<boost::json::value> jsonVals;
  size_t bytesConsumed;
};

/*!
 * Json serialize/deserialize from/to string (bytes sequence),
 */
class MessageConverter {
public:

  /// From JsonValue to string
  std::string serialize(const boost::json::value &);

  /*! From string to (multiple) JsonValue, and return how many bytes are consumed
  * Support incremental parsing
  */
  MessageConvertResult deserialize(const std::string &);

}; // class MessageConverter
} // namespace WsBinance

#endif //__MESSAGE_CONVERTER_H__
