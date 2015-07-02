#pragma once
#include "Head.h"
#include "Maths.h"
#include "StringUtils.h"

struct Rational {

	typedef ll TBase;

	static const Rational ZERO;
	static const Rational ONE;
	static const Rational TWO;
	static const double EPS;

	TBase a, b;

	Rational() : a(0), b(1) { }

	Rational(TBase _a) : a(_a), b(1) { }

	Rational(TBase _a, TBase _b) : a(_a), b(_b) {
		norm();
	}

	operator bool() const {
		return (a != 0);
	}

	operator long long() const {
		return a / b;
	}

	operator long double() const {
		return (a + 0.) / b;
	}

	operator string() const {
		return toa(a) + "/" + toa(b);
	}

	bool operator !() const {
		return !a;
	}

	bool operator && (const bool x) const {
		return (a > 0 && x);
	}

	Rational operator - () const {
		return Rational(-a, b);
	}

	Rational operator + () const {
		return Rational(a, b);
	}

	Rational& operator ++ () {
		a += b;
		return *this;
	}

	Rational operator ++ (int) {
		a += b;
		return Rational(a - b, b);
	}

	Rational& operator -- () {
		a -= b;
		return *this;
	}

	Rational operator -- (int) {
		a -= b;
		return Rational(a + b, b);
	}

	Rational operator + (const Rational& x) const {
		return Rational(a * x.b + x.a * b, b * x.b);
	}

	Rational operator - (const Rational& x) const {
		return Rational(a * x.b - x.a * b, b * x.b);
	}

	Rational operator * (const Rational& x) const {
		return Rational(a * x.a, b * x.b);
	}

	Rational operator / (const Rational& x) const {
		return Rational(a * x.b, b * x.a);
	}

	Rational& operator += (const Rational& x) {
		a = a * x.b + x.a * b;
		b *= x.b;
		norm();
		return *this;
	}

	Rational& operator -= (const Rational& x) {
		a = a * x.b - x.a * b;
		b *= x.b;
		norm();
		return *this;
	}

	Rational& operator *= (const Rational& x) {
		a *= x.a;
		b *= x.b;
		norm();
		return *this;
	}

	Rational& operator /= (const Rational& x) {
		a *= x.b;
		b *= x.a;
		norm();
		return *this;
	}

	Rational operator + (const int x) const {
		return Rational(a + (TBase)x * b, b);
	}

	Rational operator - (const int x) const {
		return Rational(a - (TBase)x * b, b);
	}

	Rational operator * (const int x) const {
		return Rational(a * (TBase)x, b);
	}

	Rational operator / (const int x) const {
		return Rational(a, b * (TBase)x);
	}

	Rational& operator += (const int x) {
		a += b * (TBase)x;
		return *this;
	}

	Rational& operator -= (const int x) {
		a -= b * (TBase)x;
		return *this;
	}

	Rational& operator *= (const int x) {
		a *= (TBase)x;
		norm();
		return *this;
	}

	Rational& operator /= (const int x) {
		b *= (TBase)x;
		norm();
		return *this;
	}

	Rational operator + (const ll x) const {
		return Rational(a + (TBase)x * b, b);
	}

	Rational operator - (const ll x) const {
		return Rational(a - (TBase)x * b, b);
	}

	Rational operator * (const ll x) const {
		return Rational(a * (TBase)x, b);
	}

	Rational operator / (const ll x) const {
		return Rational(a, b * (TBase)x);
	}

	Rational& operator += (const ll x) {
		a += b * (TBase)x;
		return *this;
	}

	Rational& operator -= (const ll x) {
		a -= b * (TBase)x;
		return *this;
	}

	Rational& operator *= (const ll x) {
		a *= (TBase)x;
		norm();
		return *this;
	}

	Rational& operator /= (const ll x) {
		b *= (TBase)x;
		norm();
		return *this;
	}

	double operator + (const double x) const {
		return (a + 0.) / b + x;
	}

	double operator - (const double x) const {
		return (a + 0.) / b - x;
	}

	double operator * (const double x) const {
		return (a + 0.) / b * x;
	}

	double operator / (const double x) const {
		return (a + 0.) / b / x;
	}

	bool operator == (const Rational& x) const {
		return (a == x.a && b == x.b);
	}

