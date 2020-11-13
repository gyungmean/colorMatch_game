#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>
#define BOARD_SIZE_R 15
#define BOARD_SIZE_C 19

#define LEFT 75
#define RIGHT 77

#define TRUE 1
#define FALSE 0

#define UP 0
#define DOWN 1

typedef struct Position
{
	int x, y;
}Position;

typedef struct Ball
{
	Position pos;
	int color;
}Ball;

typedef struct Shoot
{
	Position pos;
	int color, direction, distance;
	clock_t moveTime;
	clock_t oldTime;
}Shoot;

void gotoxy(int x, int y)
{
	COORD Pos;
	Pos.X = 2 * x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
int inputKey() {//입력
	if (_kbhit()) return _getch();
	return -1;
}
void removeCursor()
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void boardDraw() //테두리 그리기
{
	system("cls");
	int i;
	gotoxy(0, 0);
	for (i = 0; i < BOARD_SIZE_R; i++)
		printf("□");
	for (i = 1; i < BOARD_SIZE_C; i++) {
		gotoxy(0, i);
		printf("□");
		gotoxy(BOARD_SIZE_R - 1, i);
		printf("□");
	}
	gotoxy(0, BOARD_SIZE_C);
	for (i = 0; i < BOARD_SIZE_R; i++)
		printf("□");
}
void showInfo() 
{
	boardDraw();
	gotoxy(2, 3); printf("→ or ← 방향키 : 회전");
	gotoxy(2, 5); printf("같은 색 공 끼리");
	gotoxy(2, 7); printf("충돌 시 : 1점 획득");
	gotoxy(2, 9); printf("다른색 공 충돌시");
	gotoxy(2, 11); printf("게임오버");
	gotoxy(2, 13); printf("'enter' to return");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	gotoxy(2, 17); printf("lastday9.tistory.com");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	while (TRUE) if (_kbhit()) return;
}//도움말
void printBall(Ball b)
{
	if(b.color == 1)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	else if(b.color == 2)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

	gotoxy(b.pos.x, b.pos.y);
	printf("●");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}
void printShoot(Shoot s)
{
	if (s.color == 1)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	else if (s.color == 2)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

	gotoxy(s.pos.x, s.pos.y);
	printf("●");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}
void printScore(int point)
{
	gotoxy(0, 0);
	printf("POINT : %d", point);
}
void moveShoot(Shoot* s)
{
	clock_t CurTime = clock();
	if (CurTime - s->oldTime > s->moveTime) {
		s->oldTime = CurTime;
		if (s->direction == UP) {
			s->pos.y--;
		}
		else if (s->direction == DOWN) {
			s->pos.y++;
		}
	}
}
void initShoot(Shoot* s)
{
	s->direction = rand() % 2;
	s->pos.x = 7;
	if (s->direction == DOWN) {
		s->pos.y = 0;
	}
	else {
		s->pos.y = 19;
	}
	s->moveTime = 100;
	s->oldTime = clock();
	s->distance = 1;
	s->color = rand() % 2 + 1;
}
void gamePlay()
{
	system("cls");
	int key; //방향키
	int point = 0; //점수
	Ball b1 = { 7, 9, 1 }; 
	Ball b2 = { 7, 10, 2 };
	Ball temp; //교환용
	Shoot s; //이동하는 공

	initShoot(&s);
	while (TRUE) {
		system("cls");
		printBall(b1);
		printBall(b2);
		printScore(point);

		moveShoot(&s);
		if (s.pos.y == 9) {//공과 충돌시
			if (b1.color == s.color) {
				point++;
				initShoot(&s);
				continue;
			}
			else break;
		}
		else if (s.pos.y == 10) {
			if (b2.color == s.color) {
				point++;
				initShoot(&s);
				continue;
			}
			else break;
		}
		printShoot(s);
		if (_kbhit()) {
			key = _getch();
			if (key == 224 || key == 0) {
				key = _getch();
				if (key == LEFT || key == RIGHT) {
					temp.color = b1.color;
					b1.color = b2.color;
					b2.color = temp.color;
				}
				else continue;
			}
		}
	}
	system("cls");
	gotoxy(4, 9); printf("GAME OVER");
	gotoxy(4, 10); printf("POINT : %d", point);
	Sleep(3000);
	return;
}
void gameStart()
{
	int time = 3;
	int x = 2;
	boardDraw();
	while (time > 0) {
		gotoxy(x++, 9);
		printf("%d", time--);
		Sleep(1000);
	}
	gotoxy(6, 9); printf("GAME START!");
	Sleep(700);
	gamePlay();
}//게임 시작 전 타이머
int mainMenu() //메인화면
{
	boardDraw();
	gotoxy(2, 5); printf("1.HELP\n");
	gotoxy(2, 7); printf("2.GAME START!\n");
	//gotoxy(2, 9); printf("3.SCORE");
	gotoxy(2, 11); printf("Exit : x\n");
	gotoxy(0, 27);
	while (1) {
		int key = inputKey();
		if (key == '1') return 1;
		else if (key == '2') return 2;
		//else if (key == '3') return 3;
		else if (key == 'x' || key == 'X') return FALSE;
		else continue;
	}
}
int main()
{
	int process;
	srand(time(NULL));
	system("mode con cols=30 lines=20");
	removeCursor();
	gotoxy(0, 0);
	while (TRUE) {
		process = mainMenu();
		if (process == FALSE) break;
		else {
			switch (process) {
			case 1: showInfo(); break;
			case 2: gameStart(); break;
			default: continue;
			}
		}
	}
	gotoxy(30, 30);
}