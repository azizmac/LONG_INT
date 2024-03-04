/*

#include "LONGINT.h"
#include <iostream>
using namespace std;

int main() {
	LONG_INT bi("2"), bi2 = 100;
	cout << bi.pow(1000) << endl;
	LONG_INT f = 1;
	for (LONG_INT i = 2; i <= bi2; ++i) f *= i;
	cout << f << endl;
}
*/
#include <iostream>
#include <cassert>
#include "LONGINT.h"
#include <iostream>
using namespace std;

int generateRandomNumber(int min, int max) {
	return rand() % (max - min + 1) + min;
}


int main() {

	srand(time(NULL));
	for (int i = 0; i < 1000000; i++) {
		int aa, bb, cc, dd;
		aa = generateRandomNumber(-45000, 45000);
		bb = generateRandomNumber(-45000, 45000);
		cc = aa * bb;
		dd = cc / bb;


		LONG_INT a = aa;
		LONG_INT b = bb;
		LONG_INT c = a * b;
		LONG_INT c2 = aa * bb;

		LONG_INT d = c / b;
		//LONG_INT d = cc / bb;

		if (d != a) {
			cout << "UNCORRECT d != a" << endl;
		}

		if (dd != aa) {
			cout << "UNCORRECT dd != aa" << endl;
		}
		if (d != dd) {
			cout << "UNCORRECT d != dd" << endl;
		}

	}
	
}
