#define _CRT_SECURE_NO_WARNINGS

#include <graphics.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 70//格子大小
#define ROW 10//行
#define COL 9//列
#define RIVER L"楚河        汉界"//方便后续打印
#define DCOLOR RGB(180, 145, 105)//暗色
#define BCOLOR RGB(250, 235, 195)//亮色
#define COLOR RGB(225, 190, 150)//整体颜色
#define ROU 30//棋子半径大小
#define FONT 36//棋子字体大小
#define TAB_W 400//选项卡宽
#define TAB_H 90//选项卡高
#define TURN 100//数组最大步数
#define KEY 70//功能键大小
#define KEY_E 25//功能键露出区域大小
#define SIZE_TRUMPET 45//小格子大小
#define ROU_TRUMPET 19//小棋子半径大小
#define FONT_TRUMPET 23//小棋子字体大小

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

struct Chess map_turn[TURN][ROW][COL] = { 0 };//记录全部历史
struct Chess map_test[ROW][COL] = { 0 };//测试移动后是否被将军的

MOUSEMSG msg;

short Begin[] = { -1, -1 }, End[] = { -1, -1 };//记录起始和初始位置

POINT point_left[] = { { 215, 567 }, { 140, 610 }, { 215, 653 } };//左三角
POINT point_right[] = { { 485, 567 } ,{ 560, 610 }, { 485, 653 } };//右三角

int memsize_map = sizeof(struct Chess) * ROW * COL;//记录一个map所占字节数
bool routype = 1;//记录回合是红方还是黑方
bool rouend = 1;//记录对局是否结束
short during = 0;//鼠标判断逻辑,0主界面,1游戏中,2查看历史,3局内功能键,4提示送将,5输赢界面
short turn = 1;//记录回合数的变量
short Judge = 1;//记录对局是否没有结束,0为结束
bool now = 1;//记录执棋所在方,0:黑方1:红方
bool is = 1;//记录是否可以继续游戏
short history = 1;//记录读取的文件位置
bool oi = 0;//判断打印退出还是进入,0:进入,1:退出
short his;//关于历史记录的对局数
short num;//读取到的局数

const wchar_t* redlist[7] = { L"車", L"馬", L"相", L"仕", L"帅", L"炮", L"兵" };//红棋名称
const wchar_t* blacklist[7] = { L"車", L"馬", L"象", L"士", L"将", L"炮", L"卒" };//黑棋名称
const wchar_t* tab[4] = { L"新游戏", L"继续游戏", L"历史战绩", L"退出" };//选项卡名称
const wchar_t* key[4] = { L"返回游戏", L"悔棋", L"反转棋盘", L"返回菜单" };//功能键名称

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
	BeginBatchDraw();//防止闪烁
	cleardevice();//刷新背景
	for (int i = 0; i < 4; i++)//绘制选项卡
	{
		roundrect((getwidth() - TAB_W) / 2, getheight() * (i + 1) * 2 / 11, (getwidth() + TAB_W) / 2,
			getheight() * (i + 1) * 2 / 11 + TAB_H, 30, 60);
	}
	for (int i = 0; i < 4; i++)//打印文字
	{
		if (1 == i && !is)
		{
			settextcolor(RGB(100, 100, 100));
			settextstyle(40, 0, L"宋体");
			outtextxy((getwidth() - textwidth(tab[i])) / 2,
				getheight() * (i + 1) * 2 / 11 + (TAB_H - textheight(tab[i])) / 2, tab[i]);
			settextcolor(BLACK);
		}
		else
		{
			settextcolor(BLACK);
			settextstyle(40, 0, L"宋体");
			outtextxy((getwidth() - textwidth(tab[i])) / 2,
				getheight() * (i + 1) * 2 / 11 + (TAB_H - textheight(tab[i])) / 2, tab[i]);
		}
	}
	settextstyle(FONT, 0, L"楷体");
	EndBatchDraw();//关闭双缓冲绘图
}

void BoardDraw(int s, int f, int f_r, int l)//画棋盘s:格子大小,f:字体大小,f_r:楚河汉界大小,l:线条粗细
{
	int i = 0;
	cleardevice();//刷新背景
	setlinestyle(PS_SOLID, l);//设置线条效果
	for (i = 0; i < ROW; i++)//画横线
	{
		line(s, (i + 1) * s, s * COL, (i + 1) * s);
	}
	for (i = 0; i < COL; i++)//画竖线
	{
		line((i + 1) * s, s, (i + 1) * s, s * ROW);
	}
	rectangle(s - 5, s - 5, s * COL + 5, s * ROW + 5);//画外边框
	fillrectangle(s, (ROW / 2) * s, s * COL, (ROW / 2 + 1) * s);//覆盖中间的竖线
	line(4 * s, s, 6 * s, 3 * s);
	line(4 * s, 3 * s, 6 * s, s);
	line(4 * s, 10 * s, 6 * s, 8 * s);
	line(4 * s, 8 * s, 6 * s, 10 * s);//以上四条画棋盘上的斜线
	settextcolor(BLACK);//字体颜色设置
	settextstyle(f_r, 0, L"楷体");//用于楚河汉界的字体设置
	outtextxy(((COL + 1) * s - textwidth(RIVER)) / 2,
		((ROW + 1) * s - textheight(RIVER)) / 2, RIVER);//棋盘中间加上楚河汉界
	settextstyle(f, 0, L"楷体");//用于棋子的字体设置
	if (1 == during)//判断是否画出功能键
	{
		setfillcolor(DCOLOR);//设置填充颜色
		solidroundrect((getwidth() - KEY) / 2, KEY_E - KEY, (getwidth() + KEY) / 2, KEY_E, 10, 10);//画出功能键
		setfillcolor(COLOR);//恢复填充颜色
		setlinestyle(PS_SOLID, 6);//设置线条效果
		setlinecolor(RGB(200, 175, 145));//设置线条颜色
		line((getwidth() - KEY) / 2 + 16, KEY_E / 2, (getwidth() + KEY) / 2 - 16, KEY_E / 2);//画出功能键的内饰
	}
	setlinestyle(PS_SOLID, 2);//恢复线条效果
	setlinecolor(BLACK);//恢复线条颜色
}

