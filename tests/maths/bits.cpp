#include <gtest/gtest.h>

#include "maths/bits.hpp"

TEST(Bits, popcount) {
	EXPECT_EQ(popcount(0), 0UL);
	EXPECT_EQ(popcount(0LL), 0UL);

	EXPECT_EQ(popcount(1), 1UL);
	EXPECT_EQ(popcount(1 << 20), 1UL);
	EXPECT_EQ(popcount((1LL << 63) - 1), 63UL);
	EXPECT_EQ(popcount(1LL << 63), 1UL);

	EXPECT_EQ(popcount(-1), 32UL);
	EXPECT_EQ(popcount(static_cast<int64_t>(-1)), 64UL);

	EXPECT_EQ(popcount(1000000007), 16UL);
	EXPECT_EQ(popcount(1000000007LL * 1000000007), 32UL);
}

TEST(Bits, countl_zero) {
	EXPECT_EQ(countl_zero(0), 32UL);
	EXPECT_EQ(countl_zero<uint16_t>(0), 16UL);
	EXPECT_EQ(countl_zero<int64_t>(0), 64UL);

	EXPECT_EQ(countl_zero(1), 31UL);
	EXPECT_EQ(countl_zero(1 << 20), 11UL);
	EXPECT_EQ(countl_zero((1LL << 62) - 1), 2UL);
	EXPECT_EQ(countl_zero(1LL << 62), 1UL);
	EXPECT_EQ(countl_zero(1LL << 63), 0UL);

	EXPECT_EQ(countl_zero(-1), 0UL);
	EXPECT_EQ(countl_zero(static_cast<int64_t>(-1)), 0UL);

	EXPECT_EQ(countl_zero(1000000007), 2UL);
	EXPECT_EQ(countl_zero(1000000007LL * 1000000007), 4UL);
}

TEST(Bits, countl_one) {
	EXPECT_EQ(countl_one(~0), 32UL);
	EXPECT_EQ(countl_one<uint16_t>(~0), 16UL);
	EXPECT_EQ(countl_one<int64_t>(~0), 64UL);

	EXPECT_EQ(countl_one(~1), 31UL);
	EXPECT_EQ(countl_one(~(1 << 20)), 11UL);
	EXPECT_EQ(countl_one(~((1LL << 62) - 1)), 2UL);
	EXPECT_EQ(countl_one(~(1LL << 62)), 1UL);
	EXPECT_EQ(countl_one(~(1LL << 63)), 0UL);

	EXPECT_EQ(countl_one(~-1), 0UL);
	EXPECT_EQ(countl_one(~static_cast<int64_t>(-1)), 0UL);

	EXPECT_EQ(countl_one(~1000000007), 2UL);
	EXPECT_EQ(countl_one(~(1000000007LL * 1000000007)), 4UL);
}

TEST(Bits, countr_zero) {
	EXPECT_EQ(countr_zero(0), 32UL);
	EXPECT_EQ(countr_zero<uint16_t>(0), 16UL);
	EXPECT_EQ(countr_zero<int64_t>(0), 64UL);

	EXPECT_EQ(countr_zero(1), 0UL);
	EXPECT_EQ(countr_zero(1 << 20), 20UL);
	EXPECT_EQ(countr_zero((1LL << 62) - 1), 0UL);
	EXPECT_EQ(countr_zero(1LL << 62), 62UL);
	EXPECT_EQ(countr_zero(1LL << 63), 63UL);

	EXPECT_EQ(countr_zero(-1), 0UL);
	EXPECT_EQ(countr_zero(static_cast<int64_t>(-1)), 0UL);

	EXPECT_EQ(countr_zero(1000000008), 3UL);
	EXPECT_EQ(countr_zero(1000000007LL * 1000000007 - 1), 4UL);
}

