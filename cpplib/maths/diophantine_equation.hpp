#pragma once


template<typename T>
T extended_gcd(const T a, const T b, T& x, T& y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }
    const T p = b / a;
    const T g = extended_gcd(b - p * a, a, y, x);
    x -= p * y;
    return g;
}

// |x|, |y| <= max(|a|, |b|, |c|)
template<typename T>
bool solve_diophantine(const T a, const T b, const T c, T& x, T& y, T& g) {
    if (a == 0 && b == 0) {
        if (c == 0) {
            x = 0;
            y = 0;
            g = 0;
            return true;
        }
        return false;
    }
    if (a == 0) {
        if (c % b == 0) {
            x = 0;
            y = c / b;
            g = std::abs(b);
            return true;
        }
        return false;
    }
    if (b == 0) {
        if (c % a == 0) {
            x = c / a;
            y = 0;
            g = std::abs(a);
            return true;
        }
        return false;
    }
    g = extended_gcd(a, b, x, y);
    if (c % g != 0) {
        return false;
    }
    T adjusted_c = c;
    const T dx = adjusted_c / a;
    adjusted_c -= dx * a;
    const T dy = adjusted_c / b;
    adjusted_c -= dy * b;
    x = dx + static_cast<T>(static_cast<__int128>(x) * (adjusted_c / g) % b);
    y = dy + static_cast<T>(static_cast<__int128>(y) * (adjusted_c / g) % a);
    g = std::abs(g);
    return true;
}
