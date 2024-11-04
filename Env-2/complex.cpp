#include "complex.h"
#include <cmath>
#include <iostream>

Complex::Complex() : _re(0), _im(0) {}

Complex::Complex(int re, int im) : _re(re), _im(im) {}

Complex Complex::operator+(const Complex& other)
{
	Complex res;
	res._im  = _im + other._im;
	res._re = _re + other._re;

	return res;
}

Complex Complex::operator-(const Complex& other)
{
	Complex res;
	res._re  = _re - other._re;
	res._im = _im - other._im;

	return res;
}

Complex Complex::operator*(const int& val)
{
	Complex res(*this);
	res._re *= val;
	res._im *= val;

	return res;
}

bool Complex::operator<(const Complex& other)
{
	return this->abs() < other.abs();
}


double Complex::abs() const
{
	return std::sqrt(_im * _im + _re * _re);
}

void Complex::print()
{
	if(_im < 0)
	{
		std::cout << _re << _im << 'i';
	}

	if(_im == 0)
		std::cout << _re;

	if(_im > 0)
		std::cout << _re << '+' << _im << 'i';

	std::cout << std::endl;

}
