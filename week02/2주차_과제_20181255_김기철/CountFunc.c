#include "Header.h"

// page의 자릿수마다 숫자들을 구해서 더해준다
void PageCount(int* arr, int page, int digit)
{
	// 0이 될 때가지 10으로 나눠준다
	while (page > 0)
	{
		// 자릿수를 의미하는 digit만큼씩 더해준다
		arr[page % 10] += digit;
		page /= 10; 
	}
}

void TotalCount(int* arr, int total, int digit)
{
	// digit의 자리에 0부터 9까지 total개씩 나오므로 total*digit만큼씩 더해준다
	for (int i = 0; i < LEN; i++)
		arr[i] += total * digit;
}
