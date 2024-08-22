#!/bin/bash

# Build the container
docker build -t ws_binance .

# Run the container
docker run \
  --interactive \
  --rm \
  --volume ./:/build \
  --entrypoint /build/build.sh \
  ws_binance