#pragma once
#include "Head.h"
#include "Maths.h"
#include "StringUtils.h"

class IO;

extern IO io;

class Rational {
public:
	typedef ll TBase;

	static const Rational ZERO;
	static const Rational ONE;
	static const Rational TWO;
	static const double EPS;

	TBase a, b;

	Rational() : a(0), b(1) {}
	Rational(TBase _a) : a(_a), b(1) {}
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
		return Rational(a + static_cast<TBase>(x) * b, b);
	}

	Rational operator - (const int x) const {
		return Rational(a - static_cast<TBase>(x) * b, b);
	}

	Rational operator * (const int x) const {
		return Rational(a * static_cast<TBase>(x), b);
	}

	Rational operator / (const int x) const {
		return Rational(a, b * static_cast<TBase>(x));
	}

	Rational& operator += (const int x) {
		a += b * static_cast<TBase>(x);
		return *this;
	}

	Rational& operator -= (const int x) {
		a -= b * static_cast<TBase>(x);
		return *this;
	}

	Rational& operator *= (const int x) {
		a *= static_cast<TBase>(x);
		norm();
		return *this;
	}

	Rational& operator /= (const int x) {
		b *= static_cast<TBase>(x);
		norm();
		return *this;
	}

	Rational operator + (const ll x) const {
		return Rational(a + static_cast<TBase>(x) * b, b);
	}

	Rational operator - (const ll x) const {
		return Rational(a - static_cast<TBase>(x) * b, b);
	}

	Rational operator * (const ll x) const {
		return Rational(a * static_cast<TBase>(x), b);
	}

	Rational operator / (const ll x) const {
		return Rational(a, b * static_cast<TBase>(x));
	}

	Rational& operator += (const ll x) {
		a += b * static_cast<TBase>(x);
		return *this;
	}

	Rational& operator -= (const ll x) {
		a -= b * static_cast<TBase>(x);
		return *this;
	}

	Rational& operator *= (const ll x) {
		a *= static_cast<TBase>(x);
		norm();
		return *this;
	}

	Rational& operator /= (const ll x) {
		b *= static_cast<TBase>(x);
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
		return (a == static_cast<TBase>(x) * b);
	}

	bool operator != (const int x) const {
		return (a != static_cast<TBase>(x) * b);
	}

	bool operator < (const int x) const {
		return (a < static_cast<TBase>(x) * b);
	}

	bool operator >(const int x) const {
		return (a > static_cast<TBase>(x) * b);
	}

	bool operator <= (const int x) const {
		return (a <= static_cast<TBase>(x) * b);
	}

	bool operator >= (const int x) const {
		return (a >= static_cast<TBase>(x) * b);
	}

	bool operator == (const ll x) const {
		return (a == static_cast<TBase>(x) * b);
	}

	bool operator != (const ll x) const {
		return (a != static_cast<TBase>(x) * b);
	}

	bool operator < (const ll x) const {
		return (a < static_cast<TBase>(x) * b);
	}

	bool operator >(const ll x) const {
		return (a > static_cast<TBase>(x) * b);
	}

	bool operator <= (const ll x) const {
		return (a <= static_cast<TBase>(x) * b);
	}

	bool operator >= (const ll x) const {
		return (a >= static_cast<TBase>(x) * b);
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

	void norm() {
		TBase g = gcd(a, b);
		a /= g;
		b /= g;
		if (b < 0) {
			a = -a;
			b = -b;
		}
	}

	void read() {
		io >> a >> b;
		norm();
	}

	void read(istream &in) {
		in >> a >> b;
		norm();
	}

	void print() const {
		io << a << " " << b;
	}

	void print(ostream& out) const {
		out << a << " " << b;
	}

	void readSlash() {
		char ch;
		io >> a >> ch >> b;
		norm();
	}

	void readSlash(istream& in) {
		char ch;
		in >> a >> ch >> b;
		norm();
	}

	void printSlash() const {
		io << a << "/" << b;
	}

	void printSlash(ostream& out) const {
		out << a << "/" << b;
	}

	Rational negate() const {
		return Rational(-a, b);
	}

	Rational abs() const {
		return (a < 0 ? Rational(-a, b) : Rational(a, b));
	}

	Rational inverse() const {
		return Rational(b, a);
	}

	int sign() const {
		return (a < 0 ? -1 : (a > 0));
	}

	double doubleValue() const {
		return (a + 0.) / b;
	}

	TBase Floor() const {
		return a / b;
	}

	TBase Ceil() const {
		return static_cast<TBase>(ceil((a + 0.) / b - EPS));
	}

	TBase Round() const {
		double cur = (a + 0.) / b;
		return static_cast<TBase>((cur < 0 ? ceil(cur - .5 - EPS) : floor(cur + .5 + EPS)));
	}

	double frac() const {
		return (a + 0.) / b - a / b;
	}

	ll hashCode() const {
		return a * 877117 + b;
	}

	Rational pow(ll n) const {
		return Rational(ppow(a, n), ppow(b, n));
	}

};

const Rational Rational::ZERO = Rational(0);
const Rational Rational::ONE = Rational(1);
const Rational Rational::TWO = Rational(2);
const double Rational::EPS = EPS;