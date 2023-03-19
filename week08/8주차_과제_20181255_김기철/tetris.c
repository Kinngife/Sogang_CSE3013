#include "tetris.h"

static struct sigaction act, oact;

int main() {
	int exit = 0;
	initscr();
	noecho();
	keypad(stdscr, TRUE);

	srand((unsigned int)time(NULL));

	createRankList();

	while (!exit) {
		clear();
		switch (menu()) {
		case MENU_PLAY: play(); break;
		case MENU_RANK: rank(); break;
		case MENU_REC_PLAY: recommendedPlay(); break;
		case MENU_EXIT: exit = 1; break;
		default: break;
		}
	}

	endwin();
	system("clear");
	return 0;
}

void InitTetris() {
	int i, j;

	for (j = 0; j < HEIGHT; j++)
		for (i = 0; i < WIDTH; i++)
			field[j][i] = 0;

	nextBlock[0] = rand() % 7;
	nextBlock[1] = rand() % 7;
	nextBlock[2] = rand() % 7;
	blockRotate = 0;
	blockY = -1;
	blockX = WIDTH / 2 - 2;
	score = 0;
	gameOver = 0;
	timed_out = 0;
	recRoot = malloc(sizeof(RecNode));
	recRoot->level = 0, recRoot->accumulatedScore = 0;
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
			recRoot->recField[i][j] = field[i][j];
	}

	DrawOutline();
	DrawField();
	DrawBlock(blockY, blockX, nextBlock[0], blockRotate, ' ');
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline() {
	int i, j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0, 0, HEIGHT, WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2, WIDTH + 10);
	printw("NEXT BLOCK");
	DrawBox(3, WIDTH + 10, 4, 8);
	DrawBox(9, WIDTH + 10, 4, 8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(15, WIDTH + 10);
	printw("SCORE");
	DrawBox(16, WIDTH + 10, 1, 8);
}

int GetCommand() {
	int command;
	command = wgetch(stdscr);
	switch (command) {
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command) {
	int ret = 1;
	int drawFlag = 0;
	switch (command) {
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if ((drawFlag = CheckToMove(field, nextBlock[0], (blockRotate + 1) % 4, blockY, blockX)))
			blockRotate = (blockRotate + 1) % 4;
		break;
	case KEY_DOWN:
		if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX + 1)))
			blockX++;
		break;
	case KEY_LEFT:
		if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX - 1)))
			blockX--;
		break;
	default:
		break;
	}
	if (drawFlag) DrawChange(field, command, nextBlock[0], blockRotate, blockY, blockX);
	return ret;
}

