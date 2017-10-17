// complex_numbers.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

//could the add, subtract methods be templated to take in a Complex number and an int??
template <typename T>
class Complex
{
public:
	inline Complex() : re(0), im(0) { }
	inline Complex(int i, int r) : re(i), im(r) { }
	~Complex() { }
	Complex<T>& operator=(const Complex<T>& rhs);
	template <typename U>
	friend Complex<U>& operator+(const Complex<U>& a, const Complex<U>& b);
	template <typename U>
	friend Complex<U>& operator-(const Complex<U>& a, const Complex<U>& b);
	template <typename U>
	friend Complex<U>& operator*(const Complex<U>& a, const Complex<U>& b);
	template <typename U>
	friend ostream& operator<<(ostream& a, const Complex<U>& b);
private:
	T im;
	T re;
};

template <typename T>
Complex<T>& Complex<T>::operator=(const Complex<T>& rhs)
{
	if (this == &rhs)
		return *this;

	re = rhs.re;
	im = rhs.im;
	return *this;
}

template <typename U>
Complex<U>& operator+(const Complex<U>& a, const Complex<U>& b)
{
	Complex<U> result;

	result.re = a.re + b.re;
	result.im = a.im + b.im;
	return result;
}

template <typename U>
Complex<U>& operator-(const Complex<U>& a, const Complex<U>& b)
{
	Complex<U> result;

	result.re = a.re - b.re;
	result.im = a.im - b.im;
	return result;
}

template <typename U>
Complex<U>& operator*(const Complex<U>& a, const Complex<U>& b)
{
	Complex<U> result;

	result.re = (a.re * b.re) - (a.im * b.im);
	result.im = (a.re * b.im) + (a.im * b.re);
	return result;
}

template <typename U>
ostream& operator<<(ostream& ostr, const Complex<U>& rhs)
{
	cout << rhs.re << " + " << rhs.im << "i";

	return ostr;
}

int main()
{
	Complex<int> c(1, 2);

	cout << c;

    return 0;
}

