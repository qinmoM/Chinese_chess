#define _CRT_SECURE_NO_WARNINGS

#include <graphics.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 70//���Ӵ�С
#define ROW 10//��
#define COL 9//��
#define RIVER L"����        ����"//���������ӡ
#define DCOLOR RGB(180, 145, 105)//��ɫ
#define BCOLOR RGB(250, 235, 195)//��ɫ
#define COLOR RGB(225, 190, 150)//������ɫ
#define ROU 30//���Ӱ뾶��С
#define FONT 36//���������С
#define TAB_W 400//ѡ���
#define TAB_H 90//ѡ���
#define TURN 100//���������
#define KEY 70//���ܼ���С
#define KEY_E 25//���ܼ�¶�������С
#define SIZE_TRUMPET 45//С���Ӵ�С
#define ROU_TRUMPET 19//С���Ӱ뾶��С
#define FONT_TRUMPET 23//С���������С

struct Chess
{
	wchar_t name[8];//����
	int x;//x����
	int y;//y����
	bool riv;//�Ƿ����
	bool type;//�ں췽(0�ڷ�,1�췽),ע��:ԭֵΪ0,����ʹ��֮ǰ��Ҫ���ж��Ƿ����
	bool sur;//�Ƿ���
	bool exist;//�Ƿ�������
}map[ROW][COL];

struct Chess map_turn[TURN][ROW][COL] = { 0 };//��¼ȫ����ʷ
struct Chess map_test[ROW][COL] = { 0 };//�����ƶ����Ƿ񱻽�����

MOUSEMSG msg;

short Begin[] = { -1, -1 }, End[] = { -1, -1 };//��¼��ʼ�ͳ�ʼλ��

POINT point_left[] = { { 215, 567 }, { 140, 610 }, { 215, 653 } };//������
POINT point_right[] = { { 485, 567 } ,{ 560, 610 }, { 485, 653 } };//������

int memsize_map = sizeof(struct Chess) * ROW * COL;//��¼һ��map��ռ�ֽ���
bool routype = 1;//��¼�غ��Ǻ췽���Ǻڷ�
bool rouend = 1;//��¼�Ծ��Ƿ����
short during = 0;//����ж��߼�,0������,1��Ϸ��,2�鿴��ʷ,3���ڹ��ܼ�,4��ʾ�ͽ�,5��Ӯ����
short turn = 1;//��¼�غ����ı���
short Judge = 1;//��¼�Ծ��Ƿ�û�н���,0Ϊ����
bool now = 1;//��¼ִ�����ڷ�,0:�ڷ�1:�췽
bool is = 1;//��¼�Ƿ���Լ�����Ϸ
short history = 1;//��¼��ȡ���ļ�λ��
bool oi = 0;//�жϴ�ӡ�˳����ǽ���,0:����,1:�˳�
short his;//������ʷ��¼�ĶԾ���
short num;//��ȡ���ľ���

const wchar_t* redlist[7] = { L"܇", L"�R", L"��", L"��", L"˧", L"��", L"��" };//��������
const wchar_t* blacklist[7] = { L"܇", L"�R", L"��", L"ʿ", L"��", L"��", L"��" };//��������
const wchar_t* tab[4] = { L"����Ϸ", L"������Ϸ", L"��ʷս��", L"�˳�" };//ѡ�����
const wchar_t* key[4] = { L"������Ϸ", L"����", L"��ת����", L"���ز˵�" };//���ܼ�����

void Init()//��ʼ����ʽ����ɫ��
{
	setbkcolor(COLOR);//���ñ�����ɫ
	setfillcolor(COLOR);//�����ɫ
	setlinecolor(BLACK);//������ɫ
	setlinestyle(PS_SOLID, 2);//������ʽ
}

void GameInit()//��ʼ�����ӵ�
{
	int i = 0, j = 0;
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			//��ʼʱ��û�й���
			map[i][j].riv = 0;
			//ÿ���㸳ֵ������
			map[i][j].x = (j + 1) * SIZE, map[i][j].y = (i + 1) * SIZE;
			//д������,û��Ϊ�ո�
			if (i < ROW / 2)
			{
				if (!i)//���жϵ�һ��
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
				else if (2 == i && (1 == j || 7 == j))//�ж���
				{
					wcscpy(map[i][j].name, blacklist[5]);
				}
				else if (3 == i && j % 2 == 0)//�ж���
				{
					wcscpy(map[i][j].name, blacklist[6]);
				}
				else//�����Ǹ�ֵ�ո�
				{
					wcscpy(map[i][j].name, L" ");
				}
			}
			else
			{
				if (9 == i)//���жϵ�һ��
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
				else if (7 == i && (1 == j || 7 == j))//�ж���
				{
					wcscpy(map[i][j].name, redlist[5]);
				}
				else if (6 == i && j % 2 == 0)//�жϱ�
				{
					wcscpy(map[i][j].name, redlist[6]);
				}
				else//�����Ǹ�ֵ�ո�
				{
					wcscpy(map[i][j].name, L" ");
				}
			}
			//�������ָ�ֵ�Ƿ����
			if (wcscmp(map[i][j].name, L" "))
			{
				map[i][j].exist = 1;
			}
			else
			{
				map[i][j].exist = 0;
			}
			//�����Ƿ���ڸ�ֵ�Ƿ���ͺں���
			if (map[i][j].exist)
			{
				map[i][j].sur = 1;//�����ӵĵط��������
				if (i < (ROW / 2) - 1)
				{
					map[i][j].type = 0;//�����ӵĵط�,���������ϰ�Ϊ����
				}
				else
				{
					map[i][j].type = 1;//�����ӵĵط�,���������°�Ϊ����
				}
			}
		}
	}
}

void GameUpdate(int t)//�Ѹı���t��д��t + 1��
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			map_turn[t + 1][i][j] = map_turn[t][i][j];
		}
	}
}

void MainMenu()//�����˵�
{
	BeginBatchDraw();//��ֹ��˸
	cleardevice();//ˢ�±���
	for (int i = 0; i < 4; i++)//����ѡ�
	{
		roundrect((getwidth() - TAB_W) / 2, getheight() * (i + 1) * 2 / 11, (getwidth() + TAB_W) / 2,
			getheight() * (i + 1) * 2 / 11 + TAB_H, 30, 60);
	}
	for (int i = 0; i < 4; i++)//��ӡ����
	{
		if (1 == i && !is)
		{
			settextcolor(RGB(100, 100, 100));
			settextstyle(40, 0, L"����");
			outtextxy((getwidth() - textwidth(tab[i])) / 2,
				getheight() * (i + 1) * 2 / 11 + (TAB_H - textheight(tab[i])) / 2, tab[i]);
			settextcolor(BLACK);
		}
		else
		{
			settextcolor(BLACK);
			settextstyle(40, 0, L"����");
			outtextxy((getwidth() - textwidth(tab[i])) / 2,
				getheight() * (i + 1) * 2 / 11 + (TAB_H - textheight(tab[i])) / 2, tab[i]);
		}
	}
	settextstyle(FONT, 0, L"����");
	EndBatchDraw();//�ر�˫�����ͼ
}

