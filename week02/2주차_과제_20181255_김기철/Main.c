#include <stdio.h>
#include "Header.h"

int main()
{	
	// 길이가 LEN(10)인 배열 arr 생성
	int arr[LEN];
	int T; 
	scanf("%d", &T); 
	while (T--) 
	{
		int N;
		scanf("%d", &N);
		// arr 초기화
		ArrayInit(arr);
		// 1부터 N까지 1의 자리부터 계산
		RecursiveFunc(arr, 1, N, 1);
		// arr 출력
		ArrayOutput(arr);
	}
	return 0;
}
