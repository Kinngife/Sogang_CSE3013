#include <iostream>
#include <cstring>
#include "Str.h"
using namespace std;

int main()
{
	Str a("I'm a girl");
	cout << a.contents();
	a = "I'm a boy\n";
	cout << a.contents();
	cout << a.compare("I'm a a\n") << endl;
	return 0;
}
