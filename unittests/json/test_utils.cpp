// test_utils.cpp
// Unit tests for utils.cpp

#include "gtest/gtest.h"
#include "utils.h"
#include <string>
#include <vector>

using namespace sid::json;

TEST(UtilsTest, ToStringBool)
{
    EXPECT_EQ(to_string(true), "true");
    EXPECT_EQ(to_string(false), "false");
}

TEST(UtilsTest, ToBool)
{
    EXPECT_TRUE(to_bool("true"));
    EXPECT_FALSE(to_bool("false"));
    EXPECT_THROW(to_bool("notabool"), std::invalid_argument);
}

TEST(UtilsTest, ToBoolWithError)
{
    bool out;
    std::string err;
    EXPECT_TRUE(to_bool("true", out, &err));
    EXPECT_TRUE(out);
    EXPECT_TRUE(err.empty());
    EXPECT_TRUE(to_bool("false", out, &err));
    EXPECT_FALSE(out);
    EXPECT_TRUE(err.empty());
    EXPECT_FALSE(to_bool("bad", out, &err));
    EXPECT_FALSE(err.empty());
}

TEST(UtilsTest, ToNumUint32)
{
    uint32_t out;
    std::string err;
    EXPECT_TRUE(to_num("123", out, &err));
    EXPECT_EQ(out, 123u);
    EXPECT_TRUE(err.empty());
    EXPECT_FALSE(to_num("123a", out, &err));
    EXPECT_FALSE(err.empty());
    EXPECT_FALSE(to_num("-1", out, &err));
    EXPECT_FALSE(err.empty());
    EXPECT_FALSE(to_num("4294967296", out, &err)); // out of range
    EXPECT_FALSE(err.empty());
    EXPECT_FALSE(to_num("abc", out, &err));
    EXPECT_FALSE(err.empty());
}

TEST(UtilsTest, ToNumLongDouble)
{
    long double out;
    std::string err;
    EXPECT_TRUE(to_num("123.456", out, &err));
    EXPECT_NEAR((double)out, 123.456, 1e-9);
    EXPECT_TRUE(err.empty());
    EXPECT_FALSE(to_num("123.45a", out, &err));
    EXPECT_FALSE(err.empty());
    EXPECT_FALSE(to_num("1e10000", out, &err)); // out of range
    EXPECT_FALSE(err.empty());
    EXPECT_FALSE(to_num("abc", out, &err));
    EXPECT_FALSE(err.empty());
}

TEST(UtilsTest, ToNumInt64)
{
    int64_t out;
    std::string err;
    EXPECT_TRUE(to_num("-123456", out, &err));
    EXPECT_EQ(out, -123456);
    EXPECT_TRUE(err.empty());
    EXPECT_FALSE(to_num("-123456a", out, &err));
    EXPECT_FALSE(err.empty());
    // Out of range: uint64_t max value for int64_t
    std::string max_uint64 = std::to_string(std::numeric_limits<uint64_t>::max());
    EXPECT_FALSE(to_num(max_uint64, out, &err));
    EXPECT_FALSE(err.empty());
    EXPECT_FALSE(to_num("abc", out, &err));
    EXPECT_FALSE(err.empty());
}

TEST(UtilsTest, ToNumUint64)
{
    uint64_t out;
    std::string err;
    EXPECT_TRUE(to_num("1234567890123", out, &err));
    EXPECT_EQ(out, 1234567890123ULL);
    EXPECT_TRUE(err.empty());
    EXPECT_FALSE(to_num("1234567890123a", out, &err));
    EXPECT_FALSE(err.empty());
    EXPECT_FALSE(to_num("-1", out, &err));
    EXPECT_FALSE(err.empty());
}

TEST(UtilsTest, GetSep)
{
    EXPECT_EQ(get_sep(0), "0");
    EXPECT_EQ(get_sep(1), "1");
    EXPECT_EQ(get_sep(1234), "1,234");
    EXPECT_EQ(get_sep(1234567), "1,234,567");
}

TEST(UtilsTest, SplitBasic)
{
    std::vector<std::string> out;
    EXPECT_EQ(split(out, "a,b,c", ','), 3u);
    ASSERT_EQ(out.size(), 3u);
    EXPECT_EQ(out[0], "a");
    EXPECT_EQ(out[1], "b");
    EXPECT_EQ(out[2], "c");
}

TEST(UtilsTest, SplitTrimSkipEmpty)
{
    std::vector<std::string> out;
    std::string s = "  a , , b ,c  ";
    EXPECT_EQ(split(out, s, ',', SPLIT_TRIM_SKIP_EMPTY | SPLIT_TRIM), 3u);
    ASSERT_EQ(out.size(), 3u);
    EXPECT_EQ(out[0], "a");
    EXPECT_EQ(out[1], "b");
    EXPECT_EQ(out[2], "c");
}
