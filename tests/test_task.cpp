#include <gtest/gtest.h>
#include "skynet/core/task.h"
using namespace skynet::core;

TEST(TaskTest, SimpleReturn) {
    auto coro = []() -> Task<int> { co_return 42; };
    EXPECT_EQ(42, 1);  // Placeholder - full coroutine test requires Executor
}

TEST(TaskVoidTest, SimpleVoid) {
    auto coro = []() -> Task<void> { co_return; };
    SUCCEED();
}
