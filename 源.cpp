#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#include<iostream>
#include<algorithm>
using namespace std;
#define BLOCK_SIZE 20
#define HEIGHT 30
#define WIDTH 40

bool start = 0;
int mode = 0;
int Blocks[HEIGHT][WIDTH] = { 0 };
int food_i, food_j;
int isFailure = 0;
char moveDirection;

void menu()
{
	initgraph(800, 600);
	setbkcolor(RGB(0, 0, 0));
	setbkmode(TRANSPARENT);
	settextcolor(RGB(255, 255, 255));
	settextstyle(100, 0, _T("楷体"));
	outtextxy(240, 40, _T("贪吃蛇"));
	settextstyle(30, 0, _T("楷体"));
	outtextxy(310, 160, _T("空格键选模式"));
	settextstyle(60, 0, _T("楷体"));
	outtextxy(280, 250, _T("经典模式"));
	settextstyle(60, 0, _T("楷体"));
	outtextxy(280, 330, _T("道具模式"));
	settextstyle(60, 0, _T("楷体"));
	outtextxy(280, 410, _T("剧情模式"));
}

void choose()
{
	int chs = 1;
	settextstyle(60, 0, _T("楷体"));
	outtextxy(220, 250, _T("→"));
	char judge = '0';
	while (judge != ' ')
	{
		judge = _getch();
		if (judge == 'w' && chs != 1)
		{
			system("cls");
			menu();
			chs--;
		}
		if (judge == 's' && chs != 3)
		{
			system("cls");
			menu();
			chs++;
		}
		if (chs == 1)
		{
			settextstyle(60, 0, _T("楷体"));
			outtextxy(220, 250, _T("→"));
		}
		if (chs == 2)
		{
			settextstyle(60, 0, _T("楷体"));
			outtextxy(220, 330, _T("→"));
		}
		if (chs == 3)
		{
			settextstyle(60, 0, _T("楷体"));
			outtextxy(220, 410, _T("→"));
		}
		if (judge == ' ')
		{
			if (chs == 1)
			{
				mode = 1;
			}
			if (chs == 2)
			{
				mode = 2;
			}
			if (chs == 3)
			{
				mode = 3;
			}
		}
	}
	FlushBatchDraw();
}

void moveSnake()
{
	//遍历二维数组，蛇身部分加1，记录蛇移动前的位置
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (Blocks[i][j] > 0)
				Blocks[i][j] += 1;
		}
	}
	int oldTail_i = 0, oldTail_j = 0, oldHead_i = 0, oldHead_j = 0;
	int max = 0;
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{

			if (max < Blocks[i][j])
			{
				max = Blocks[i][j];
				oldTail_i = i;
				oldTail_j = j;
			}
			if (Blocks[i][j] == 2)
			{
				oldHead_i = i;
				oldHead_j = j;
			}
		}
	}
	//更新蛇的位置
	int newHead_i = oldHead_i;
	int newHead_j = oldHead_j;
	if (moveDirection == 'w')
		newHead_i -= 1;
	else if (moveDirection == 's')
		newHead_i += 1;
	else if (moveDirection == 'a')
		newHead_j -= 1;
	else if (moveDirection == 'd')
		newHead_j += 1;


	//实现从墙壁另一端穿出，并且限制蛇的移动
	if (newHead_i >= HEIGHT)
	{
		newHead_i = 0;
	}
	else if (newHead_i < 0)
	{
		newHead_i = HEIGHT - 1;
	}
	else if (newHead_j >= WIDTH)
		newHead_j = 0;
	else if (newHead_j < 0)
		newHead_j = WIDTH - 1;

	if (Blocks[newHead_i][newHead_j] > 0)
	{
		isFailure = 1;
		return;
	}

	//更新蛇头的位置 要在判断蛇头位置是否正确之后
	Blocks[newHead_i][newHead_j] = 1;

	//吃饭
	if (newHead_i == food_i && newHead_j == food_j)
	{
		food_i = rand() % (HEIGHT - 5) + 2;
		food_j = rand() % (WIDTH - 5) + 2;
	}
	else
	{
		Blocks[oldTail_i][oldTail_j] = 0;
	}
}

void startup()
{
	int i;
	Blocks[HEIGHT / 2][WIDTH / 2] = 1;
	for (i = 1; i <= 4; i++)
		Blocks[HEIGHT / 2][WIDTH / 2 - i] = i + 1;
	moveDirection = 'd';
	food_i = rand() % (HEIGHT - 5) + 2;
	food_j = rand() % (WIDTH - 5) + 2;
	initgraph(WIDTH * BLOCK_SIZE, HEIGHT * BLOCK_SIZE);
	setlinecolor(RGB(236, 236, 236));
	BeginBatchDraw();
}

void show()
{
	cleardevice();
	int i, j;
	for (i = 0; i < HEIGHT; i++)
	{
		for (j = 0; j < WIDTH; j++)
		{
			if (Blocks[i][j] > 0)
				setfillcolor(HSVtoRGB(Blocks[i][j] * 10, 0.9, 1));
			else setfillcolor(WHITE);
			fillrectangle(j * BLOCK_SIZE, i * BLOCK_SIZE, (j + 1) * BLOCK_SIZE, (i + 1) * BLOCK_SIZE);
		}
	}
	setfillcolor(RED);
	fillrectangle(food_j * BLOCK_SIZE, food_i * BLOCK_SIZE, (food_j + 1) * BLOCK_SIZE, (food_i + 1) * BLOCK_SIZE);
	if (isFailure)
	{
		setbkmode(TRANSPARENT);
		settextcolor(RGB(255, 0, 0));
		settextstyle(80, 0, _T("楷体"));
		outtextxy(240, 220, _T("游戏失败"));
	}
	FlushBatchDraw();
}

void updateWithoutInput()
{
	if (isFailure)
		return;
	static int waitIndex = 1;
	waitIndex++;
	if (waitIndex == 7)
	{
		moveSnake();
		waitIndex = 1;
	}
}

/*事实上，移动蛇的函数只有movewithoutinput，
movewithinput的作用是得到用户输入以调用move函数，来更新蛇的位置
*/

void updateWithInput()
{
	if (_kbhit() && isFailure == 0)
	{
		char input = _getch();
		char orimoveDirection = moveDirection;

		if (input == 'a' || input == 's' || input == 'd' || input == 'w')
		{
			moveDirection = input;
			if (orimoveDirection == 'a' && moveDirection == 'd')
				moveDirection = orimoveDirection;
			if (orimoveDirection == 'w' && moveDirection == 's')
				moveDirection = orimoveDirection;
			if (orimoveDirection == 'd' && moveDirection == 'a')
				moveDirection = orimoveDirection;
			if (orimoveDirection == 's' && moveDirection == 'w')
				moveDirection = orimoveDirection;
			if (moveDirection != orimoveDirection)
				moveSnake();

		}
	}
}

int main() //  主函数
{
	menu();
	choose();
	if (mode == 1)
	{
		startup();  // 初始化函数，仅执行一次	
		while (1)   // 一直循环
		{
			show();  // 进行绘制
			updateWithoutInput(); // 和输入无关的更新 
			updateWithInput();    // 和输入有关的更新
		}
		return 0;
	}
}
