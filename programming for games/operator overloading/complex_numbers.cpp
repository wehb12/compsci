// complex_numbers.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "matrix.h"

using namespace std;

//could the add, subtract methods be templated to take in a Complex number and an int??
template <typename T>
class Complex
{
public:
	inline Complex() : re(0), im(0) { }
	inline Complex(T i, T r) : re(i), im(r) { }
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
	Matrix<Complex<int>> mtx1;
	mtx1.set_element(0, 0, Complex<int>(5, 6));
	mtx1.set_element(1, 1, Complex<int>(4, 3));
	mtx1.set_element(2, 2, Complex<int>(1, 7));
	cout << "matrix 1:\n" << mtx1;

	Matrix<Complex<int>> mtx2;
	mtx2.set_element(0, 0, Complex<int>(1, 1));
	mtx2.set_element(1, 1, Complex<int>(2, 6));
	mtx2.set_element(2, 2, Complex<int>(5, 2));

	cout << "matrix 2:\n" << mtx2;

	Matrix<Complex<int>> mtx3;
	mtx3 = mtx2;
	cout << "matrix 3 assigned from matrix 2:\n" << mtx3;

	Matrix<Complex<int>> product = (mtx1 * mtx3);
	cout << "matrix product assigned from mtx1 * mtx3:\n" << product;

	Matrix<Complex<int>> add = (mtx1 + mtx3);
	cout << "matrix addition assigned from mtx1 + mtx3:\n" << add;

	Matrix<Complex<int>> sub = (mtx3 - mtx1);
	cout << "matrix subtraction assigned from mtx3 - mtx1:\n" << sub;

	/*if (mtx3 == mtx2)
		cout << "mtx3 == mtx2" << endl;
	if (!(mtx3 == mtx1))
		cout << "mtx3 != mtx1";*/

	return 0;
}
