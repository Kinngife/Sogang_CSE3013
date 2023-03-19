#ifndef __HEADER_H__ // 헤더파일이 정의되어있지 않으면 define
#define __HEADER_H__

// 배열 길이 10
#define LEN 10

// 배열 초기화 함수
void ArrayInit(int* arr);
// 배열 출력 함수
void ArrayOutput(int* arr);
// 0부터 9까지 몇 개 나오는지 구하는 재귀함수
void RecursiveFunc(int* arr, int startp, int endp, int digit);
void PageCount(int* arr, int page, int digit); 
void TotalCount(int* arr, int total, int digit); 

#endif