void PiecesDraw(int s, int f, int r, int l)//画棋子s:格子大小,f:字体大小,r:棋子半径大小,l:线条粗细
{
	setlinestyle(PS_SOLID, l);//设置线条效果
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (map_turn[turn][i][j].sur)
			{
				if (now)//判断什么视角打印
				{
					fillcircle(s * (j + 1), s * (i + 1), r);//棋子的背景
					if (map_turn[turn][i][j].type)
					{
						settextcolor(RED);//字体颜色设置
						outtextxy(s * (j + 1) - f / 2,
							s * (i + 1) - f / 2, map_turn[turn][i][j].name);//棋子的字
					}
					else
					{
						settextcolor(BLACK);//字体颜色设置
						outtextxy(s * (j + 1) - f / 2,
							s * (i + 1) - f / 2, map_turn[turn][i][j].name);//棋子的字
					}
				}
				else
				{
					fillcircle(s * (COL - j), s * (ROW - i), r);//棋子的背景
					if (map_turn[turn][i][j].type)
					{
						settextcolor(RED);//字体颜色设置
						outtextxy(s * (COL - j) - f / 2,
							s * (ROW - i) - f / 2, map_turn[turn][i][j].name);//棋子的字
					}
					else
					{
						settextcolor(BLACK);//字体颜色设置
						outtextxy(s * (COL - j) - f / 2,
							s * (ROW - i) - f / 2, map_turn[turn][i][j].name);//棋子的字
					}
				}
			}
		}
	}
	setlinestyle(PS_SOLID, 2);//设置线条效果
}

bool judge(short x, short y, int i, int j)//红方视角判断光标落点
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
			if (map_turn[turn][(by + ey) / 2][(bx + ex) / 2].sur)//判断是否两点之间是否有棋子
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
			if (map_turn[turn][(by + ey) / 2][(by + ey) / 2].sur)//判断是否两点之间是否有棋子
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
				if (4 == ey)//判断位置是否过河
				{
					map_turn[turn][by][bx].riv = 1;//赋值为过河
				}
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
				if (5 == ey)//判断位置是否过河
				{
					map_turn[turn][by][bx].riv = 1;//赋值为过河
				}
				return 1;
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
	else
	{
		printf("decide函数出错了，未找到棋子的名称");
		exit(1);
	}
}

void DarkBoardDraw()//画暗棋盘
{
	int i = 0;
	setbkcolor(DCOLOR);//设置背景颜色
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
	setfillcolor(DCOLOR);//设置填充颜色
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
	setfillcolor(COLOR);//恢复填充颜色
	setbkcolor(COLOR);//恢复背景颜色
}

void DarkPiecesDraw()//画棋子
{
	setbkcolor(DCOLOR);//设置背景颜色
	setfillcolor(DCOLOR);//设置填充颜色
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (map_turn[turn][i][j].sur)
			{
				if (now)//判断视角
				{
					fillcircle(SIZE * (j + 1), SIZE * (i + 1), ROU);//棋子的背景
					if (map_turn[turn][i][j].type)
					{
						settextcolor(RGB(120, 0, 0));//字体颜色设置
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
				else
				{
					fillcircle(SIZE * (COL - j), SIZE * (ROW - i), ROU);//棋子的背景
					if (map_turn[turn][i][j].type)
					{
						settextcolor(RGB(120, 0, 0));//字体颜色设置
						outtextxy(SIZE * (COL - j) - FONT / 2,
							SIZE * (ROW - i) - FONT / 2, map_turn[turn][i][j].name);//棋子的字
					}
					else
					{
						settextcolor(BLACK);//字体颜色设置
						outtextxy(SIZE * (COL - j) - FONT / 2,
							SIZE * (ROW - i) - FONT / 2, map_turn[turn][i][j].name);//棋子的字
					}
				}
			}
		}
	}
	setbkcolor(COLOR);//恢复背景颜色
	setfillcolor(COLOR);//恢复填充颜色
}

void OverDraw()//绘制对局结束
{
	BeginBatchDraw();//防止闪烁
	DarkBoardDraw();//暗棋盘
	DarkPiecesDraw();//暗棋子
	fillroundrect((getwidth() - TAB_W) / 2, getheight() * 2 / 11,
		(getwidth() + TAB_W) / 2, getheight() * 4 / 11 + TAB_H, 370, 250);//画出选项卡
	fillroundrect(1.5 * SIZE, getheight() * 6 / 11, 4 * SIZE,
		getheight() * 6 / 11 + TAB_H, 30, 30);//画出选项卡
	fillroundrect(6 * SIZE, getheight() * 6 / 11, 8.5 * SIZE,
		getheight() * 6 / 11 + TAB_H, 30, 30);//画出选项卡
	settextcolor(BLACK);
	outtextxy((5.5 * SIZE - textwidth(tab[0])) / 2, getheight() * 6 / 11
		+ (TAB_H - textheight(tab[0])) / 2, tab[0]);//打印"新游戏"
	outtextxy((14.5 * SIZE - textwidth(key[3])) / 2, getheight() * 6 / 11
		+ (TAB_H - textheight(key[3])) / 2, key[3]);//打印"返回菜单"
	settextstyle(getheight() / 11, 0, L"楷体");//对应的字体设置
	if (!routype)//分辨红胜利
	{
		settextcolor(RED);
		outtextxy((getwidth() - textwidth(L"红方胜利")) / 2, getheight() * 3 / 11
			+ 45 - textheight(L"红方胜利") / 2, L"红方胜利");// 打印"红方胜利"
	}
	else//分辨黑胜利
	{
		settextcolor(BLACK);
		outtextxy((getwidth() - textwidth(L"黑方胜利")) / 2, getheight() * 3 / 11
			+ 45 - textheight(L"黑方胜利") / 2, L"黑方胜利");// 打印"黑方胜利"
	}
	EndBatchDraw();//关闭双缓冲绘图
	settextcolor(BLACK);
	settextstyle(FONT, 0, L"楷体");// 恢复字体设置
}

void FunctionKey()
{
	DarkBoardDraw();
	DarkPiecesDraw();
	setfillcolor(COLOR);//设置填充颜色
	settextstyle(40, 0, L"宋体");//设置字体
	for (int i = 0; i < 4; i++)//绘制选项卡
	{
		fillroundrect((getwidth() - TAB_W) / 2, getheight() * (i + 1) * 2 / 11, (getwidth() + TAB_W) / 2,
			getheight() * (i + 1) * 2 / 11 + TAB_H, 30, 60);
	}
	for (int i = 0; i < 4; i++)//打印文字
	{
		settextcolor(BLACK);
		settextstyle(40, 0, L"宋体");
		if (i == 1 && turn == 1)//第一轮的悔棋为浅色
		{
			settextcolor(RGB(110, 110, 110));
			outtextxy((getwidth() - textwidth(key[i])) / 2,
				getheight() * (i + 1) * 2 / 11 + (TAB_H - textheight(key[i])) / 2, key[i]);
			continue;
		}
		outtextxy((getwidth() - textwidth(key[i])) / 2,
			getheight() * (i + 1) * 2 / 11 + (TAB_H - textheight(key[i])) / 2, key[i]);
	}
	settextstyle(FONT, 0, L"楷体");//恢复字体
}

