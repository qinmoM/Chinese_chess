#define _CRT_SECURE_NO_WARNINGS

#include <graphics.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 70//格子大小
#define ROW 10//行
#define COL 9//列
#define RIVER L"楚河        汉界"//方便后续打印
#define LCOLOR RGB(250, 235, 195)//亮色
#define COLOR RGB(225, 190, 150)//整体颜色
#define ROU 30//棋子半径大小
#define FONT 36//棋子字体大小
#define TAB_W 400//选项卡宽
#define TAB_H 90//选项卡高
#define TURN 100//数组最大步数

struct Chess
{
	wchar_t name[8];//名字
	int x;//x坐标
	int y;//y坐标
	bool riv;//是否过河
	bool type;//黑红方(0黑方,1红方),注意:原值为0,所以使用之前需要先判断是否存在
	bool sur;//是否存活
	bool exist;//是否有棋子
}map[ROW][COL];

struct Chess map_turn[TURN][ROW][COL] = { 0 };

MOUSEMSG msg;

POINT begin = { -1, -1 }, end = { -1, -1 };

short Begin[] = { -1, -1 }, End[] = { -1, -1 };

bool routype = 1;//记录回合是红方还是黑方
bool rouend = 1;//记录对局是否结束
short during = 0;//鼠标判断逻辑,0主界面,1游戏中,2查看历史
short turn = 1;//记录回合数的变量

const wchar_t* redlist[7] = { L"車", L"馬", L"相", L"仕", L"帅", L"炮", L"兵" };//红棋名称
const wchar_t* blacklist[7] = { L"車", L"馬", L"象", L"士", L"将", L"炮", L"卒" };//黑棋名称
const wchar_t* tab[4] = { L"新游戏", L"继续游戏", L"历史战绩", L"退出" };//选项卡名称

void Init()//初始化样式、颜色等
{
	setbkcolor(COLOR);//设置背景颜色
	setfillcolor(COLOR);//填充颜色
	setlinecolor(BLACK);//线条颜色
	setlinestyle(PS_SOLID, 2);//线条样式
}

void GameInit()//初始化棋子等
{
	int i = 0, j = 0;
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			//初始时都没有过河
			map[i][j].riv = 0;
			//每个点赋值其坐标
			map[i][j].x = (j + 1) * SIZE, map[i][j].y = (i + 1) * SIZE;
			//写入名字,没有为空格
			if (i < ROW / 2)
			{
				if (!i)//先判断第一排
				{
					if (j <= 4)
					{
						wcscpy(map[i][j].name, blacklist[j]);
					}
					else
					{
						wcscpy(map[i][j].name, blacklist[COL - j - 1]);
					}
				}
				else if (2 == i && (1 == j || 7 == j))//判断炮
				{
					wcscpy(map[i][j].name, blacklist[5]);
				}
				else if (3 == i && j % 2 == 0)//判断卒
				{
					wcscpy(map[i][j].name, blacklist[6]);
				}
				else//都不是赋值空格
				{
					wcscpy(map[i][j].name, L" ");
				}
			}
			else
			{
				if (9 == i)//先判断第一排
				{
					if (j <= 4)
					{
						wcscpy(map[i][j].name, redlist[j]);
					}
					else
					{
						wcscpy(map[i][j].name, redlist[COL - j - 1]);
					}
				}
				else if (7 == i && (1 == j || 7 == j))//判断炮
				{
					wcscpy(map[i][j].name, redlist[5]);
				}
				else if (6 == i && j % 2 == 0)//判断兵
				{
					wcscpy(map[i][j].name, redlist[6]);
				}
				else//都不是赋值空格
				{
					wcscpy(map[i][j].name, L" ");
				}
			}
			//根据名字赋值是否存在
			if (wcscmp(map[i][j].name, L" "))
			{
				map[i][j].exist = 1;
			}
			else
			{
				map[i][j].exist = 0;
			}
			//根据是否存在赋值是否存活和黑红棋
			if (map[i][j].exist)
			{
				map[i][j].sur = 1;//有棋子的地方标作存活
				if (i < (ROW / 2) - 1)
				{
					map[i][j].type = 0;//有棋子的地方,且在棋盘上半为黑棋
				}
				else
				{
					map[i][j].type = 1;//有棋子的地方,且在棋盘下半为红棋
				}
			}
		}
	}
}

