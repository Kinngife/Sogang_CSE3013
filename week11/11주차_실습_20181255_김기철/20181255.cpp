#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <map>
#include <algorithm>
#include <string>
#include <cstring>
#include <climits>
#include <random>
#include <cmath>
#include <set>
#include <ctime>
using namespace std;
using ll = long long;
typedef struct {
	int num;
	int rwall;
	int dwall;
} Room;

int main()
{
	ios_base::sync_with_stdio(0);
	cin.tie(0), cout.tie(0);
	srand(time(NULL));

	int WIDTH, HEIGHT;
	int roomnum = 1, randwall;
	cin >> WIDTH >> HEIGHT;
	vector<vector<Room>>arr(HEIGHT, vector<Room>(WIDTH));
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			arr[i][j].num = roomnum++;
			arr[i][j].rwall = 1;
			arr[i][j].dwall = 1;
		}
	}

	for (int i = 0; i < HEIGHT; i++)
	{
		// 첫 번째 줄
		if (i == 0)
		{
			for (int j = 0; j < WIDTH - 1; j++)
			{
				randwall = rand() % 2;
				if (randwall == 0)
				{
					arr[i][j].rwall = 0;
					arr[i][j + 1].num = arr[i][j].num;
				}
			}
		}

		// 중간 줄
		else if (0 < i && i < HEIGHT - 1)
		{
			int flag;
			for (int j = 0; j < WIDTH; j++)
			{
				if (j == 0)
				{
					arr[i - 1][j].dwall = 0;
					arr[i][j].num = arr[i - 1][j].num;
				}
				else
				{
					randwall = rand() % 2;
					if (arr[i - 1][j - 1].num != arr[i - 1][j].num)
						flag = 1;
					else
						flag = 0;
					if (flag == 1)
					{
						arr[i - 1][j].dwall = 0;
						arr[i][j].num = arr[i - 1][j].num;
					}
					else
					{
						if (randwall == 0)
						{
							arr[i - 1][j].dwall = 0;
							arr[i][j].num = arr[i - 1][j].num;
						}
					}
				}
			}
			for (int j = 0; j < WIDTH - 1; j++)
			{
				randwall = rand() % 2;
				if (randwall == 0 && arr[i][j].num != arr[i][j + 1].num)
				{
					arr[i][j].rwall = 0;
					arr[i][j + 1].num = arr[i][j].num;
				}
			}
			for (int j = 0; j < WIDTH; j++)
			{
				if (arr[i - 1][j].dwall == 0)
				{
					if (arr[i - 1][j].num < arr[i][j].num)
						arr[i][j].num = arr[i - 1][j].num;
					else
						arr[i - 1][j].num = arr[i][j].num;
				}
			}
		}

		// 마지막 줄
		else
		{
			int flag;
			for (int j = 0; j < WIDTH; j++)
			{
				if (j == 0)
				{
					arr[i - 1][j].dwall = 0;
					arr[i][j].num = arr[i - 1][j].num;
				}
				else
				{
					randwall = rand() % 2;
					if (arr[i - 1][j - 1].num != arr[i - 1][j].num)
						flag = 1;
					else
						flag = 0;
					if (flag == 1)
					{
						arr[i - 1][j].dwall = 0;
						arr[i][j].num = arr[i - 1][j].num;
					}
					else
					{
						if (randwall == 0)
						{
							arr[i - 1][j].dwall = 0;
							arr[i][j].num = arr[i - 1][j].num;
						}
					}
				}
			}
			for (int j = 0; j < WIDTH - 1; j++)
			{
				if (arr[i][j].rwall == 1 && arr[i][j].num != arr[i][j + 1].num)
				{
					arr[i][j].rwall = 0;
					if (arr[i][j].num < arr[i][j + 1].num)
						arr[i][j + 1].num = arr[i][j].num;
					else
						arr[i][j].num = arr[i][j + 1].num;
				}
			}
		}
	}

	FILE* fp = fopen("maze.maz", "w");
	fprintf(fp, "+");
	for (int i = 0; i < WIDTH; i++)
		fprintf(fp, "-+");
	fprintf(fp, "\n");

	for (int i = 0; i < HEIGHT; i++)
	{
		fprintf(fp, "| ");
		for (int j = 0; j < WIDTH; j++)
		{
			if (arr[i][j].rwall == 1)
				fprintf(fp, "| ");
			else
				fprintf(fp, "  ");
		}
		fprintf(fp, "\n+");
		for (int j = 0; j < WIDTH; j++)
		{
			if (arr[i][j].dwall == 1)
				fprintf(fp, "-+");
			else
				fprintf(fp, " +");
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
	return 0;
}