void HistoryDraw()//绘制历史记录样式
{
	BeginBatchDraw();//防止闪烁
	BoardDraw(SIZE_TRUMPET, FONT_TRUMPET, 32, 1);//画棋盘
	PiecesDraw(SIZE_TRUMPET, FONT_TRUMPET, ROU_TRUMPET, 1);//画棋子
	for (int i = 1; i < 3; i++)//绘制选项卡
	{
		roundrect(10 * SIZE_TRUMPET, (4 * i - 1) * SIZE_TRUMPET,
			getwidth() - SIZE_TRUMPET, (4 * i + 1) * SIZE_TRUMPET, 30, 30);
	}
	settextcolor(BLACK);
	settextstyle(30, 0, L"宋体");
	outtextxy((getwidth() + 9 * SIZE_TRUMPET - textwidth(L"返回菜单")) / 2,
		5 * SIZE_TRUMPET - textwidth(L"返回菜单") / 2, L"返回菜单");
	if (oi)
	{
		outtextxy((getwidth() + 9 * SIZE_TRUMPET - textwidth(L"退出推演")) / 2,
			9 * SIZE_TRUMPET - textwidth(L"退出推演") / 2, L"退出推演");
	}
	else
	{
		outtextxy((getwidth() + 9 * SIZE_TRUMPET - textwidth(L"进入推演")) / 2,
			9 * SIZE_TRUMPET - textwidth(L"进入推演") / 2, L"进入推演");
	}
	circle(190, 610, 85);//绘制左圆
	circle(510, 610, 85);//绘制右圆
	setfillcolor(DCOLOR);//设置填充颜色
	setlinecolor(DCOLOR);//设置线条颜色
	fillpolygon(point_left, 3);//绘制左三角
	fillpolygon(point_right, 3);//绘制右三角
	settextstyle(40, 0, L"宋体");
	setfillcolor(COLOR);//恢复填充颜色
	setlinecolor(BLACK);//恢复线条颜色
	EndBatchDraw();//关闭双缓冲绘图
}

void reset(Chess(*m)[COL], short x, short y)//重置一个二维数组的一个元素为空棋子
{
	m[x][y].exist = 0;//赋值为不存在
	wcscpy(m[x][y].name, L" ");//名字赋值为空
	m[x][y].sur = 0;//生存状态赋值为0
	m[x][y].type = 0;//重置黑红方
}

void TestUpdate(short x, short y, short i, short j)//创建一个棋盘检测移动后的将军情况
{

	for (int u = 0; u < ROW; u++)
	{
		for (int t = 0; t < COL; t++)
		{
			map_test[u][t] = map_turn[turn][u][t];
		}
	}
	map_test[i][j] = map_test[x][y];
	reset(map_test, x, y);
}