void GameUpdate(int t)//把改变后的t组写到t + 1组
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			map_turn[t + 1][i][j] = map_turn[t][i][j];
		}
	}
}

void MainMenu()//画主菜单
{
	cleardevice();//刷新背景
	for (int i = 0; i < 4; i++)//绘制选项卡
	{
		roundrect((getwidth() - TAB_W) / 2, getheight() * (i + 1) * 2 / 11, (getwidth() + TAB_W) / 2,
			getheight() * (i + 1) * 2 / 11 + TAB_H, 30, 60);//参数4可以是:getheight() * ((i + 1) * 2 + 1) / 11
	}
	for (int i = 0; i < 4; i++)//打印文字
	{
		settextcolor(BLACK);
		settextstyle(40, 0, L"宋体");
		outtextxy((getwidth() - textwidth(tab[i])) / 2,
			getheight() * (i + 1) * 2 / 11 + (TAB_H - textheight(tab[i])) / 2, tab[i]);
	}
	settextstyle(FONT, 0, L"楷体");
}

void BoardDraw()//画棋盘
{
	int i = 0;
	cleardevice();//刷新背景
	for (i = 0; i < ROW; i++)//画横线
	{
		line(SIZE, (i + 1) * SIZE, getwidth() - SIZE, (i + 1) * SIZE);
	}
	for (i = 0; i < COL; i++)//画竖线
	{
		line((i + 1) * SIZE, SIZE, (i + 1) * SIZE, getheight() - SIZE);
	}
	rectangle(SIZE - 5, SIZE - 5, getwidth() - SIZE + 5, getheight() - SIZE + 5);//画外边框
	fillrectangle(SIZE, (ROW / 2) * SIZE, getwidth() - SIZE, (ROW / 2 + 1) * SIZE);//覆盖中间的竖线
	line(4 * SIZE, SIZE, 6 * SIZE, 3 * SIZE);
	line(4 * SIZE, 3 * SIZE, 6 * SIZE, SIZE);
	line(4 * SIZE, 10 * SIZE, 6 * SIZE, 8 * SIZE);
	line(4 * SIZE, 8 * SIZE, 6 * SIZE, 10 * SIZE);//以上四条画棋盘上的斜线
	settextcolor(BLACK);//字体颜色设置
	settextstyle(50, 0, L"楷体");//用于楚河汉界的字体设置
	outtextxy((getwidth() - textwidth(RIVER)) / 2,
		(getheight() - textheight(RIVER)) / 2, RIVER);//棋盘中间加上楚河汉界
	settextstyle(FONT, 0, L"楷体");//用于棋子的字体设置
}

void PiecesDraw()//画棋子
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (map_turn[turn][i][j].sur)
			{
				fillcircle(SIZE * (j + 1), SIZE * (i + 1), ROU);//棋子的背景
				if (map_turn[turn][i][j].type)
				{
					settextcolor(RED);//字体颜色设置
					outtextxy(SIZE * (j + 1) - FONT / 2,
						SIZE * (i + 1) - FONT / 2, map_turn[turn][i][j].name);//棋子的字
				}
				else
				{
					settextcolor(BLACK);//字体颜色设置
					outtextxy(SIZE * (j + 1) - FONT / 2,
						SIZE * (i + 1) - FONT / 2, map_turn[turn][i][j].name);//棋子的字
				}
			}
		}
	}
}