void BoardDraw(int s, int f, int f_r, int l)//������s:���Ӵ�С,f:�����С,f_r:���Ӻ����С,l:������ϸ
{
	int i = 0;
	cleardevice();//ˢ�±���
	setlinestyle(PS_SOLID, l);//��������Ч��
	for (i = 0; i < ROW; i++)//������
	{
		line(s, (i + 1) * s, s * COL, (i + 1) * s);
	}
	for (i = 0; i < COL; i++)//������
	{
		line((i + 1) * s, s, (i + 1) * s, s * ROW);
	}
	rectangle(s - 5, s - 5, s * COL + 5, s * ROW + 5);//����߿�
	fillrectangle(s, (ROW / 2) * s, s * COL, (ROW / 2 + 1) * s);//�����м������
	line(4 * s, s, 6 * s, 3 * s);
	line(4 * s, 3 * s, 6 * s, s);
	line(4 * s, 10 * s, 6 * s, 8 * s);
	line(4 * s, 8 * s, 6 * s, 10 * s);//���������������ϵ�б��
	settextcolor(BLACK);//������ɫ����
	settextstyle(f_r, 0, L"����");//���ڳ��Ӻ������������
	outtextxy(((COL + 1) * s - textwidth(RIVER)) / 2,
		((ROW + 1) * s - textheight(RIVER)) / 2, RIVER);//�����м���ϳ��Ӻ���
	settextstyle(f, 0, L"����");//�������ӵ���������
	if (1 == during)//�ж��Ƿ񻭳����ܼ�
	{
		setfillcolor(DCOLOR);//���������ɫ
		solidroundrect((getwidth() - KEY) / 2, KEY_E - KEY, (getwidth() + KEY) / 2, KEY_E, 10, 10);//�������ܼ�
		setfillcolor(COLOR);//�ָ������ɫ
		setlinestyle(PS_SOLID, 6);//��������Ч��
		setlinecolor(RGB(200, 175, 145));//����������ɫ
		line((getwidth() - KEY) / 2 + 16, KEY_E / 2, (getwidth() + KEY) / 2 - 16, KEY_E / 2);//�������ܼ�������
	}
	setlinestyle(PS_SOLID, 2);//�ָ�����Ч��
	setlinecolor(BLACK);//�ָ�������ɫ
}

void PiecesDraw(int s, int f, int r, int l)//������s:���Ӵ�С,f:�����С,r:���Ӱ뾶��С,l:������ϸ
{
	setlinestyle(PS_SOLID, l);//��������Ч��
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (map_turn[turn][i][j].sur)
			{
				if (now)//�ж�ʲô�ӽǴ�ӡ
				{
					fillcircle(s * (j + 1), s * (i + 1), r);//���ӵı���
					if (map_turn[turn][i][j].type)
					{
						settextcolor(RED);//������ɫ����
						outtextxy(s * (j + 1) - f / 2,
							s * (i + 1) - f / 2, map_turn[turn][i][j].name);//���ӵ���
					}
					else
					{
						settextcolor(BLACK);//������ɫ����
						outtextxy(s * (j + 1) - f / 2,
							s * (i + 1) - f / 2, map_turn[turn][i][j].name);//���ӵ���
					}
				}
				else
				{
					fillcircle(s * (COL - j), s * (ROW - i), r);//���ӵı���
					if (map_turn[turn][i][j].type)
					{
						settextcolor(RED);//������ɫ����
						outtextxy(s * (COL - j) - f / 2,
							s * (ROW - i) - f / 2, map_turn[turn][i][j].name);//���ӵ���
					}
					else
					{
						settextcolor(BLACK);//������ɫ����
						outtextxy(s * (COL - j) - f / 2,
							s * (ROW - i) - f / 2, map_turn[turn][i][j].name);//���ӵ���
					}
				}
			}
		}
	}
	setlinestyle(PS_SOLID, 2);//��������Ч��
}

bool judge(short x, short y, int i, int j)//�췽�ӽ��жϹ�����
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