bool Check(Chess(*ma)[COL])//检测己方老将是否被将军,被将军返回0
{
	int i = 0, j = 0, u = 0, t = 0, m = 0, n = 0;
	if (routype)//区分红黑方
	{
		for (i = 0; i < ROW; i++)//检测红帅的位置
		{
			for (j = 0; j < COL; j++)
			{
				if (!wcscmp(ma[i][j].name, L"帅"))
				{
					u = i, t = j;
				}
			}
		}
		for (i = 0; i < ROW; i++)//检测黑方所有棋子
		{
			for (j = 0; j < COL; j++)
			{
				if (ma[i][j].sur && !ma[i][j].type)//检测是否为黑方棋子
				{
					if (!wcscmp(ma[i][j].name, L"車"))
					{
						n = 0;
						if (i == u)//检测是否和帅是同一排
						{
							for (j > t ? m = 1 : m = -1; j > t ? m < j - t : m > j - t;
								j > t ? m++ : m--)//判断是否两点间是否有阻挡
							{
								if (ma[i][j - m].sur)
								{
									n++;
								}
							}
						}
						else if (j == t)//检测是否和帅是同一列
						{
							for (i > u ? m = 1 : m = -1; i > u ? m < i - u : m > i - u;
								i > u ? m++ : m--)//判断是否两点间是否有阻挡
							{
								if (ma[i - m][j].sur)
								{
									n++;
								}
							}
						}
						else
						{
							n = 1;
						}
						if (!n)
						{
							return 0;
						}
					}
					else if (!wcscmp(ma[i][j].name, L"馬"))
					{
						if (t == j - 2 && (u == i - 1 || u == i + 1))//判断行棋是否为日字:判断左
						{
							if (!ma[i][j - 1].sur)//判断是否没有阻挡
							{
								return 0;
							}
						}
						else if (t == j + 2 && (u == i - 1 || u == i + 1))//判断行棋是否为日字:判断右
						{
							if (!ma[i][j + 1].sur)//判断是否没有阻挡
							{
								return 0;
							}
						}
						else if (u == i - 2 && (t == j - 1 || t == j + 1))//判断行棋是否为日字:判断上
						{
							if (!ma[i - 1][j].sur)//判断是否没有阻挡
							{
								return 0;
							}
						}
						else if (u == i + 2 && (t == j - 1 || t == j + 1))//判断行棋是否为日字:判断下
						{
							if (!ma[i + 1][j].sur)//判断是否没有阻挡
							{
								return 0;
							}
						}
					}
					else if (!wcscmp(ma[i][j].name, L"炮"))
					{
						n = 0;
						if (i == u)//检测是否和帅是同一排
						{
							for (j > t ? m = 1 : m = -1; j > t ? m < j - t : m > j - t;
								j > t ? m++ : m--)//判断是否两点间有多少阻挡
							{
								if (ma[i][j - m].sur)
								{
									n++;
								}
							}
						}
						else if (j == t)//检测是否和帅是同一列
						{
							for (i > u ? m = 1 : m = -1; i > u ? m < i - u : m > i - u;
								i > u ? m++ : m--)//判断是否两点间有多少阻挡
							{
								if (ma[i - m][j].sur)
								{
									n++;
								}
							}
						}
						if (1 == n)
						{
							return 0;
						}
					}
					else if (!wcscmp(ma[i][j].name, L"卒"))
					{
						if (i == u)//检测是否和帅是同一排
						{
							if (t == j - 1 || t == j + 1)
							{
								return 0;
							}
						}
						else if (j == t)//检测是否和帅是同一列
						{
							if (u == i + 1)
							{
								return 0;
							}
						}
					}
					else if (!wcscmp(ma[i][j].name, L"将"))
					{
						n = 0;
						if (j == t)//检测是否和将是同一列
						{
							for (i > u ? m = 1 : m = -1; i > u ? m < i - u : m > i - u;
								i > u ? m++ : m--)//判断是否两点间是否有阻挡
							{
								if (ma[i - m][j].sur)
								{
									n++;
								}
							}
							if (!n)
							{
								return 0;
							}
						}
					}
				}
			}
		}
		return 1;
	}
	else
	{
		for (i = 0; i < ROW; i++)//检测黑将的位置
		{
			for (j = 0; j < COL; j++)
			{
				if (!wcscmp(ma[i][j].name, L"将"))
				{
					u = i, t = j;
				}
			}
		}
		for (i = 0; i < ROW; i++)//检测红方所有棋子
		{
			for (j = 0; j < COL; j++)
			{
				if (ma[i][j].sur && ma[i][j].type)//检测是否为红方棋子
				{
					if (!wcscmp(ma[i][j].name, L"車"))
					{
						n = 0;
						if (i == u)//检测是否和将是同一排
						{
							for (j > t ? m = 1 : m = -1; j > t ? m < j - t : m > j - t;
								j > u ? m++ : m--)//判断是否两点间是否有阻挡
							{
								if (ma[i][j - m].sur)
								{
									n++;
								}
							}
						}
						else if (j == t)//检测是否和将是同一列
						{
							for (i > u ? m = 1 : m = -1; i > u ? m < i - u : m > i - u;
								i > u ? m++ : m--)//判断是否两点间是否有阻挡
							{
								if (ma[i - m][j].sur)
								{
									n++;
								}
							}
						}
						else
						{
							n = 1;
						}
						if (!n)
						{
							return 0;
						}
					}
					else if (!wcscmp(ma[i][j].name, L"馬"))
					{
						if (t == j - 2 && (u == i - 1 || u == i + 1))//判断行棋是否为日字:判断左
						{
							if (!ma[i][j - 1].sur)//判断是否没有阻挡
							{
								return 0;
							}
						}
						else if (t == j + 2 && (u == i - 1 || u == i + 1))//判断行棋是否为日字:判断右
						{
							if (!ma[i][j + 1].sur)//判断是否没有阻挡
							{
								return 0;
							}
						}
						else if (u == i - 2 && (t == j - 1 || t == j + 1))//判断行棋是否为日字:判断上
						{
							if (!ma[i - 1][j].sur)//判断是否没有阻挡
							{
								return 0;
							}
						}
						else if (u == i + 2 && (t == j - 1 || t == j + 1))//判断行棋是否为日字:判断下
						{
							if (!ma[i + 1][j].sur)//判断是否没有阻挡
							{
								return 0;
							}
						}
					}
					else if (!wcscmp(ma[i][j].name, L"炮"))
					{
						n = 0;
						if (i == u)//检测是否和将是同一排
						{
							for (j > t ? m = 1 : m = -1; j > t ? m < j - t : m > j - t;
								j > u ? m++ : m--)//判断是否两点间有多少阻挡
							{
								if (ma[i][j - m].sur)
								{
									n++;
								}
							}
						}
						else if (j == t)//检测是否和将是同一列
						{
							for (i > u ? m = 1 : m = -1; i > u ? m < i - u : m > i - u;
								i > u ? m++ : m--)//判断是否两点间有多少阻挡
							{
								if (ma[i - m][j].sur)
								{
									n++;
								}
							}
						}
						if (1 == n)
						{
							return 0;
						}
					}
					else if (!wcscmp(ma[i][j].name, L"兵"))
					{
						if (i == u)//检测是否和将是同一排
						{
							if (t == j - 1 || t == j + 1)
							{
								return 0;
							}
						}
						else if (j == t)//检测是否和将是同一列
						{
							if (u == i + 1)
							{
								return 0;
							}
						}
					}
					else if (!wcscmp(ma[i][j].name, L"帅"))
					{
						n = 0;
						if (j == t)//检测是否和帅是同一列
						{
							for (i > u ? m = 1 : m = -1; i > u ? m < i - u : m > i - u;
								i > u ? m++ : m--)//判断是否两点间是否有阻挡
							{
								if (ma[i - m][j].sur)
								{
									n++;
								}
							}
							if (!n)
							{
								return 0;
							}
						}
					}
				}
			}
		}
		return 1;
	}
}

