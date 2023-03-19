#include <iostream>
#include <string>
#include "Stack.h"
using namespace std;

int main()
{
	double dVal;
	string strVal;
	LinkedList<double> dList;
	LinkedList<string> strList;

	dList.Insert(3.14);
	dList.Insert(123456);
	dList.Insert(-0.987654);
	dList.Print();

	dList.Delete(dVal);
	cout << "삭제된 마지막 원소: " << dVal << "\n";
	dList.Print();
	
	dList.Insert(777.777);
	dList.Delete(dVal);
	cout << "삭제된 마지막 원소: " << dVal << "\n";
	dList.Delete(dVal);
	cout << "삭제된 마지막 원소: " << dVal << "\n";
	dList.Print();

	dList.Delete(dVal);
	cout << "삭제된 마지막 원소: " << dVal << "\n";
	dList.Print();

	strList.Insert("This");
	strList.Insert("is a");
	strList.Insert("Template");
	strList.Insert("Example");
	strList.Print();

	strList.Delete(strVal);
	cout << "삭제된 마지막 원소: " << strVal << "\n";
	strList.Insert("Class");
	strList.Print();

	return 0;
}
