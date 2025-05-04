#define _CRT_SECURE_NO_WARNINGS

#include <graphics.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 70//���Ӵ�С
#define ROW 10//��
#define COL 9//��
#define RIVER L"����        ����"//���������ӡ
#define LCOLOR RGB(250, 235, 195)//��ɫ
#define COLOR RGB(225, 190, 150)//������ɫ
#define ROU 30//���Ӱ뾶��С
#define FONT 36//���������С
#define TAB_W 400//ѡ���
#define TAB_H 90//ѡ���
#define TURN 100//���������

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

struct Chess map_turn[TURN][ROW][COL] = { 0 };

MOUSEMSG msg;

POINT begin = { -1, -1 }, end = { -1, -1 };

short Begin[] = { -1, -1 }, End[] = { -1, -1 };

bool routype = 1;//��¼�غ��Ǻ췽���Ǻڷ�
bool rouend = 1;//��¼�Ծ��Ƿ����
short during = 0;//����ж��߼�,0������,1��Ϸ��,2�鿴��ʷ
short turn = 1;//��¼�غ����ı���

const wchar_t* redlist[7] = { L"܇", L"�R", L"��", L"��", L"˧", L"��", L"��" };//��������
const wchar_t* blacklist[7] = { L"܇", L"�R", L"��", L"ʿ", L"��", L"��", L"��" };//��������
const wchar_t* tab[4] = { L"����Ϸ", L"������Ϸ", L"��ʷս��", L"�˳�" };//ѡ�����

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
	cleardevice();//ˢ�±���
	for (int i = 0; i < 4; i++)//����ѡ�
	{
		roundrect((getwidth() - TAB_W) / 2, getheight() * (i + 1) * 2 / 11, (getwidth() + TAB_W) / 2,
			getheight() * (i + 1) * 2 / 11 + TAB_H, 30, 60);//����4������:getheight() * ((i + 1) * 2 + 1) / 11
	}
	for (int i = 0; i < 4; i++)//��ӡ����
	{
		settextcolor(BLACK);
		settextstyle(40, 0, L"����");
		outtextxy((getwidth() - textwidth(tab[i])) / 2,
			getheight() * (i + 1) * 2 / 11 + (TAB_H - textheight(tab[i])) / 2, tab[i]);
	}
	settextstyle(FONT, 0, L"����");
}

void BoardDraw()//������
{
	int i = 0;
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
}

