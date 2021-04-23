#pragma once

#include "matrix.hpp"

template<typename T, typename size_type>
T get_nth_fibonacci(const size_type n)
// returns nth fibonacci number
// get_nth_fibonacci(0) = 1
// get_nth_fibonacci(1) = 1
// get_nth_fibonacci(2) = 2
{
    const Matrix<T> fib = Matrix<T>({{0, 1}, {1, 1}}).binpow(n);
    return fib[0][0] + fib[1][0];
}
