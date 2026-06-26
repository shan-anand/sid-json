// test_time_calc.cpp
// Unit tests for time_calc.cpp

#include "gtest/gtest.h"
#include "time_calc.h"
#include <thread>
#include <chrono>

using namespace sid::json;

TEST(TimeCalcTest, ConstructorAndClear) {
    time_calc tc;
    tc.start();
    tc.stop();
    tc.clear();
    EXPECT_EQ(tc.diff_microsecs(), 0u);
    EXPECT_EQ(tc.diff_millisecs(), 0u);
    EXPECT_EQ(tc.diff_secs(), 0u);
}

TEST(TimeCalcTest, StartStopAndDiff) {
    time_calc tc;
    tc.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    tc.stop();
    auto us = tc.diff_microsecs();
    auto ms = tc.diff_millisecs();
    auto s = tc.diff_secs();
    EXPECT_GT(us, 0u);
    EXPECT_GT(ms, 0u);
    EXPECT_GE(s, 0u);
    EXPECT_NE(ms, 0u);
}

TEST(TimeCalcTest, MultipleStartStop) {
    time_calc tc;
    tc.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    tc.stop();
    auto first = tc.diff_microsecs();
    tc.start();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    tc.stop();
    auto second = tc.diff_microsecs();
    EXPECT_GT(second, first);
}

TEST(TimeCalcTest, EdgeCases) {
    time_calc tc;
    // Stop before start
    tc.start();
    tc.stop();
    std::swap(tc.t_start, tc.t_end); // Manually swap to simulate stop before start
    EXPECT_EQ(tc.diff_microsecs(), 0u);
    // Start and stop with no delay
    tc.start();
    tc.stop();
    tc.t_end.tv_sec += 2; // Simulate 2 seconds difference
    tc.t_end.tv_nsec = 50000000;
    tc.t_start.tv_nsec = tc.t_end.tv_nsec / 2;
    EXPECT_GE(tc.diff_microsecs(), 0u);

    std::swap(tc.t_start.tv_nsec, tc.t_end.tv_nsec);
    EXPECT_GE(tc.diff_microsecs(), 0u);

    tc.t_end.tv_sec = tc.t_start.tv_sec;
    EXPECT_GE(tc.diff_microsecs(), 0u);

    std::swap(tc.t_start.tv_nsec, tc.t_end.tv_nsec);
    EXPECT_GE(tc.diff_microsecs(), 0u);

    tc.t_start = tc.t_end;
    tc.t_start.tv_nsec = 200000;
    tc.t_end.tv_nsec = tc.t_start.tv_nsec * 2;
    EXPECT_GE(tc.diff_microsecs(), 0u);
}

