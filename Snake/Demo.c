//2018.10.29
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>		//光标设置API
#include <time.h>			//随机食物
#include <conio.h>			//捕获按键
#pragma warning (disable : 4996)

#define MAPHEIGHT 25			//地图高度
#define MAPWIDTH 60				//地图宽度
#define SNAKESIZE 50			//蛇身的最大长度

int key = 'w';			//蛇初始的出发方向
int changflag = 0;		//蛇是否变化

//食物的结构体
struct
{
	//食物出现的坐标
	int x;
	int y;
}food;
//蛇的结构体
struct
{
	//蛇身坐标
	int x[SNAKESIZE];
	int y[SNAKESIZE];
	int len;			//蛇的长度
	int speed;			//蛇的速度
}snake;

//生成地图
void drawMap();
//生成食物
void createFood();
//捕获按键
void keyDown();
//蛇的状态
int sankeStatus();
//光标移动
void gotoxy(int x, int y);
//是否反方向
void direction();

int main()
{
	drawMap();
	gotoxy(0, MAPHEIGHT + 1);
	while (1)
	{
		createFood();
		Sleep(snake.speed);
		keyDown();
		if (!sankeStatus())
			break;
	}
	gotoxy(MAPWIDTH / 2, MAPHEIGHT / 2);
	printf("Game Over!");

	getchar();
	return 0;
}

void drawMap()
{
	//画地图
	int i;
	//地图的两边
	for (i = 0; i < MAPHEIGHT; i++)
	{
		gotoxy(0, i);
		printf("■");
		gotoxy(MAPWIDTH - 2, i);
		printf("■");
	}
	//地图的上下边
	for (i = 0; i < MAPWIDTH; i += 2)
	{
		gotoxy(i, 0);
		printf("■");
		gotoxy(i, MAPHEIGHT - 1);
		printf("■");
	}
	//画蛇头
	snake.x[0] = MAPWIDTH / 2;	//蛇头的初始位置
	snake.y[0] = MAPHEIGHT / 2;
	gotoxy(snake.x[0], snake.y[0]);
	printf("■");
	//画蛇身
	snake.len = 3;				//初始化蛇身的长度
	snake.speed = 300;
	int n;
	for (n = 1; n < snake.len; n++)
	{
		snake.x[n] = snake.x[n - 1];
		snake.y[n] = snake.y[n - 1] + 1;
		gotoxy(snake.x[n], snake.y[n]);
		printf("■");
	}

	srand((unsigned int)time(NULL));			//随机食物的种子
	food.x = rand() % (MAPWIDTH - 4) + 2;			//随机食物的坐标，避免生成在墙壁里
	food.y = rand() % (MAPHEIGHT - 2) + 1;
	while (food.x % 2 != 0)
		food.x = rand() % (MAPWIDTH - 4) + 2;
	gotoxy(food.x, food.y);		//设置食物的生成坐标
	printf("□");				//打印食物
}

void createFood()
{
	srand((unsigned int)time(NULL));
	if (snake.x[0] == food.x && snake.y[0] == food.y)
	{
		while (1)
		{
			int flag = 1;
			food.x = rand() % (MAPWIDTH - 4) + 2;
			food.y = rand() % (MAPHEIGHT - 2) + 1;
			int n = 0;
			for (n = 0; n < snake.len; n++)
			{
				//判断食物是否和蛇身重叠
				if (snake.x[n] == food.x && snake.y[n] == food.y)
				{
					flag = 0;
					break;
				}
			}
			//判断坐标食物横坐标是否偶数
			if (flag && food.x % 2 == 0)
			{
				break;
			}
		}
		gotoxy(food.x, food.y);
		printf("□");
		snake.len++;
		changflag = 1;
	}
}
int sankeStatus()
{
	if ((snake.x[0] == 0 || snake.x[0] == MAPWIDTH - 2) || (snake.y[0] == 0 || snake.y[0] == MAPHEIGHT - 1))
	{
		return 0;
	}
	int i = 0;
	for (i = 1; i < snake.len; i++)
	{
		if (snake.x[0] == snake.x[i] && snake.y[0] == snake.y[i])
		{
			return 0;
		}
	}

	return 1;
}

char temp = 0;
void keyDown()
{
	if (_kbhit())
	{
		fflush(stdin);
		key = getch();
		getch();
	}

	if (!changflag)
	{
		gotoxy(snake.x[snake.len - 1], snake.y[snake.len - 1]);
		printf("  ");
	}
	int i = 0;
	for (i = snake.len - 1; i > 0; i--)
	{
		snake.x[i] = snake.x[i - 1];
		snake.y[i] = snake.y[i - 1];
	}
	if (key == 'a')
	{
		if (temp + key == 197)
			key = temp;
	}
	//检查是否反方向
	direction();
	switch (key)
	{
	case 'a':
	case 'A':
		snake.x[0] -= 2;
		break;
	case 'w':
	case 'W':
		snake.y[0] -= 1;
		break;
	case 'd':
	case 'D':
		snake.x[0] += 2;
		break;
	case 's':
	case 'S':
		snake.y[0] += 1;
		break;
	}
	gotoxy(snake.x[0], snake.y[0]);
	printf("■");
	changflag = 0;
	gotoxy(0, 0);
	temp = key;
}
void direction()
{
	if (temp == 'a' || temp == 'A')
		if (key - temp == 3)
			key = temp;
	if (temp == 'd' || temp == 'D')
		if (temp - key == 3)
			key = temp;
	if (temp == 'w' || temp == 'W')
		if (temp - key == 4)
			key = temp;
	if (temp == 's' || temp == 'S')
		if (key - temp == 4)
			key = temp;
}

void gotoxy(int x, int y)
{
	//调用Win32 API设置控制台的光标位置
	COORD coord;			//光标的结构体
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);			//找到控制台窗口
	coord.X = x;			//设置坐标
	coord.Y = y;
	SetConsoleCursorPosition(handle, coord);					//同步控制台
}
