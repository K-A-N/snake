//2018.10.29
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>		//�������API
#include <time.h>			//���ʳ��
#include <conio.h>			//���񰴼�
#pragma warning (disable : 4996)

#define MAPHEIGHT 25			//��ͼ�߶�
#define MAPWIDTH 60				//��ͼ���
#define SNAKESIZE 50			//�������󳤶�

int key = 'w';			//�߳�ʼ�ĳ�������
int changflag = 0;		//���Ƿ�仯

//ʳ��Ľṹ��
struct
{
	//ʳ����ֵ�����
	int x;
	int y;
}food;
//�ߵĽṹ��
struct
{
	//��������
	int x[SNAKESIZE];
	int y[SNAKESIZE];
	int len;			//�ߵĳ���
	int speed;			//�ߵ��ٶ�
}snake;

//���ɵ�ͼ
void drawMap();
//����ʳ��
void createFood();
//���񰴼�
void keyDown();
//�ߵ�״̬
int sankeStatus();
//����ƶ�
void gotoxy(int x, int y);
//�Ƿ񷴷���
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
	//����ͼ
	int i;
	//��ͼ������
	for (i = 0; i < MAPHEIGHT; i++)
	{
		gotoxy(0, i);
		printf("��");
		gotoxy(MAPWIDTH - 2, i);
		printf("��");
	}
	//��ͼ�����±�
	for (i = 0; i < MAPWIDTH; i += 2)
	{
		gotoxy(i, 0);
		printf("��");
		gotoxy(i, MAPHEIGHT - 1);
		printf("��");
	}
	//����ͷ
	snake.x[0] = MAPWIDTH / 2;	//��ͷ�ĳ�ʼλ��
	snake.y[0] = MAPHEIGHT / 2;
	gotoxy(snake.x[0], snake.y[0]);
	printf("��");
	//������
	snake.len = 3;				//��ʼ������ĳ���
	snake.speed = 300;
	int n;
	for (n = 1; n < snake.len; n++)
	{
		snake.x[n] = snake.x[n - 1];
		snake.y[n] = snake.y[n - 1] + 1;
		gotoxy(snake.x[n], snake.y[n]);
		printf("��");
	}

	srand((unsigned int)time(NULL));			//���ʳ�������
	food.x = rand() % (MAPWIDTH - 4) + 2;			//���ʳ������꣬����������ǽ����
	food.y = rand() % (MAPHEIGHT - 2) + 1;
	while (food.x % 2 != 0)
		food.x = rand() % (MAPWIDTH - 4) + 2;
	gotoxy(food.x, food.y);		//����ʳ�����������
	printf("��");				//��ӡʳ��
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
				//�ж�ʳ���Ƿ�������ص�
				if (snake.x[n] == food.x && snake.y[n] == food.y)
				{
					flag = 0;
					break;
				}
			}
			//�ж�����ʳ��������Ƿ�ż��
			if (flag && food.x % 2 == 0)
			{
				break;
			}
		}
		gotoxy(food.x, food.y);
		printf("��");
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
	//����Ƿ񷴷���
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
	printf("��");
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
	//����Win32 API���ÿ���̨�Ĺ��λ��
	COORD coord;			//���Ľṹ��
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);			//�ҵ�����̨����
	coord.X = x;			//��������
	coord.Y = y;
	SetConsoleCursorPosition(handle, coord);					//ͬ������̨
}
