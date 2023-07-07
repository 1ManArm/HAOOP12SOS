#include <iostream>
#include <vector>
#include <string>
#include "DynArr.h"

class A {
public:
	void f00()const {}
};

template<typename T>
void f01(T a){}

int main() {
	setlocale(LC_ALL, "Russian");

	f01(1);
	f01<int>(.1);
	extern DynamicArr<int> b13;
	std::cout << "Something\n";
	std::vector<int> a11;
	std::vector<A> a12(31);
	std::vector<int> a13{10, 20, 60};
	std::vector<int> a14{b13.begin(), b13.end()};
	a11.front();
	a11.back();
	a11.push_back(30);
	int a = a11.capacity();
	int b = a11.size();

	DynamicArr<int> b11;
	DynamicArr<A> b12(40);

	DynamicArr<A>::iterator tmp = b12.begin(); //E0265 ссылается на 30 строку файла DynArr.h. Пробовал думать, но пока никак...
	A* pTmp = new A;
	pTmp->f00();
	tmp->f00();
	A t;
	/*DynamicArr<int> b13{70, 90, 110};
	DynamicArr<int> b14(a13.begin(), a13.end());*/

	return 0;
}