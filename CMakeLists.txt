cmake_minimum_required(VERSION 3.12)
project(WsBinance)

set(CMAKE_CXX_STANDARD 17)

# Boost
find_package(Boost REQUIRED)

# OpenSSL
set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)


add_library(WsBinance
        src/apis/order.cpp
        src/connection/ws_connection.cpp
        src/connection/message_converter.cpp
        src/connection/ws_service.cpp
        src/connection/hmac_crypto_service.cpp
        src/apis/api.cpp
        src/common/utils.cpp
        src/main_app.cpp
)


target_include_directories(
        WsBinance
        PUBLIC
        ${Boost_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(
        WsBinance
        PUBLIC
        OpenSSL::SSL
        OpenSSL::Crypto
)

add_executable(example examples/main.cpp)

target_link_libraries(example PUBLIC WsBinance)