TEST(Bits, countr_one) {
	EXPECT_EQ(countr_one(~0), 32UL);
	EXPECT_EQ(countr_one<uint16_t>(~0), 16UL);
	EXPECT_EQ(countr_one<int64_t>(~0), 64UL);

	EXPECT_EQ(countr_one(~1), 0UL);
	EXPECT_EQ(countr_one(~(1 << 20)), 20UL);
	EXPECT_EQ(countr_one(~((1LL << 62) - 1)), 0UL);
	EXPECT_EQ(countr_one(~(1LL << 62)), 62UL);
	EXPECT_EQ(countr_one(~(1LL << 63)), 63UL);

	EXPECT_EQ(countr_one(~-1), 0UL);
	EXPECT_EQ(countr_one(~static_cast<int64_t>(-1)), 0UL);

	EXPECT_EQ(countr_one(~1000000008), 3UL);
	EXPECT_EQ(countr_one(~(1000000007LL * 1000000007 - 1)), 4UL);
}

TEST(Bits, has_single_bit) {
	EXPECT_FALSE(has_single_bit(0));
	EXPECT_TRUE(has_single_bit(1));
	EXPECT_TRUE(has_single_bit(1 << 20));
	EXPECT_TRUE(has_single_bit(1LL << 62));
	EXPECT_FALSE(has_single_bit((1LL << 62) - 1));
	EXPECT_FALSE(has_single_bit(~0));
	EXPECT_FALSE(has_single_bit(7));
	EXPECT_TRUE(has_single_bit(8));
}

TEST(Bits, bit_ceil) {
	EXPECT_EQ(bit_ceil(0), 1);
	EXPECT_EQ(bit_ceil(1), 1);
	EXPECT_EQ(bit_ceil(2), 2);
	EXPECT_EQ(bit_ceil(3), 4);
	EXPECT_EQ(bit_ceil(4), 4);

	EXPECT_EQ(bit_ceil<int64_t>(~0), 1);
	EXPECT_EQ(bit_ceil((1LL << 62) - 1), 1LL << 62);
	EXPECT_EQ(bit_ceil(1LL << 63), 1LL << 63);

	EXPECT_EQ(bit_ceil(1000000007), 1 << 30);
	EXPECT_EQ(bit_ceil(1000000007LL * 1000000007 - 1), 1LL << 60);
}

TEST(Bits, bit_floor) {
	EXPECT_EQ(bit_floor(0), 0);
	EXPECT_EQ(bit_floor(1), 1);
	EXPECT_EQ(bit_floor(2), 2);
	EXPECT_EQ(bit_floor(3), 2);
	EXPECT_EQ(bit_floor(4), 4);

	EXPECT_EQ(bit_floor<int64_t>(~0), 1LL << 63);
	EXPECT_EQ(bit_floor((1LL << 62) - 1), 1LL << 61);
	EXPECT_EQ(bit_floor(1LL << 63), 1LL << 63);

	EXPECT_EQ(bit_floor(1000000007), 1 << 29);
	EXPECT_EQ(bit_floor(1000000007LL * 1000000007 - 1), 1LL << 59);
}

TEST(Bits, bit_width) {
	EXPECT_EQ(bit_width(0), 0UL);
	EXPECT_EQ(bit_width(1), 1UL);
	EXPECT_EQ(bit_width(2), 2UL);
	EXPECT_EQ(bit_width(3), 2UL);
	EXPECT_EQ(bit_width(4), 3UL);

	EXPECT_EQ(bit_width<int64_t>(~0), 64UL);
	EXPECT_EQ(bit_width(~2), 32UL);
	EXPECT_EQ(bit_width<int32_t>(1 << 31), 32UL);
	EXPECT_EQ(bit_width((1LL << 62) - 1), 62UL);
	EXPECT_EQ(bit_width(1LL << 63), 64UL);

	EXPECT_EQ(bit_width(1000000007), 30UL);
	EXPECT_EQ(bit_width(1000000007LL * 1000000007 - 1), 60UL);
}
