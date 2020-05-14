#include <gtest/gtest.h>

#include "maths/fibonacci.hpp"
#include "maths/mod_int.hpp"

TEST(Fibonacci, GetNthFibonacci) {
	EXPECT_EQ(get_nth_fibonacci<int32_t>(0), 1);
	EXPECT_EQ(get_nth_fibonacci<int32_t>(1), 1);

	uint64_t a = 1;
	uint64_t b = 1;
	for (size_t cnt = 2; cnt < 30; ++cnt) {
		const uint64_t tmp = a + b;
		a = b;
		b = tmp;
		EXPECT_EQ(get_nth_fibonacci<uint64_t>(cnt), b);
	}
}

TEST(Fibonacci, GetNthFibonacciModular) {
	using Int = ModInt<int, 1000000007>;

	EXPECT_EQ(get_nth_fibonacci<Int>(998), 589888339);
	EXPECT_EQ(get_nth_fibonacci<Int>(999888777666555LL), 734647414);
}
