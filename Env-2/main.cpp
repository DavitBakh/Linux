#include <iostream>
#include "sort.h"
#include "complex.h"

int main()
{
	Complex* arr = new Complex[4];
	arr[0] = Complex(1,0);
	arr[1] = Complex(2,3);
	arr[2] = Complex(1,2);
	arr[3] = Complex(0,0);

	sort(arr, 4);

	for(int i = 0; i < 4; ++i)
	{
		arr[i].print();
	}


	return 0;
}
