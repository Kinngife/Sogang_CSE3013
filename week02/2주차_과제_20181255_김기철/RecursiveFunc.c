#include "Header.h"

void RecursiveFunc(int* arr, int startp, int endp, int digit)
{
	// startp를 1씩 증가시키면서 1의자리를 0으로 만들어 준다
	while (startp % 10 > 0)
	{
		// startp가 endp보다 크면 탈출
		if (startp > endp)
			break;
		// 1씩 증가할 때마다 각 page에 있는 숫자들을 count
		PageCount(arr, startp, digit);
		startp++;
	}
	// startp가 endp보다 크면 종료
	if (startp > endp) 
		return;
	// endp를 1씩 감소시키면서 1의자리를 9로 만들어 준다
	while (endp % 10 < 9)
	{
		// startp가 endp보다 크면 탈출
		if (startp > endp)
			break;
		// 1씩 감소할 때마다 각 page에 있는 숫자들을 count
		PageCount(arr, endp, startp <= endp);
		endp--;
	}
	
	// digit의 자리에 0부터 9까지 숫자가 각각 total개씩 나온다
	int total = (endp / 10 - startp / 10 + 1);
	// 각각의 자리에 total*digit만큼씩 더해준다
	TotalCount(arr, total, digit);

	// page를 1의 자리부터 자릿수를 늘려가면서 재귀함수에 넣어준다
	startp /= 10, endp /= 10, digit *= 10;
	RecursiveFunc(arr, startp, endp, digit);
}