void PiecesDraw()//������
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (map_turn[turn][i][j].sur)
			{
				fillcircle(SIZE * (j + 1), SIZE * (i + 1), ROU);//���ӵı���
				if (map_turn[turn][i][j].type)
				{
					settextcolor(RED);//������ɫ����
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
			if (map_turn[turn][by > ey ? by - ey : ey - by][by > ey ? by - ey : ey - by].sur)//�ж��Ƿ�����֮���Ƿ�������
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
			if (map_turn[turn][by > ey ? by - ey : ey - by][by > ey ? by - ey : ey - by].sur)//�ж��Ƿ�����֮���Ƿ�������
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
	//else
	//{
	//	printf("decide���������ˣ�δ�ҵ����ӵ�����");
	//	exit(1);
	//}
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
					setlinecolor(LCOLOR);//ʹѡ�������ɫ����
					roundrect((getwidth() - TAB_W) / 2, getheight() * 2 / 11,
						(getwidth() + TAB_W) / 2, getheight() * 2 / 11 + TAB_H, 30, 60);//����ѡ��߿�
					setlinecolor(BLACK);//�ָ�ѡ�������ɫ
				}
				if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 4 * getheight() / 11 && msg.y < 4 * getheight() / 11 + TAB_H)
				{
					setlinecolor(LCOLOR);//ʹѡ�������ɫ����
					roundrect((getwidth() - TAB_W) / 2, getheight() * 2 * 2 / 11,
						(getwidth() + TAB_W) / 2, getheight() * 2 * 2 / 11 + TAB_H, 30, 60);//����ѡ��߿�
					setlinecolor(BLACK);//�ָ�ѡ�������ɫ
				}
				if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 6 * getheight() / 11 && msg.y < 6 * getheight() / 11 + TAB_H)
				{
					setlinecolor(LCOLOR);//ʹѡ�������ɫ����
					roundrect((getwidth() - TAB_W) / 2, getheight() * 3 * 2 / 11,
						(getwidth() + TAB_W) / 2, getheight() * 3 * 2 / 11 + TAB_H, 30, 60);//����ѡ��߿�
					setlinecolor(BLACK);//�ָ�ѡ�������ɫ
				}
				if (msg.x > (getwidth() - TAB_W) / 2 && msg.x < (getwidth() + TAB_W) / 2
					&& msg.y > 8 * getheight() / 11 && msg.y < 8 * getheight() / 11 + TAB_H)
				{
					setlinecolor(LCOLOR);//ʹѡ�������ɫ����
					roundrect((getwidth() - TAB_W) / 2, getheight() * 4 * 2 / 11,
						(getwidth() + TAB_W) / 2, getheight() * 4 * 2 / 11 + TAB_H, 30, 60);//����ѡ��߿�
					setlinecolor(BLACK);//�ָ�ѡ�������ɫ
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
					during = 1;//�ı�����ж��߼�Ϊ����
					BoardDraw();//������
					PiecesDraw();//������
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
							+ TAB_W) / 2, getheight() * (i + 1) * 2 / 11 + TAB_H, 30, 60);//�ָ�ѡ�
					}
				}
				break;
			case 1:
				for (i = 0; i < ROW; i++)//��������
				{
					for (j = 0; j < COL; j++)
					{
						if (-1 == Begin[0])//�ж���һ���Ƿ�û��ѡ������
						{
							if (map_turn[turn][i][j].sur && judge(msg.x, msg.y, i, j))//�Դ��������жϵ��Ƿ������ķ�Χ
							{
								if (map_turn[turn][i][j].sur && routype == map_turn[turn][i][j].type)//�ж������Ƿ�Ϊ��Ӧ�غϷ�
								{
									Begin[0] = i, Begin[1] = j;//����ǰ���ӵ���Ϣ
								}
							}
						}
						else
						{
							if (judge(msg.x, msg.y, i, j))//�ж��Ƿ�ѡ��������λ��
							{
								if (map_turn[turn][i][j].sur && map_turn[turn][i][j].type == routype)
									//�ж�λ���Ƿ�Ϊ�Ѿ�
								{
									Begin[0] = i, Begin[1] = j;//�л�ǰ���ӵ���Ϣ
									printf("xsb");

								}
								else
								{
									End[0] = i, End[1] = j;//�����������Ϣ
									if (decide(Begin, End))//�ж�λ���Ƿ����!!!!!!!!!!!!!(�����Ͻ��Ƿ񱻽���)
									{
										printf(" %d%d->%d%d ", Begin[0], Begin[1], End[0], End[1]);//����ʹ��
										
										map_turn[turn][End[0]][End[1]] = map_turn[turn][Begin[0]][Begin[1]];
											//��ʼλ�õ����Ӹ��Ƶ�ĩλ��
										map_turn[turn][Begin[0]][Begin[1]].exist = 0;
										wcscpy(map_turn[turn][Begin[0]][Begin[1]].name, L" ");
										map_turn[turn][Begin[0]][Begin[1]].sur = 0;
										map_turn[turn][Begin[0]][Begin[1]].type = 0;//ԭʼλ�ñ�Ϊ������
										BoardDraw();//������
										PiecesDraw();//������
										GameUpdate(turn++);//��ǰ������Ϣд����һ�غ�,Ϊ���ж���׼��,���һغϼ�һ
										routype = !routype;//�ж���ɽ�����ڵĻغ�
										Begin[0] = -1;//��Ϊδѡ������ģʽ
									}
									else//��������ʾһ��
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
	initgraph(700, 770, SHOWCONSOLE);//��������
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
	while (1)//��ѭ��
	{
		GameControl();//�����Ϣ����
	}
	return 0;
}