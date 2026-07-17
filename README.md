<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-20-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++20"/>
  <img src="https://img.shields.io/badge/Build-CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white" alt="CMake"/>
  <img src="https://img.shields.io/badge/Tests-9%20passing-brightgreen?style=for-the-badge" alt="Tests"/>
  <img src="https://img.shields.io/badge/License-MIT-blue?style=for-the-badge" alt="MIT"/>
  <img src="https://img.shields.io/badge/Coroutines-C%2B%2B20-purple?style=for-the-badge" alt="C++20"/>
</p>

<h1 align="center">🌐 SkyNet</h1>

<p align="center">
  <b>C++20 Coroutine Network Framework &amp; HTTP Reverse Proxy</b><br/>
  <i>Zero-cost abstractions &bull; Event-driven &bull; Production-grade</i>
</p>

---

## Overview

SkyNet is a **modern C++20 network framework** built on coroutines. It provides a high-level async I/O model where you write sequential-looking code that executes concurrently -- like Go's goroutines but with C++'s zero-cost abstractions.

```
                        SkyNet Architecture
+---------------------------------------------------+
|                  Application Layer                |
|              (HTTP Server / Reverse Proxy)        |
+---------------------------------------------------+
|              HTTP/1.1 Parser & Router             |
+---------------------------------------------------+
|         Coroutine Executor (Task Scheduler)       |
+---------------------------------------------------+
|         TCP Layer (Acceptor / TcpStream)          |
+---------------------------------------------------+
|           Epoll Event Loop (Linux)                |
+---------------------------------------------------+
```

---

## Core Concepts

| Component | What It Does |
|-----------|-------------|
| **Task** | A coroutine that returns a value. The fundamental async unit. |
| **Executor** | Schedules and runs tasks on an event loop. |
| **TimerWheel** | Efficient timer management for timeouts and heartbeats. |
| **Socket** | Coroutine-aware TCP socket with read/write await. |
| **Acceptor** | Accepts incoming connections, produces TcpStream objects. |
| **TcpStream** | Bidirectional coroutine TCP stream. |
| **HTTP Parser** | Parses HTTP/1.1 requests and responses. |
| **LoadBalancer** | Round-robin / least-connections upstream selection. |
| **HealthChecker** | Periodic upstream health checks. |
| **ConnectionPool** | Reuses backend connections for efficiency. |

---

## Quick Start

```bash
# Clone
git clone https://github.com/Thezx-a/SkyNet.git
cd SkyNet

# Build & Test
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
  -DENABLE_TESTS=ON -DCMAKE_CXX_COMPILER=g++-12
cmake --build build -j$(nproc)
ctest --test-dir build --output-on-failure
```

### Basic Coroutine Example

```cpp
#include <skynet/executor.h>
#include <skynet/task.h>
#include <skynet/tcp_socket.h>

skynet::Task<int> handle_client(skynet::TcpStream stream) {
    auto data = co_await stream.read(1024);
    co_await stream.write("Hello!\n");
    co_return 0;
}

int main() {
    skynet::Executor executor;
    skynet::Acceptor acceptor(executor, 8080);
    while (auto conn = co_await acceptor.accept()) {
        executor.spawn(handle_client(std::move(*conn)));
    }
}
```

### Coroutine vs Thread

| | Thread-per-connection | SkyNet Coroutines |
|---|---|---|
| 10K connections | 10K threads (crash) | 1 thread (smooth) |
| Memory per connection | ~1MB stack | ~200 bytes frame |
| Context switch | OS kernel (expensive) | User-space (cheap) |
| Blocking I/O | Thread stalls | Coroutine suspends |

---

## Use as a Library

```cmake
add_subdirectory(path/to/SkyNet)
target_link_libraries(your_app skynet)
```

---

## Project Structure

```
SkyNet/
├── include/skynet/          Public API headers
│   ├── task.h               Coroutine Task type
│   ├── executor.h           Event loop & task scheduler
│   ├── tcp_socket.h         Coroutine TCP socket
│   ├── acceptor.h           Connection acceptor
│   ├── tcp_stream.h         Bidirectional TCP stream
│   ├── timer_wheel.h        Timer management
│   ├── upstream.h           Load balancer
│   └── http_parser.h        HTTP/1.1 parser
├── src/
│   ├── core/                Scheduler internals
│   ├── net/                 TCP layer
│   ├── http/                HTTP parser
│   └── proxy/               Reverse proxy logic
├── gateway/                 Gateway application
├── examples/                Example programs
├── tests/                   9 unit tests
└── docker/                  Docker deployment
```

---

## Tests

9 unit tests covering:

| Module | Tests | What's Verified |
|--------|-------|-----------------|
| Task | 2 | Basic await, exception propagation |
| Executor | 2 | Spawn, concurrent execution |
| TimerWheel | 2 | Add/remove, expiry |
| Upstream | 3 | Selection, health checks, removal |

---

## Tech Stack

`C++20` `Coroutines` `CMake` `Ninja` `Epoll` `Google Test` `Docker`

---

## License

[MIT](LICENSE)
