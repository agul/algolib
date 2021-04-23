#pragma once
#include <vector>

template<typename T>
class Polynom : public std::vector<T> {
public:
    using size_type = typename std::vector<T>::size_type;

    Polynom() : Polynom(0) {}

    explicit Polynom(const size_type degree) : std::vector<T>(degree + 1) {}

    template<typename I>
    Polynom(const I first, const I last) : std::vector<T>(first, last) {}

    Polynom(std::initializer_list<T>&& list) : std::vector<T>(std::move(list)) {}

    constexpr size_type degree() const {
        return this->size() - 1;
    }

    Polynom operator -() const {
        return negate();
    }

    Polynom operator +() const {
        return *this;
    }

    Polynom add(const Polynom& rhs) const {
        Polynom res(std::max(rhs.degree(), degree()));
        std::copy(this->begin(), this->end(), res.begin());
        for (size_t i = 0; i < rhs.size(); ++i) {
            res[i] += rhs[i];
        }
        res.normalize();
        return res;
    }

    Polynom multiply(const Polynom& rhs) const {
        Polynom res(rhs.degree() + degree());
        for (size_t i = 0; i < this->size(); ++i) {
            for (size_t j = 0; j < rhs.size(); ++j) {
                res[i + j] += this->data()[i] * rhs[j];
            }
        }
        res.normalize();
        return res;
    }

    Polynom multiply(const T& rhs) const {
        Polynom res(*this);
        for (auto& it : res) {
            it *= rhs;
        }
        res.normalize();
        return res;
    }

    Polynom subtract(const Polynom& rhs) const {
        return add(rhs.negate());
    }

    Polynom operator *(const Polynom& rhs) const {
        return multiply(rhs);
    }

    Polynom operator *(const T& rhs) const {
        return multiply(rhs);
    }

    Polynom operator +(const Polynom& rhs) const {
        return add(rhs);
    }

    Polynom operator -(const Polynom& rhs) const {
        return subtract(rhs);
    }

    Polynom& operator +=(const Polynom& rhs) {
        Polynom res = add(rhs);
        this->swap(res);
        return *this;
    }

    template<typename U>
    Polynom& operator *=(const U& rhs) {
        Polynom res = multiply(rhs);
        this->swap(res);
        return *this;
    }

    Polynom& operator -=(const Polynom& rhs) {
        Polynom res = subtract(rhs);
        this->swap(res);
        return *this;
    }

    static Polynom one() {
        Polynom res;
        res.front() = 1;
        return res;
    }

    T operator()(const T& x) const {
        T result = 0;
        T deg = 1;
        for (const auto& it : *this) {
            result += it * deg;
            deg *= x;
        }
        return result;
    }

    Polynom negate() const {
        Polynom res = *this;
        for (auto& it : res) {
            it = -it;
        }
        return res;
    }

    void reverse() {
        std::reverse(this->begin(), this->end());
    }

    void normalize() {
        while (this->size() > 1 && this->back() == T(0)) {
            this->pop_back();
        }
    }
};