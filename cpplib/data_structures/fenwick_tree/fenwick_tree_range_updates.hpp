#pragma once
#include <algorithm>

template<typename T>
class FenwickTreeSumRangeUpdates {
public:
    T * add, * mul;
    int N;

    FenwickTreeSumRangeUpdates(const int N) : N(N) {
        add = new T[N];
        mul = new T[N];
        std::fill_n(add, N, 0);
        std::fill_n(mul, N, 0);
    }

    ~FenwickTreeSumRangeUpdates() {
        delete[] add;
        delete[] mul;
    }

    void clear() {
        std::fill_n(add, N, 0);
        std::fill_n(mul, N, 0);
    }

    void setSize(const int n) {
        N = n;
    }

    void build(T a[], const int n) {
        N = n;
        for (int i = 0; i < N; ++i) {
            update(i, i, a[i]);
        }
    }

    void update(const int left, const int right, const T by) {
        if (left > right) {
            return;
        }
        _update(left, by, -by * (left - 1));
        _update(right, -by, by * right);
    }

    T query(int at) const {
        T resMul = 0;
        T resAdd = 0;
        int start = at;
        while (at >= 0) {
            resMul += mul[at];
            resAdd += add[at];
            at = (at & (at + 1)) - 1;
        }
        return resMul * start + resAdd;
    }

    T query(const int left, const int right) const {
        return query(right) - query(left - 1);
    }

private:
    void _update(int at, const T updMul, const T updAdd) {
        while (at < N) {
            mul[at] += updMul;
            add[at] += updAdd;
            at |= (at + 1);
        }
    }

};