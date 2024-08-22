#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <boost/json.hpp>

namespace WsBinance {

using Request = boost::json::object;
using Response = boost::json::object;
} // namespace WsBinance

#endif //__MESSAGE_H__