bool decide(short* Begin, short* End)//�ж������Ƿ�������ƥ��
{
	int i = 0;
	short by = Begin[0], bx = Begin[1], ey = End[0], ex = End[1];
	if (!wcscmp(map_turn[turn][by][bx].name, L"܇"))
	{
		if (bx == ex && !(by == ey && bx == ex))//���ж��Ƿ�����ֱ�����Ҳ���ԭ��
		{
			for (by > ey ? i = 1 : i = -1; by > ey ? i < by - ey : i > by - ey;
				by > ey ? i++ : i--)//�ж��Ƿ�������Ƿ����赲
			{
				if (map_turn[turn][by - i][bx].sur)
				{
					return 0;
				}
			}
			return 1;
		}
		if (by == ey && !(by == ey && bx == ex))//���ж��Ƿ���ˮƽ�����Ҳ���ԭ��
		{
			for (bx > ex ? i = 1 : i = -1; bx > ex ? i < bx - ex : i > bx - ex;
				bx > ex ? i++ : i--)//�ж��Ƿ�������Ƿ����赲
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
	else if (!wcscmp(map_turn[turn][by][bx].name, L"�R"))
	{
		if (ex < 0 || ex > 8 || ey < 0 || ey > 9)//�жϷ�Χ
		{
			return 0;
		}
		if (ex == bx - 2 && (ey == by - 1 || ey == by + 1))//�ֱ��ж������Ƿ�Ϊ����:�ж���
		{
			if (!map_turn[turn][by][bx - 1].sur)//�ж��Ƿ�û���赲
			{
				return 1;
			}
		}
		else if (ex == bx + 2 && (ey == by - 1 || ey == by + 1))//�ֱ��ж������Ƿ�Ϊ����:�ж���
		{
			if (!map_turn[turn][by][bx + 1].sur)//�ж��Ƿ�û���赲
			{
				return 1;
			}
		}
		else if (ey == by - 2 && (ex == bx - 1 || ex == bx + 1))//�ֱ��ж������Ƿ�Ϊ����:�ж���
		{
			if (!map_turn[turn][by - 1][bx].sur)//�ж��Ƿ�û���赲
			{
				return 1;
			}
		}
		else if (ey == by + 2 && (ex == bx - 1 || ex == bx + 1))//�ֱ��ж������Ƿ�Ϊ����:�ж���
		{
			if (!map_turn[turn][by + 1][bx].sur)//�ж��Ƿ�û���赲
			{
				return 1;
			}
		}
		return 0;
	}
	else if (!wcscmp(map_turn[turn][by][bx].name, L"��"))
	{
		if (map_turn[turn][ey][ex].sur)//�ж��Ƿ����
		{
			int j = 0;
			if (bx == ex && !(by == ey && bx == ex))//���ж��Ƿ�����ֱ�����Ҳ���ԭ��
			{
				for (by > ey ? i = 1 : i = -1; by > ey ? i < by - ey : i > by - ey;
					by > ey ? i++ : i--)//�ж��Ƿ�������Ƿ��ж����赲
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
			if (by == ey && !(by == ey && bx == ex))//���ж��Ƿ���ˮƽ�����Ҳ���ԭ��
			{
				for (bx > ex ? i = 1 : i = -1; bx > ex ? i < bx - ex : i > bx - ex;
					bx > ex ? i++ : i--)//�ж��Ƿ�������Ƿ��ж����赲
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
		else//δ����
		{
			if (bx == ex && !(by == ey && bx == ex))//���ж��Ƿ�����ֱ�����Ҳ���ԭ��
			{
				for (by > ey ? i = 1 : i = -1; by > ey ? i < by - ey : i > by - ey;
					by > ey ? i++ : i--)//�ж��Ƿ�������Ƿ����赲
				{
					if (map_turn[turn][by - i][bx].sur)
					{
						return 0;
					}
				}
				return 1;
			}
			if (by == ey && !(by == ey && bx == ex))//���ж��Ƿ���ˮƽ�����Ҳ���ԭ��
			{
				for (bx > ex ? i = 1 : i = -1; bx > ex ? i < bx - ex : i > bx - ex;
					bx > ex ? i++ : i--)//�ж��Ƿ�������Ƿ����赲
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
	else if (!wcscmp(map_turn[turn][by][bx].name, L"��"))
	{
		if ((ex < 0 || ex > 8) && (ey < 5 || ey > 9))//�ж�����λ���Ƿ��ڱ�������
		{
			return 0;
		}
		if (ex % 2 || ex + ey != 7 && ex + ey != 11 && ex + ey != 15)//�ж������Ƿ�Ϊż������,�����Ƿ񲻷��Ϻ���
		{
			return 0;
		}
		if ((2 == by - ey || -2 == by - ey) && (2 == bx - ex || -2 == bx - ex))//�ж������Ƿ�Ϊ����һ���б��
		{
			if (map_turn[turn][(by + ey) / 2][(bx + ex) / 2].sur)//�ж��Ƿ�����֮���Ƿ�������
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
	else if (!wcscmp(map_turn[turn][by][bx].name, L"��"))
	{
		if ((ex < 0 || ex > 8) && (ey < 0 || ey > 4))//�ж�����λ���Ƿ��ڱ�������
		{
			return 0;
		}
		if (ex % 2 || ex + ey != 2 && ex + ey != 6 && ex + ey != 10)//�ж������Ƿ�Ϊż������,�����Ƿ񲻷��Ϻ���
		{
			return 0;
		}
		if ((2 == by - ey || -2 == by - ey) && (2 == bx - ex || -2 == bx - ex))//�ж������Ƿ�Ϊ����һ���б��
		{
			if (map_turn[turn][(by + ey) / 2][(by + ey) / 2].sur)//�ж��Ƿ�����֮���Ƿ�������
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
	else if (!wcscmp(map_turn[turn][by][bx].name, L"��"))
	{
		if ((ex < 3 || ex > 5) && (ey < 7 || ey > 9) && ex + ey != 11 &&
			ex + ey != 13)//�ж�����λ���Ƿ��ھŹ�����ض�λ����
		{
			return 0;
		}
		if ((1 == by - ey || -1 == by - ey) && (1 == bx - ex || -1 == bx - ex))//�ж��Ƿ�Ϊб���ƶ�һ��
		{
			return 1;
		}
		return 0;
	}
	else if (!wcscmp(map_turn[turn][by][bx].name, L"ʿ"))
	{
		if ((ex < 3 || ex > 5) && (ey < 0 || ey > 2) && ex + ey != 4 &&
			ex + ey != 6)//�ж�����λ���Ƿ��ھŹ�����ض�λ����
		{
			return 0;
		}
		if ((1 == by - ey || -1 == by - ey) && (1 == bx - ex || -1 == bx - ex))//�ж��Ƿ�Ϊб���ƶ�һ��
		{
			return 1;
		}
		return 0;
	}
	else if (!wcscmp(map_turn[turn][by][bx].name, L"��"))
	{
		if (map_turn[turn][by][bx].riv)//�ж��Ƿ����
		{
			if (-1 == ey - by && ex == bx)//�ж��Ƿ�Ϊǰһ����û�������ƶ�
			{
				return 1;
			}
			else if (ey == by && (ex - bx == 1 || ex - bx == -1))//�ж��Ƿ�Ϊ����ƽ�Ʋ���û�������ƶ�
			{
				return 1;
			}
		}
		else//û����
		{
			if (-1 == ey - by && ex == bx)//�ж��Ƿ�Ϊǰһ����û�������ƶ�
			{
				if (4 == ey)//�ж�λ���Ƿ����
				{
					map_turn[turn][by][bx].riv = 1;//��ֵΪ����
				}
				return 1;
			}
		}
		return 0;
	}
	else if (!wcscmp(map_turn[turn][by][bx].name, L"��"))
	{
		if (map_turn[turn][by][bx].riv)//�ж��Ƿ����
		{
			if (1 == ey - by && ex == bx)//�ж��Ƿ�Ϊǰһ����û�������ƶ�
			{
				return 1;
			}
			else if (ey == by && (ex - bx == 1 || ex - bx == -1))//�ж��Ƿ�Ϊ����ƽ�Ʋ���û�������ƶ�
			{
				return 1;
			}
		}
		else//û����
		{
			if (1 == ey - by && ex == bx)//�ж��Ƿ�Ϊǰһ����û�������ƶ�
			{
				if (5 == ey)//�ж�λ���Ƿ����
				{
					map_turn[turn][by][bx].riv = 1;//��ֵΪ����
				}
				return 1;
				return 1;
			}
		}
		return 0;
	}
	else if (!wcscmp(map_turn[turn][by][bx].name, L"˧"))
	{
		if ((ex < 3 || ex > 5) && (ey < 7 || ey > 9))//�ж�����λ���Ƿ��ھŹ�����
		{
			return 0;
		}
		if ((1 == by - ey || -1 == by - ey) && 0 == bx - ex ||
			(1 == bx - ex || -1 == bx - ex) && 0 == by - ey)//�ж��Ƿ�Ϊ�ƶ�һ��
		{
			return 1;
		}
		return 0;
	}
	else if (!wcscmp(map_turn[turn][by][bx].name, L"��"))
	{
		if ((ex < 3 || ex > 5) && (ey < 0 || ey > 7))//�ж�����λ���Ƿ��ھŹ�����
		{
			return 0;
		}
		if ((1 == by - ey || -1 == by - ey) && 0 == bx - ex ||
			(1 == bx - ex || -1 == bx - ex) && 0 == by - ey)//�ж��Ƿ�Ϊ�ƶ�һ��
		{
			return 1;
		}
		return 0;
	}
	else
	{
		printf("decide���������ˣ�δ�ҵ����ӵ�����");
		exit(1);
	}
}

void DarkBoardDraw()//��������
{
	int i = 0;
	setbkcolor(DCOLOR);//���ñ�����ɫ
	cleardevice();//ˢ�±���
	for (i = 0; i < ROW; i++)//������
	{
		line(SIZE, (i + 1) * SIZE, getwidth() - SIZE, (i + 1) * SIZE);
	}
	for (i = 0; i < COL; i++)//������
	{
		line((i + 1) * SIZE, SIZE, (i + 1) * SIZE, getheight() - SIZE);
	}
	rectangle(SIZE - 5, SIZE - 5, getwidth() - SIZE + 5, getheight() - SIZE + 5);//����߿�
	setfillcolor(DCOLOR);//���������ɫ
	fillrectangle(SIZE, (ROW / 2) * SIZE, getwidth() - SIZE, (ROW / 2 + 1) * SIZE);//�����м������
	line(4 * SIZE, SIZE, 6 * SIZE, 3 * SIZE);
	line(4 * SIZE, 3 * SIZE, 6 * SIZE, SIZE);
	line(4 * SIZE, 10 * SIZE, 6 * SIZE, 8 * SIZE);
	line(4 * SIZE, 8 * SIZE, 6 * SIZE, 10 * SIZE);//���������������ϵ�б��
	settextcolor(BLACK);//������ɫ����
	settextstyle(50, 0, L"����");//���ڳ��Ӻ������������
	outtextxy((getwidth() - textwidth(RIVER)) / 2,
		(getheight() - textheight(RIVER)) / 2, RIVER);//�����м���ϳ��Ӻ���
	settextstyle(FONT, 0, L"����");//�������ӵ���������
	setfillcolor(COLOR);//�ָ������ɫ
	setbkcolor(COLOR);//�ָ�������ɫ
}

void DarkPiecesDraw()//������
{
	setbkcolor(DCOLOR);//���ñ�����ɫ
	setfillcolor(DCOLOR);//���������ɫ
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (map_turn[turn][i][j].sur)
			{
				if (now)//�ж��ӽ�
				{
					fillcircle(SIZE * (j + 1), SIZE * (i + 1), ROU);//���ӵı���
					if (map_turn[turn][i][j].type)
					{
						settextcolor(RGB(120, 0, 0));//������ɫ����
						outtextxy(SIZE * (j + 1) - FONT / 2,
							SIZE * (i + 1) - FONT / 2, map_turn[turn][i][j].name);//���ӵ���
					}
					else
					{
						settextcolor(BLACK);//������ɫ����
						outtextxy(SIZE * (j + 1) - FONT / 2,
							SIZE * (i + 1) - FONT / 2, map_turn[turn][i][j].name);//���ӵ���
					}
				}
				else
				{
					fillcircle(SIZE * (COL - j), SIZE * (ROW - i), ROU);//���ӵı���
					if (map_turn[turn][i][j].type)
					{
						settextcolor(RGB(120, 0, 0));//������ɫ����
						outtextxy(SIZE * (COL - j) - FONT / 2,
							SIZE * (ROW - i) - FONT / 2, map_turn[turn][i][j].name);//���ӵ���
					}
					else
					{
						settextcolor(BLACK);//������ɫ����
						outtextxy(SIZE * (COL - j) - FONT / 2,
							SIZE * (ROW - i) - FONT / 2, map_turn[turn][i][j].name);//���ӵ���
					}
				}
			}
		}
	}
	setbkcolor(COLOR);//�ָ�������ɫ
	setfillcolor(COLOR);//�ָ������ɫ
}

void OverDraw()//���ƶԾֽ���
{
	BeginBatchDraw();//��ֹ��˸
	DarkBoardDraw();//������
	DarkPiecesDraw();//������
	fillroundrect((getwidth() - TAB_W) / 2, getheight() * 2 / 11,
		(getwidth() + TAB_W) / 2, getheight() * 4 / 11 + TAB_H, 370, 250);//����ѡ�
	fillroundrect(1.5 * SIZE, getheight() * 6 / 11, 4 * SIZE,
		getheight() * 6 / 11 + TAB_H, 30, 30);//����ѡ�
	fillroundrect(6 * SIZE, getheight() * 6 / 11, 8.5 * SIZE,
		getheight() * 6 / 11 + TAB_H, 30, 30);//����ѡ�
	settextcolor(BLACK);
	outtextxy((5.5 * SIZE - textwidth(tab[0])) / 2, getheight() * 6 / 11
		+ (TAB_H - textheight(tab[0])) / 2, tab[0]);//��ӡ"����Ϸ"
	outtextxy((14.5 * SIZE - textwidth(key[3])) / 2, getheight() * 6 / 11
		+ (TAB_H - textheight(key[3])) / 2, key[3]);//��ӡ"���ز˵�"
	settextstyle(getheight() / 11, 0, L"����");//��Ӧ����������
	if (!routype)//�ֱ��ʤ��
	{
		settextcolor(RED);
		outtextxy((getwidth() - textwidth(L"�췽ʤ��")) / 2, getheight() * 3 / 11
			+ 45 - textheight(L"�췽ʤ��") / 2, L"�췽ʤ��");// ��ӡ"�췽ʤ��"
	}
	else//�ֱ��ʤ��
	{
		settextcolor(BLACK);
		outtextxy((getwidth() - textwidth(L"�ڷ�ʤ��")) / 2, getheight() * 3 / 11
			+ 45 - textheight(L"�ڷ�ʤ��") / 2, L"�ڷ�ʤ��");// ��ӡ"�ڷ�ʤ��"
	}
	EndBatchDraw();//�ر�˫�����ͼ
	settextcolor(BLACK);
	settextstyle(FONT, 0, L"����");// �ָ���������
}

void FunctionKey()
{
	DarkBoardDraw();
	DarkPiecesDraw();
	setfillcolor(COLOR);//���������ɫ
	settextstyle(40, 0, L"����");//��������
	for (int i = 0; i < 4; i++)//����ѡ�
	{
		fillroundrect((getwidth() - TAB_W) / 2, getheight() * (i + 1) * 2 / 11, (getwidth() + TAB_W) / 2,
			getheight() * (i + 1) * 2 / 11 + TAB_H, 30, 60);
	}
	for (int i = 0; i < 4; i++)//��ӡ����
	{
		settextcolor(BLACK);
		settextstyle(40, 0, L"����");
		if (i == 1 && turn == 1)//��һ�ֵĻ���Ϊǳɫ
		{
			settextcolor(RGB(110, 110, 110));
			outtextxy((getwidth() - textwidth(key[i])) / 2,
				getheight() * (i + 1) * 2 / 11 + (TAB_H - textheight(key[i])) / 2, key[i]);
			continue;
		}
		outtextxy((getwidth() - textwidth(key[i])) / 2,
			getheight() * (i + 1) * 2 / 11 + (TAB_H - textheight(key[i])) / 2, key[i]);
	}
	settextstyle(FONT, 0, L"����");//�ָ�����
}

void HistoryDraw()//������ʷ��¼��ʽ
{
	BeginBatchDraw();//��ֹ��˸
	BoardDraw(SIZE_TRUMPET, FONT_TRUMPET, 32, 1);//������
	PiecesDraw(SIZE_TRUMPET, FONT_TRUMPET, ROU_TRUMPET, 1);//������
	for (int i = 1; i < 3; i++)//����ѡ�
	{
		roundrect(10 * SIZE_TRUMPET, (4 * i - 1) * SIZE_TRUMPET,
			getwidth() - SIZE_TRUMPET, (4 * i + 1) * SIZE_TRUMPET, 30, 30);
	}
	settextcolor(BLACK);
	settextstyle(30, 0, L"����");
	outtextxy((getwidth() + 9 * SIZE_TRUMPET - textwidth(L"���ز˵�")) / 2,
		5 * SIZE_TRUMPET - textwidth(L"���ز˵�") / 2, L"���ز˵�");
	if (oi)
	{
		outtextxy((getwidth() + 9 * SIZE_TRUMPET - textwidth(L"�˳�����")) / 2,
			9 * SIZE_TRUMPET - textwidth(L"�˳�����") / 2, L"�˳�����");
	}
	else
	{
		outtextxy((getwidth() + 9 * SIZE_TRUMPET - textwidth(L"��������")) / 2,
			9 * SIZE_TRUMPET - textwidth(L"��������") / 2, L"��������");
	}
	circle(190, 610, 85);//������Բ
	circle(510, 610, 85);//������Բ
	setfillcolor(DCOLOR);//���������ɫ
	setlinecolor(DCOLOR);//����������ɫ
	fillpolygon(point_left, 3);//����������
	fillpolygon(point_right, 3);//����������
	settextstyle(40, 0, L"����");
	setfillcolor(COLOR);//�ָ������ɫ
	setlinecolor(BLACK);//�ָ�������ɫ
	EndBatchDraw();//�ر�˫�����ͼ
}

void reset(Chess(*m)[COL], short x, short y)//����һ����ά�����һ��Ԫ��Ϊ������
{
	m[x][y].exist = 0;//��ֵΪ������
	wcscpy(m[x][y].name, L" ");//���ָ�ֵΪ��
	m[x][y].sur = 0;//����״̬��ֵΪ0
	m[x][y].type = 0;//���úں췽
}

void TestUpdate(short x, short y, short i, short j)//����һ�����̼���ƶ���Ľ������
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

bool Check(Chess(*ma)[COL])//��⼺���Ͻ��Ƿ񱻽���,����������0
{
	int i = 0, j = 0, u = 0, t = 0, m = 0, n = 0;
	if (routype)//���ֺ�ڷ�
	{
		for (i = 0; i < ROW; i++)//����˧��λ��
		{
			for (j = 0; j < COL; j++)
			{
				if (!wcscmp(ma[i][j].name, L"˧"))
				{
					u = i, t = j;
				}
			}
		}
		for (i = 0; i < ROW; i++)//���ڷ���������
		{
			for (j = 0; j < COL; j++)
			{
				if (ma[i][j].sur && !ma[i][j].type)//����Ƿ�Ϊ�ڷ�����
				{
					if (!wcscmp(ma[i][j].name, L"܇"))
					{
						n = 0;
						if (i == u)//����Ƿ��˧��ͬһ��
						{
							for (j > t ? m = 1 : m = -1; j > t ? m < j - t : m > j - t;
								j > t ? m++ : m--)//�ж��Ƿ�������Ƿ����赲
							{
								if (ma[i][j - m].sur)
								{
									n++;
								}
							}
						}
						else if (j == t)//����Ƿ��˧��ͬһ��
						{
							for (i > u ? m = 1 : m = -1; i > u ? m < i - u : m > i - u;
								i > u ? m++ : m--)//�ж��Ƿ�������Ƿ����赲
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
					else if (!wcscmp(ma[i][j].name, L"�R"))
					{
						if (t == j - 2 && (u == i - 1 || u == i + 1))//�ж������Ƿ�Ϊ����:�ж���
						{
							if (!ma[i][j - 1].sur)//�ж��Ƿ�û���赲
							{
								return 0;
							}
						}
						else if (t == j + 2 && (u == i - 1 || u == i + 1))//�ж������Ƿ�Ϊ����:�ж���
						{
							if (!ma[i][j + 1].sur)//�ж��Ƿ�û���赲
							{
								return 0;
							}
						}
						else if (u == i - 2 && (t == j - 1 || t == j + 1))//�ж������Ƿ�Ϊ����:�ж���
						{
							if (!ma[i - 1][j].sur)//�ж��Ƿ�û���赲
							{
								return 0;
							}
						}
						else if (u == i + 2 && (t == j - 1 || t == j + 1))//�ж������Ƿ�Ϊ����:�ж���
						{
							if (!ma[i + 1][j].sur)//�ж��Ƿ�û���赲
							{
								return 0;
							}
						}
					}
					else if (!wcscmp(ma[i][j].name, L"��"))
					{
						n = 0;
						if (i == u)//����Ƿ��˧��ͬһ��
						{
							for (j > t ? m = 1 : m = -1; j > t ? m < j - t : m > j - t;
								j > t ? m++ : m--)//�ж��Ƿ�������ж����赲
							{
								if (ma[i][j - m].sur)
								{
									n++;
								}
							}
						}
						else if (j == t)//����Ƿ��˧��ͬһ��
						{
							for (i > u ? m = 1 : m = -1; i > u ? m < i - u : m > i - u;
								i > u ? m++ : m--)//�ж��Ƿ�������ж����赲
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
					else if (!wcscmp(ma[i][j].name, L"��"))
					{
						if (i == u)//����Ƿ��˧��ͬһ��
						{
							if (t == j - 1 || t == j + 1)
							{
								return 0;
							}
						}
						else if (j == t)//����Ƿ��˧��ͬһ��
						{
							if (u == i + 1)
							{
								return 0;
							}
						}
					}
					else if (!wcscmp(ma[i][j].name, L"��"))
					{
						n = 0;
						if (j == t)//����Ƿ�ͽ���ͬһ��
						{
							for (i > u ? m = 1 : m = -1; i > u ? m < i - u : m > i - u;
								i > u ? m++ : m--)//�ж��Ƿ�������Ƿ����赲
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
		for (i = 0; i < ROW; i++)//���ڽ���λ��
		{
			for (j = 0; j < COL; j++)
			{
				if (!wcscmp(ma[i][j].name, L"��"))
				{
					u = i, t = j;
				}
			}
		}
		for (i = 0; i < ROW; i++)//���췽��������
		{
			for (j = 0; j < COL; j++)
			{
				if (ma[i][j].sur && ma[i][j].type)//����Ƿ�Ϊ�췽����
				{
					if (!wcscmp(ma[i][j].name, L"܇"))
					{
						n = 0;
						if (i == u)//����Ƿ�ͽ���ͬһ��
						{
							for (j > t ? m = 1 : m = -1; j > t ? m < j - t : m > j - t;
								j > u ? m++ : m--)//�ж��Ƿ�������Ƿ����赲
							{
								if (ma[i][j - m].sur)
								{
									n++;
								}
							}
						}
						else if (j == t)//����Ƿ�ͽ���ͬһ��
						{
							for (i > u ? m = 1 : m = -1; i > u ? m < i - u : m > i - u;
								i > u ? m++ : m--)//�ж��Ƿ�������Ƿ����赲
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
					else if (!wcscmp(ma[i][j].name, L"�R"))
					{
						if (t == j - 2 && (u == i - 1 || u == i + 1))//�ж������Ƿ�Ϊ����:�ж���
						{
							if (!ma[i][j - 1].sur)//�ж��Ƿ�û���赲
							{
								return 0;
							}
						}
						else if (t == j + 2 && (u == i - 1 || u == i + 1))//�ж������Ƿ�Ϊ����:�ж���
						{
							if (!ma[i][j + 1].sur)//�ж��Ƿ�û���赲
							{
								return 0;
							}
						}
						else if (u == i - 2 && (t == j - 1 || t == j + 1))//�ж������Ƿ�Ϊ����:�ж���
						{
							if (!ma[i - 1][j].sur)//�ж��Ƿ�û���赲
							{
								return 0;
							}
						}
						else if (u == i + 2 && (t == j - 1 || t == j + 1))//�ж������Ƿ�Ϊ����:�ж���
						{
							if (!ma[i + 1][j].sur)//�ж��Ƿ�û���赲
							{
								return 0;
							}
						}
					}
					else if (!wcscmp(ma[i][j].name, L"��"))
					{
						n = 0;
						if (i == u)//����Ƿ�ͽ���ͬһ��
						{
							for (j > t ? m = 1 : m = -1; j > t ? m < j - t : m > j - t;
								j > u ? m++ : m--)//�ж��Ƿ�������ж����赲
							{
								if (ma[i][j - m].sur)
								{
									n++;
								}
							}
						}
						else if (j == t)//����Ƿ�ͽ���ͬһ��
						{
							for (i > u ? m = 1 : m = -1; i > u ? m < i - u : m > i - u;
								i > u ? m++ : m--)//�ж��Ƿ�������ж����赲
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
					else if (!wcscmp(ma[i][j].name, L"��"))
					{
						if (i == u)//����Ƿ�ͽ���ͬһ��
						{
							if (t == j - 1 || t == j + 1)
							{
								return 0;
							}
						}
						else if (j == t)//����Ƿ�ͽ���ͬһ��
						{
							if (u == i + 1)
							{
								return 0;
							}
						}
					}
					else if (!wcscmp(ma[i][j].name, L"˧"))
					{
						n = 0;
						if (j == t)//����Ƿ��˧��ͬһ��
						{
							for (i > u ? m = 1 : m = -1; i > u ? m < i - u : m > i - u;
								i > u ? m++ : m--)//�ж��Ƿ�������Ƿ����赲
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
			if (map_turn[turn][i][j].sur && map_turn[turn][i][j].type == routype)//���������Ƿ�Ϊ��ǰ�غϵ�
			{
				if (!wcscmp(map_turn[turn][i][j].name, L"܇"))
				{
					for (u = i + 1; u < 10; u++)//������ֱ�����ƶ�
					{
						if (map_turn[turn][u][j].sur && map_turn[turn][u][j].type == routype)//��������
						{
							break;
						}
						else if (map_turn[turn][u][j].sur && map_turn[turn][u][j].type != routype)//��������
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
					for (u = i - 1; u > -1; u--)//������ֱ�ݼ��ƶ�
					{
						if (map_turn[turn][u][j].sur && map_turn[turn][u][j].type == routype)//��������
						{
							break;
						}
						else if (map_turn[turn][u][j].sur && map_turn[turn][u][j].type != routype)//��������
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
					for (t = j + 1; u < 9; u++)//����ˮƽ�����ƶ�
					{
						if (map_turn[turn][i][t].sur && map_turn[turn][i][t].type == routype)//��������
						{
							break;
						}
						else if (map_turn[turn][i][t].sur && map_turn[turn][i][t].type != routype)//��������
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
					for (t = j - 1; u > -1; u--)//����ˮƽ�����ƶ�
					{
						if (map_turn[turn][i][t].sur && map_turn[turn][i][t].type == routype)//��������
						{
							break;
						}
						else if (map_turn[turn][i][t].sur && map_turn[turn][i][t].type != routype)//��������
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
				else if (!wcscmp(map_turn[turn][i][j].name, L"�R"))
				{
					if (i > 1 && !map_turn[turn][i - 1][j].sur)//��
					{
						if (j > 0 && !(map_turn[turn][i - 2][j - 1].sur &&
							map_turn[turn][i - 2][j - 1].type == routype))//����
						{
							TestUpdate(i, j, i - 2, j - 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
						if (j < 8 && !(map_turn[turn][i - 2][j + 1].sur &&
							map_turn[turn][i - 2][j + 1].type == routype))//����
						{
							TestUpdate(i, j, i - 2, j + 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
					if (i < 8 && !map_turn[turn][i + 1][j].sur)//��
					{
						if (j > 0 && !(map_turn[turn][i + 2][j - 1].sur &&
							map_turn[turn][i + 2][j - 1].type == routype))//����
						{
							TestUpdate(i, j, i + 2, j - 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
						if (j < 8 && !(map_turn[turn][i + 2][j + 1].sur &&
							map_turn[turn][i + 2][j + 1].type == routype))//����
						{
							TestUpdate(i, j, i + 2, j + 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
					if (j > 1 && !map_turn[turn][i][j - 1].sur)//��
					{
						if (i < 9 && !(map_turn[turn][i + 1][j - 2].sur &&
							map_turn[turn][i + 1][j - 2].type == routype))//����
						{
							TestUpdate(i, j, i + 2, j - 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
						if (i > 0 && !(map_turn[turn][i - 1][j - 2].sur &&
							map_turn[turn][i - 1][j - 2].type == routype))//����
						{
							TestUpdate(i, j, i - 1, j - 2);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
					if (j < 7 && !map_turn[turn][i][j + 1].sur)//��
					{
						if (i < 9 && !(map_turn[turn][i + 1][j - 2].sur &&
							map_turn[turn][i + 1][j - 2].type == routype))//����
						{
							TestUpdate(i, j, i + 2, j - 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
						if (i > 0 && !(map_turn[turn][i - 1][j - 2].sur &&
							map_turn[turn][i - 1][j - 2].type == routype))//����
						{
							TestUpdate(i, j, i - 1, j - 2);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
				}
				else if (!wcscmp(map_turn[turn][i][j].name, L"��"))
				{
					t = 0;//���ڼ�����������
					for (u = i + 1; u < ROW; u++)//������ֱ�����ƶ�
					{
						if (!map_turn[turn][u][j].sur)//���жϲ�����
						{
							TestUpdate(i, j, u, j);
							if (Check(map_test))
							{
								return 1;
							}
						}
						else//�ж��ܷ����
						{
							t++;//���ڼ�����������
							if (2 == t)//���ڶ�������
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
					t = 0;//���ڼ�����������
					for (u = i - 1; u > -1; u--)//������ֱ�ݼ��ƶ�
					{
						if (!map_turn[turn][u][j].sur)//���жϲ�����
						{
							TestUpdate(i, j, u, j);
							if (Check(map_test))
							{
								return 1;
							}
						}
						else//�ж��ܷ����
						{
							t++;//���ڼ�����������
							if (2 == t)//���ڶ�������
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
					u = 0;//���ڼ�����������
					for (t = j + 1; t < COL; t++)//����ˮƽ�����ƶ�
					{
						if (!map_turn[turn][i][t].sur)//���жϲ�����
						{
							TestUpdate(i, j, i, t);
							if (Check(map_test))
							{
								return 1;
							}
						}
						else//�ж��ܷ����
						{
							u++;//���ڼ�����������
							if (2 == u)//���ڶ�������
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
					u = 0;//���ڼ�����������
					for (t = j - 1; t > -1; t--)//����ˮƽ�����ƶ�
					{
						if (!map_turn[turn][i][t].sur)//���жϲ�����
						{
							TestUpdate(i, j, i, t);
							if (Check(map_test))
							{
								return 1;
							}
						}
						else//�ж��ܷ����
						{
							u++;//���ڼ�����������
							if (2 == u)//���ڶ�������
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
				else if (!wcscmp(map_turn[turn][i][j].name, L"��"))
				{
					if (map_turn[turn][i][j].riv)//���˺�
					{
						if (i < ROW - 1)//��ǰ
						{
							TestUpdate(i, j, i + 1, j);
							if (Check(map_test))
							{
								return 1;
							}
						}
						if (j < COL - 1)//ˮƽ����
						{
							TestUpdate(i, j, i, j + 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
						if (j > 0)//ˮƽ�ݼ�
						{
							TestUpdate(i, j, i, j - 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
					else//û����
					{
						TestUpdate(i, j, i + 1, j);
						if (Check(map_test))
						{
							return 1;
						}
					}
				}
				else if (!wcscmp(map_turn[turn][i][j].name, L"��"))
				{
					if (map_turn[turn][i][j].riv)//���˺�
					{
						if (i > 0)//ˮƽ�ݼ�
						{
							TestUpdate(i, j, i - 1, j);
							if (Check(map_test))
							{
								return 1;
							}
						}
						if (j < COL - 1)//ˮƽ����
						{
							TestUpdate(i, j, i, j + 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
						if (j > 0)//ˮƽ�ݼ�
						{
							TestUpdate(i, j, i, j - 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
					else//û����
					{
						TestUpdate(i, j, i + 1, j);
						if (Check(map_test))
						{
							return 1;
						}
					}
				}
				else if (!wcscmp(map_turn[turn][i][j].name, L"˧"))
				{
					if (i < ROW - 1)//����
					{
						if (!(map_turn[turn][i + 1][j].sur && map_turn[turn][i + 1][j].type == routype))//�Ƿ������
						{
							TestUpdate(i, j, i + 1, j);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
					if (i > 7)//����
					{
						if (!(map_turn[turn][i - 1][j].sur && map_turn[turn][i - 1][j].type == routype))//�Ƿ������
						{
							TestUpdate(i, j, i - 1, j);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
					if (j > 3)//����
					{
						if (!(map_turn[turn][i][j - 1].sur && map_turn[turn][i][j - 1].type == routype))//�Ƿ������
						{
							TestUpdate(i, j, i, j - 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
					if (i < 5)//����
					{
						if (!(map_turn[turn][i][j + 1].sur && map_turn[turn][i][j + 1].type == routype))//�Ƿ������
						{
							TestUpdate(i, j, i, j + 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
				}
				else if (!wcscmp(map_turn[turn][i][j].name, L"��"))
				{
					if (i < 2)//����
					{
						if (!(map_turn[turn][i + 1][j].sur && map_turn[turn][i + 1][j].type == routype))//�Ƿ������
						{
							TestUpdate(i, j, i + 1, j);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
					if (i > 0)//����
					{
						if (!(map_turn[turn][i - 1][j].sur && map_turn[turn][i - 1][j].type == routype))//�Ƿ������
						{
							TestUpdate(i, j, i - 1, j);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
					if (j > 3)//����
					{
						if (!(map_turn[turn][i][j - 1].sur && map_turn[turn][i][j - 1].type == routype))//�Ƿ������
						{
							TestUpdate(i, j, i, j - 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
					if (i < 5)//����
					{
						if (!(map_turn[turn][i][j + 1].sur && map_turn[turn][i][j + 1].type == routype))//�Ƿ������
						{
							TestUpdate(i, j, i, j + 1);
							if (Check(map_test))
							{
								return 1;
							}
						}
					}
				}
				else if (!wcscmp(map_turn[turn][i][j].name, L"��"))
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
				else if (!wcscmp(map_turn[turn][i][j].name, L"��"))
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
				else if (!wcscmp(map_turn[turn][i][j].name, L"ʿ"))
				{
					if (!(map_turn[turn][i - 1][j - 1].sur && map_turn[turn][i - 1][j - 1].type == routype))//����
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
					if (!(map_turn[turn][i - 1][j + 1].sur && map_turn[turn][i - 1][j - 1].type == routype))//����
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
					if (!(map_turn[turn][i + 1][j + 1].sur && map_turn[turn][i - 1][j - 1].type == routype))//����
					{
						if (i == 1 && j == 4 || i == 0 && j == 3)
							TestUpdate(i, j, i + 1, j + 1);
						if (Check(map_test))
						{
							return 1;
						}
					}
					if (!(map_turn[turn][i + 1][j - 1].sur && map_turn[turn][i - 1][j - 1].type == routype))//����
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
				else if (!wcscmp(map_turn[turn][i][j].name, L"��"))
				{
					if (i == 8 && j == 4 || i == 9 && j == 5)
					{
						if (!(map_turn[turn][i - 1][j - 1].sur && map_turn[turn][i - 1][j - 1].type == routype))//����
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
						if (!(map_turn[turn][i - 1][j + 1].sur && map_turn[turn][i - 1][j - 1].type == routype))//����
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
						if (!(map_turn[turn][i + 1][j + 1].sur && map_turn[turn][i - 1][j - 1].type == routype))//����
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
						if (!(map_turn[turn][i + 1][j - 1].sur && map_turn[turn][i - 1][j - 1].type == routype))//����
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

//void AppendFile(short t)//׷��һ��t���ݵ��ļ�
//{
//	FILE* pf = fopen("chinese_chess.txt", "ab");
//	if (NULL == pf)
//	{
//		fprintf(stdout, "%s\n", strerror(errno));
//		exit(1);
//	}
//	fwrite(&map_turn[t], memsize_map, 1, pf);//д��Ծ���Ϣ
//	fclose(pf);
//	pf = NULL;
//}

void AppendAllFile()//׷��ȫ�����ݵ��ļ�
{
	FILE* pf = fopen("chinese_chess.txt", "ab");
	if (NULL == pf)
	{
		fprintf(stdout, "%s\n", strerror(errno));
		exit(1);
	}
	fwrite(&turn, 2, 1, pf);//��д������
	fwrite(map_turn, memsize_map, TURN, pf);//д��ȫ���Ծ���Ϣ
	fclose(pf);
	pf = NULL;
}

long ReadFile()//��ȡ�����Ϸȫ�����ݣ���������Ϸ��
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
	fread(&turn, sizeof(short), 1, pf);//��ȡһ��������
	fread(map_turn, memsize_map, TURN, pf);//��ȡһ��������Ϣ
	fseek(pf, 0, SEEK_SET);
	fclose(pf);
	pf = NULL;
	return i;
}

void GameControl()//�����Ϣ���ƾ�����Ϣ
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
					setlinecolor(BCOLOR);//ʹѡ�������ɫ����
					roundrect((getwidth() - TAB_W) / 2, getheight() * 2 / 11,
						(getwidth() + TAB_W) / 2, getheight() * 2 / 11 + TAB_H, 30, 60);//����ѡ��߿�
					setlinecolor(BLACK);//�ָ�ѡ�������ɫ
				}
				if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 4 * getheight() / 11 && msg.y < 4 * getheight() / 11 + TAB_H)
				{
					setlinecolor(BCOLOR);//ʹѡ�������ɫ����
					roundrect((getwidth() - TAB_W) / 2, getheight() * 2 * 2 / 11,
						(getwidth() + TAB_W) / 2, getheight() * 2 * 2 / 11 + TAB_H, 30, 60);//����ѡ��߿�
					setlinecolor(BLACK);//�ָ�ѡ�������ɫ
				}
				if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 6 * getheight() / 11 && msg.y < 6 * getheight() / 11 + TAB_H)
				{
					setlinecolor(BCOLOR);//ʹѡ�������ɫ����
					roundrect((getwidth() - TAB_W) / 2, getheight() * 3 * 2 / 11,
						(getwidth() + TAB_W) / 2, getheight() * 3 * 2 / 11 + TAB_H, 30, 60);//����ѡ��߿�
					setlinecolor(BLACK);//�ָ�ѡ�������ɫ
				}
				if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 8 * getheight() / 11 && msg.y < 8 * getheight() / 11 + TAB_H)
				{
					setlinecolor(BCOLOR);//ʹѡ�������ɫ����
					roundrect((getwidth() - TAB_W) / 2, getheight() * 4 * 2 / 11,
						(getwidth() + TAB_W) / 2, getheight() * 4 * 2 / 11 + TAB_H, 30, 60);//����ѡ��߿�
					setlinecolor(BLACK);//�ָ�ѡ�������ɫ
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
			if (!during)//��������˵�
			{
				for (int i = 0; i < 4; i++)//����ѡ�
				{
					roundrect((getwidth() - TAB_W) / 2, getheight() * (i + 1) * 2 / 11, (getwidth() + TAB_W) / 2,
						getheight() * (i + 1) * 2 / 11 + TAB_H, 30, 60);
				}
			}
			switch (during)
			{
			case 0:
				if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 2 * getheight() / 11 && msg.y < 2 * getheight() / 11 + TAB_H)//����Ϸ
				{
					is = 1;//����ʹ�ü�����Ϸ
					routype = 1;//�л��췽
					turn = 1;//������ʷ��¼
					GameUpdate(0);//��ʼ����һ��
					during = 1;//�ı�����ж��߼�Ϊ����
					BeginBatchDraw();//��ֹ��˸
					BoardDraw(SIZE, FONT, 50, 2);//������
					PiecesDraw(SIZE, FONT, ROU, 2);//������
					EndBatchDraw();//�ر�˫�����ͼ
				}
				else if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 4 * getheight() / 11 && msg.y < 4 * getheight() / 11 + TAB_H)//������Ϸ
				{
					if (is)
					{
						during = 1;//�ı�����ж��߼�Ϊ����
						BeginBatchDraw();//��ֹ��˸
						BoardDraw(SIZE, FONT, 50, 2);//������
						PiecesDraw(SIZE, FONT, ROU, 2);//������
						EndBatchDraw();//�ر�˫�����ͼ
					}
				}
				else if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 6 * getheight() / 11 && msg.y < 6 * getheight() / 11 + TAB_H)//��ʷս��
				{
					if (!(num = ReadFile()))//�ж��Ƿ�Ϊδ��ȡ������
					{
						break;
					}
					during = 2;//�ı�����߼�Ϊ��ʷ��¼
					HistoryDraw();//������ʷ��¼��ʽ
				}
				else if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 8 * getheight() / 11 && msg.y < 8 * getheight() / 11 + TAB_H)//�˳�
				{
					exit(0);
				}
				else
				{
					setlinecolor(BLACK);
					for (i = 0; i < 4; i++)
					{
						roundrect((getwidth() - TAB_W) / 2, getheight() * (i + 1) * 2 / 11, (getwidth()
							+ TAB_W) / 2, getheight() * (i + 1) * 2 / 11 + TAB_H, 30, 60);//�ָ�ѡ�
					}
				}
				break;
			case 1:
				if (msg.x <= (getwidth() + KEY) / 2 && msg.x >= (getwidth() - KEY) / 2 &&
					msg.y <= KEY_E && msg.y >= 0)//���ܼ�λ��
				{
					FunctionKey();//���ƹ��ܼ�
					during = 3;//�ж��߼���Ϊ��ݼ�
				}
				for (i = 0; i < ROW; i++)//��������,����λ��
				{
					for (j = 0; j < COL; j++)
					{
						if (-1 == Begin[0])//�ж���һ���Ƿ�û��ѡ������
						{
							if (map_turn[turn][i][j].sur && judge(now ? msg.x : getwidth() - msg.x,
								now ? msg.y : getheight() - msg.y, i, j))//�Դ��������жϵ��Ƿ������ķ�Χ
							{
								if (map_turn[turn][i][j].sur && routype == map_turn[turn][i][j].type)
									//�ж������Ƿ�Ϊ��Ӧ�غϷ�
								{
									Begin[0] = i, Begin[1] = j;//����ǰ���ӵ���Ϣ
								}
							}
						}
						else
						{
							if (judge(now ? msg.x : getwidth() - msg.x,
								now ? msg.y : getheight() - msg.y, i, j))//�ж��Ƿ�ѡ����ĳ������λ��
							{
								if (map_turn[turn][i][j].sur && map_turn[turn][i][j].type == routype)
									//�ж�λ���Ƿ�Ϊ�Ѿ�
								{
									Begin[0] = i, Begin[1] = j;//�л�ǰ���ӵ���Ϣ
								}
								else
								{
									End[0] = i, End[1] = j;//�����������Ϣ
									TestUpdate(Begin[0], Begin[1], End[0], End[1]);//����һ�����̼���ƶ��󽫾����
									if (decide(Begin, End) && Check(map_test))//�ж�λ���Ƿ����,�Ƿ�ᱻ����
									{
										map_turn[turn][End[0]][End[1]] = map_turn[turn][Begin[0]][Begin[1]];
										//��ʼλ�õ����Ӹ��Ƶ�ĩλ��
										reset(map_turn[turn], Begin[0], Begin[1]);//ԭʼλ�ñ�Ϊ������
										BeginBatchDraw();//��ֹ��˸
										BoardDraw(SIZE, FONT, 50, 2);//������
										PiecesDraw(SIZE, FONT, ROU, 2);//������
										EndBatchDraw();//�ر�˫�����ͼ
										routype = !routype;//�ж���ɽ�����ڵĻغ�
										Begin[0] = -1;//��Ϊδѡ������ģʽ
										if (!Check(map_turn[turn]))//�ж�
										{
											Judge = GameOver();//�ж���Ӯ
										}
										if (Judge)//���δ��ʤ��д����һ�غ�
										{
											GameUpdate(turn++);//��ǰ������Ϣд����һ�غ�,Ϊ���ж���׼��,���һغϼ�һ
										}
										else
										{
											OverDraw();//���ƶԾֽ���
											is = 0;//������ʹ�ü�����Ϸ
											AppendAllFile();//д������
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
					msg.y > 3 * SIZE_TRUMPET && msg.y < 5 * SIZE_TRUMPET)//���ز˵�
				{
					during = 0;//�߼���Ϊ���˵�
					oi = 0;//�л�ģʽ
					MainMenu();//�����˵�
				}
				else if (msg.x > 10 * SIZE_TRUMPET && msg.x < getwidth() - SIZE_TRUMPET &&
					msg.y > 7 * SIZE_TRUMPET && msg.y < 9 * SIZE_TRUMPET)//��������/�˳�����
				{
					oi = !oi;//�л�ģʽ
					if (oi)//��������
					{
						his = turn;//���Ծ�
						turn = 0;//���þ���
						HistoryDraw();//������ʷ
					}
					else//�˳�����
					{
						turn = his;//ĩ�Ծ�
						HistoryDraw();//������ʷ
					}
				}
				else if (7225 >= (190 - msg.x) * (190 - msg.x) + (610 - msg.y) * (610 - msg.y))//��ť
				{
					if (!oi)//δ����Ծּ�¼
					{
						if (history < num)//�ж��Ƿ�û�е�ͷ
						{
							history += 1;//����ǰһ���Ծ�
							ReadFile();//��ȡǰһ���Ծ�
							HistoryDraw();//������ʷ��¼��ʽ
						}
					}
					else//����Ծּ�¼
					{
						if (0 < turn)//�Ծ��Ƿ�û��ͷ
						{
							turn--;//���ڶԾּ�һ
							HistoryDraw();//������ʷ��¼��ʽ
						}
					}
				}
				else if (7225 >= (510 - msg.x) * (510 - msg.x) + (610 - msg.y) * (610 - msg.y))//�Ұ�ť
				{
					if (!oi)//δ����Ծּ�¼
					{
						if (history > 1)//�ж��Ƿ�û�е�ͷ
						{
							history -= 1;//������һ���Ծ�
							ReadFile();//��ȡ��һ���Ծ�
							HistoryDraw();//������ʷ��¼��ʽ
						}
					}
					else//����Ծּ�¼
					{
						if (his > turn)//�Ծ��Ƿ�û��ͷ
						{
							turn++;//���ڶԾּ�һ
							HistoryDraw();//������ʷ��¼��ʽ
						}
					}
				}
				break;
			case 3:
				if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 2 * getheight() / 11 && msg.y < 2 * getheight() / 11 + TAB_H)//������Ϸ
				{
					during = 1;//�ı�����ж��߼�Ϊ����
					BeginBatchDraw();//��ֹ��˸
					BoardDraw(SIZE, FONT, 50, 2);//������
					PiecesDraw(SIZE, FONT, ROU, 2);//������
					EndBatchDraw();//�ر�˫�����ͼ
				}
				else if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 4 * getheight() / 11 && msg.y < 4 * getheight() / 11 + TAB_H)//����
				{
					if (turn > 1)//�ж��Ƿ�Ϊ��һ��
					{
						GameUpdate(--turn - 1);//�˻�һ�ֲ��Ҹı��ӡ������
						during = 1;//�ı�����ж��߼�Ϊ����
						BeginBatchDraw();//��ֹ��˸
						BoardDraw(SIZE, FONT, 50, 2);//������
						PiecesDraw(SIZE, FONT, ROU, 2);//������
						routype = !routype;//�л���ڷ�
						EndBatchDraw();//�ر�˫�����ͼ
					}
				}
				else if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 6 * getheight() / 11 && msg.y < 6 * getheight() / 11 + TAB_H)//��ת����
				{
					now = !now;//��ת�ӽ�
					during = 1;//�ı�����ж��߼�Ϊ����
					BeginBatchDraw();//��ֹ��˸
					BoardDraw(SIZE, FONT, 50, 2);//������
					PiecesDraw(SIZE, FONT, ROU, 2);//������
					EndBatchDraw();//�ر�˫�����ͼ
				}
				else if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 8 * getheight() / 11 && msg.y < 8 * getheight() / 11 + TAB_H)//���ز˵�
				{
					during = 0;//�߼���Ϊ���˵�
					MainMenu();//�����˵�
				}
				else
				{
					setlinecolor(BLACK);
					for (i = 0; i < 4; i++)
					{
						roundrect((getwidth() - TAB_W) / 2, getheight() * (i + 1) * 2 / 11, (getwidth()
							+ TAB_W) / 2, getheight() * (i + 1) * 2 / 11 + TAB_H, 30, 60);//�ָ�ѡ�
					}
				}
				break;
			case 4:
				during = 1;
				BoardDraw(SIZE, FONT, 50, 2);//������
				PiecesDraw(SIZE, FONT, ROU, 2);//������
				break;
			default:
				break;
			}
			break;
		}
	}
}

void GameOverControl()//��Ϸ����ʱ�������Ϣ����
{
	if (MouseHit())
	{
		msg = GetMouseMsg();
		if (WM_LBUTTONUP == msg.uMsg)
		{
			if (1.5 * SIZE <= msg.x && getheight() * 6 / 11 <= msg.y && 4 * SIZE >= msg.x
				&& getheight() * 6 / 11 + TAB_H >= msg.y)//�жϵ�һ��ѡ�
			{
				is = 1;//����ʹ�ü�����Ϸ
				routype = 1;//�л��췽
				turn = 1;//������ʷ��¼
				GameUpdate(0);//��ʼ����һ��
				during = 1;//�ı�����ж��߼�Ϊ����
				BeginBatchDraw();//��ֹ��˸
				BoardDraw(SIZE, FONT, 50, 2);//������
				PiecesDraw(SIZE, FONT, ROU, 2);//������
				EndBatchDraw();//�ر�˫�����ͼ
				Judge = 1;//�ĶԾֽ����ж�Ϊδ����
			}
			if (6 * SIZE <= msg.x && getheight() * 6 / 11 <= msg.y && 8.5 * SIZE >= msg.x
				&& getheight() * 6 / 11 + TAB_H >= msg.y)
			{
				during = 0;//�߼���Ϊ���˵�
				MainMenu();//�����˵�
				Judge = 1;//�ĶԾֽ����ж�Ϊδ����
			}
		}
	}
}

int main()
{
	initgraph(700, 770);//��������
	Init();//��ʼ����ʽ����ɫ��
	GameInit();//��ʼ�����ӵ�
	for (int i = 0; i < ROW; i++)//�ѵ�ǰ����д��������������
	{
		for (int j = 0; j < COL; j++)
		{
			map_turn[0][i][j] = map[i][j];
		}
	}
	GameUpdate(0);//��ʼ����һ��
	MainMenu();//�����˵�
	do//������Ϸ�������ѭ��
	{
		while (Judge)//��ѭ��
		{
			GameControl();//�����Ϣ����
		}
		GameOverControl();//��Ϸ����ʱ�������Ϣ����
	} while (1);
	return 0;
}