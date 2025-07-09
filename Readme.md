# OddNumberChecker gRPC C++ Example

This project demonstrates a simple gRPC service in C++ that checks if a number is odd. It uses Protocol Buffers for message serialization.

---

## Prerequisites

- **C++ Compiler** (e.g., g++, clang++)
- **CMake** (>= 3.15)
- **Conan** (C++ package manager)
- **Protobuf** (>= 3.12)
- **gRPC** (>= 1.30)

---

## Installing Protobuf, gRPC, and Conan on Ubuntu

```sh
# Install dependencies
sudo apt update
sudo apt install -y build-essential autoconf libtool pkg-config cmake git python3-pip

# Install Conan (if not already installed)
pip3 install --user conan

# Install Protobuf and gRPC (system packages, optional if using Conan for all dependencies)
sudo apt-get install libgrpc++-dev protobuf-compiler-grpc
sudo apt install -y protobuf-compiler libprotobuf-dev
sudo apt install -y libgrpc++-dev grpc-proto
```

> **Note:**  
> If you need newer versions, build from source:  
> See [gRPC C++ Quick Start](https://grpc.io/docs/languages/cpp/quickstart/) and [Protobuf Releases](https://github.com/protocolbuffers/protobuf/releases).

---

## Building the Project (with Conan)

1. **Install dependencies with Conan:**

    ```sh
    conan install . --output-folder=build --build=missing
    ```

2. **Configure and build with CMake:**

    ```sh
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release
    cmake --build .
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
- If you change `proto/numberchecker.proto`, just rebuild to regenerate the code.
- Conan will handle C++ library dependencies (see `conanfile.txt`).

---

## References

- [gRPC C++ Documentation](https://grpc.io/docs/languages/cpp/)
- [Protocol Buffers Documentation](https://developers.google.com/protocol-buffers)
- [Conan C++ Package Manager](https://conan.io/)

---

**Enjoy experimenting with gRPC and Protocol Buffers in C++!**
