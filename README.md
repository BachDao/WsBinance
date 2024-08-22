## WSBinance

WebSocket client that communicate with Binance SPOT Testnet, 
provide simple API for order management.

### Basic building block

Organize as layer, each one solving one aspect of a problem:

- **WsConnection**: handling WebSocket details, provide api for sending and receive data
- **CryptoService**: payload signing, with specific implementation for each HMAC, RSA...
- **MessageConverter**: Json Serialize/Deserialize
- **WsService**: coordinator, leverage above class to provide high-level api:
  - Place Order
  - For each request, allow register Response handling fucntion

### Implementation notes
- boost-beast for socket handling
- openssl for ssl related function, also with crypto signing.
- boost-json for serialize/deserialize message
- Message is committed one-by-one, response is process before next request is transfer
- Currently, subsribe to websocket stream is not supported

### Build
**Dependency**

- Boost 1.84 or higher
- CMake 3.12 or higher
- lib-openssl
- C++17 compatible compiler
On a *nix system with the required dependencies, execute build.sh to start the build.

Otherwise, run docker_build.sh to create a Docker container with the necessary dependencies and build inside it.

### Usage