bool judge(short x, short y, int i, int j)
{
	i = SIZE * (i + 1);
	j = SIZE * (j + 1);
	if ((i - y) * (i - y) + (j - x) * (j - x) <= ROU * ROU)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool decide(short* Begin, short* End)//判断两点是否与类型匹配
{
	int i = 0;
	short by = Begin[0], bx = Begin[1], ey = End[0], ex = End[1];
	if (!wcscmp(map_turn[turn][by][bx].name, L"車"))
	{
		if (bx == ex && !(by == ey && bx == ex))//先判断是否在竖直方向并且不在原点
		{
			for (by > ey ? i = 1 : i = -1; by > ey ? i < by - ey : i > by - ey;
				by > ey ? i++ : i--)//判断是否两点间是否有阻挡
			{
				if (map_turn[turn][by - i][bx].sur)
				{
					return 0;
				}
			}
			return 1;
		}
		if (by == ey && !(by == ey && bx == ex))//先判断是否在水平方向并且不在原点
		{
			for (bx > ex ? i = 1 : i = -1; bx > ex ? i < bx - ex : i > bx - ex;
				bx > ex ? i++ : i--)//判断是否两点间是否有阻挡
			{
				if (map_turn[turn][by][bx - i].sur)
				{
					return 0;
				}
			}
			return 1;
		}
		return 0;
	}
	else if (!wcscmp(map_turn[turn][by][bx].name, L"馬"))
	{
		if (ex < 0 || ex > 8 || ey < 0 || ey > 9)//判断范围
		{
			return 0;
		}
		if (ex == bx - 2 && (ey == by - 1 || ey == by + 1))//分别判断行棋是否为日字:判断左
		{
			if (!map_turn[turn][by][bx - 1].sur)//判断是否没有阻挡
			{
				return 1;
			}
		}
		else if (ex == bx + 2 && (ey == by - 1 || ey == by + 1))//分别判断行棋是否为日字:判断右
		{
			if (!map_turn[turn][by][bx + 1].sur)//判断是否没有阻挡
			{
				return 1;
			}
		}
		else if (ey == by - 2 && (ex == bx - 1 || ex == bx + 1))//分别判断行棋是否为日字:判断上
		{
			if (!map_turn[turn][by - 1][bx].sur)//判断是否没有阻挡
			{
				return 1;
			}
		}
		else if (ey == by + 2 && (ex == bx - 1 || ex == bx + 1))//分别判断行棋是否为日字:判断下
		{
			if (!map_turn[turn][by + 1][bx].sur)//判断是否没有阻挡
			{
				return 1;
			}
		}
		return 0;
	}
	else if (!wcscmp(map_turn[turn][by][bx].name, L"炮"))
	{
		if (map_turn[turn][ey][ex].sur)//判断是否吃子
		{
			int j = 0;
			if (bx == ex && !(by == ey && bx == ex))//先判断是否在竖直方向并且不在原点
			{
				for (by > ey ? i = 1 : i = -1; by > ey ? i < by - ey : i > by - ey;
					by > ey ? i++ : i--)//判断是否两点间是否有多少阻挡
				{
					if (map_turn[turn][by - i][bx].sur)
					{
						j++;
					}
				}
				if (1 == j)
				{
					return 1;
				}
			}
			if (by == ey && !(by == ey && bx == ex))//先判断是否在水平方向并且不在原点
			{
				for (bx > ex ? i = 1 : i = -1; bx > ex ? i < bx - ex : i > bx - ex;
					bx > ex ? i++ : i--)//判断是否两点间是否有多少阻挡
				{
					if (map_turn[turn][by][bx - i].sur)
					{
						j++;
					}
				}
				if (1 == j)
				{
					return 1;
				}
			}
			return 0;
		}
		else//未吃子
		{
			if (bx == ex && !(by == ey && bx == ex))//先判断是否在竖直方向并且不在原点
			{
				for (by > ey ? i = 1 : i = -1; by > ey ? i < by - ey : i > by - ey;
					by > ey ? i++ : i--)//判断是否两点间是否有阻挡
				{
					if (map_turn[turn][by - i][bx].sur)
					{
						return 0;
					}
				}
				return 1;
			}
			if (by == ey && !(by == ey && bx == ex))//先判断是否在水平方向并且不在原点
			{
				for (bx > ex ? i = 1 : i = -1; bx > ex ? i < bx - ex : i > bx - ex;
					bx > ex ? i++ : i--)//判断是否两点间是否有阻挡
				{
					if (map_turn[turn][by][bx - i].sur)
					{
						return 0;
					}
				}
				return 1;
			}
			return 0;
		}
	}
	else if (!wcscmp(map_turn[turn][by][bx].name, L"相"))
	{
		if ((ex < 0 || ex > 8) && (ey < 5 || ey > 9))//判断落棋位置是否不在本领土内
		{
			return 0;
		}
		if (ex % 2 || ex + ey != 7 && ex + ey != 11 && ex + ey != 15)//判断坐标是否不为偶数坐标,并且是否不符合函数
		{
			return 0;
		}
		if ((2 == by - ey || -2 == by - ey) && (2 == bx - ex || -2 == bx - ex))//判断两点是否为隔着一格的斜角
		{
			if (map_turn[turn][by > ey ? by - ey : ey - by][by > ey ? by - ey : ey - by].sur)//判断是否两点之间是否有棋子
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		return 0;
	}
	else if (!wcscmp(map_turn[turn][by][bx].name, L"象"))
	{
		if ((ex < 0 || ex > 8) && (ey < 0 || ey > 4))//判断落棋位置是否不在本领土内
		{
			return 0;
		}
		if (ex % 2 || ex + ey != 2 && ex + ey != 6 && ex + ey != 10)//判断坐标是否不为偶数坐标,并且是否不符合函数
		{
			return 0;
		}
		if ((2 == by - ey || -2 == by - ey) && (2 == bx - ex || -2 == bx - ex))//判断两点是否为隔着一格的斜角
		{
			if (map_turn[turn][by > ey ? by - ey : ey - by][by > ey ? by - ey : ey - by].sur)//判断是否两点之间是否有棋子
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		return 0;
	}
	else if (!wcscmp(map_turn[turn][by][bx].name, L"仕"))
	{
		if ((ex < 3 || ex > 5) && (ey < 7 || ey > 9) && ex + ey != 11 &&
			ex + ey != 13)//判断落棋位置是否不在九宫格的特定位置内
		{
			return 0;
		}
		if ((1 == by - ey || -1 == by - ey) && (1 == bx - ex || -1 == bx - ex))//判断是否为斜向移动一格
		{
			return 1;
		}
		return 0;
	}
	else if (!wcscmp(map_turn[turn][by][bx].name, L"士"))
	{
		if ((ex < 3 || ex > 5) && (ey < 0 || ey > 2) && ex + ey != 4 &&
			ex + ey != 6)//判断落棋位置是否不在九宫格的特定位置内
		{
			return 0;
		}
		if ((1 == by - ey || -1 == by - ey) && (1 == bx - ex || -1 == bx - ex))//判断是否为斜向移动一格
		{
			return 1;
		}
		return 0;
	}
	else if (!wcscmp(map_turn[turn][by][bx].name, L"兵"))
	{
		if (map_turn[turn][by][bx].riv)//判断是否过河
		{
			if (-1 == ey - by && ex == bx)//判断是否为前一格并且没有左右移动
			{
				return 1;
			}
			else if (ey == by && (ex - bx == 1 || ex - bx == -1))//判断是否为左右平移并且没有上下移动
			{
				return 1;
			}
		}
		else//没过河
		{
			if (-1 == ey - by && ex == bx)//判断是否为前一格并且没有左右移动
			{
				return 1;
			}
		}
		return 0;
	}
	else if (!wcscmp(map_turn[turn][by][bx].name, L"卒"))
	{
		if (map_turn[turn][by][bx].riv)//判断是否过河
		{
			if (1 == ey - by && ex == bx)//判断是否为前一格并且没有左右移动
			{
				return 1;
			}
			else if (ey == by && (ex - bx == 1 || ex - bx == -1))//判断是否为左右平移并且没有上下移动
			{
				return 1;
			}
		}
		else//没过河
		{
			if (1 == ey - by && ex == bx)//判断是否为前一格并且没有左右移动
			{
				return 1;
			}
		}
		return 0;
	}
	else if (!wcscmp(map_turn[turn][by][bx].name, L"帅"))
	{
		if ((ex < 3 || ex > 5) && (ey < 7 || ey > 9))//判断落棋位置是否不在九宫格内
		{
			return 0;
		}
		if ((1 == by - ey || -1 == by - ey) && 0 == bx - ex ||
			(1 == bx - ex || -1 == bx - ex) && 0 == by - ey)//判断是否为移动一格
		{
			return 1;
		}
		return 0;
	}
	else if (!wcscmp(map_turn[turn][by][bx].name, L"将"))
	{
		if ((ex < 3 || ex > 5) && (ey < 0 || ey > 7))//判断落棋位置是否不在九宫格内
		{
			return 0;
		}
		if ((1 == by - ey || -1 == by - ey) && 0 == bx - ex ||
			(1 == bx - ex || -1 == bx - ex) && 0 == by - ey)//判断是否为移动一格
		{
			return 1;
		}
		return 0;
	}
	//else
	//{
	//	printf("decide函数出错了，未找到棋子的名称");
	//	exit(1);
	//}
}

void GameControl()//鼠标信息控制局内消息
{
	int i = 0, j = 0;
	if (MouseHit())
	{
		msg = GetMouseMsg();
		switch (msg.uMsg)
		{
		case WM_LBUTTONDOWN:
			switch (during)
			{
			case 0:
				if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 2 * getheight() / 11 && msg.y < 2 * getheight() / 11 + TAB_H)
				{
					setlinecolor(LCOLOR);//使选项卡线条颜色变亮
					roundrect((getwidth() - TAB_W) / 2, getheight() * 2 / 11,
						(getwidth() + TAB_W) / 2, getheight() * 2 / 11 + TAB_H, 30, 60);//画出选项卡边框
					setlinecolor(BLACK);//恢复选项卡线条颜色
				}
				if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 4 * getheight() / 11 && msg.y < 4 * getheight() / 11 + TAB_H)
				{
					setlinecolor(LCOLOR);//使选项卡线条颜色变亮
					roundrect((getwidth() - TAB_W) / 2, getheight() * 2 * 2 / 11,
						(getwidth() + TAB_W) / 2, getheight() * 2 * 2 / 11 + TAB_H, 30, 60);//画出选项卡边框
					setlinecolor(BLACK);//恢复选项卡线条颜色
				}
				if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 6 * getheight() / 11 && msg.y < 6 * getheight() / 11 + TAB_H)
				{
					setlinecolor(LCOLOR);//使选项卡线条颜色变亮
					roundrect((getwidth() - TAB_W) / 2, getheight() * 3 * 2 / 11,
						(getwidth() + TAB_W) / 2, getheight() * 3 * 2 / 11 + TAB_H, 30, 60);//画出选项卡边框
					setlinecolor(BLACK);//恢复选项卡线条颜色
				}
				if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 8 * getheight() / 11 && msg.y < 8 * getheight() / 11 + TAB_H)
				{
					setlinecolor(LCOLOR);//使选项卡线条颜色变亮
					roundrect((getwidth() - TAB_W) / 2, getheight() * 4 * 2 / 11,
						(getwidth() + TAB_W) / 2, getheight() * 4 * 2 / 11 + TAB_H, 30, 60);//画出选项卡边框
					setlinecolor(BLACK);//恢复选项卡线条颜色
				}
				break;
			case 1:

				break;
			case 2:
				break;
			default:
				break;
			}
			break;
		case WM_LBUTTONUP:
			switch (during)
			{
			case 0:
				if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 2 * getheight() / 11 && msg.y < 2 * getheight() / 11 + TAB_H)
				{
					during = 1;//改变鼠标判断逻辑为局内
					BoardDraw();//画棋盘
					PiecesDraw();//画棋子
				}
				else if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 4 * getheight() / 11 && msg.y < 4 * getheight() / 11 + TAB_H)
				{

				}
				else if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 6 * getheight() / 11 && msg.y < 6 * getheight() / 11 + TAB_H)
				{

				}
				else if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 8 * getheight() / 11 && msg.y < 8 * getheight() / 11 + TAB_H)
				{
					exit(0);
				}
				else
				{
					setlinecolor(BLACK);
					for (i = 0; i < 4; i++)
					{
						roundrect((getwidth() - TAB_W) / 2, getheight() * (i + 1) * 2 / 11, (getwidth()
							+ TAB_W) / 2, getheight() * (i + 1) * 2 / 11 + TAB_H, 30, 60);//恢复选项卡
					}
				}
				break;
			case 1:
				for (i = 0; i < ROW; i++)//遍历数组
				{
					for (j = 0; j < COL; j++)
					{
						if (-1 == Begin[0])//判断上一步是否没有选中棋子
						{
							if (map_turn[turn][i][j].sur && judge(msg.x, msg.y, i, j))//对存活的棋子判断点是否在他的范围
							{
								if (map_turn[turn][i][j].sur && routype == map_turn[turn][i][j].type)//判断行棋是否为对应回合方
								{
									Begin[0] = i, Begin[1] = j;//储存前棋子的信息
								}
							}
						}
						else
						{
							if (judge(msg.x, msg.y, i, j))//判断是否选中了落棋位置
							{
								if (map_turn[turn][i][j].sur && map_turn[turn][i][j].type == routype)
									//判断位置是否为友军
								{
									Begin[0] = i, Begin[1] = j;//切换前棋子的信息
									printf("xsb");

								}
								else
								{
									End[0] = i, End[1] = j;//储存后棋子信息
									if (decide(Begin, End))//判断位置是否合理!!!!!!!!!!!!!(并且老将是否被将军)
									{
										printf(" %d%d->%d%d ", Begin[0], Begin[1], End[0], End[1]);//调试使用
										
										map_turn[turn][End[0]][End[1]] = map_turn[turn][Begin[0]][Begin[1]];
											//初始位置的棋子复制到末位置
										map_turn[turn][Begin[0]][Begin[1]].exist = 0;
										wcscpy(map_turn[turn][Begin[0]][Begin[1]].name, L" ");
										map_turn[turn][Begin[0]][Begin[1]].sur = 0;
										map_turn[turn][Begin[0]][Begin[1]].type = 0;//原始位置标为无棋子
										BoardDraw();//画棋盘
										PiecesDraw();//画棋子
										GameUpdate(turn++);//当前棋子信息写到下一回合,为其行动做准备,并且回合加一
										routype = !routype;//行动完成交换红黑的回合
										Begin[0] = -1;//变为未选中棋子模式
									}
									else//不合理提示一下
									{
										printf("sb");
									}
								}
							}
						}
					}
				}
				break;
			case 2:
				break;
			default:
				break;
			}
			break;
		}
	}
}

int main()
{
	initgraph(700, 770, SHOWCONSOLE);//创建画面
	Init();//初始化样式、颜色等
	GameInit();//初始化棋子等
	for (int i = 0; i < ROW; i++)//把当前数组写到总流程数组内
	{
		for (int j = 0; j < COL; j++)
		{
			map_turn[0][i][j] = map[i][j];
		}
	}
	GameUpdate(0);//初始化第一轮
	MainMenu();//画主菜单
	while (1)//主循环
	{
		GameControl();//鼠标信息控制
	}
	return 0;
}