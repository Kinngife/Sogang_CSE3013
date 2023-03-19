#include <iostream>
#include <cstdlib>
#include <cstring>
#include "Str.h"
using namespace std;

Str::Str(int leng)
{
	str = new char[leng]; // string의 길이인 leng만큼 동적할당
	len = leng; // len에 leng 저장
}
Str::Str(const char* neyong)
{
	str = new char[strlen(neyong) + 1]; // neyong의 길이 + '\0'만큼 동적할당
	strcpy(str, neyong); // str에 neyong 복사
	len = strlen(neyong); // len에 neyong 길이 저장
}
Str::~Str()
{
	delete[]str; // str 소멸, 할당된 메모리를 해제
}
int Str::length(void)
{
	return len; // len 반환
}
char* Str::contents(void)
{
	return str; // str 반환
}
int Str::compare(class Str& a)
{
	return strcmp(str, a.str); // str과 a클래스의 str을 비교 (크면 1, 같으면 0, 작으면 -1 반환)
}
int Str::compare(const char* a)
{
	return strcmp(str, a); // str과 a를 비교 (크면 1, 같으면 0, 작으면 -1 반환)
}
void Str::operator=(const char* a)
{
	strcpy(str, a); // str에 a 복사
	len = strlen(a); // len에 a 길이 저장
}
void Str::operator=(class Str& a)
{
	strcpy(str, a.str); // str에 a클래스의 str 복사
	len = a.len; // len에 a클래스의 len 저장
}