bool GameOver()
{
	int i = 0, j = 0, u = 0, t = 0;
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			if (map_turn[turn][i][j].sur && map_turn[turn][i][j].type == routype)//遍历棋子是否为当前回合的
			{
				if (!wcscmp(map_turn[turn][i][j].name, L"車"))
				{
					for (u = i + 1; u < 10; u++)//遍历垂直递增移动
					{
						if (map_turn[turn][u][j].sur && map_turn[turn][u][j].type == routype)//遇到黑棋
						{
							break;
						}
						else if (map_turn[turn][u][j].sur && map_turn[turn][u][j].type != routype)//遇到红棋
						{
							TestUpdate(i, j, u, j);
							if (Check(map_test))
							{
								return 1;
							}
							break;
						}
						TestUpdate(i, j, u, j);
						if (Check(map_test))
						{
							return 1;
						}
					}
					for (u = i - 1; u > -1; u--)//遍历垂直递减移动
					{
						if (map_turn[turn][u][j].sur && map_turn[turn][u][j].type == routype)//遇到黑棋
						{
							break;
						}
						else if (map_turn[turn][u][j].sur && map_turn[turn][u][j].type != routype)//遇到红棋
						{
							TestUpdate(i, j, u, j);
							if (Check(map_test))
							{
								return 1;
							}
							break;
						}
						TestUpdate(i, j, u, j);
						if (Check(map_test))
						{
							return 1;
						}
					}
					for (t = j + 1; u < 9; u++)//遍历水平递增移动
					{
						if (map_turn[turn][i][t].sur && map_turn[turn][i][t].type == routype)//遇到黑棋
						{
							break;
						}
						else if (map_turn[turn][i][t].sur && map_turn[turn][i][t].type != routype)//遇到红棋
						{
							TestUpdate(i, j, i, t);
							if (Check(map_test))
							{
								return 1;
							}
							break;
						}
						TestUpdate(i, j, i, t);
						if (Check(map_test))
						{
							return 1;
						}
					}
					for (t = j - 1; u > -1; u--)//遍历水平递增移动
					{
						if (map_turn[turn][i][t].sur && map_turn[turn][i][t].type == routype)//遇到黑棋
						{
							break;
						}
						else if (map_turn[turn][i][t].sur && map_turn[turn][i][t].type != routype)//遇到红棋
						{
							TestUpdate(i, j, i, t);
							if (Check(map_test))
							{
								return 1;
							}
							break;
						}
						TestUpdate(i, j, i, t);
						if (Check(map_test))
						{
							return 1;
						}
					}
				}
				else if (!wcscmp(map_turn[turn][i][j].name, L"馬"))
				{
					if (i > 1 && !map_turn[turn][i - 1][j].sur)//上
					{
						if (j > 0 && !(map_turn[turn][i - 2][j - 1].sur &&
							map_turn[turn][i - 2][j - 1].type == routype))//上左
						{
							TestUpdate(i, j, i - 2, j - 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
						if (j < 8 && !(map_turn[turn][i - 2][j + 1].sur &&
							map_turn[turn][i - 2][j + 1].type == routype))//上右
						{
							TestUpdate(i, j, i - 2, j + 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
					if (i < 8 && !map_turn[turn][i + 1][j].sur)//下
					{
						if (j > 0 && !(map_turn[turn][i + 2][j - 1].sur &&
							map_turn[turn][i + 2][j - 1].type == routype))//下左
						{
							TestUpdate(i, j, i + 2, j - 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
						if (j < 8 && !(map_turn[turn][i + 2][j + 1].sur &&
							map_turn[turn][i + 2][j + 1].type == routype))//下右
						{
							TestUpdate(i, j, i + 2, j + 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
					if (j > 1 && !map_turn[turn][i][j - 1].sur)//左
					{
						if (i < 9 && !(map_turn[turn][i + 1][j - 2].sur &&
							map_turn[turn][i + 1][j - 2].type == routype))//左下
						{
							TestUpdate(i, j, i + 2, j - 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
						if (i > 0 && !(map_turn[turn][i - 1][j - 2].sur &&
							map_turn[turn][i - 1][j - 2].type == routype))//左上
						{
							TestUpdate(i, j, i - 1, j - 2);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
					if (j < 7 && !map_turn[turn][i][j + 1].sur)//右
					{
						if (i < 9 && !(map_turn[turn][i + 1][j - 2].sur &&
							map_turn[turn][i + 1][j - 2].type == routype))//右下
						{
							TestUpdate(i, j, i + 2, j - 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
						if (i > 0 && !(map_turn[turn][i - 1][j - 2].sur &&
							map_turn[turn][i - 1][j - 2].type == routype))//右上
						{
							TestUpdate(i, j, i - 1, j - 2);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
				}
				else if (!wcscmp(map_turn[turn][i][j].name, L"炮"))
				{
					t = 0;//用于计数遇到棋子
					for (u = i + 1; u < ROW; u++)//遍历垂直递增移动
					{
						if (!map_turn[turn][u][j].sur)//先判断不吃子
						{
							TestUpdate(i, j, u, j);
							if (Check(map_test))
							{
								return 1;
							}
						}
						else//判断能否吃子
						{
							t++;//用于计数遇到棋子
							if (2 == t)//检测第二个棋子
							{
								if (map_turn[turn][u][j].sur && map_turn[turn][u][j].type != routype)
								{
									TestUpdate(i, j, u, j);
									if (Check(map_test))
									{
										return 1;
									}
									break;
								}
							}
						}
					}
					t = 0;//用于计数遇到棋子
					for (u = i - 1; u > -1; u--)//遍历垂直递减移动
					{
						if (!map_turn[turn][u][j].sur)//先判断不吃子
						{
							TestUpdate(i, j, u, j);
							if (Check(map_test))
							{
								return 1;
							}
						}
						else//判断能否吃子
						{
							t++;//用于计数遇到棋子
							if (2 == t)//检测第二个棋子
							{
								if (map_turn[turn][u][j].sur && map_turn[turn][u][j].type != routype)
								{
									TestUpdate(i, j, u, j);
									if (Check(map_test))
									{
										return 1;
									}
									break;
								}
							}
						}
					}
					u = 0;//用于计数遇到棋子
					for (t = j + 1; t < COL; t++)//遍历水平递增移动
					{
						if (!map_turn[turn][i][t].sur)//先判断不吃子
						{
							TestUpdate(i, j, i, t);
							if (Check(map_test))
							{
								return 1;
							}
						}
						else//判断能否吃子
						{
							u++;//用于计数遇到棋子
							if (2 == u)//检测第二个棋子
							{
								if (map_turn[turn][i][t].sur && map_turn[turn][i][t].type != routype)
								{
									TestUpdate(i, j, i, t);
									if (Check(map_test))
									{
										return 1;
									}
									break;
								}
							}
						}
					}
					u = 0;//用于计数遇到棋子
					for (t = j - 1; t > -1; t--)//遍历水平递增移动
					{
						if (!map_turn[turn][i][t].sur)//先判断不吃子
						{
							TestUpdate(i, j, i, t);
							if (Check(map_test))
							{
								return 1;
							}
						}
						else//判断能否吃子
						{
							u++;//用于计数遇到棋子
							if (2 == u)//检测第二个棋子
							{
								if (map_turn[turn][i][t].sur && map_turn[turn][i][t].type != routype)
								{
									TestUpdate(i, j, i, t);
									if (Check(map_test))
									{
										return 1;
									}
									break;
								}
							}
						}
					}
				}
				else if (!wcscmp(map_turn[turn][i][j].name, L"卒"))
				{
					if (map_turn[turn][i][j].riv)//过了河
					{
						if (i < ROW - 1)//向前
						{
							TestUpdate(i, j, i + 1, j);
							if (Check(map_test))
							{
								return 1;
							}
						}
						if (j < COL - 1)//水平递增
						{
							TestUpdate(i, j, i, j + 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
						if (j > 0)//水平递减
						{
							TestUpdate(i, j, i, j - 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
					else//没过河
					{
						TestUpdate(i, j, i + 1, j);
						if (Check(map_test))
						{
							return 1;
						}
					}
				}
				else if (!wcscmp(map_turn[turn][i][j].name, L"兵"))
				{
					if (map_turn[turn][i][j].riv)//过了河
					{
						if (i > 0)//水平递减
						{
							TestUpdate(i, j, i - 1, j);
							if (Check(map_test))
							{
								return 1;
							}
						}
						if (j < COL - 1)//水平递增
						{
							TestUpdate(i, j, i, j + 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
						if (j > 0)//水平递减
						{
							TestUpdate(i, j, i, j - 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
					else//没过河
					{
						TestUpdate(i, j, i + 1, j);
						if (Check(map_test))
						{
							return 1;
						}
					}
				}
				else if (!wcscmp(map_turn[turn][i][j].name, L"帅"))
				{
					if (i < ROW - 1)//向下
					{
						if (!(map_turn[turn][i + 1][j].sur && map_turn[turn][i + 1][j].type == routype))//是否可以走
						{
							TestUpdate(i, j, i + 1, j);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
					if (i > 7)//向上
					{
						if (!(map_turn[turn][i - 1][j].sur && map_turn[turn][i - 1][j].type == routype))//是否可以走
						{
							TestUpdate(i, j, i - 1, j);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
					if (j > 3)//向左
					{
						if (!(map_turn[turn][i][j - 1].sur && map_turn[turn][i][j - 1].type == routype))//是否可以走
						{
							TestUpdate(i, j, i, j - 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
					if (i < 5)//向右
					{
						if (!(map_turn[turn][i][j + 1].sur && map_turn[turn][i][j + 1].type == routype))//是否可以走
						{
							TestUpdate(i, j, i, j + 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
				}
				else if (!wcscmp(map_turn[turn][i][j].name, L"将"))
				{
					if (i < 2)//向下
					{
						if (!(map_turn[turn][i + 1][j].sur && map_turn[turn][i + 1][j].type == routype))//是否可以走
						{
							TestUpdate(i, j, i + 1, j);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
					if (i > 0)//向上
					{
						if (!(map_turn[turn][i - 1][j].sur && map_turn[turn][i - 1][j].type == routype))//是否可以走
						{
							TestUpdate(i, j, i - 1, j);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
					if (j > 3)//向左
					{
						if (!(map_turn[turn][i][j - 1].sur && map_turn[turn][i][j - 1].type == routype))//是否可以走
						{
							TestUpdate(i, j, i, j - 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
					if (i < 5)//向右
					{
						if (!(map_turn[turn][i][j + 1].sur && map_turn[turn][i][j + 1].type == routype))//是否可以走
						{
							TestUpdate(i, j, i, j + 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
				}
				else if (!wcscmp(map_turn[turn][i][j].name, L"相"))
				{
					if (j < COL - 1)
					{
						if (i < ROW - 1)
						{
							if (!map_turn[turn][i + 1][j + 1].sur)
							{
								TestUpdate(i, j, i + 2, j + 2);
								if (Check(map_test))
								{
									return 1;
								}
							}
						}
						if (i > 5)
						{
							if (!map_turn[turn][i - 1][j + 1].sur)
							{
								TestUpdate(i, j, i - 2, j + 2);
								if (Check(map_test))
								{
									return 1;
								}
							}
						}
					}
					if (j > 0)
					{
						if (i < ROW - 1)
						{
							if (!map_turn[turn][i + 1][j - 1].sur)
							{
								TestUpdate(i, j, i + 2, j - 2);
								if (Check(map_test))
								{
									return 1;
								}
							}
						}
						if (i > 5)
						{
							if (!map_turn[turn][i - 1][j - 1].sur)
							{
								TestUpdate(i, j, i - 2, j - 2);
								if (Check(map_test))
								{
									return 1;
								}
							}
						}
					}
				}
				else if (!wcscmp(map_turn[turn][i][j].name, L"象"))
				{
					if (j < COL - 1)
					{
						if (i < 4)
						{
							if (!map_turn[turn][i + 1][j + 1].sur)
							{
								TestUpdate(i, j, i + 2, j + 2);
								if (Check(map_test))
								{
									return 1;
								}
							}
						}
						if (i > 0)
						{
							if (!map_turn[turn][i - 1][j + 1].sur)
							{
								TestUpdate(i, j, i - 2, j + 2);
								if (Check(map_test))
								{
									return 1;
								}
							}
						}
					}
					if (j > 0)
					{
						if (i < 4)
						{
							if (!map_turn[turn][i + 1][j - 1].sur)
							{
								TestUpdate(i, j, i + 2, j - 2);
								if (Check(map_test))
								{
									return 1;
								}
							}
						}
						if (i > 0)
						{
							if (!map_turn[turn][i - 1][j - 1].sur)
							{
								TestUpdate(i, j, i - 2, j - 2);
								if (Check(map_test))
								{
									return 1;
								}
							}
						}
					}
				}
				else if (!wcscmp(map_turn[turn][i][j].name, L"士"))
				{
					if (!(map_turn[turn][i - 1][j - 1].sur && map_turn[turn][i - 1][j - 1].type == routype))//左上
					{
						if (i == 1 && j == 4 || i == 2 && j == 5)
						{
							TestUpdate(i, j, i - 1, j - 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
					if (!(map_turn[turn][i - 1][j + 1].sur && map_turn[turn][i - 1][j - 1].type == routype))//右上
					{
						if (i == 1 && j == 4 || i == 2 && j == 3)
						{
							TestUpdate(i, j, i - 1, j + 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
					if (!(map_turn[turn][i + 1][j + 1].sur && map_turn[turn][i - 1][j - 1].type == routype))//右下
					{
						if (i == 1 && j == 4 || i == 0 && j == 3)
							TestUpdate(i, j, i + 1, j + 1);
						if (Check(map_test))
						{
							return 1;
						}
					}
					if (!(map_turn[turn][i + 1][j - 1].sur && map_turn[turn][i - 1][j - 1].type == routype))//左下
					{
						if (i == 1 && j == 4 || i == 0 && j == 5)
						{
							TestUpdate(i, j, i + 1, j - 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
				}
				else if (!wcscmp(map_turn[turn][i][j].name, L"仕"))
				{
					if (i == 8 && j == 4 || i == 9 && j == 5)
					{
						if (!(map_turn[turn][i - 1][j - 1].sur && map_turn[turn][i - 1][j - 1].type == routype))//左上
						{
							TestUpdate(i, j, i - 1, j - 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
					if (i == 8 && j == 4 || i == 9 && j == 3)
					{
						if (!(map_turn[turn][i - 1][j + 1].sur && map_turn[turn][i - 1][j - 1].type == routype))//右上
						{
							TestUpdate(i, j, i - 1, j + 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
					if (i == 8 && j == 4 || i == 7 && j == 3)
					{
						if (!(map_turn[turn][i + 1][j + 1].sur && map_turn[turn][i - 1][j - 1].type == routype))//右下
						{
							TestUpdate(i, j, i + 1, j + 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
					if (i == 8 && j == 4 || i == 7 && j == 5)
					{
						if (!(map_turn[turn][i + 1][j - 1].sur && map_turn[turn][i - 1][j - 1].type == routype))//左下
						{
							TestUpdate(i, j, i + 1, j - 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
				}
			}
		}
	}
	return 0;
}

//void AppendFile(short t)//追加一轮t数据到文件
//{
//	FILE* pf = fopen("chinese_chess.txt", "ab");
//	if (NULL == pf)
//	{
//		fprintf(stdout, "%s\n", strerror(errno));
//		exit(1);
//	}
//	fwrite(&map_turn[t], memsize_map, 1, pf);//写入对局信息
//	fclose(pf);
//	pf = NULL;
//}

void AppendAllFile()//追加全部数据到文件
{
	FILE* pf = fopen("chinese_chess.txt", "ab");
	if (NULL == pf)
	{
		fprintf(stdout, "%s\n", strerror(errno));
		exit(1);
	}
	fwrite(&turn, 2, 1, pf);//先写总轮数
	fwrite(map_turn, memsize_map, TURN, pf);//写入全部对局信息
	fclose(pf);
	pf = NULL;
}

long ReadFile()//读取最后游戏全部数据，返回总游戏数
{
	FILE* pf = fopen("chinese_chess.txt", "rb");
	if (NULL == pf)
	{
		fprintf(stdout, "%s\n", strerror(errno));
		return 0;
	}
	fseek(pf, 0, SEEK_END);
	long i = ftell(pf) / (memsize_map * TURN + sizeof(short));
	fseek(pf, (memsize_map * TURN + sizeof(short)) * -history, SEEK_END);
	fread(&turn, sizeof(short), 1, pf);//读取一个总轮数
	fread(map_turn, memsize_map, TURN, pf);//读取一个总轮信息
	fseek(pf, 0, SEEK_SET);
	fclose(pf);
	pf = NULL;
	return i;
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
					setlinecolor(BCOLOR);//使选项卡线条颜色变亮
					roundrect((getwidth() - TAB_W) / 2, getheight() * 2 / 11,
						(getwidth() + TAB_W) / 2, getheight() * 2 / 11 + TAB_H, 30, 60);//画出选项卡边框
					setlinecolor(BLACK);//恢复选项卡线条颜色
				}
				if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 4 * getheight() / 11 && msg.y < 4 * getheight() / 11 + TAB_H)
				{
					setlinecolor(BCOLOR);//使选项卡线条颜色变亮
					roundrect((getwidth() - TAB_W) / 2, getheight() * 2 * 2 / 11,
						(getwidth() + TAB_W) / 2, getheight() * 2 * 2 / 11 + TAB_H, 30, 60);//画出选项卡边框
					setlinecolor(BLACK);//恢复选项卡线条颜色
				}
				if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 6 * getheight() / 11 && msg.y < 6 * getheight() / 11 + TAB_H)
				{
					setlinecolor(BCOLOR);//使选项卡线条颜色变亮
					roundrect((getwidth() - TAB_W) / 2, getheight() * 3 * 2 / 11,
						(getwidth() + TAB_W) / 2, getheight() * 3 * 2 / 11 + TAB_H, 30, 60);//画出选项卡边框
					setlinecolor(BLACK);//恢复选项卡线条颜色
				}
				if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 8 * getheight() / 11 && msg.y < 8 * getheight() / 11 + TAB_H)
				{
					setlinecolor(BCOLOR);//使选项卡线条颜色变亮
					roundrect((getwidth() - TAB_W) / 2, getheight() * 4 * 2 / 11,
						(getwidth() + TAB_W) / 2, getheight() * 4 * 2 / 11 + TAB_H, 30, 60);//画出选项卡边框
					setlinecolor(BLACK);//恢复选项卡线条颜色
				}
				break;
			case 1:

				break;
			case 2:

				break;
			case 3:

				break;
			default:
				break;
			}
			break;
		case WM_LBUTTONUP:
			if (!during)//如果在主菜单
			{
				for (int i = 0; i < 4; i++)//绘制选项卡
				{
					roundrect((getwidth() - TAB_W) / 2, getheight() * (i + 1) * 2 / 11, (getwidth() + TAB_W) / 2,
						getheight() * (i + 1) * 2 / 11 + TAB_H, 30, 60);
				}
			}
			switch (during)
			{
			case 0:
				if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 2 * getheight() / 11 && msg.y < 2 * getheight() / 11 + TAB_H)//新游戏
				{
					is = 1;//可以使用继续游戏
					routype = 1;//切换红方
					turn = 1;//重置历史记录
					GameUpdate(0);//初始化第一轮
					during = 1;//改变鼠标判断逻辑为局内
					BeginBatchDraw();//防止闪烁
					BoardDraw(SIZE, FONT, 50, 2);//画棋盘
					PiecesDraw(SIZE, FONT, ROU, 2);//画棋子
					EndBatchDraw();//关闭双缓冲绘图
				}
				else if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 4 * getheight() / 11 && msg.y < 4 * getheight() / 11 + TAB_H)//继续游戏
				{
					if (is)
					{
						during = 1;//改变鼠标判断逻辑为局内
						BeginBatchDraw();//防止闪烁
						BoardDraw(SIZE, FONT, 50, 2);//画棋盘
						PiecesDraw(SIZE, FONT, ROU, 2);//画棋子
						EndBatchDraw();//关闭双缓冲绘图
					}
				}
				else if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 6 * getheight() / 11 && msg.y < 6 * getheight() / 11 + TAB_H)//历史战绩
				{
					if (!(num = ReadFile()))//判断是否为未读取到数据
					{
						break;
					}
					during = 2;//改变控制逻辑为历史记录
					HistoryDraw();//绘制历史记录样式
				}
				else if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 8 * getheight() / 11 && msg.y < 8 * getheight() / 11 + TAB_H)//退出
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
				if (msg.x <= (getwidth() + KEY) / 2 && msg.x >= (getwidth() - KEY) / 2 &&
					msg.y <= KEY_E && msg.y >= 0)//功能键位置
				{
					FunctionKey();//绘制功能键
					during = 3;//判断逻辑改为快捷键
				}
				for (i = 0; i < ROW; i++)//遍历数组,棋子位置
				{
					for (j = 0; j < COL; j++)
					{
						if (-1 == Begin[0])//判断上一步是否没有选中棋子
						{
							if (map_turn[turn][i][j].sur && judge(now ? msg.x : getwidth() - msg.x,
								now ? msg.y : getheight() - msg.y, i, j))//对存活的棋子判断点是否在他的范围
							{
								if (map_turn[turn][i][j].sur && routype == map_turn[turn][i][j].type)
									//判断行棋是否为对应回合方
								{
									Begin[0] = i, Begin[1] = j;//储存前棋子的信息
								}
							}
						}
						else
						{
							if (judge(now ? msg.x : getwidth() - msg.x,
								now ? msg.y : getheight() - msg.y, i, j))//判断是否选中了某个落棋位置
							{
								if (map_turn[turn][i][j].sur && map_turn[turn][i][j].type == routype)
									//判断位置是否为友军
								{
									Begin[0] = i, Begin[1] = j;//切换前棋子的信息
								}
								else
								{
									End[0] = i, End[1] = j;//储存后棋子信息
									TestUpdate(Begin[0], Begin[1], End[0], End[1]);//创建一个棋盘检测移动后将军情况
									if (decide(Begin, End) && Check(map_test))//判断位置是否合理,是否会被将军
									{
										map_turn[turn][End[0]][End[1]] = map_turn[turn][Begin[0]][Begin[1]];
										//初始位置的棋子复制到末位置
										reset(map_turn[turn], Begin[0], Begin[1]);//原始位置标为无棋子
										BeginBatchDraw();//防止闪烁
										BoardDraw(SIZE, FONT, 50, 2);//画棋盘
										PiecesDraw(SIZE, FONT, ROU, 2);//画棋子
										EndBatchDraw();//关闭双缓冲绘图
										routype = !routype;//行动完成交换红黑的回合
										Begin[0] = -1;//变为未选中棋子模式
										if (!Check(map_turn[turn]))//判断
										{
											Judge = GameOver();//判断输赢
										}
										if (Judge)//如果未分胜负写到下一回合
										{
											GameUpdate(turn++);//当前棋子信息写到下一回合,为其行动做准备,并且回合加一
										}
										else
										{
											OverDraw();//绘制对局结束
											is = 0;//不能再使用继续游戏
											AppendAllFile();//写入数据
										}
									}
								}
							}
						}
					}
				}
				break;
			case 2:
				if (msg.x > 10 * SIZE_TRUMPET && msg.x < getwidth() - SIZE_TRUMPET &&
					msg.y > 3 * SIZE_TRUMPET && msg.y < 5 * SIZE_TRUMPET)//返回菜单
				{
					during = 0;//逻辑改为主菜单
					oi = 0;//切换模式
					MainMenu();//画主菜单
				}
				else if (msg.x > 10 * SIZE_TRUMPET && msg.x < getwidth() - SIZE_TRUMPET &&
					msg.y > 7 * SIZE_TRUMPET && msg.y < 9 * SIZE_TRUMPET)//进入推演/退出推演
				{
					oi = !oi;//切换模式
					if (oi)//进入推演
					{
						his = turn;//初对局
						turn = 0;//重置局数
						HistoryDraw();//绘制历史
					}
					else//退出推演
					{
						turn = his;//末对局
						HistoryDraw();//绘制历史
					}
				}
				else if (7225 >= (190 - msg.x) * (190 - msg.x) + (610 - msg.y) * (610 - msg.y))//左按钮
				{
					if (!oi)//未进入对局记录
					{
						if (history < num)//判断是否没有到头
						{
							history += 1;//跳到前一个对局
							ReadFile();//读取前一个对局
							HistoryDraw();//绘制历史记录样式
						}
					}
					else//进入对局记录
					{
						if (0 < turn)//对局是否没到头
						{
							turn--;//局内对局减一
							HistoryDraw();//绘制历史记录样式
						}
					}
				}
				else if (7225 >= (510 - msg.x) * (510 - msg.x) + (610 - msg.y) * (610 - msg.y))//右按钮
				{
					if (!oi)//未进入对局记录
					{
						if (history > 1)//判断是否没有到头
						{
							history -= 1;//跳到后一个对局
							ReadFile();//读取后一个对局
							HistoryDraw();//绘制历史记录样式
						}
					}
					else//进入对局记录
					{
						if (his > turn)//对局是否没到头
						{
							turn++;//局内对局加一
							HistoryDraw();//绘制历史记录样式
						}
					}
				}
				break;
			case 3:
				if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 2 * getheight() / 11 && msg.y < 2 * getheight() / 11 + TAB_H)//返回游戏
				{
					during = 1;//改变鼠标判断逻辑为局内
					BeginBatchDraw();//防止闪烁
					BoardDraw(SIZE, FONT, 50, 2);//画棋盘
					PiecesDraw(SIZE, FONT, ROU, 2);//画棋子
					EndBatchDraw();//关闭双缓冲绘图
				}
				else if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 4 * getheight() / 11 && msg.y < 4 * getheight() / 11 + TAB_H)//悔棋
				{
					if (turn > 1)//判断是否不为第一局
					{
						GameUpdate(--turn - 1);//退回一轮并且改变打印的棋盘
						during = 1;//改变鼠标判断逻辑为局内
						BeginBatchDraw();//防止闪烁
						BoardDraw(SIZE, FONT, 50, 2);//画棋盘
						PiecesDraw(SIZE, FONT, ROU, 2);//画棋子
						routype = !routype;//切换红黑方
						EndBatchDraw();//关闭双缓冲绘图
					}
				}
				else if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 6 * getheight() / 11 && msg.y < 6 * getheight() / 11 + TAB_H)//反转棋盘
				{
					now = !now;//反转视角
					during = 1;//改变鼠标判断逻辑为局内
					BeginBatchDraw();//防止闪烁
					BoardDraw(SIZE, FONT, 50, 2);//画棋盘
					PiecesDraw(SIZE, FONT, ROU, 2);//画棋子
					EndBatchDraw();//关闭双缓冲绘图
				}
				else if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 8 * getheight() / 11 && msg.y < 8 * getheight() / 11 + TAB_H)//返回菜单
				{
					during = 0;//逻辑改为主菜单
					MainMenu();//画主菜单
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
			case 4:
				during = 1;
				BoardDraw(SIZE, FONT, 50, 2);//画棋盘
				PiecesDraw(SIZE, FONT, ROU, 2);//画棋子
				break;
			default:
				break;
			}
			break;
		}
	}
}

void GameOverControl()//游戏结束时的鼠标信息控制
{
	if (MouseHit())
	{
		msg = GetMouseMsg();
		if (WM_LBUTTONUP == msg.uMsg)
		{
			if (1.5 * SIZE <= msg.x && getheight() * 6 / 11 <= msg.y && 4 * SIZE >= msg.x
				&& getheight() * 6 / 11 + TAB_H >= msg.y)//判断第一个选项卡
			{
				is = 1;//可以使用继续游戏
				routype = 1;//切换红方
				turn = 1;//重置历史记录
				GameUpdate(0);//初始化第一轮
				during = 1;//改变鼠标判断逻辑为局内
				BeginBatchDraw();//防止闪烁
				BoardDraw(SIZE, FONT, 50, 2);//画棋盘
				PiecesDraw(SIZE, FONT, ROU, 2);//画棋子
				EndBatchDraw();//关闭双缓冲绘图
				Judge = 1;//改对局结束判定为未结束
			}
			if (6 * SIZE <= msg.x && getheight() * 6 / 11 <= msg.y && 8.5 * SIZE >= msg.x
				&& getheight() * 6 / 11 + TAB_H >= msg.y)
			{
				during = 0;//逻辑改为主菜单
				MainMenu();//画主菜单
				Judge = 1;//改对局结束判定为未结束
			}
		}
	}
}

int main()
{
	initgraph(700, 770);//创建画面
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
	do//用于游戏结束后的循环
	{
		while (Judge)//主循环
		{
			GameControl();//鼠标信息控制
		}
		GameOverControl();//游戏结束时的鼠标信息控制
	} while (1);
	return 0;
}