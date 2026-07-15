# SkyNet

> 基于 C++20 协程的异步网络框架 & HTTP 反向代理网关

[![CI](https://github.com/USERNAME/SkyNet/actions/workflows/ci.yml/badge.svg)](https://github.com/USERNAME/SkyNet/actions)

## 性能指标

(待压测填充)

## 快速开始

`ash
cd docker && docker compose up -d
curl http://localhost:8080/
`

## 从源码构建

`ash
cmake -B build -DCMAKE_BUILD_TYPE=Release -DENABLE_TESTS=ON -DCMAKE_CXX_COMPILER=g++-12
cmake --build build -j4
ctest --test-dir build --output-on-failure
`

## 项目结构
`
include/skynet/      框架公共 API
src/core/            协程调度器 (Task, Executor, TimerWheel)
src/net/             协程化 TCP (Socket, Acceptor, TcpStream)
src/http/            HTTP/1.1 解析器
src/proxy/           负载均衡 + 健康检查 + 连接池
gateway/             网关应用入口
examples/            示例程序
`

## 技术栈
C++20 Coroutines, CMake, Epoll, Google Test, Docker

## License
MIT