	bool operator != (const Rational& x) const {
		return (a != x.a || b != x.b);
	}

	bool operator < (const Rational& x) const {
		return (a * x.b < x.a * b);
	}

	bool operator >(const Rational& x) const {
		return (a * x.b > x.a * b);
	}

	bool operator <= (const Rational& x) const {
		return (a * x.b <= x.a * b);
	}

	bool operator >= (const Rational& x) const {
		return (a * x.b >= x.a * b);
	}

	bool operator == (const int x) const {
		return (a == (TBase)x * b);
	}

	bool operator != (const int x) const {
		return (a != (TBase)x * b);
	}

	bool operator < (const int x) const {
		return (a < (TBase)x * b);
	}

	bool operator >(const int x) const {
		return (a > (TBase)x * b);
	}

	bool operator <= (const int x) const {
		return (a <= (TBase)x * b);
	}

	bool operator >= (const int x) const {
		return (a >= (TBase)x * b);
	}

	bool operator == (const ll x) const {
		return (a == (TBase)x * b);
	}

	bool operator != (const ll x) const {
		return (a != (TBase)x * b);
	}

	bool operator < (const ll x) const {
		return (a < (TBase)x * b);
	}

	bool operator >(const ll x) const {
		return (a > (TBase)x * b);
	}

	bool operator <= (const ll x) const {
		return (a <= (TBase)x * b);
	}

	bool operator >= (const ll x) const {
		return (a >= (TBase)x * b);
	}

	bool operator == (const double x) const {
		return (fabs((a + 0.) / b - x) < EPS);
	}

	bool operator != (const double x) const {
		return (fabs((a + 0.) / b - x) > EPS);
	}

	bool operator < (const double x) const {
		return ((a + 0.) / b + EPS < x);
	}

	bool operator > (const double x) const {
		return ((a + 0.) / b > x + EPS);
	}

	bool operator <= (const double x) const {
		return ((a + 0.) / b + EPS <= x);
	}

	bool operator >= (const double x) const {
		return ((a + 0.) / b >= x + EPS);
	}

	friend ostream& operator << (ostream &out, Rational& x) {
		return out << x.a << " " << x.b;
	}

	friend istream& operator >> (istream &in, Rational& x) {
		return in >> x.a >> x.b;
	}

	inline void norm() {
		TBase g = gcd(a, b);
		a /= g;
		b /= g;
		if (b < 0) {
			a = -a;
			b = -b;
		}
	}

	inline void read() {
		scanf(LLD LLD, &a, &b);
		norm();
	}

	inline void read(istream &in) {
		in >> a >> b;
		norm();
	}

	inline void print() const {
		printf(LLD " " LLD, a, b);
	}

	inline void print(ostream& out) const {
		out << a << " " << b;
	}

	inline void readSlash() {
		scanf(LLD "/" LLD, &a, &b);
		norm();
	}

	inline void readSlash(istream& in) {
		char ch;
		in >> a >> ch >> b;
		norm();
	}

	inline void printSlash() const {
		printf(LLD "/" LLD, a, b);
	}

	inline void printSlash(ostream& out) const {
		out << a << "/" << b;
	}

	inline Rational negate() const {
		return Rational(-a, b);
	}

	inline Rational abs() const {
		if (a < 0) return Rational(-a, b); else return Rational(a, b);
	}

	inline Rational inverse() const {
		return Rational(b, a);
	}

	inline int sign() const {
		return (a < 0 ? -1 : (a > 0));
	}

	inline double doubleValue() const {
		return (a + 0.) / b;
	}

	inline TBase Floor() const {
		return a / b;
	}

	inline TBase Ceil() const {
		return (TBase)ceil((a + 0.) / b - EPS);
	}

	inline TBase Round() const {
		double cur = (a + 0.) / b;
		return (TBase)(cur < 0 ? ceil(cur - .5 - EPS) : floor(cur + .5 + EPS));
	}

	inline double frac() const {
		return (a + 0.) / b - a / b;
	}

	inline ll hashCode() const {
		return a * 877117 + b;
	}

	inline Rational pow(ll n) const {
		return Rational(ppow(a, n), ppow(b, n));
	}

};

const Rational Rational::ZERO = Rational(0);
const Rational Rational::ONE = Rational(1);
const Rational Rational::TWO = Rational(2);
const double Rational::EPS = EPS;