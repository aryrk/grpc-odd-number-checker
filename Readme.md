# OddNumberChecker gRPC C++ Example

This project demonstrates a simple gRPC service in C++ that checks if a number is odd. It uses Protocol Buffers for message serialization.

---

## Prerequisites

- **C++ Compiler** (e.g., g++, clang++)
- **CMake** (>= 3.15)
- **Protobuf** (>= 3.12)
- **gRPC** (>= 1.30)

---

## Installing Protobuf and gRPC on Ubuntu

```sh
# Install dependencies
sudo apt update
sudo apt install -y build-essential autoconf libtool pkg-config cmake git

# Install Protobuf
sudo apt install -y protobuf-compiler libprotobuf-dev

# Install gRPC and its dependencies
sudo apt install -y libgrpc++-dev grpc-proto
```

> **Note:**  
> If you need newer versions, build from source:  
> See [gRPC C++ Quick Start](https://grpc.io/docs/languages/cpp/quickstart/) and [Protobuf Releases](https://github.com/protocolbuffers/protobuf/releases).

---

## Building the Project

1. **Create a build directory and run CMake:**

    ```sh
    mkdir build
    cd build
    cmake ..
    make
    ```

    This will generate the `build/client` and `build/server` executables in the `build` directory.

---

## Running the Server and Client

1. **Start the server:**

    ```sh
    ./server
    ```

    The server will listen on `0.0.0.0:50051`.

2. **In another terminal, run the client:**

    ```sh
    ./client
    ```

    Enter a number when prompted to check if it is odd.

---

## Project Structure

```
.
├── CMakeLists.txt
├── proto/
│   └── numberchecker.proto
├── src/
│   ├── client.cpp
│   └── server.cpp
├── build/
│   ├── client
│   └── server
└── ...
```

- **proto/**: Protocol Buffers definitions
- **src/**: C++ source code for client and server
- **build/**: Build output (after running CMake and make)

---

## Cleaning Up

To remove build files:

```sh
rm -rf build/
```

---

## Notes

- The build system will automatically generate C++ source/header files from your `.proto` file.
- If you change `proto/numberchecker.proto`, just rebuild (`make`) to regenerate the code.

---

## References

- [gRPC C++ Documentation](https://grpc.io/docs/languages/cpp/)
- [Protocol Buffers Documentation](https://developers.google.com/protocol-buffers)

---

**Enjoy experimenting with gRPC and Protocol Buffers in C++!**
