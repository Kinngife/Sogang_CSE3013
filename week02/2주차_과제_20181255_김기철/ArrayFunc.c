#include <stdio.h>
#include "Header.h"

void ArrayInit(int* arr) 
{	
	// 배열의 모든 원소를 0으로 초기화
	for (int i = 0; i < LEN; i++)
		arr[i] = 0;
}
void ArrayOutput(int* arr)
{
	// 배열의 모든 원소를 출력
	for (int i = 0; i < LEN; i++)
		printf("%d ", arr[i]);
	printf("\n");
}