void DrawField() {
	int i, j;
	for (j = 0; j < HEIGHT; j++) {
		move(j + 1, 1);
		for (i = 0; i < WIDTH; i++) {
			if (field[j][i] == 1) {
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score) {
	move(17, WIDTH + 11);
	printw("%8d", score);
}

void DrawNextBlock(int* nextBlock) {
	int i, j;
	for (i = 0; i < 4; i++) {
		move(4 + i, WIDTH + 13);
		for (j = 0; j < 4; j++) {
			if (block[nextBlock[1]][0][i][j] == 1) {
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
	for (i = 0; i < 4; i++) {
		move(10 + i, WIDTH + 13);
		for (j = 0; j < 4; j++) {
			if (block[nextBlock[2]][0][i][j] == 1) {
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID, int blockRotate, char tile) {
	int i, j;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++) {
			if (block[blockID][blockRotate][i][j] == 1 && i + y >= 0) {
				move(i + y + 1, j + x + 1);
				attron(A_REVERSE);
				printw("%c", tile);
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT, WIDTH + 10);
}

void DrawBox(int y, int x, int height, int width) {
	int i, j;
	move(y, x);
	addch(ACS_ULCORNER);
	for (i = 0; i < width; i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for (j = 0; j < height; j++) {
		move(y + j + 1, x);
		addch(ACS_VLINE);
		move(y + j + 1, x + width + 1);
		addch(ACS_VLINE);
	}
	move(y + j + 1, x);
	addch(ACS_LLCORNER);
	for (i = 0; i < width; i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play() {
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM, &act, &oact);
	InitTetris();
	recommend(recRoot);
	do {
		if (timed_out == 0) {
			alarm(1);
			timed_out = 1;
		}

		command = GetCommand();
		if (ProcessCommand(command) == QUIT) {
			alarm(0);
			DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
			move(HEIGHT / 2, WIDTH / 2 - 4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	} while (!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
	move(HEIGHT / 2, WIDTH / 2 - 4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu() {
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX) {
	// user code
	for (int i = 0; i < BLOCK_HEIGHT; i++)
	{
		for (int j = 0; j < BLOCK_WIDTH; j++)
		{
			if (block[currentBlock][blockRotate][i][j])
			{
				int Y = i + blockY, X = j + blockX;
				if (f[Y][X])
					return 0;
				if (Y >= HEIGHT || X < 0 || X >= WIDTH)
					return 0;
			}
		}
	}
	return 1;
}

void DrawChange(char f[HEIGHT][WIDTH], int command, int currentBlock, int blockRotate, int blockY, int blockX) {
	// user code

	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	int prevR = blockRotate, prevY = blockY, prevX = blockX;
	switch (command)
	{
	case KEY_UP:
		prevR--;
		if (prevR == -1)
			prevR = 3;
		break;
	case KEY_DOWN:
		prevY--;
		break;
	case KEY_RIGHT:
		prevX--;
		break;
	case KEY_LEFT:
		prevX++;
		break;
	default:
		break;
	}
	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	for (int i = 0; i < BLOCK_HEIGHT; i++)
	{
		for (int j = 0; j < BLOCK_WIDTH; j++)
		{
			if (block[currentBlock][prevR][i][j] == 1 && i + prevY >= 0) {
				move(i + prevY + 1, j + prevX + 1);
				printw(".");
			}
		}
	}
	// 이전 그림자 블록 지우기
	int shadowY = MoveShadow(prevY, prevX, currentBlock, prevR);
	for (int i = 0; i < BLOCK_HEIGHT; i++)
	{
		for (int j = 0; j < BLOCK_WIDTH; j++)
		{
			if (block[currentBlock][prevR][i][j] == 1 && i + shadowY >= 0) {
				move(i + shadowY + 1, j + prevX + 1);
				printw(".");
			}
		}
	}
	//3. 새로운 블록 정보를 그린다. 
	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
	move(HEIGHT, WIDTH + 10);
}

void BlockDown(int sig) {
	// user code
	//강의자료 p26-27의 플로우차트를 참고한다.
	if (CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX))
	{
		blockY++;
		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
	}
	else
	{
		if (blockY == -1)
			gameOver = 1;
		score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
		score += DeleteLine(field);
		nextBlock[0] = nextBlock[1], nextBlock[1] = nextBlock[2], nextBlock[2] = rand() % 7;
		blockRotate = 0, blockY = -1, blockX = WIDTH / 2 - 2;
		for (int i = 0; i < HEIGHT; i++)
		{
			for (int j = 0; j < WIDTH; j++)
				recRoot->recField[i][j] = field[i][j];
		}
		recommend(recRoot);
		DrawNextBlock(nextBlock);
		PrintScore(score);
		DrawField();
	}
	timed_out = 0;
}

int AddBlockToField(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX) {
	// user code

	int cnt = 0;
	//Block이 추가된 영역의 필드값을 바꾼다.
	for (int i = 0; i < BLOCK_HEIGHT; i++)
	{
		for (int j = 0; j < BLOCK_WIDTH; j++)
		{
			int Y = i + blockY, X = j + blockX;
			if (block[currentBlock][blockRotate][i][j])
			{
				f[Y][X] = 1;
				if (f[Y + 1][X] || Y == HEIGHT - 1)
					cnt++;
			}
		}
	}
	return cnt * 10;
}

int DeleteLine(char f[HEIGHT][WIDTH]) {
	// user code

	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	int cnt = 0;
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (!f[i][j])
				break;
			else
			{
				//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
				if (j == WIDTH - 1)
				{
					cnt++;
					for (int l = i; l > 0; l--)
					{
						for (int k = 0; k <= j; k++)
							f[l][k] = f[l - 1][k];
					}
				}
			}

		}
	}
	return cnt * cnt * 100;
}

///////////////////////////////////////////////////////////////////////////

int MoveShadow(int y, int x, int blockID, int blockRotate) {
	while (CheckToMove(field, blockID, blockRotate, y + 1, x))
		y++;
	return y;
}

void DrawShadow(int y, int x, int blockID, int blockRotate) {
	// user code
	int shadowY = MoveShadow(y, x, blockID, blockRotate);
	DrawBlock(shadowY, x, blockID, blockRotate, '/');
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate) {
	DrawBlock(y, x, blockID, blockRotate, ' ');
	DrawShadow(y, x, blockID, blockRotate);
	DrawRecommend(recommendY, recommendX, blockID, recommendR);
}

void createRankList() {
	// 목적: Input파일인 "rank.txt"에서 랭킹 정보를 읽어들임, 읽어들인 정보로 랭킹 목록 생성
	// 1. "rank.txt"열기
	// 2. 파일에서 랭킹정보 읽어오기
	// 3. LinkedList로 저장
	// 4. 파일 닫기
	FILE* fp;
	head = malloc(sizeof(Node));
	head->prev = NULL, head->next = NULL;
	rear = malloc(sizeof(Node));
	rear->prev = NULL, rear->next = NULL;
	Node* curnode = head;

	//1. 파일 열기
	fp = fopen("rank.txt", "r");

	// 2. 정보읽어오기 및 3. 리스트에 저장
	if (fscanf(fp,"%d",&score_number) != EOF)
	{
		for (int i = 0; i < score_number; i++)
		{
			Node* newnode = (Node*)malloc(sizeof(Node));
			fscanf(fp, "%s %d", newnode->name, &(newnode->score));
			newnode->prev = curnode;
			newnode->next = rear;
			rear->prev = newnode;
			curnode->next = newnode;
			curnode = newnode;
		}
	}

	// 4. 파일닫기
	fclose(fp);
}

void rank() {
	//목적: rank 메뉴를 출력하고 점수 순으로 X부터~Y까지 출력함
	//1. 문자열 초기화
	int X = 1, Y = score_number, ch;
	Node* curnode = head;
	clear();

	//2. printw()로 3개의 메뉴출력
	printw("1. list ranks from X to Y\n");
	printw("2. list ranks by a specific name\n");
	printw("3. delete a specific rank\n");

	//3. wgetch()를 사용하여 변수 ch에 입력받은 메뉴번호 저장
	ch = wgetch(stdscr);

	//4. 각 메뉴에 따라 입력받을 값을 변수에 저장
	//4-1. 메뉴1: X, Y를 입력받고 적절한 input인지 확인 후(X<=Y), X와 Y사이의 rank 출력
	if (ch == '1')
	{
		printw("X: ");
		echo();
		scanw("%d", &X);
		printw("Y: ");
		scanw("%d", &Y);
		noecho();

		printw("       name       |    score   \n");
		printw("-------------------------------\n");
		if (X > Y)
			printw("search failure: no rank in the list\n");
		else
		{
			for (int i = 1; i <= Y; i++)
			{
				curnode = curnode->next;
				if (curnode == rear)
					break;
				if (i >= X)
					printw(" %-17s| %-10d\n", curnode->name, curnode->score);
			}
		}
	}

	//4-2. 메뉴2: 문자열을 받아 저장된 이름과 비교하고 이름에 해당하는 리스트를 출력
	else if (ch == '2')
	{
		char str[NAMELEN + 1];
		int check = 0;
		curnode = head;
		printw("input the name: ");
		echo();
		scanw("%s", str);
		noecho();

		printw("       name       |    score   \n");
		printw("-------------------------------\n");
		for (int i = 0; i < score_number; i++)
		{
			curnode = curnode->next;
			if (!strcmp(str, curnode->name))
			{
				printw(" %-17s| %-10d\n", curnode->name, curnode->score);
				check = 1;
			}
		}
		if (check == 0)
			printw("search failure: no name in the list\n");
	}

	//4-3. 메뉴3: rank번호를 입력받아 리스트에서 삭제
	else if (ch == '3')
	{
		int num;
		Node* delnode;
		curnode = head;
		printw("input the rank: ");
		echo();
		scanw("%d", &num);
		noecho();

		for (int i = 1; i <= score_number; i++)
		{
			if (num > score_number)
			{
				printw("\nsearch failure: the rank not in the list\n");
				break;
			}

			curnode = curnode->next;
			if (i == num)
			{
				delnode = curnode;
				delnode->prev->next = delnode->next;
				delnode->next->prev=delnode->prev;
				free(delnode);
				score_number--;
				printw("\nresult: the rank deleted\n");
			}
		}
	writeRankFile();
	}
	getch();
}

void writeRankFile() {
	// 목적: 추가된 랭킹 정보가 있으면 새로운 정보를 "rank.txt"에 쓰고 없으면 종료
	//1. "rank.txt" 연다
	FILE* fp = fopen("rank.txt", "w");
	Node* curnode = head;

	//2. 랭킹 정보들의 수를 "rank.txt"에 기록
	fprintf(fp, "%d\n", score_number);

	//3. 탐색할 노드가 더 있는지 체크하고 있으면 다음 노드로 이동, 없으면 종료
	for (int i = 0; i < score_number; i++)
	{
		curnode = curnode->next;
		fprintf(fp, "%s %d\n", curnode->name, curnode->score);
	}
	fclose(fp);
}

void newRank(int score) {
	// 목적: GameOver시 호출되어 사용자 이름을 입력받고 score와 함께 리스트의 적절한 위치에 저장
	//1. 사용자 이름을 입력받음
	Node* curnode = head;
	clear();
	Node* newnode = (Node*)malloc(sizeof(Node));
	printw("your name: ");
	echo();
	scanw("%s", newnode->name);
	noecho();
	newnode->score = score;

	//2. 새로운 노드를 리스트에 연결
	for (int i = 0; i <= score_number; i++)
	{
		curnode = curnode->next;
		if (score > curnode->score || curnode == rear)
		{
			newnode->prev = curnode->prev;
			newnode->next = curnode;
			curnode->prev->next = newnode;
			curnode->prev = newnode;
			score_number++;
			break;
		}
	}
	writeRankFile();
}

void DrawRecommend(int y, int x, int blockID, int blockRotate) {
	// user code
	DrawBlock(y, x, blockID, blockRotate, 'R');
}

int recommend(RecNode* root) {
	int recScore, maxScore = 0;
	int R, X, Y, idx = 0;
	for (R = 0; R < NUM_OF_ROTATE; R++) 
	{
		if ((nextBlock[root->level] == 0 || nextBlock[root->level] >= 5) && R >= 2)
			continue;
		if (nextBlock[root->level] == 4 && R >= 1)
			continue;

		for (X = -1; X < WIDTH; X++)
		{
			Y = 0;
			if (CheckToMove(root->recField, nextBlock[root->level], R, Y, X))
			{
				RecNode* cnode = (RecNode*)malloc(sizeof(RecNode));
				cnode->level = (root->level) + 1;
				cnode->accumulatedScore = 0;
				for (int i = 0; i < HEIGHT; i++)
				{
					for (int j = 0; j < WIDTH; j++)
						cnode->recField[i][j] = root->recField[i][j];
				}
				while (CheckToMove(cnode->recField, nextBlock[root->level], R, Y + 1, X))
					Y++;
				cnode->accumulatedScore += AddBlockToField(cnode->recField, nextBlock[root->level], R, Y, X);
				cnode->accumulatedScore += DeleteLine(cnode->recField);
				recScore = cnode->accumulatedScore;
				root->child[idx++] = cnode;

				if (cnode->level != BLOCK_NUM)
					recScore += recommend(cnode);

				if (maxScore < recScore)
				{
					maxScore = recScore;
					if (root->level == 0)
					{
						recommendR = R;
						recommendY = Y;
						recommendX = X;
					}
				}
			}
		}
	}
	return maxScore;
}

void recommendedPlay() {
	// user code
	time_t start, stop;
	double duration;
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM, &act, &oact);
	start = time(NULL);
	InitTetris();
	recommend(recRoot);
	do {
		if (timed_out == 0) {
			alarm(1);
			timed_out = 1;
		}
		command = GetCommand();
		if (ProcessCommand(command) == QUIT) {
			alarm(0);
			DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
			move(HEIGHT / 2, WIDTH / 2 - 4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}

		ProcessCommand(KEY_DOWN);
		for (int i = 0; i < NUM_OF_ROTATE; i++) 
		{
			if (recommendR != blockRotate)
				ProcessCommand(KEY_UP);
			else 
				break;
		}
		for (int i = 0; i < HEIGHT; i++) 
		{
			if (recommendX < blockX)
				ProcessCommand(KEY_LEFT);
			else if (recommendX > blockX)
				ProcessCommand(KEY_RIGHT);
			else
				break;
		}

		// FALL
		while(CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX))
			blockY++;
		blockY--;

	} while (!gameOver);
	stop = time(NULL);
	duration = (double)difftime(stop, start);

	alarm(0);
	getch();
	DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
	move(HEIGHT / 2, WIDTH / 2 - 4);
	printw("GameOver!!");
	move(20, WIDTH + 11);
	printw("Play Time : %lf seconds", duration);
	refresh();
	getch();
	newRank(score);
}
