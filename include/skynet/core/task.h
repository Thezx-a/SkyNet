#pragma once
#include <coroutine>
#include <exception>
#include <optional>
#include <variant>

namespace skynet {
namespace core {

template <typename T = void>
class Task {
public:
    struct promise_type {
        std::optional<T> result_;
        std::exception_ptr exception_;
        std::coroutine_handle<> awaiter_;

        Task get_return_object() {
            return Task{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_value(T v) { result_ = std::move(v); }
        void unhandled_exception() { exception_ = std::current_exception(); }
    };

    using handle_type = std::coroutine_handle<promise_type>;

    Task() = default;
    explicit Task(handle_type h) : handle_(h) {}
    ~Task() { if (handle_) handle_.destroy(); }
    Task(Task&& other) noexcept : handle_(other.handle_) { other.handle_ = nullptr; }
    Task& operator=(Task&& other) noexcept {
        if (this != &other) { if (handle_) handle_.destroy(); handle_ = other.handle_; other.handle_ = nullptr; }
        return *this;
    }
    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;

    bool await_ready() const { return false; }
    void await_suspend(std::coroutine_handle<> awaiter) {
        handle_.promise().awaiter_ = awaiter;
        handle_.resume();
    }
    T await_resume() {
        if (handle_.promise().exception_) std::rethrow_exception(handle_.promise().exception_);
        return std::move(*handle_.promise().result_);
    }

    handle_type handle() const { return handle_; }

private:
    handle_type handle_;
};

// Task<void> specialization
template <>
class Task<void> {
public:
    struct promise_type {
        std::exception_ptr exception_;
        std::coroutine_handle<> awaiter_;

        Task get_return_object() {
            return Task{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { exception_ = std::current_exception(); }
    };

    using handle_type = std::coroutine_handle<promise_type>;

    Task() = default;
    explicit Task(handle_type h) : handle_(h) {}
    ~Task() { if (handle_) handle_.destroy(); }
    Task(Task&& other) noexcept : handle_(other.handle_) { other.handle_ = nullptr; }
    Task& operator=(Task&& other) noexcept {
        if (this != &other) { if (handle_) handle_.destroy(); handle_ = other.handle_; other.handle_ = nullptr; }
        return *this;
    }

    bool await_ready() const { return false; }
    void await_suspend(std::coroutine_handle<> awaiter) {
        handle_.promise().awaiter_ = awaiter;
        handle_.resume();
    }
    void await_resume() {
        if (handle_.promise().exception_) std::rethrow_exception(handle_.promise().exception_);
    }

    handle_type handle() const { return handle_; }

private:
    handle_type handle_;
};

}  // namespace core
}  // namespace skynet
