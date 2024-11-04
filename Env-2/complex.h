#ifndef COMPLEX_H
#define COMPLEX_H

class Complex
{
private:
	int _re;
	int _im;
public:

	Complex();
	Complex(int re, int im);

	Complex operator+(const Complex& other);
	Complex operator-(const Complex& ohter);
	Complex operator*(const int& val);

	bool operator<(const Complex& other);

	double abs() const;

	void print();
};

#endif
