#include "message_converter.h"

using namespace WsBinance;

std::string MessageConverter::serialize(const boost::json::value &jsonVal) {
  if (jsonVal.is_null())
    return {};
  return boost::json::serialize(jsonVal);
}

MessageConvertResult MessageConverter::deserialize(const std::string &inStr) {
  boost::json::stream_parser parser;
  MessageConvertResult parseResult;
  auto copyInStr = inStr;

  while (true) {
    auto consumed = parser.write_some(copyInStr);

    if (!parser.done())
      break;

    parseResult.jsonVals.push_back(parser.release());
    parseResult.bytesConsumed += consumed;

    copyInStr.erase(0, consumed);
    parser.reset();
  }
  return parseResult;
}

MessageConvertResult::MessageConvertResult() : jsonVals(), bytesConsumed(0) {}
