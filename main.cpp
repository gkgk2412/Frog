#include "MyImage.h"
#include "DSpriteManager.h"
#include "myCharacter.h"
#include "resource.h"
#include "meLeaf.h"
#include "MyKey.h"
#include "myFood.h"
#include "water.h"
#include "Enemy.h"
#include "myAnimal.h" //�䳢
#include "myAnimal_2.h" //�ٶ���
#include "myAnimal_3.h" //��
#include "myAnimal_4.h" //��
#include "myAnimal_5.h" // stage2 ��
#include <MMSystem.h> //playsound�Լ��������
#include "Digitalv.h" //mci �������


#define FROGY 70 //�������� ������ ��������� y ��ġ ����
#define FROGYSTAGE2_Y 50 //�������� �������� 2�� ������ y ��ġ ����
#define FROGYSTAGE2_X 180 //�������� �������� 2�� ������ x ��ġ ����


//���������ؼ�(playsound, mci)
/*========================================*/
#pragma comment(lib, "winmm.lib" ) 

MCI_OPEN_PARMS m_mciOpenParms;
MCI_PLAY_PARMS m_mciPlayParms;
DWORD m_dwDeviceID;
MCI_OPEN_PARMS mciOpen;
MCI_PLAY_PARMS mciPlay;

MCI_OPEN_PARMS mciOpen2;
MCI_PLAY_PARMS mciPlay2;

//���λ��� ����
int soundplayCheck; 
//��������2 ���� ����
int soundplayCheck2;


/*===========================================================*/

Graphics *g_BackBuffer;
Graphics *g_MainBuffer;
Bitmap   *g_Bitmap;

CHARACTER g_myChar;
LEAF g_myleaf; //����
KEY g_mykey; //�����
FOOD g_myfood; //����
ENEMY g_myenemy; //��
WATER g_mywater; //��
ANIMAL g_myanimal; //�䳢
ANIMAL2 g_myanimal2; //�ٶ���
ANIMAL3 g_myanimal3; //��
ANIMAL4 g_myanimal4; //��
ANIMAL5 g_myanimal5; // stage 2 ��
					 
//���ӽ������� (false�� ��Ʈ��, true�� ���ӽ���)
bool isGameStart = false;



/*===========================================================*/
//������ ���ٿ� ���� �ʾ��� ���� ����
#define MMAX_JUMP	16
POINT g_pt[] = {{ 0,-20 },{ 0,-40 },{ 0,-50 },{ 0,-60 }
			 ,{ 0,60 } ,{ 0,50 },{ 0,40 }, { 0,20 } , { 0,0 },{ 0,0 },{ 0,0 },{ 0,0 },{ 0,0 },{ 0,0 },{ 0,0 },{ 0,0 } };

int g_jmpIndex = 0;
bool g_isJump = false;
 

//������ ���ٿ� ����� ���� ����
#define MMAX_JUMP2	20
POINT g_pt2[] = { { 0,-20 },{ 0,-40 },{ 0,-50 },{ 0,-60 },{ 0,-80 },{ 0,-90 }
,{ 0,90 } ,{ 0,80 } ,{ 0,60 } ,{ 0,50 },{ 0,40 },{ 0,20 } ,{ 0,0 },{ 0,0 },{ 0,0 },{ 0,0 },{ 0,0 },{ 0,0 },{ 0,0 },{ 0,0 } };

int g_jmpIndex2 = 0;
bool g_isJump2 = false;
/*===========================================================*/

bool isScore[3] = { false,false,false };


DWORD g_duration = 0; //�ð�

DWORD g_Interval = 0; //������
DWORD g_Interval2 = 0; //�䳢
DWORD g_Interval3 = 0; //�ٶ���
DWORD g_Interval4 = 0; //��
DWORD g_Interval5 = 0; //��
DWORD g_Interval6 = 0; //stage 2 ��


void OnUpdate(HWND hWnd, DWORD tick);
void CreateBuffer(HWND hWnd, HDC hDC);
void ReleaseBuffer(HWND hWnd, HDC hDC);
void MYImageDraw(HWND hWnd, DWORD tick);
void MYImageHit(HWND hWnd);
void Initial_content();

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,    int nCmdShow)
{
	WNDCLASS   wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wndclass.hCursor = NULL;
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = "HelloWin";
	MSGBOXPARAMS m_MsgParam = { 0, };

	
	if(RegisterClass(&wndclass) == 0)
	{
		return 0;		
	}

	HWND hwnd = CreateWindow("HelloWin", "Game Window!!", 
                              WS_OVERLAPPEDWINDOW,
                              400, 70, 
                              900, 900,
                              NULL, NULL, hInstance, NULL);

	if(hwnd == NULL)
	{
		return 0;
	}

	HDC hDC = GetDC(hwnd);

	CreateBuffer(hwnd, hDC);

	//����
	/*=====================================================================*/

		mciOpen.lpstrElementName = "./Data/sound/MAIN.mp3";
		mciOpen.lpstrDeviceType = "mpegvideo";

		mciOpen2.lpstrElementName = "./Data/sound/STAGE2.mp3";
		mciOpen2.lpstrDeviceType = "mpegvideo";

		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
			(DWORD)(LPVOID)&mciOpen);

		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
			(DWORD)(LPVOID)&mciOpen2);


		soundplayCheck = mciOpen.wDeviceID;
		soundplayCheck2 = mciOpen2.wDeviceID;


		mciSendCommand(soundplayCheck, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // ��ӹݺ� //��������
			(DWORD)(LPVOID)&m_mciPlayParms);


	/*=====================================================================*/



	/*�̹������� �ҷ�����*/
	/*=====================================================================*/

	//����
	for (int i = 0; i < MAXLEAF; i++)
	{
		g_myleaf.LoadImage_leaf("./Data/Image/LEAF.png", i);
		//g_myleaf.LoadImage_leaf("./Data/Image/lotusleaf.png", i);
	}
	
	//����
	for (int i = 0; i < MAXKEY; i++)
	{
		g_mykey.LoadImage_key("./Data/Image/fish.png", i);
	}

	//��
	for (int i = 0; i < MAXENEMY; i++)
	{
		g_myenemy.LoadImage_enemy("./Data/Image/spike.png", i);
	}

	//����
	for (int i = 0; i < MAXFOOD; i++)
	{
		g_myfood.LoadImage_food("./Data/Image/food.png", i);
	}

	//��
	for (int i = 0; i < MAXWATER; i++)
	{
		g_mywater.LoadImage_water("./Data/Image/water.png", i);
	}


	//����
	g_myanimal.LoadImage_animal("./Data/Image/rabbit.png", 0);
	g_myanimal2.LoadImage_animal2("./Data/Image/squirrel.png", 0);
	g_myanimal3.LoadImage_animal3("./Data/Image/snake.png", 0);
	g_myanimal4.LoadImage_animal4("./Data/Image/bird1.png", 0);
	g_myanimal5.LoadImage_animal5("./Data/Image/birdstage2.png", 0);

	//������Ʈ
	g_myChar.LoadImage("./Data/Image/wall.png", 0);
	g_myChar.LoadImage("./Data/Image/floor.png", 1);
	g_myChar.LoadImage("./Data/Image/sidewall.png", 2);
	g_myChar.LoadImage("./Data/Image/sidewall_2.png", 3);
	g_myChar.LoadImage("./Data/Image/frog.png", 4);
	g_myChar.LoadImage("./Data/Image/sky.png", 5);
	g_myChar.LoadImage("./Data/Image/press.png", 6);
	g_myChar.LoadImage("./Data/Image/title3.png", 7);
	g_myChar.LoadImage("./Data/Image/100.png", 8);
	g_myChar.LoadImage("./Data/Image/200.png", 9);
	g_myChar.LoadImage("./Data/Image/300.png", 10);
	g_myChar.LoadImage("./Data/Image/Background2.png", 11);
	//g_myChar.LoadImage("./Data/Image/water.png", 12);
	g_myChar.LoadImage("./Data/Image/Cloud.png", 13);
	g_myChar.LoadImage("./Data/Image/fish.png", 14);
	g_myChar.LoadImage("./Data/Image/Grass.png", 15);



	ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
	/*=====================================================================*/


	//���������� �о����
	/*=====================================================================*/

	g_myChar.LoadAniInfo("ani_data_01.txt"); //������
	g_myleaf.LoadAniInfo_leaf("ani_data_02.txt"); //����
	g_myfood.LoadAniInfo_food("ani_data_03.txt"); //����
	g_myanimal.LoadAniInfo_animal("ani_data_04.txt"); //�䳢
	g_myanimal2.LoadAniInfo_animal2("ani_data_05.txt"); //�ٶ���
	g_myanimal3.LoadAniInfo_animal3("ani_data_06.txt"); //��
	g_myanimal4.LoadAniInfo_animal4("ani_data_07.txt"); //��
	g_myanimal5.LoadAniInfo_animal5("ani_data_08.txt"); //stage 2 ��
	g_mykey.LoadAniInfo_key("ani_data_09.txt"); //stage 2 ����
	g_myenemy.LoadAniInfo_enemy("ani_data_10.txt"); //stage 2 ��
	g_mywater.LoadAniInfo_water("ani_data_11.txt"); //stage 2 ��


	//�ʱ�����
	Initial_content();

	MSG msg;
	DWORD tick = GetTickCount();
	while (1)
	{
		//������ �޼����� ������� �޼����� ó���Ѵ�.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		else //�޼����� ���� ��� ���� ������ �����Ѵ�.
		{
			char buff[128];

			DWORD curTick = GetTickCount();
			DWORD deltatime = curTick - tick;

			OnUpdate(hwnd, deltatime);
			tick = curTick;

			g_duration += deltatime;
			//wsprintf(buff, "g_duration = %d", g_duration);
			wsprintf(buff, "�칰���� ������");
			SetWindowText(hwnd, buff);

			g_MainBuffer->DrawImage(g_Bitmap, 0, 0);
		}
	}

	ReleaseBuffer(hwnd, hDC);

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
	{
	}
	break;
	case WM_LBUTTONDOWN:
	{
	}
	break;
	case WM_CREATE:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}


	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void CreateBuffer(HWND hWnd, HDC hDC)
{
	GdiplusStartupInput			gdiplusStartupInput;
	ULONG_PTR					gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	RECT rc;
	GetClientRect(hWnd, &rc);

	g_Bitmap = new Bitmap(rc.right - rc.left, rc.bottom - rc.top);
	g_BackBuffer = new Graphics(g_Bitmap);
	g_BackBuffer->SetPageUnit(Gdiplus::Unit::UnitPixel);

	g_MainBuffer = new Graphics(hDC);
	g_MainBuffer->SetPageUnit(Gdiplus::Unit::UnitPixel);
}

void ReleaseBuffer(HWND hWnd, HDC hDC)
{
	ReleaseDC(hWnd, hDC);

	delete g_Bitmap;
	delete g_BackBuffer;
	delete g_MainBuffer;
}

void OnUpdate(HWND hWnd, DWORD tick)
{

	//���ӽ����� �ȴ�������
	if (isGameStart == false)
	{
		MYImageDraw(hWnd, tick);

		BYTE  key[256];
		::GetKeyboardState(key);


		//ESCŰ ����
		if (key[VK_ESCAPE] & 0x80)
		{

			if (MessageBox(hWnd, "������ �����Ͻðھ��?", "����", MB_YESNO) == IDYES)
			{
				exit(0);
			}

			else
			{}
		}


		//�����̽��� ������ ���ӽ���
		if (key[VK_SPACE] & 0x80)
		{
			isGameStart = true;
		}
	}
	
	
	//���ӽ����� ��������
	if (isGameStart == true)
	{
		if (hWnd == NULL)
			return;

		BYTE  key[256];
		::GetKeyboardState(key);

		//��������
		int xDir = 0, yDir = 0;

		//ESCŰ ����
		if (key[VK_ESCAPE] & 0x80)
		{

			if (MessageBox(hWnd, "������ �����Ͻðھ��?", "����", MB_YESNO) == IDYES)
			{
				exit(0);
			}

			else
			{}
		}

		//������Ű ����
		if (key[VK_RIGHT] & 0x80)
		{
			if (g_myChar.isStage == false)
			{
				/*�������� ������ ���� �浹ó��*/
				//���������ٿ� �����ʾ�����
				if (g_myleaf.isHit[16] == true)
				{
					//�������� x��ǥ�� 750���� ũ�ų� ������
					if (g_myChar.x >= 750)
					{
						//�������� x��ǥ�� 749�� ������ش�.
						g_myChar.x = 749;
					}

				}

			}
	
			//�������� �ִϸ��̼��� ���������θ�����ش�.
			g_myChar.isLeft = false;
		}

		//����Ű ����
		if (key[VK_LEFT] & 0x80)
		{
			if (g_myChar.isStage == false)
			{
				/*�������� ���ʺ����� �浹ó��*/
				//���������ٿ� �����ʾ��� ���
				if (g_myleaf.isHit[16] == true)
				{
					//�������� x��ǥ�� 50���� �۰ų� �������
					if (g_myChar.x <= 50)
					{
						//�������� x��ǥ�� 51�� ������ش�.
						g_myChar.x = 51;
					}
				}
			}
			//�������� �ִϸ��̼��� �������� ������ش�.
			g_myChar.isLeft = true;
		}

		//�����̽�Ű ����
		if (key[VK_SPACE] & 0x80)
		{
			//��������
			if (g_isJump == false)
			{
				sndPlaySoundA("./Data/sound/jumpsound.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
			}
			//�������� �ִϸ��̼��� ������ְ�
			g_myChar.isMove = true;

			//������ �����ʾ��� ���
			if (g_isJump == false)
			{
				//������ �ڰɷθ�����ش�.
				g_isJump = true;
				g_jmpIndex = 0;
			}
		}

		//���� && �����̽�Ű ����
		if ((key[VK_LEFT] & 0x80) && (key[VK_SPACE] & 0x80))
		{
			//�������� �ִϸ��̼��� ������ְ�
			g_myChar.isMove = true;

			//���ʸ���� ���
			g_myChar.isLeft = true;

			//�������� �̵��� ������(����)���� ������ش�.
			xDir = -1;

			//�������� �����ʾ������
			if (g_isJump == false)
			{
				//�ڰɷ� ������ش�
				g_isJump = true;
				g_jmpIndex = 0;
			}

			//�������� ������ ���ٿ� �浹���� ��� �׸��� �������� 1�� ���
			if (g_myleaf.isHit[16] == false && g_myChar.isStage == false)
			{
				//junp2�� ���̷� ������ �Ѵ�.
				if (g_isJump2 == false)
				{
					g_isJump2 = true;
					g_jmpIndex2 = 0;
				}
			}


		}

		//������ && �����̽� Ű ����
		if ((key[VK_RIGHT] & 0x80) && (key[VK_SPACE] & 0x80))
		{
			//�������� �ִϸ��̼��� ������ְ�
			g_myChar.isMove = true;

			//�������� �����ʾִϸ��̼��� ����Ѵ�.
			g_myChar.isLeft = false;

			//�������� �̵��� ���(������)���� ������ش�.
			xDir = 1;

			//������ ���� �ʾ��� ���
			if (g_isJump == false)
			{
				//�����Ѱ����� ������ش�
				g_isJump = true;
				g_jmpIndex = 0;
			}

			//�������� ������������ ����� ��� �׸��� �������� 1�� ���
			if (g_myleaf.isHit[16] == false && g_myChar.isStage == false)
			{
				//jump2�� ���̷� �����Ѵ�.
				if (g_isJump2 == false)
				{
					g_isJump2 = true;
					g_jmpIndex2 = 0;
				}
			}

		}


		//������Ű�� ��Ʈ��Ű(��������2�� ��������)
		if ((key[VK_CONTROL] & 0x80) && (key[VK_RIGHT] & 0x80))
		{	

			//��������
			if (g_isJump == false)
			{
				sndPlaySoundA("./Data/sound/jumpsound.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
			}

			

			//�������� �ִϸ��̼��� ������ְ�
			g_myChar.isMove = true;

			//�������� �̵��� ���(������)���� ������ش�.
			xDir = 1;

			if (g_myChar.isStage == true)     
			{
				//junp2�� ���̷� ������ �Ѵ�.
				if (g_isJump2 == false)
				{
					g_isJump2 = true;
					g_jmpIndex2 = 0;
				}
			}

		}


		//����Ű�� ��Ʈ��Ű(��������2�� ��������)
		if ((key[VK_CONTROL] & 0x80) && (key[VK_LEFT] & 0x80))
		{

			//��������
			if (g_isJump == false)
			{
				sndPlaySoundA("./Data/sound/jumpsound.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
			}

			

			//�������� �ִϸ��̼��� ������ְ�
			g_myChar.isMove = true;

			//�������� �̵��� ���(������)���� ������ش�.
			xDir = -1;

			if (g_myChar.isStage == true)
			{
				//junp2�� ���̷� ������ �Ѵ�.
				if (g_isJump2 == false)
				{
					g_isJump2 = true;
					g_jmpIndex2 = 0;
				}
			}

		}


		g_myChar.Move(xDir, yDir, tick);

		//����
		if (g_isJump)
		{
			g_myChar.x += g_pt[g_jmpIndex].x;
			g_myChar.y += g_pt[g_jmpIndex].y;
			g_jmpIndex++;

			if (g_jmpIndex >= MMAX_JUMP - 8) //�ִϸ��̼� �����̸� �� (8��ŭ)
			{
				g_isJump = false;
				g_myChar.isMove = false;
			}

		}


		//���� (�������� ���������ٿ� ����� ���)
		if (g_myleaf.isHit[16] == false || g_myChar.isStage == true)
		{
			if (g_isJump2)
			{
				g_myChar.x += g_pt2[g_jmpIndex2].x;
				g_myChar.y += g_pt2[g_jmpIndex2].y;
				g_jmpIndex2++;

				if (g_jmpIndex2 >= MMAX_JUMP2 - 8)
				{
					g_isJump2 = false;
					g_myChar.isMove = false;
				}

			}

		}

		MYImageDraw(hWnd, tick); //�̹����ϴ� ����Լ�

		//����Ʈ�� �ִ� �κ�
		if (g_myChar.isQuest == false)
		{
			if(MessageBox(hWnd, "�������� �칰Ż���� �����ּ���!! \n�ö󰡴� ���� ������ ������ ���� �� �־��!\n�� ������ ������ �䳢�� �ٶ���, ���̿���\n�� �����鿡�� �������� ������ �ʵ��� ������ �־���մϴ�.\n�칰���� ������ ������ ����ּ���. ������ 1�� �̻��� �������� ������ �� �����\n�׷� ������!!", "����Ʈ�� �����߽��ϴ�.", MB_OK) == IDOK);
			{
				g_myChar.isQuest = true; // �޼��� �ڽ��� ���̻� ��µ����ʵ��� isQuest�� true�� ������ش�.
			}			
		}

		MYImageHit(hWnd); //�浹ó���Լ�
	}

	
}


//�ʱ�����
void Initial_content()
{
	//ĳ��������
	/*=====================================================================*/

	//�������� ����
	g_myChar.speed = 500;
	g_myChar.x = 60;
	g_myChar.y = 745;
	g_myChar.isCreat = true;


	//�䳢�� ����
	g_myanimal.x[0] = 750;
	g_myanimal.y[0] = 0;
	g_myanimal.Check1 = false;
	g_myanimal.Check2 = false;


	//�ٶ����� ����
	g_myanimal2.x[0] = 640;
	g_myanimal2.y[0] = 90;
	g_myanimal2.Check1 = false;
	g_myanimal2.Check2 = false;


	//���� ����
	g_myanimal3.x[0] = 740;
	g_myanimal3.y[0] = 120;
	g_myanimal3.Check1 = false;
	g_myanimal3.Check2 = false;

	//���� ����
	g_myanimal4.x[0] = 100;
	g_myanimal4.y[0] = 100;
	g_myanimal4.speed = 300;
	g_myanimal4.xDir = -1;


	//stage 2 ���� ����
	g_myanimal5.x[0] = 100;
	g_myanimal5.y[0] = 300;
	g_myanimal5.speed = 50;
	g_myanimal5.xDir = 1;


	
	//��������
	/*=====================================================================*/

	/*1*/
	g_myleaf.x[0] = 180;
	g_myleaf.y[0] = 640;
	g_myleaf.isHit[0] = true;
	g_myleaf.check[0] = false;

	/*2*/
	g_myleaf.x[1] = 520;
	g_myleaf.y[1] = 520;
	g_myleaf.isHit[1] = true;
	g_myleaf.check[1] = false;

	/*3*/
	g_myleaf.x[2] = 150;
	g_myleaf.y[2] = 320;
	g_myleaf.isHit[2] = true;
	g_myleaf.check[2] = false;

	/*4*/
	g_myleaf.x[3] = 460;
	g_myleaf.y[3] = 120;
	g_myleaf.isHit[3] = true;
	g_myleaf.check[3] = false;

	/*5*/
	g_myleaf.x[4] = 660;
	g_myleaf.y[4] = 100;
	g_myleaf.isHit[4] = true;
	g_myleaf.check[4] = false;

	/*6*/
	g_myleaf.x[5] = 240;
	g_myleaf.y[5] = 160;
	g_myleaf.isHit[5] = true;
	g_myleaf.check[5] = false;

	/*7*/
	g_myleaf.x[6] = 540;
	g_myleaf.y[6] = 180;
	g_myleaf.isHit[6] = true;
	g_myleaf.check[6] = false;

	/*8*/
	g_myleaf.x[7] = 240;
	g_myleaf.y[7] = 160;
	g_myleaf.isHit[7] = true;
	g_myleaf.check[7] = false;

	/*9*/
	g_myleaf.x[8] = 80;
	g_myleaf.y[8] = 150;
	g_myleaf.isHit[8] = true;
	g_myleaf.check[8] = false;

	/*10*/
	g_myleaf.x[9] = 350;
	g_myleaf.y[9] = 150;
	g_myleaf.isHit[9] = true;
	g_myleaf.check[9] = false;

	/*11*/
	g_myleaf.x[10] = 540;
	g_myleaf.y[10] = 150;
	g_myleaf.isHit[10] = true;
	g_myleaf.check[10] = false;

	/*12*/
	g_myleaf.x[11] = 340;
	g_myleaf.y[11] = 150;
	g_myleaf.isHit[11] = true;
	g_myleaf.check[11] = false;

	/*13*/
	g_myleaf.x[12] = 640;
	g_myleaf.y[12] = 150;
	g_myleaf.isHit[12] = true;
	g_myleaf.check[12] = false;

	/*14*/
	g_myleaf.x[13] = 340;
	g_myleaf.y[13] = 150;
	g_myleaf.isHit[13] = true;
	g_myleaf.check[13] = false;

	/*15*/
	g_myleaf.x[14] = 80;
	g_myleaf.y[14] = 150;
	g_myleaf.isHit[14] = true;
	g_myleaf.check[14] = false;

	/*16*/
	g_myleaf.x[15] = 350;
	g_myleaf.y[15] = 150;
	g_myleaf.isHit[15] = true;
	g_myleaf.check[15] = false;

	/*17*/
	g_myleaf.x[16] = 640;
	g_myleaf.y[16] = 150;
	g_myleaf.isHit[16] = true;
	g_myleaf.check[16] = false;

	/*18*/
	g_myleaf.x[17] = 340;
	g_myleaf.y[17] = 150;
	g_myleaf.isHit[17] = true;
	g_myleaf.check[17] = false;

	/*19*/
	g_myleaf.x[18] = 140;
	g_myleaf.y[18] = 150;
	g_myleaf.isHit[18] = true;
	g_myleaf.check[18] = false;




	/*��������*/
	/*=====================================================================*/

	//������
	g_myfood.x[0] = 180;
	g_myfood.y[0] = 260;
	g_myfood.isCreat[0] = true;

	//���
	g_myfood.x[1] = 400;
	g_myfood.y[1] = 740;
	g_myfood.isCreat[1] = true;

	//���ٱ�
	g_myfood.isCreat[2] = false;

	//����
	g_myfood.x[3] = 270;
	g_myfood.y[3] = 110;
	g_myfood.isCreat[3] = true;

	//���丮
	g_myfood.x[4] = 750;
	g_myfood.y[4] = 160;
	g_myfood.isCreat[4] = true;

	//���
	g_myfood.x[5] = 750;
	g_myfood.y[5] = 80;
	g_myfood.isCreat[5] = true;

	//����
	g_myfood.x[6] = 110;
	g_myfood.y[6] = 80;
	g_myfood.isCreat[6] = true;
	/*=====================================================================*/

	//���� �䳢, �ٶ���, ���� ���, ���丮, ��⸦ �Ծ������� üũ
	g_myfood.isanimalEat = false;
	g_myfood.isanimalEat2 = false;
	g_myfood.isanimalEat3 = false;

	
	/*==================================================================*/
}


//�̹����׸�, ����������
void MYImageDraw(HWND hWnd, DWORD tick)
{
	Color color(255, 255, 255);
	g_BackBuffer->Clear(color);

	//���ӽ����� �������ʾ�����
	if (isGameStart == false)
	{
		g_myChar.image[0].Draw(g_BackBuffer, -1, 0); //�޹�� ���
		g_myChar.image[1].Draw(g_BackBuffer, 0, 320);//�ٴ�
		g_myChar.image[2].Draw(g_BackBuffer, 0, 0);//���ʺ�
		g_myChar.image[3].Draw(g_BackBuffer, 460, 0);//�����ʺ�
		g_myChar.image[6].Draw(g_BackBuffer, -25, 300);//pressthespace
		g_myChar.image[7].Draw(g_BackBuffer, -20, -150);//Ÿ��Ʋ
	}



	//���ӽ����� ��������
	if (isGameStart == true)
	{
		//�������� 1�� ���
		if (g_myChar.isStage == false)
		{
			/*�̹������*/

			//�������� ������ ���ٿ� �ö�Ÿ�� �ʾ��� ��, �޹�� ���
			if (g_myleaf.isHit[16] == true)
			{
				g_myChar.image[0].Draw(g_BackBuffer, 0, 0);
			}

			//�������� ������ ���ٿ� �ö����� ��, �޹�� ���
			if (g_myleaf.isHit[16] == false)
			{
				g_myChar.image[0].Draw(g_BackBuffer, 0, 200);
			}

			//�������� ������ ���ٿ� �ö����� ��, �ϴ�, �칰���� ���
			if (g_myleaf.isHit[16] == false)
			{
				g_myChar.image[5].Draw(g_BackBuffer, 0, -500);
				//g_myChar.image[6].Draw(g_BackBuffer, 0, -370); //���°� ����?

			}

			//�������� 4��° ���ٿ� �ö�Ÿ���ʾ�����, �ٴ���� => �ö�Ÿ�� �ٴ� ���X
			if (g_myleaf.isHit[1] == true)
			{
				g_myChar.image[1].Draw(g_BackBuffer, 0, 320);//�ٴ�
			}

			//�������� ������ ���ٿ� �ö�Ÿ�� �ʾ��� ��, �� ���
			if (g_myleaf.isHit[16] == true)
			{
				g_myChar.image[2].Draw(g_BackBuffer, 0, 0);//���ʺ�
				g_myChar.image[3].Draw(g_BackBuffer, 460, 0);//�����ʺ�
			}

			//�������� ������ ���ٿ� �ö����� ��, �� ���
			if (g_myleaf.isHit[16] == false)
			{
				g_myChar.image[2].Draw(g_BackBuffer, 0, 150);//���ʺ�
				g_myChar.image[3].Draw(g_BackBuffer, 460, 150);//�����ʺ�
			}

			/*���ھ���*/
			if (g_myfood.isanimalEat == true && isScore[0] == false)
			{
				g_myChar.image[8].Draw(g_BackBuffer, 750, 10);//���ھ���
			}

			if (g_myfood.isanimalEat2 == true && isScore[1] == false)
			{
				g_myChar.image[9].Draw(g_BackBuffer, 750, 10);//���ھ���
			}

			if (g_myfood.isanimalEat3 == true && isScore[2] == false)
			{
				g_myChar.image[10].Draw(g_BackBuffer, 750, 10);//���ھ���
			}

			/*==========================����================================*/
			//1
			if (g_myleaf.isHit[1] == true) //����1�� �����Ҷ� ���
			{
				g_myleaf.leaf_image[0].Draw(g_BackBuffer, g_myleaf.x[0], g_myleaf.y[0],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//2
			if (g_myleaf.isHit[2] == true) //����2�� �����Ҷ� ���
			{
				g_myleaf.leaf_image[1].Draw(g_BackBuffer, g_myleaf.x[1], g_myleaf.y[1],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//3
			if (g_myleaf.isHit[3] == true) //����3�� �����Ҷ� ���
			{
				g_myleaf.leaf_image[2].Draw(g_BackBuffer, g_myleaf.x[2], g_myleaf.y[2],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//4
			if (g_myleaf.isHit[4] == true) //����4�� �����Ҷ� ���
			{
				g_myleaf.leaf_image[3].Draw(g_BackBuffer, g_myleaf.x[3], g_myleaf.y[3],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//5
			if (g_myleaf.isHit[5] == true && g_myleaf.isHit[1] == false) //����5�� �����ϰ� ����1�� �������������� ���
			{
				g_myleaf.leaf_image[4].Draw(g_BackBuffer, g_myleaf.x[4], g_myleaf.y[4],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//6
			if (g_myleaf.isHit[6] == true && g_myleaf.isHit[2] == false && g_myfood.isanimalEat == true) //������° ���ٰ� �浹�߰�, �ι�° ������ �������, �䳢�� ����� ���� �����϶� ��Ÿ��
			{
				g_myleaf.leaf_image[5].Draw(g_BackBuffer, g_myleaf.x[5], g_myleaf.y[5],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//7
			if (g_myleaf.isHit[7] == true && g_myleaf.isHit[3] == false) //����7�� �����ϰ� ����3�� �������������� ���
			{
				g_myleaf.leaf_image[6].Draw(g_BackBuffer, g_myleaf.x[6], g_myleaf.y[6],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//8
			if (g_myleaf.isHit[8] == true && g_myleaf.isHit[4] == false) //����7�� �����ϰ� ����4�� �������������� ���
			{
				g_myleaf.leaf_image[7].Draw(g_BackBuffer, g_myleaf.x[7], g_myleaf.y[7],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//9
			if (g_myleaf.isHit[9] == true && g_myleaf.isHit[5] == false) //����9�� �����ϰ� ����5�� �������������� ���
			{
				g_myleaf.leaf_image[8].Draw(g_BackBuffer, g_myleaf.x[8], g_myleaf.y[8],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//10
			if (g_myleaf.isHit[10] == true && g_myleaf.isHit[6] == false)//����10 �����ϰ� ����6 �������������� ���
			{
				g_myleaf.leaf_image[9].Draw(g_BackBuffer, g_myleaf.x[9], g_myleaf.y[9],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//11
			if (g_myleaf.isHit[11] == true && g_myleaf.isHit[7] == false)//����11 �����ϰ� ����7 �������������� ���
			{
				g_myleaf.leaf_image[10].Draw(g_BackBuffer, g_myleaf.x[10], g_myleaf.y[10],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//12
			if (g_myleaf.isHit[12] == true && g_myleaf.isHit[8] == false && g_myfood.isanimalEat2 == true)//����12 �����ϰ� ����8 ���������ʰ� �ٶ��㰡 ������ �Ծ����� ���
			{
				g_myleaf.leaf_image[11].Draw(g_BackBuffer, g_myleaf.x[11], g_myleaf.y[11],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//13
			if (g_myleaf.isHit[13] == true && g_myleaf.isHit[9] == false)
			{
				g_myleaf.leaf_image[12].Draw(g_BackBuffer, g_myleaf.x[12], g_myleaf.y[12],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//14
			if (g_myleaf.isHit[14] == true && g_myleaf.isHit[10] == false)
			{
				g_myleaf.leaf_image[13].Draw(g_BackBuffer, g_myleaf.x[13], g_myleaf.y[13],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//15
			if (g_myleaf.isHit[15] == true && g_myleaf.isHit[11] == false)
			{
				g_myleaf.leaf_image[14].Draw(g_BackBuffer, g_myleaf.x[14], g_myleaf.y[14],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//16
			if (g_myleaf.isHit[16] == true && g_myleaf.isHit[12] == false)
			{
				g_myleaf.leaf_image[15].Draw(g_BackBuffer, g_myleaf.x[15], g_myleaf.y[15],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//17
			if (g_myleaf.isHit[17] == true && g_myleaf.isHit[13] == false)
			{
				g_myleaf.leaf_image[16].Draw(g_BackBuffer, g_myleaf.x[16], g_myleaf.y[16],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//18
			if (g_myleaf.isHit[18] == true && g_myleaf.isHit[14] == false && g_myfood.isanimalEat3 == true)
			{
				g_myleaf.leaf_image[17].Draw(g_BackBuffer, g_myleaf.x[17], g_myleaf.y[17],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//19
			if (g_myleaf.isHit[15] == false)
			{
				g_myleaf.leaf_image[18].Draw(g_BackBuffer, g_myleaf.x[18], g_myleaf.y[18],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}


			//�浹�ڽ�����
			g_myleaf.BoxCol();


			/*==========================����===============================*/

			//������
			if (g_myfood.isCreat[0] == true) //�������� �����Ҷ�
			{
				g_myfood.food_index = 0; //�޾ƿ� �� 0��°

				g_myfood.food_image[0].Draw(g_BackBuffer, g_myfood.x[0], g_myfood.y[0],
					g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
					g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);
			}

			//���
			if (g_myfood.isCreat[1] == true && g_myleaf.isHit[1] == true) //����� �����ϰ� ���� ������� �ʾ��� ��(�׹�° ���� �������ʾ�����)
			{
				g_myfood.food_index = 1;//�޾ƿ� �� 1��°

				g_myfood.food_image[1].Draw(g_BackBuffer, g_myfood.x[1], g_myfood.y[1],
					g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
					g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);
			}

			//2�� ���ٱ�

			//����
			if (g_myfood.isCreat[3] == true && g_myleaf.isHit[2] == false && g_myfood.isanimalEat == true) //������ �����ϰ� �ټ���° ���ٹ����� �䳢�� ������ �Ծ�����
			{
				g_myfood.food_index = 3;//�޾ƿ� �� 3��°

				g_myfood.food_image[3].Draw(g_BackBuffer, g_myfood.x[3], g_myfood.y[3],
					g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
					g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);
			}

			//���丮
			if (g_myfood.isCreat[4] == true && g_myleaf.isHit[3] == false)
			{
				g_myfood.food_index = 4;//�޾ƿ� �� 4��°

				g_myfood.food_image[4].Draw(g_BackBuffer, g_myfood.x[4], g_myfood.y[4],
					g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
					g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);
			}

			//���
			if (g_myfood.isCreat[5] == true && g_myleaf.isHit[9] == false)
			{
				g_myfood.food_index = 5;//�޾ƿ� �� 5��°

				g_myfood.food_image[5].Draw(g_BackBuffer, g_myfood.x[5], g_myfood.y[5],
					g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
					g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);
			}

			//����
			if (g_myfood.isCreat[6] == true && g_myleaf.isHit[11] == false)
			{
				g_myfood.food_index = 6;//�޾ƿ� �� 6��°

				g_myfood.food_image[6].Draw(g_BackBuffer, g_myfood.x[6], g_myfood.y[6],
					g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
					g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);
			}

			//�浹�ڽ�����
			g_myfood.BoxCol_food();



			/*============================����================================*/
			//�䳢
			if (g_myleaf.isHit[5] == true && g_myleaf.isHit[1] == false)
			{
				g_myanimal.animal_image[0].Draw(g_BackBuffer, g_myanimal.x[0], g_myanimal.y[0],
					g_myanimal.animal[g_myanimal.animal_index].x, g_myanimal.animal[g_myanimal.animal_index].y,
					g_myanimal.animal[g_myanimal.animal_index].width, g_myanimal.animal[g_myanimal.animal_index].height);


				g_Interval2 += tick;

				if (g_Interval2 >= g_myanimal.animal[g_myanimal.animal_index].animal_time)
				{
					g_Interval2 = 0;
					g_myanimal.animal_index++;
					if (g_myanimal.animal_index >= 3)
						g_myanimal.animal_index = 0;
				}
				//�浹�ڽ�����
				g_myanimal.BoxCol_animal();
			}


			//�ٶ���
			if (g_myleaf.isHit[7] == false)
			{
				g_myanimal2.animal2_image[0].Draw(g_BackBuffer, g_myanimal2.x[0], g_myanimal2.y[0],
					g_myanimal2.animal2[g_myanimal2.animal2_index].x, g_myanimal2.animal2[g_myanimal2.animal2_index].y,
					g_myanimal2.animal2[g_myanimal2.animal2_index].width, g_myanimal2.animal2[g_myanimal2.animal2_index].height);

				g_Interval3 += tick;

				if (g_Interval3 >= g_myanimal2.animal2[g_myanimal2.animal2_index].animal_time)
				{
					g_Interval3 = 0;

					g_myanimal2.animal2_index++;
					if (g_myanimal2.animal2_index >= 3)
						g_myanimal2.animal2_index = 0;
				}
				//�浹�ڽ�����
				g_myanimal2.BoxCol_animal2();

			}


			//��
			if (g_myleaf.isHit[13] == false)
			{
				g_myanimal3.animal3_image[0].Draw(g_BackBuffer, g_myanimal3.x[0], g_myanimal3.y[0],
					g_myanimal3.animal3[g_myanimal3.animal3_index].x, g_myanimal3.animal3[g_myanimal3.animal3_index].y,
					g_myanimal3.animal3[g_myanimal3.animal3_index].width, g_myanimal3.animal3[g_myanimal3.animal3_index].height);

				g_Interval4 += tick;

				if (g_Interval4 >= g_myanimal3.animal3[g_myanimal3.animal3_index].animal_time)
				{
					g_Interval4 = 0;

					g_myanimal3.animal3_index++;
					if (g_myanimal3.animal3_index >= 17)
						g_myanimal3.animal3_index = 0;
				}


				//�浹�ڽ�����
				g_myanimal3.BoxCol_animal3();
			}


			//��
			if (g_myleaf.isHit[16] == false)
			{
				g_myanimal4.animal4_image[0].Draw(g_BackBuffer, g_myanimal4.x[0], g_myanimal4.y[0],
					g_myanimal4.animal4[g_myanimal4.animal4_index].x, g_myanimal4.animal4[g_myanimal4.animal4_index].y,
					g_myanimal4.animal4[g_myanimal4.animal4_index].width, g_myanimal4.animal4[g_myanimal4.animal4_index].height);

				g_Interval5 += tick;


				//���ʸ��
				if (g_myanimal4.xDir == -1)
				{

					if (g_Interval5 >= g_myanimal4.animal4[g_myanimal4.animal4_index].animal_time)
					{
						g_Interval5 = 0;

						g_myanimal4.animal4_index = 8;

						g_myanimal4.Move(g_myanimal4.xDir, g_myanimal4.yDir, tick);
					}

					if (g_myanimal4.x[0] <= 50)
					{
						g_myanimal4.xDir = 1;
						g_myanimal4.yDir = -1;
					}

				}

				//�����ʸ��
				if (g_myanimal4.xDir == 1)
				{
					if (g_Interval5 >= g_myanimal4.animal4[g_myanimal4.animal4_index].animal_time)
					{
						g_Interval5 = 0;

						g_myanimal4.animal4_index++;
						if (g_myanimal4.animal4_index >= 7)
							g_myanimal4.animal4_index = 0;

						g_myanimal4.Move(g_myanimal4.xDir, g_myanimal4.yDir, tick);

					}

					if (g_myanimal4.x[0] >= 500)
					{
						g_myanimal4.xDir = -1;
						g_myanimal4.yDir = 1;
					}
				}


				//�浹�ڽ�����
				g_myanimal4.BoxCol_animal4();
			}


			//������
			if (g_myChar.isCreat == true)
			{
				if (g_myChar.isMove == true) //�����̽�Ű�� ����
				{
					//if (g_myChar.x == g_myfood.x[0] && g_myChar.y == g_myfood.y[0])
					//{

					//}

					//����(&& �����̽�)
					if (g_myChar.isLeft == true)
					{
						g_myChar.image[4].Draw(g_BackBuffer, g_myChar.x, g_myChar.y,
							g_myChar.ani[g_myChar.index].x, g_myChar.ani[g_myChar.index].y,
							g_myChar.ani[g_myChar.index].width, g_myChar.ani[g_myChar.index].height);

						g_Interval += tick;

						if (g_Interval >= g_myChar.ani[g_myChar.index].ani_time)
						{
							g_Interval = 0;
							g_myChar.index++;
							if (g_myChar.index >= 3) //���ʸ��
								g_myChar.index = 0;
						}
					}

					//������(&& �����̽�)
					else
					{
						g_myChar.image[4].Draw(g_BackBuffer, g_myChar.x, g_myChar.y,
							g_myChar.ani[g_myChar.index].x, g_myChar.ani[g_myChar.index].y,
							g_myChar.ani[g_myChar.index].width, g_myChar.ani[g_myChar.index].height);

						g_Interval += tick;
						if (g_Interval >= g_myChar.ani[g_myChar.index].ani_time)
						{
							g_Interval = 0;
							g_myChar.index++;
							if (g_myChar.index >= 7) //�����ʸ��
								g_myChar.index = 4;
						}
					}
				}

				else //�����̽�Ű�� �ȴ���
				{
					//����Ű�� ����
					if (g_myChar.isLeft == true)
					{
						g_myChar.image[4].Draw(g_BackBuffer, g_myChar.x, g_myChar.y,
							g_myChar.ani[11].x, g_myChar.ani[11].y,
							g_myChar.ani[11].width, g_myChar.ani[11].height);

						g_Interval += tick;
						if (g_Interval >= g_myChar.ani[11].ani_time)
						{
							g_Interval = 0;
							g_myChar.index = 11; //������ ������ �������� �����(��鸮�������ʱ�����)

						}
					}

					//������Ű�� ����
					else if (g_myChar.isLeft == false)
					{
						g_myChar.image[4].Draw(g_BackBuffer, g_myChar.x, g_myChar.y,
							g_myChar.ani[12].x, g_myChar.ani[12].y,
							g_myChar.ani[12].width, g_myChar.ani[12].height);

						g_Interval += tick;

						if (g_Interval >= g_myChar.ani[12].ani_time)
						{
							g_Interval = 0;
							g_myChar.index = 12;//������ ������ ���������� �����(��鸮�������ʱ�����)

						}
					}
				}
			}

		}

		//�������� 2�� ���
		else
		{
			/*=======================����===========================*/
			mciSendCommandW(soundplayCheck, MCI_CLOSE, 0, NULL); //���������� �����ϰ�
			mciSendCommand(soundplayCheck2, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // ��ӹݺ� //��������
				(DWORD)(LPVOID)&m_mciPlayParms);



			/*================����Ʈ�� �ִ� �κ�==================*/
			if (g_myChar.isQuest2 == false)
			{
				if (MessageBox(hWnd, "�������� �칰���� Ż���߽��ϴ�!\n������ ������ ��� �������� �����ʿ��ؿ�\n��ó�� �������� �������� �����ְ� �����ּ���!", "����Ʈ�� �����߽��ϴ�.", MB_OK) == IDOK);
				{
					g_myChar.isQuest2 = true; // �޼��� �ڽ��� ���̻� ��µ����ʵ��� isQuest�� true�� ������ش�.
				}
			}


			/*======================�̹������==========================*/
			//�޹�� ���
			g_myChar.image[11].Draw(g_BackBuffer, -1, -150);

			//���� ���
			g_myChar.image[13].Draw(g_BackBuffer, g_myChar.myback_x[0] + 350, g_myChar.myback_y[0] + 60);

			g_myChar.myback_x[0] -= 5;

			if (g_myChar.myback_x[0] < -600)
			{
				g_myChar.myback_x[0] = 400;
			}

			//���� ��� 
			g_myChar.image[13].Draw(g_BackBuffer, g_myChar.myback_x[1] + 700, g_myChar.myback_y[1] + 280);

			g_myChar.myback_x[1] -= 5;

			if (g_myChar.myback_x[1] < -850)
			{
				g_myChar.myback_x[1] = 200;
			}

			//���� ���
			g_myChar.image[13].Draw(g_BackBuffer, g_myChar.myback_x[2] + 120, g_myChar.myback_y[2] + 300);

			g_myChar.myback_x[2] -= 5;

			if (g_myChar.myback_x[2] < -320)

			{
				g_myChar.myback_x[2] = 700;
			}

			//���踦 3�� �Ա��������� �ٴ��� ��� ����ϰ�,
			if (g_mykey.keyscore < 3)
			{
				//���� �ٴ�2�� x��ǥ�� 0�����۾�����
				if (g_myChar.myback_x[4] < 0)
				{
					//�ٴ�1�� x��ǥ�� �ٴ� 2�� x��ǥ +930���� ����
					g_myChar.myback_x[3] = g_myChar.myback_x[4] + 930;

					//���� �ٴ� 1�� x��ǥ�� 0���� �۾�����
					if (g_myChar.myback_x[3] < 0)
					{
						//�ٴ� 2�� x��ǥ�� �ٴ� 1�� x��ǥ +930���� ����
						g_myChar.myback_x[4] = g_myChar.myback_x[3] + 930;
					}
				}

			}

			//���踦 3�������� ���� ���
			else if (g_mykey.keyscore == 3)
			{
				if (g_myChar.myback_x[3] < 0)
				{
					g_mywater.x[0] = g_myChar.myback_x[3] + 933  +933;
					g_mywater.y[0] = 760;
					//g_myChar.image[12].Draw(g_BackBuffer, g_myChar.myback_x[8] + 933, g_myChar.myback_y[8] + 620);


					g_mywater.water_image[0].Draw(g_BackBuffer, g_mywater.x[0], g_mywater.y[0] ,
						g_mywater.water[g_mywater.water_index].x, g_mywater.water[g_mywater.water_index].y,
						g_mywater.water[g_mywater.water_index].width, g_mywater.water[g_mywater.water_index].height);
				}

				else if (g_myChar.myback_x[4] < 0)
				{
					g_mywater.x[0] = g_myChar.myback_x[4] + 933 + 933;
					g_mywater.y[0] = 760;
				/*	g_myChar.image[12].Draw(g_BackBuffer, g_myChar.myback_x[8] + 933, g_myChar.myback_y[8] + 620);*/

					g_mywater.water_image[0].Draw(g_BackBuffer, g_mywater.x[0], g_mywater.y[0],
						g_mywater.water[g_mywater.water_index].x, g_mywater.water[g_mywater.water_index].y,
						g_mywater.water[g_mywater.water_index].width, g_mywater.water[g_mywater.water_index].height);
				}

				g_mywater.BoxCol();

			}

			//�ٴ�1 ���
			g_myChar.image[15].Draw(g_BackBuffer, g_myChar.myback_x[3], g_myChar.myback_y[3] +600);


			//�ٴ�2 ���
			g_myChar.image[15].Draw(g_BackBuffer, g_myChar.myback_x[4], g_myChar.myback_y[4] +600);


			/*================================= ����� ========================================*/

			//�������� �����ӿ� ���� �������� �����ϱ����ؼ� ��ǥ����
			g_myenemy.x[0] = g_myChar.myback_x[9] + 100;
			g_myenemy.x[1] = g_myChar.myback_x[10] + 800;
			g_myenemy.x[2] = g_myChar.myback_x[11] + 1400;
			g_myenemy.x[3] = g_myChar.myback_x[12] + 2000;
			g_myenemy.x[4] = g_myChar.myback_x[13] + 2400;

			g_myenemy.y[0] = g_myChar.myback_y[9] + 720;
			g_myenemy.y[1] = g_myChar.myback_y[10] + 720;
			g_myenemy.y[2] = g_myChar.myback_y[11] + 720;
			g_myenemy.y[3] = g_myChar.myback_y[12] + 720;
			g_myenemy.y[4] = g_myChar.myback_y[13] + 720;

			for (int i = 0; i < 5; i++)
			{
				g_myenemy.enemy_image[i].Draw(g_BackBuffer, g_myenemy.x[i], g_myenemy.y[i],
					g_myenemy.enemy[g_myenemy.enemy_index].x, g_myenemy.enemy[g_myenemy.enemy_index].y,
					g_myenemy.enemy[g_myenemy.enemy_index].width, g_myenemy.enemy[g_myenemy.enemy_index].height);
			}


			g_myenemy.BoxCol();



			/*===============================���� ���=====================================*/

			//�������� �����ӿ� ���� ��������� �����ϱ����ؼ� ��ǥ����
			g_mykey.x[0] = g_myChar.myback_x[5] + 1200;
			g_mykey.x[1] = g_myChar.myback_x[6] + 1600;
			g_mykey.x[2] = g_myChar.myback_x[7] + 2700;

			g_mykey.y[0] = g_myChar.myback_y[5] + 400;
			g_mykey.y[1] = g_myChar.myback_y[6] + 400;
			g_mykey.y[2] = g_myChar.myback_y[7] + 400;

			if(g_mykey.isCreat[0] == true)
			{
				g_mykey.key_image[0].Draw(g_BackBuffer, g_mykey.x[0], g_mykey.y[0],
					g_mykey.key[g_mykey.key_index].x, g_mykey.key[g_mykey.key_index].y,
					g_mykey.key[g_mykey.key_index].width, g_mykey.key[g_mykey.key_index].height);
			}

			if (g_mykey.isCreat[1] == true)
			{
				g_mykey.key_image[1].Draw(g_BackBuffer, g_mykey.x[1], g_mykey.y[1],
					g_mykey.key[g_mykey.key_index].x, g_mykey.key[g_mykey.key_index].y,
					g_mykey.key[g_mykey.key_index].width, g_mykey.key[g_mykey.key_index].height);
			}

			if (g_mykey.isCreat[2] == true)
			{
				g_mykey.key_image[2].Draw(g_BackBuffer, g_mykey.x[2], g_mykey.y[2],
					g_mykey.key[g_mykey.key_index].x, g_mykey.key[g_mykey.key_index].y,
					g_mykey.key[g_mykey.key_index].width, g_mykey.key[g_mykey.key_index].height);
			}

			g_mykey.BoxCol();
			/*==============================================================================*/




			/*==========����⸦ ������ ���ʻ�ܿ� ���===========*/
			if (g_mykey.isCreat[0] == false)
			{
				g_myChar.image[14].Draw(g_BackBuffer, -30,-50);
			}

			if (g_mykey.isCreat[1] == false)
			{
				g_myChar.image[14].Draw(g_BackBuffer, -30, 0);
			}

			if (g_mykey.isCreat[2] == false)
			{
				g_myChar.image[14].Draw(g_BackBuffer, -30, 50);
			}
			/*=========================================================*/




			//�������� x��ǥ�� 200�� �ʰ��ϸ� object���� 14�� �ڷ� ����
			if (g_myChar.x > 200)
			{
				
				for (int i = 3; i < 15; i++)
				{
					g_myChar.myback_x[i] -= 14;
				}
	
			}


			//�������� x��ǥ�� 200���� ����
			if (g_myChar.x >= 201)
			{
				g_myChar.x = 200;
			}

			






			/*======================���� �ִϸ��̼� ���=================================*/
			
			//������
			if (g_myChar.isCreat == true)
			{
				if (g_myChar.isMove == true) //�����̽�Ű�� ����
				{
					//����(&& �����̽�)
					if (g_myChar.isLeft == true)
					{
						g_myChar.image[4].Draw(g_BackBuffer, g_myChar.x + FROGYSTAGE2_X, g_myChar.y- FROGYSTAGE2_Y,
							g_myChar.ani[g_myChar.index].x, g_myChar.ani[g_myChar.index].y,
							g_myChar.ani[g_myChar.index].width, g_myChar.ani[g_myChar.index].height);

						g_Interval += tick;

						if (g_Interval >= g_myChar.ani[g_myChar.index].ani_time)
						{
							g_Interval = 0;
							g_myChar.index++;
							if (g_myChar.index >= 3) //���ʸ��
								g_myChar.index = 0;
						}
					}

					//������(&& �����̽�)
					else
					{
						g_myChar.image[4].Draw(g_BackBuffer, g_myChar.x + FROGYSTAGE2_X, g_myChar.y - FROGYSTAGE2_Y,
							g_myChar.ani[g_myChar.index].x, g_myChar.ani[g_myChar.index].y,
							g_myChar.ani[g_myChar.index].width, g_myChar.ani[g_myChar.index].height);

						g_Interval += tick;
						if (g_Interval >= g_myChar.ani[g_myChar.index].ani_time)
						{
							g_Interval = 0;
							g_myChar.index++;
							if (g_myChar.index >= 7) //�����ʸ��
								g_myChar.index = 4;
						}
					}
				}

				else //�����̽�Ű�� �ȴ���
				{
					//����Ű�� ����
					if (g_myChar.isLeft == true)
					{
						g_myChar.image[4].Draw(g_BackBuffer, g_myChar.x + FROGYSTAGE2_X, g_myChar.y - FROGYSTAGE2_Y,
							g_myChar.ani[11].x, g_myChar.ani[11].y,
							g_myChar.ani[11].width, g_myChar.ani[11].height);

						g_Interval += tick;
						if (g_Interval >= g_myChar.ani[11].ani_time)
						{
							g_Interval = 0;
							g_myChar.index = 11; //������ ������ �������� �����(��鸮�������ʱ�����)

						}
					}

					//������Ű�� ����
					else if (g_myChar.isLeft == false)
					{
						g_myChar.image[4].Draw(g_BackBuffer, g_myChar.x + FROGYSTAGE2_X, g_myChar.y - FROGYSTAGE2_Y,
							g_myChar.ani[12].x, g_myChar.ani[12].y,
							g_myChar.ani[12].width, g_myChar.ani[12].height);

						g_Interval += tick;

						if (g_Interval >= g_myChar.ani[12].ani_time)
						{
							g_Interval = 0;
							g_myChar.index = 12;//������ ������ ���������� �����(��鸮�������ʱ�����)

						}
					}
				}
			}

			

			//��
			if (g_myChar.isStage == true)
			{
				g_myanimal5.animal5_image[0].Draw(g_BackBuffer, g_myanimal5.x[0], g_myanimal5.y[0],
					g_myanimal5.animal5[g_myanimal5.animal5_index].x, g_myanimal5.animal5[g_myanimal5.animal5_index].y,
					g_myanimal5.animal5[g_myanimal5.animal5_index].width, g_myanimal5.animal5[g_myanimal5.animal5_index].height);

				g_Interval6 += tick;


				//���ʸ��
				if (g_myanimal5.xDir == -1)
				{

					if (g_Interval6 >= g_myanimal5.animal5[g_myanimal5.animal5_index].animal_time)
					{
						g_Interval6 = 0;

						g_myanimal5.animal5_index++;
						if (g_myanimal5.animal5_index >= 7)
							g_myanimal5.animal5_index = 4;

						g_myanimal5.Move(g_myanimal5.xDir, tick);
					}

					if (g_myanimal5.x[0] <= 50)
					{
						g_myanimal5.xDir = 1;
					}

				}

				//�����ʸ��
				if (g_myanimal5.xDir == 1)
				{
					if (g_Interval6 >= g_myanimal5.animal5[g_myanimal5.animal5_index].animal_time)
					{
						g_Interval6 = 0;

						g_myanimal5.animal5_index++;
						if (g_myanimal5.animal5_index >= 3)
							g_myanimal5.animal5_index = 0;

						g_myanimal5.Move(g_myanimal5.xDir, tick);

					}

					if (g_myanimal5.x[0] >= 800)
					{
						g_myanimal5.xDir = -1;
					}
				}


				//�浹�ڽ�����
				g_myanimal5.BoxCol_animal5();
			}
		}
	}
}


//�浹ó��
void MYImageHit(HWND hWnd)
{
	//�������� 1
	if (g_myChar.isStage == false)
	{
		/*�簢�� ���*/
	/*	g_myChar.DrawCollisionRect(g_BackBuffer);
		g_myleaf.DrawCollisionRect_leaf(g_BackBuffer);
		g_myfood.DrawCollisionRect_food(g_BackBuffer);
		g_myanimal.DrawCollisionRect_animal(g_BackBuffer);
		g_myanimal2.DrawCollisionRect_animal2(g_BackBuffer);
		g_myanimal3.DrawCollisionRect_animal3(g_BackBuffer);
		g_myanimal4.DrawCollisionRect_animal4(g_BackBuffer);*/


		/*=====================================================================*/
		Rect laef = g_myleaf.rcCollision_leaf[0]; //����1
		Rect laef2 = g_myleaf.rcCollision_leaf[1];//����2
		Rect laef3 = g_myleaf.rcCollision_leaf[2];//����3
		Rect laef4 = g_myleaf.rcCollision_leaf[3];//����4
		Rect laef5 = g_myleaf.rcCollision_leaf[4];//����5
		Rect laef6 = g_myleaf.rcCollision_leaf[5];//����6
		Rect laef7 = g_myleaf.rcCollision_leaf[6];//����6
		Rect laef8 = g_myleaf.rcCollision_leaf[7];//����7
		Rect laef9 = g_myleaf.rcCollision_leaf[8];//����8
		Rect laef10 = g_myleaf.rcCollision_leaf[9];//����9
		Rect laef11 = g_myleaf.rcCollision_leaf[10];//����10
		Rect laef12 = g_myleaf.rcCollision_leaf[11];//����11
		Rect laef13 = g_myleaf.rcCollision_leaf[12];//����12
		Rect laef14 = g_myleaf.rcCollision_leaf[13];//����13
		Rect laef15 = g_myleaf.rcCollision_leaf[14];//����14
		Rect laef16 = g_myleaf.rcCollision_leaf[15];//����15
		Rect laef17 = g_myleaf.rcCollision_leaf[16];//����16
		Rect laef18 = g_myleaf.rcCollision_leaf[17];//����17
		Rect laef19 = g_myleaf.rcCollision_leaf[18];//����18


		/*=====================================================================*/
		Rect corn = g_myfood.rcCollision_food[0]; //������
		Rect carrot = g_myfood.rcCollision_food[1]; //���
		Rect born = g_myfood.rcCollision_food[2]; //���ٱ�
		Rect fish = g_myfood.rcCollision_food[3]; //����
		Rect nut = g_myfood.rcCollision_food[4]; //���丮
		Rect meat = g_myfood.rcCollision_food[5]; //���
		Rect beer = g_myfood.rcCollision_food[6]; //����


		/*=====================================================================*/
		Rect rabbit = g_myanimal.rcCollision_animal[0]; //�䳢
		Rect squirrel = g_myanimal2.rcCollision_animal2[0]; //�ٶ���
		Rect snake = g_myanimal3.rcCollision_animal3[0]; //��
		Rect bird = g_myanimal4.rcCollision_animal4[0]; //��



		/*����*/
		/*=====================================================================*/
		//����1�� �������� �浹��
		if (g_myleaf.isHit[1] == true)
		{
			if (laef.Intersect(g_myChar.rcCollision) == TRUE)
			{
				g_myleaf.check[0] = true;
				g_myleaf.check[1] = false;
				g_myleaf.check[2] = false;
				g_myleaf.check[3] = false;

				//30

				g_myChar.y = g_myleaf.y[0] - FROGY;

			}
		}


		//����2�� �������� �浹��
		if (g_myleaf.isHit[2] == true)
		{
			if (laef2.Intersect(g_myChar.rcCollision) == TRUE)
			{
				g_myleaf.check[0] = false;
				g_myleaf.check[1] = true;
				g_myleaf.check[2] = false;
				g_myleaf.check[3] = false;

				g_myChar.y = g_myleaf.y[1] - FROGY;

			}
		}


		//����3�� �������� �浹��
		if (g_myleaf.isHit[3] == true)
		{
			if (laef3.Intersect(g_myChar.rcCollision) == TRUE)
			{
				g_myleaf.check[0] = false;
				g_myleaf.check[1] = false;
				g_myleaf.check[2] = true;
				g_myleaf.check[3] = false;



				g_myChar.y = g_myleaf.y[2] - FROGY;

				g_myleaf.isHit[0] = false;


			}
		}


		//����4�� �������� �浹��
		if (g_myleaf.isHit[4] == true)
		{
			if (laef4.Intersect(g_myChar.rcCollision) == TRUE)
			{

				g_myleaf.check[1] = false;
				g_myleaf.check[2] = false;
				g_myleaf.check[3] = true;
				g_myleaf.check[4] = false;




				if (g_myleaf.isHit[1] == true)
				{
					/*����*/
					g_myleaf.y[1] += 180;
					g_myleaf.y[2] += 180;
					g_myleaf.y[3] += 180;

					/*����*/
					g_myfood.y[0] += 180;
				}


				g_myChar.y = g_myleaf.y[3] - FROGY;

				g_myleaf.isHit[1] = false; //�浹�ϸ� false�� �ٲ� (����1�� ��, ����� ����, ����5�� ���)

			}
		}


		//����5�� �������� �浹��
		if (g_myleaf.isHit[5] == true)
		{
			if (laef5.Intersect(g_myChar.rcCollision) == TRUE)
			{

				g_myleaf.check[2] = false;
				g_myleaf.check[3] = false;
				g_myleaf.check[4] = true;
				g_myleaf.check[5] = false;


				if (g_myleaf.isHit[2] == true)
				{
					/*����*/
					g_myleaf.y[2] += 180;
					g_myleaf.y[3] += 180;
					g_myleaf.y[4] += 180;

					/*����*/
					g_myfood.y[0] += 180;

					/*����*/
					g_myanimal.y[0] += 180;
				}
				g_myChar.y = g_myleaf.y[4] - FROGY;

				g_myleaf.isHit[2] = false; //�浹�ϸ� false�� �ٲ� (����2 ����, ����6�� ���)

			}
		}


		//����6�� �������� �浹��
		if (g_myleaf.isHit[6] == true && g_myleaf.isHit[2] == false && g_myfood.isanimalEat == true)
		{
			if (laef6.Intersect(g_myChar.rcCollision) == TRUE)
			{

				g_myleaf.check[3] = false;
				g_myleaf.check[4] = false;
				g_myleaf.check[5] = true;
				g_myleaf.check[6] = false;


				if (g_myleaf.isHit[3] == true)
				{
					/*����*/
					g_myleaf.y[3] += 180;
					g_myleaf.y[4] += 180;
					g_myleaf.y[5] += 180;

					/*����*/
					g_myfood.y[0] += 180;
					g_myfood.y[3] += 180;

					/*����*/
					g_myanimal.y[0] += 180;
				}

				g_myChar.y = g_myleaf.y[5] - FROGY;

				g_myleaf.isHit[3] = false; //�浹�ϸ� false�� �ٲ� (����3 ����, ����7�� ���)

			}

		}


		//����7�� �������� �浹��
		if (g_myleaf.isHit[7] == true && g_myleaf.isHit[3] == false) //����2�� ������߸� �浹
		{
			if (laef7.Intersect(g_myChar.rcCollision) == TRUE)
			{

				g_myleaf.check[4] = false;
				g_myleaf.check[5] = false;
				g_myleaf.check[6] = true;
				g_myleaf.check[7] = false;



				if (g_myleaf.isHit[4] == true)
				{
					/*����*/
					g_myleaf.y[4] += 180;
					g_myleaf.y[5] += 180;
					g_myleaf.y[6] += 180;

					/*����*/
					g_myfood.y[0] += 180;
					g_myfood.y[3] += 180;
					g_myfood.y[4] += 180;

					/*����*/
					g_myanimal.y[0] += 180;
				}

				g_myChar.y = g_myleaf.y[6] - FROGY;

				g_myleaf.isHit[4] = false; //�浹�ϸ� false�� �ٲ� (����3 ����, ����7�� ���)

			}

		}


		//����8�� �������� �浹��
		if (g_myleaf.isHit[8] == true && g_myleaf.isHit[4] == false) //����2�� ������߸� �浹
		{
			if (laef8.Intersect(g_myChar.rcCollision) == TRUE)
			{

				g_myleaf.check[5] = false;
				g_myleaf.check[6] = false;
				g_myleaf.check[7] = true;
				g_myleaf.check[8] = false;


				if (g_myleaf.isHit[5] == true)
				{
					/*����*/
					g_myleaf.y[5] += 180;
					g_myleaf.y[6] += 180;
					g_myleaf.y[7] += 180;

					/*����*/
					g_myanimal.y[0] += 180;

					/*����*/
					g_myfood.y[3] += 180;
					g_myfood.y[4] += 180;

				}

				g_myChar.y = g_myleaf.y[7] - FROGY;

				g_myleaf.isHit[5] = false; //�浹�ϸ� false�� �ٲ� (����3 ����, ����7�� ���)

			}

		}


		//����9�� �������� �浹��
		if (g_myleaf.isHit[9] == true && g_myleaf.isHit[5] == false) //����2�� ������߸� �浹
		{
			if (laef9.Intersect(g_myChar.rcCollision) == TRUE)
			{

				g_myleaf.check[6] = false;
				g_myleaf.check[7] = false;
				g_myleaf.check[8] = true;
				g_myleaf.check[9] = false;



				if (g_myleaf.isHit[6] == true)
				{
					/*����*/
					g_myleaf.y[6] += 180;
					g_myleaf.y[7] += 180;
					g_myleaf.y[8] += 180;

					/*����*/
					g_myfood.y[3] += 180;
					g_myfood.y[4] += 180;
				}

				g_myChar.y = g_myleaf.y[8] - FROGY;

				g_myleaf.isHit[6] = false; //�浹�ϸ� false�� �ٲ� (����3 ����, ����7�� ���)

			}

		}


		//����10�� �������� �浹��
		if (g_myleaf.isHit[10] == true && g_myleaf.isHit[6] == false) //����2�� ������߸� �浹
		{
			if (laef10.Intersect(g_myChar.rcCollision) == TRUE)
			{

				g_myleaf.check[7] = false;
				g_myleaf.check[8] = false;
				g_myleaf.check[9] = true;
				g_myleaf.check[10] = false;


				if (g_myleaf.isHit[7] == true)
				{
					/*����*/
					g_myleaf.y[7] += 180;
					g_myleaf.y[8] += 180;
					g_myleaf.y[9] += 180;

					/*����*/
					g_myfood.y[3] += 180;
					g_myfood.y[4] += 180;
				}

				g_myChar.y = g_myleaf.y[9] - FROGY;

				g_myleaf.isHit[7] = false; //�浹�ϸ� false�� �ٲ� (����3 ����, ����7�� ���)

			}

		}


		//����11�� �������� �浹��
		if (g_myleaf.isHit[11] == true && g_myleaf.isHit[7] == false) //����2�� ������߸� �浹
		{
			if (laef11.Intersect(g_myChar.rcCollision) == TRUE)
			{

				g_myleaf.check[8] = false;
				g_myleaf.check[9] = false;
				g_myleaf.check[10] = true;
				g_myleaf.check[11] = false;



				if (g_myleaf.isHit[8] == true)
				{
					/*����*/
					g_myleaf.y[8] += 180;
					g_myleaf.y[9] += 180;
					g_myleaf.y[10] += 180;

					/*����*/
					g_myfood.y[3] += 180;
					g_myfood.y[4] += 180;

					/*����*/
					g_myanimal2.y[0] += 180;
				}

				g_myChar.y = g_myleaf.y[10] - FROGY;

				g_myleaf.isHit[8] = false; //�浹�ϸ� false�� �ٲ� (����3 ����, ����7�� ���)

			}

		}


		//����12�� �������� �浹��
		if (g_myleaf.isHit[12] == true && g_myleaf.isHit[8] == false && g_myfood.isanimalEat2 == true) //����2�� ������߸� �浹
		{
			if (laef12.Intersect(g_myChar.rcCollision) == TRUE)
			{

				g_myleaf.check[9] = false;
				g_myleaf.check[10] = false;
				g_myleaf.check[11] = true;
				g_myleaf.check[12] = false;




				if (g_myleaf.isHit[9] == true)
				{
					/*����*/
					g_myleaf.y[9] += 180;
					g_myleaf.y[10] += 180;
					g_myleaf.y[11] += 180;

					/*����*/
					g_myfood.y[4] += 180;

					/*����*/
					g_myanimal2.y[0] += 180;

				}

				g_myChar.y = g_myleaf.y[11] - FROGY;

				g_myleaf.isHit[9] = false; //�浹�ϸ� false�� �ٲ� (����3 ����, ����7�� ���)

			}

		}


		//����13�� �������� �浹��
		if (g_myleaf.isHit[13] == true && g_myleaf.isHit[9] == false) //����2�� ������߸� �浹
		{
			if (laef13.Intersect(g_myChar.rcCollision) == TRUE)
			{

				g_myleaf.check[10] = false;
				g_myleaf.check[11] = false;
				g_myleaf.check[12] = true;
				g_myleaf.check[13] = false;


				if (g_myleaf.isHit[10] == true)
				{
					/*����*/
					g_myleaf.y[10] += 180;
					g_myleaf.y[11] += 180;
					g_myleaf.y[12] += 180;

					/*����*/
					g_myanimal2.y[0] += 180;

					/*����*/
					g_myfood.y[5] += 180;



				}

				g_myChar.y = g_myleaf.y[12] - FROGY;

				g_myleaf.isHit[10] = false; //�浹�ϸ� false�� �ٲ� (����3 ����, ����7�� ���)

			}

		}


		//����13�� �������� �浹��
		if (g_myleaf.isHit[14] == true && g_myleaf.isHit[10] == false) //����2�� ������߸� �浹
		{
			if (laef14.Intersect(g_myChar.rcCollision) == TRUE)
			{

				g_myleaf.check[11] = false;
				g_myleaf.check[12] = false;
				g_myleaf.check[13] = true;
				g_myleaf.check[14] = false;


				if (g_myleaf.isHit[11] == true)
				{
					/*����*/
					g_myleaf.y[11] += 180;
					g_myleaf.y[12] += 180;
					g_myleaf.y[13] += 180;

					/*����*/
					g_myanimal2.y[0] += 180;

					/*����*/
					g_myfood.y[5] += 180;

				}

				g_myChar.y = g_myleaf.y[13] - FROGY;

				g_myleaf.isHit[11] = false; //�浹�ϸ� false�� �ٲ� (����3 ����, ����7�� ���)

			}

		}


		//����14�� �������� �浹��
		if (g_myleaf.isHit[15] == true && g_myleaf.isHit[11] == false) //����2�� ������߸� �浹
		{
			if (laef15.Intersect(g_myChar.rcCollision) == TRUE)
			{

				g_myleaf.check[12] = false;
				g_myleaf.check[13] = false;
				g_myleaf.check[14] = true;
				g_myleaf.check[15] = false;





				if (g_myleaf.isHit[12] == true)
				{
					/*����*/
					g_myleaf.y[12] += 180;
					g_myleaf.y[13] += 180;
					g_myleaf.y[14] += 180;

					/*����*/
					g_myanimal2.y[0] += 180;

					/*����*/
					g_myfood.y[5] += 180;
					g_myfood.y[6] += 180;
				}

				g_myChar.y = g_myleaf.y[14] - FROGY;

				g_myleaf.isHit[12] = false; //�浹�ϸ� false�� �ٲ� (����3 ����, ����7�� ���)

			}

		}


		//����15�� �������� �浹��
		if (g_myleaf.isHit[16] == true && g_myleaf.isHit[12] == false) //����2�� ������߸� �浹
		{
			if (laef16.Intersect(g_myChar.rcCollision) == TRUE)
			{

				g_myleaf.check[13] = false;
				g_myleaf.check[14] = false;
				g_myleaf.check[15] = true;
				g_myleaf.check[16] = false;



				if (g_myleaf.isHit[13] == true)
				{
					/*����*/
					g_myleaf.y[13] += 180;
					g_myleaf.y[14] += 180;
					g_myleaf.y[15] += 180;

					/*����*/
					g_myanimal2.y[0] += 180;

					/*����*/
					g_myfood.y[5] += 180;
					g_myfood.y[6] += 180;

				}

				g_myChar.y = g_myleaf.y[15] - FROGY;

				g_myleaf.isHit[13] = false; //�浹�ϸ� false�� �ٲ� (����3 ����, ����7�� ���)

			}

		}


		//����16�� �������� �浹��
		if (g_myleaf.isHit[17] == true && g_myleaf.isHit[13] == false)
		{
			if (laef17.Intersect(g_myChar.rcCollision) == TRUE)
			{

				g_myleaf.check[14] = false;
				g_myleaf.check[15] = false;
				g_myleaf.check[16] = true;
				g_myleaf.check[17] = false;




				if (g_myleaf.isHit[14] == true)
				{
					/*����*/
					g_myleaf.y[14] += 180;
					g_myleaf.y[15] += 180;
					g_myleaf.y[16] += 180;

					/*����*/
					g_myfood.y[5] += 180;
					g_myfood.y[6] += 180;


					/*����*/
					g_myanimal3.y[0] += 180;
				}

				g_myChar.y = g_myleaf.y[16] - FROGY;

				g_myleaf.isHit[14] = false; //�浹�ϸ� false�� �ٲ� (����3 ����, ����7�� ���)

			}

		}


		//����17�� �������� �浹��
		if (g_myleaf.isHit[18] == true && g_myleaf.isHit[14] == false && g_myfood.isanimalEat3 == true) //����2�� ������߸� �浹
		{
			if (laef18.Intersect(g_myChar.rcCollision) == TRUE)
			{

				g_myleaf.check[15] = false;
				g_myleaf.check[16] = false;
				g_myleaf.check[17] = true;
				g_myleaf.check[18] = false;



				if (g_myleaf.isHit[15] == true)
				{
					/*����*/
					g_myleaf.y[15] += 180;
					g_myleaf.y[16] += 180;
					g_myleaf.y[17] += 180;

					/*����*/
					g_myanimal3.y[0] += 180;

					/*����*/
					g_myfood.y[6] += 180;
				}

				g_myChar.y = g_myleaf.y[17] - FROGY;

				g_myleaf.isHit[15] = false; //�浹�ϸ� false�� �ٲ� (����3 ����, ����7�� ���)

			}

		}


		//����18�� �������� �浹��
		if (g_myleaf.isHit[15] == false) //����2�� ������߸� �浹
		{
			if (laef19.Intersect(g_myChar.rcCollision) == TRUE)
			{

				g_myleaf.check[15] = false;
				g_myleaf.check[16] = false;
				g_myleaf.check[17] = false;
				g_myleaf.check[18] = true;


				if (g_myleaf.isHit[16] == true)
				{
					/*����*/
					g_myleaf.y[16] += 180;
					g_myleaf.y[17] += 180;
					g_myleaf.y[18] += 180;


					/*����*/
					g_myfood.y[6] += 180;

					/*����*/
					g_myanimal3.y[0] += 180;
				}

				g_myChar.y = g_myleaf.y[18] - FROGY;

				g_myleaf.isHit[16] = false; //�浹�ϸ� false�� �ٲ� (����3 ����, ����7�� ���)

			}

		}

		/*=====================================================================*/




		/*����*/

		/*�䳢�����ֱ�
		==================================================================*/

		//�������� �����ϰ�
		if (g_myfood.isCreat[0] == true)
		{
			//�������� �������� �浹��
			if (corn.Intersect(g_myChar.rcCollision) == TRUE)
			{
				//����� �����ϴ� �����̸�(�������� ����� ��������)
				if (g_myfood.isCreat[1] == true)
				{
					sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					g_myfood.isCreat[0] = false; //�������� �����ش�.

				}

				//����� ���������ʴ� �����̸�(�������� ����� ����)
				else
				{
					sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					//����� �����ϴ� ���·� ������ش�.
					g_myfood.isCreat[1] = true;
					g_myfood.isCreat[0] = false; //�������� �����ش�.

				}
			}
		}

		//����� �����ϰ�
		if (g_myfood.isCreat[1] == true)
		{
			//��ٿ� �������� �浹��
			if (carrot.Intersect(g_myChar.rcCollision) == TRUE)
			{
				//�������� �����ϴ� �����̸�(�������� �������� ��������)
				if (g_myfood.isCreat[0] == true)
				{
					sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					g_myfood.isCreat[1] = false; //����� �����ش�.		
				}

				//�������� ���������ʴ� �����̸�(�������� �������� ����)
				else
				{
					sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					//�������� �����ϴ� ���·� ������ش�.
					g_myfood.isCreat[0] = true;
					g_myfood.isCreat[1] = false; //����� �����ش�.
				}
			}
		}


		//������������ ���� ���������� ��Ÿ��
		if (g_myfood.isCreat[0] == false)
		{
			g_myfood.food_index = 0;

			g_myfood.food_image[0].Draw(g_BackBuffer, 0, 0,
				g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
				g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);
		}


		//��ٸ����� ���� ���������� ��Ÿ��(����� �����Ҷ�, ������ �����ʾ�����)
		if (g_myfood.isCreat[1] == false && g_myfood.isanimalEat == false)
		{
			g_myfood.food_index = 1;

			g_myfood.food_image[1].Draw(g_BackBuffer, 0, 0,
				g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
				g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);
		}


		//�䳢�� �������� �浹��
		if (rabbit.Intersect(g_myChar.rcCollision) == TRUE)
		{
			if (g_myfood.isCreat[1] == false) //����� ���� ���¶��
			{
				if (g_myfood.isanimalEat == false)
				{
					sndPlaySoundA("./Data/sound/happy.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //����Ʈ�������� ���
				}

				g_myfood.isanimalEat = true; //�䳢�� �Ծ��ٰ� �ٲ���
			}

			else //����� �������� ���¶��
			{
				//�������� �����ְ�
				g_myChar.isCreat = false;

				//���ٱͰ��ȴ�.
				g_myfood.food_index = 2;
				g_myfood.food_image[2].Draw(g_BackBuffer, g_myChar.x, g_myChar.y,
					g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
					g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);

				g_myfood.isCreat[2] = true;


			}
		}

		//�������� ���ٱͷ� ���߰� check1�� false�̰�, check2�� false�϶�
		if (g_myfood.isCreat[2] == true && g_myanimal.Check1 == false && g_myanimal.Check2 == false)
		{
			//check1�� true�� �ٲ��ְ�
			g_myanimal.Check1 = true;

			//check1�� true�̸�
			if (g_myanimal.Check1 == true)
			{
				g_duration = 0; //�ð��� 0���� ������ְ�
				g_myanimal.Check1 = false; //check1�� false��
				g_myanimal.Check2 = true; //check2�� true�� �ٲپ��ش�. (�� if���� �ٽ� ��������ʰ��ϱ�����)
			}
		}


		//�ð����� 1000�� �Ѿ��(1�ʰ� ����), check2�� true�϶�
		if (g_duration >= 1000 && g_myanimal.Check2 == true)
		{
			mciSendCommandW(soundplayCheck, MCI_CLOSE, 0, NULL); //���������� �����ϰ�
			sndPlaySoundA("./Data/sound/fail.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //����Ʈ����

			if (MessageBox(hWnd, "�������� ��Ƹ������ϴ�. ����� �������� �׿����.", "����Ʈ ����!!!!!", MB_RETRYCANCEL) == IDRETRY) //RETRY��ư�� ������
			{
				mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
					(DWORD)(LPVOID)& mciOpen);

				soundplayCheck = mciOpen.wDeviceID;

				mciSendCommand(soundplayCheck, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // ��ӹݺ� //��������
					(DWORD)(LPVOID)& m_mciPlayParms);
				isGameStart = false; //������Ʈ�η� ���ư���

									 //�ʱ�����
				Initial_content(); //�ʱ�ȭ��Ų��.


			}

			else // cancle��ư�� ������
			{
				//������ �����Ѵ�.
				exit(0);
			}

		}


		/*////�������� false(����)���� ���߰� isSound�� false�̰�, isSound_corn�� false�϶�
		//if (g_myfood.isCreat[0] == false && g_myChar.isSound == false && g_myChar.isSound_corn == false)
		//{
		//	//isSound�� true�� �ٲ��ְ�
		//	g_myChar.isSound = true;

		//	//isSound�� true�̸�
		//	if (g_myChar.isSound == true)
		//	{
		//		g_duration = 0; //�ð��� 0���� ������ְ�
		//		g_myChar.isSound = false; //isSound�� false��
		//		g_myChar.isSound_corn = true; //isSound_corn�� true�� �ٲپ��ش�. (�� if���� �ٽ� ��������ʰ��ϱ�����)
		//	}
		//}

		////�ð����� 1000�� �Ѿ��(1�ʰ� ����), isSound_corn�� true�϶�
		//if (g_duration <= 400 && g_myChar.isSound_corn == true&& rabbit.Intersect(g_myChar.rcCollision) == FALSE)
		//{
		//	sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
		//}




		////�������� false(����)���� ���߰� isSound�� false�̰�, isSound_corn�� false�϶�
		//if (g_myfood.isCreat[1] == false && g_myChar.isSound2 == false && g_myChar.isSound_carrot == false)
		//{
		//	//isSound�� true�� �ٲ��ְ�
		//	g_myChar.isSound2 = true;

		//	//isSound�� true�̸�
		//	if (g_myChar.isSound2 == true)
		//	{
		//		g_duration = 0; //�ð��� 0���� ������ְ�
		//		g_myChar.isSound2 = false; //isSound�� false��
		//		g_myChar.isSound_carrot = true; //isSound_corn�� true�� �ٲپ��ش�. (�� if���� �ٽ� ��������ʰ��ϱ�����)
		//	}
		//}

		////�ð����� 1000�� �Ѿ��(1�ʰ� ����), isSound_corn�� true�϶�
		//if (g_duration <= 100 && g_myChar.isSound_carrot == true && rabbit.Intersect(g_myChar.rcCollision) == FALSE)
		//{
		//	sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
		//}*/

		/*==================================================================*/



		/*�ٶ��������ֱ�
		==================================================================*/

		//������ �����ϰ�
		if (g_myfood.isCreat[3] == true && g_myfood.isanimalEat == true)
		{
			//������ �������� �浹��
			if (fish.Intersect(g_myChar.rcCollision) == TRUE)
			{
				//������ �����ϴ� �����̸�(�������� ������ ��������)
				if (g_myfood.isCreat[4] == true)
				{
					sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					g_myfood.isCreat[3] = false; //���丮�� �����ش�.
				}

				//������ ���������ʴ� �����̸�(�������� ������ ����)
				else
				{
					sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					//������ �����ϴ� ���·� ������ش�.
					g_myfood.isCreat[4] = true;
					g_myfood.isCreat[3] = false; //���丮�� �����ش�.
				}
			}
		}

		//���丮�� �����ϰ� �䳢�� ����� �Ծ���, 7��° �������� ����� ��
		if (g_myfood.isCreat[4] == true && g_myfood.isanimalEat == true && g_myleaf.isHit[4] == false)
		{
			//���丮�� �������� �浹��
			if (nut.Intersect(g_myChar.rcCollision) == TRUE)
			{
				//���丮�� �����ϴ� �����̸�(�������� ���丮�� ��������)
				if (g_myfood.isCreat[3] == true)
				{
					sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					g_myfood.isCreat[4] = false; //���丮�� �����ش�.		
				}

				//���丮�� ���������ʴ� �����̸�(�������� ���丮�� ����)
				else
				{
					sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					//���丮�� �����ϴ� ���·� ������ش�.
					g_myfood.isCreat[3] = true;
					g_myfood.isCreat[4] = false; //������ �����ش�.
				}
			}
		}


		//������������ ���� ���������� ��Ÿ��
		if (g_myfood.isCreat[3] == false)
		{
			g_myfood.food_index = 3;

			g_myfood.food_image[3].Draw(g_BackBuffer, 0, 0,
				g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
				g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);
		}

		//���丮������ ���� ���������� ��Ÿ��(���丮�� �����Ҷ�, ������ �����ʾ�����)
		if (g_myfood.isCreat[4] == false && g_myfood.isanimalEat2 == false)
		{
			g_myfood.food_index = 4;

			g_myfood.food_image[4].Draw(g_BackBuffer, 0, 0,
				g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
				g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);
		}


		//�ٶ���� �������� �浹��
		if (squirrel.Intersect(g_myChar.rcCollision) == TRUE && g_myleaf.isHit[7] == false)
		{

			if (g_myfood.isCreat[4] == false) //���丮�� ���� ���¶��
			{
				if (g_myfood.isanimalEat2 == false)
				{
					sndPlaySoundA("./Data/sound/happy.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //����Ʈ�������� ���
				}

				isScore[0] = true; //100�� ���ھ��� ����
				g_myfood.isanimalEat2 = true; //�ٶ��㰡 �Ծ��ٰ� �ٲ���
			}

			else //���丮�� �������� ���¶��
			{
				//�������� �����ְ�
				g_myChar.isCreat = false;

				//���ٱͰ��ȴ�.
				g_myfood.food_index = 2;
				g_myfood.food_image[2].Draw(g_BackBuffer, g_myChar.x, g_myChar.y,
					g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
					g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);

				g_myfood.isCreat[2] = true;


			}
		}


		//�������� ���ٱͷ� ���߰� check1�� false�̰�, check2�� false�϶�
		if (g_myfood.isCreat[2] == true && g_myanimal2.Check1 == false && g_myanimal2.Check2 == false)
		{
			//check1�� true�� �ٲ��ְ�
			g_myanimal2.Check1 = true;

			//check1�� true�̸�
			if (g_myanimal2.Check1 == true)
			{
				g_duration = 0; //�ð��� 0���� ������ְ�
				g_myanimal2.Check1 = false; //check1�� false��
				g_myanimal2.Check2 = true; //check2�� true�� �ٲپ��ش�. (�� if���� �ٽ� ��������ʰ��ϱ�����)
			}
		}

		//�ð����� 1000�� �Ѿ��(1�ʰ� ����), check2�� true�϶�
		if (g_duration >= 1000 && g_myanimal2.Check2 == true)
		{
			mciSendCommandW(soundplayCheck, MCI_CLOSE, 0, NULL); //���������� �����ϰ�
			sndPlaySoundA("./Data/sound/fail.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //����Ʈ����
			if (MessageBox(hWnd, "�������� ��Ƹ������ϴ�. ����� �������� �׿����.", "����Ʈ ����!!!!!", MB_RETRYCANCEL) == IDRETRY) //RETRY��ư�� ������
			{
				isGameStart = false; //������Ʈ�η� ���ư���

									 //�ʱ�����
				Initial_content(); //�ʱ�ȭ��Ų��.
			}

			else // cancle��ư�� ������
			{
				//������ �����Ѵ�.
				exit(0);
			}

		}

		/*==================================================================*/




		/*�� �����ֱ�
		==================================================================*/

		//��Ⱑ �����ϰ� �ٶ��㰡 ������ ���������̸�
		if (g_myfood.isCreat[5] == true && g_myfood.isanimalEat2 == true)
		{
			//���� �������� �浹��
			if (meat.Intersect(g_myChar.rcCollision) == TRUE)
			{
				//���ְ� �����ϴ� �����̸�
				if (g_myfood.isCreat[6] == true)
				{
					sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					g_myfood.isCreat[5] = false; //��⸦ �����ش�. => ���������� ��������
				}

				//���ְ� ���������ʴ� �����̸�
				else
				{
					sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					//���ָ� �����ϴ� ���·� ������ش�.
					g_myfood.isCreat[6] = true;
					g_myfood.isCreat[5] = false; //��⸦ �����ش�.
				}
			}
		}

		//���ְ� �����ϰ�
		if (g_myfood.isCreat[6] == true && g_myfood.isanimalEat2 == true)
		{
			//���ֿ� �������� �浹��
			if (beer.Intersect(g_myChar.rcCollision) == TRUE)
			{
				//���ְ� �����ϴ� �����̸�(�������� ���ָ� ��������)
				if (g_myfood.isCreat[5] == true)
				{
					sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					g_myfood.isCreat[6] = false; //��⸦ �����ش�.
				}

				//���ְ� ���������ʴ� �����̸�(�������� ���ָ� ����)
				else
				{
					sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					//���ָ� �����ϴ� ���·� ������ش�.
					g_myfood.isCreat[5] = true;
					g_myfood.isCreat[6] = false; //��⸦ �����ش�.
				}
			}
		}


		//��⸦������ ���� ���������� ��Ÿ��
		if (g_myfood.isCreat[5] == false && g_myfood.isanimalEat3 == false)
		{
			g_myfood.food_index = 5;

			g_myfood.food_image[5].Draw(g_BackBuffer, 0, 0,
				g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
				g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);
		}


		//���ָ������� ���� ���������� ��Ÿ��
		if (g_myfood.isCreat[6] == false)
		{
			g_myfood.food_index = 6;

			g_myfood.food_image[6].Draw(g_BackBuffer, 0, 0,
				g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
				g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);
		}


		//��� �������� �浹��
		if (snake.Intersect(g_myChar.rcCollision) == TRUE && g_myleaf.isHit[12] == false)
		{

			if (g_myfood.isCreat[5] == false) //��⸦ ���� ���¶��
			{
				if (g_myfood.isanimalEat3 == false)
				{
					sndPlaySoundA("./Data/sound/happy.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //����Ʈ�������� ���
				}

				isScore[1] = true; //200�� ���ھ��� ����
				g_myfood.isanimalEat3 = true; //���� �Ծ��ٰ� �ٲ���
			}

			else //��⸦ �������� ���¶��
			{
				//�������� �����ְ�
				g_myChar.isCreat = false;

				//���ٱͰ��ȴ�.
				g_myfood.food_index = 2;
				g_myfood.food_image[2].Draw(g_BackBuffer, g_myChar.x, g_myChar.y,
					g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
					g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);

				g_myfood.isCreat[2] = true;


			}
		}


		//�������� ���ٱͷ� ���߰� check1�� false�̰�, check2�� false�϶�
		if (g_myfood.isCreat[2] == true && g_myanimal3.Check1 == false && g_myanimal3.Check2 == false)
		{
			//check1�� true�� �ٲ��ְ�
			g_myanimal3.Check1 = true;

			//check1�� true�̸�
			if (g_myanimal3.Check1 == true)
			{
				g_duration = 0; //�ð��� 0���� ������ְ�
				g_myanimal3.Check1 = false; //check1�� false��
				g_myanimal3.Check2 = true; //check2�� true�� �ٲپ��ش�. (�� if���� �ٽ� ��������ʰ��ϱ�����)
			}
		}


		//�ð����� 1000�� �Ѿ��(1�ʰ� ����), check2�� true�϶�
		if (g_duration >= 1000 && g_myanimal3.Check2 == true)
		{


			mciSendCommandW(soundplayCheck, MCI_CLOSE, 0, NULL); //���������� �����ϰ�
			sndPlaySoundA("./Data/sound/fail.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //����Ʈ����   

			if (MessageBox(hWnd, "�������� ��Ƹ������ϴ�. ����� �������� �׿����.", "����Ʈ ����!!!!!", MB_RETRYCANCEL) == IDRETRY) //RETRY��ư�� ������
			{
				isGameStart = false; //������Ʈ�η� ���ư���

									 //�ʱ�����
				Initial_content(); //�ʱ�ȭ��Ų��.
			}

			else // cancle��ư�� ������
			{
				//������ �����Ѵ�.
				exit(0);
			}

		}

		/*==================================================================*/



		/*���� �ε���
		==================================================================*/
	
		if (bird.Intersect(g_myChar.rcCollision) == TRUE)
		{
			mciSendCommandW(soundplayCheck, MCI_CLOSE, 0, NULL); //���������� �����ϰ�
			sndPlaySoundA("./Data/sound/fail.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //����Ʈ����


			if (MessageBox(hWnd, "����! ������ �������ϴ�. �ٽ� �Ͻðھ��?", "�̷�!", MB_RETRYCANCEL) == IDRETRY)
			{
				mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
					(DWORD)(LPVOID)& mciOpen);

				soundplayCheck = mciOpen.wDeviceID;

				mciSendCommand(soundplayCheck, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // ��ӹݺ� //��������
					(DWORD)(LPVOID)& m_mciPlayParms);

				isGameStart = false; //������Ʈ�η� ���ư���

									 //�ʱ�����
				Initial_content(); //�ʱ�ȭ��Ų��.
			}

			else // cancle��ư�� ������
			{
				//������ �����Ѵ�.
				exit(0);
			}

		}

		/*==================================================================*/




		/*�������� ������ ���������� ������
		==================================================================*/

		//�����̽��ٸ� �������ʾ�����
		if (g_myChar.isMove != true)
		{
			//����1�� �ѹ� �浹�� ��, ����1�� �������� ����� ��� 6
			if ((g_myChar.x < 110 || g_myChar.x > 320) && g_myleaf.check[0] == true)
			{
				//�������� y���� 50�� ������Ų��. (�������� �Ʒ��� ����߸���.)
				g_myChar.y += 50;

				//���� �������� y���� 740���� Ŀ�����
				if (g_myChar.y > 740)
				{
					//����1�� �浹�������� ���·� ������ش�.
					g_myleaf.check[0] = false;
				}
			}

			//����2�� �浹�� ��, ����2�� �������� ����� ��� 
			if ((g_myChar.x < 470 || g_myChar.x > 660) && g_myleaf.check[1] == true)
			{
				//�������� y���� 50�� ������Ų��. (�������� �Ʒ��� ����߸���.)
				g_myChar.y += 50;

				//���� �������� y���� 740���� Ŀ���鼭, ���� ����1�� ��������������(���� 4�� �浹�������� ���)
				if (g_myChar.y > 740 && g_myleaf.isHit[1] == true)
				{
					//����1�� �浹�������� ���·� ������ش�.
					g_myleaf.check[1] = false;
				}
			}

			//����3�� �浹�� ��, ����3�� �������� ����� ��� 
			if ((g_myChar.x < 130 || g_myChar.x > 310) && g_myleaf.check[2] == true)
			{
				//�������� y���� 50�� ������Ų��. (�������� �Ʒ��� ����߸���.)
				g_myChar.y += 50;

				//���� �������� y���� 740���� Ŀ���鼭, ���� ����1�� ��������������(���� 4�� �浹�������� ���)
				if (g_myChar.y > 740 && g_myleaf.isHit[1] == true)
				{
					//����1�� �浹�������� ���·� ������ش�.
					g_myleaf.check[2] = false;
				}
			}

			//����4�� �浹�� ��, ����4�� �������� ����� ��� 
			if ((g_myChar.x < 380 || g_myChar.x > 600) && g_myleaf.check[3] == true)
			{
				//�������� y���� 50�� ������Ų��. (�������� �Ʒ��� ����߸���.)
				g_myChar.y += 50;
			}

			//����5�� �浹�� ��, ����5�� �������� ����� ��� 
			if ((g_myChar.x < 600 || g_myChar.x > 800) && g_myleaf.check[4] == true)
			{
				//�������� y���� 50�� ������Ų��. (�������� �Ʒ��� ����߸���.)
				g_myChar.y += 50;
			}

			//����6�� �浹�� ��, ����6�� �������� ����� ��� 
			if ((g_myChar.x < 200 || g_myChar.x > 400) && g_myleaf.check[5] == true)
			{
				//�������� y���� 50�� ������Ų��. (�������� �Ʒ��� ����߸���.)
				g_myChar.y += 50;
			}

			//����7�� �浹�� ��, ����7�� �������� ����� ��� 
			if ((g_myChar.x < 500 || g_myChar.x > 700) && g_myleaf.check[6] == true)
			{
				//�������� y���� 50�� ������Ų��. (�������� �Ʒ��� ����߸���.)
				g_myChar.y += 50;
			}

			//����8�� �浹�� ��, ����8�� �������� ����� ��� 
			if ((g_myChar.x < 200 || g_myChar.x > 420) && g_myleaf.check[7] == true)
			{
				//�������� y���� 50�� ������Ų��. (�������� �Ʒ��� ����߸���.)
				g_myChar.y += 50;
			}

			//����9�� �浹�� ��, ����9�� �������� ����� ��� 
			if ((g_myChar.x < 80 || g_myChar.x > 220) && g_myleaf.check[8] == true)
			{
				//�������� y���� 50�� ������Ų��. (�������� �Ʒ��� ����߸���.)
				g_myChar.y += 50;
			}

			//����10�� �浹�� ��, ����10�� �������� ����� ��� 
			if ((g_myChar.x < 310 || g_myChar.x > 530) && g_myleaf.check[9] == true)
			{
				//�������� y���� 50�� ������Ų��. (�������� �Ʒ��� ����߸���.)
				g_myChar.y += 50;
			}

			//����11�� �浹�� ��, ����11�� �������� ����� ��� 
			if ((g_myChar.x < 470 || g_myChar.x > 740) && g_myleaf.check[10] == true)
			{
				//�������� y���� 50�� ������Ų��. (�������� �Ʒ��� ����߸���.)
				g_myChar.y += 50;
			}

			//����12�� �浹�� ��, ����12�� �������� ����� ��� 
			if ((g_myChar.x < 330 || g_myChar.x > 540) && g_myleaf.check[11] == true)
			{
				//�������� y���� 50�� ������Ų��. (�������� �Ʒ��� ����߸���.)
				g_myChar.y += 50;
			}

			//����13�� �浹�� ��, ����13�� �������� ����� ��� 
			if ((g_myChar.x < 570 || g_myChar.x > 880) && g_myleaf.check[12] == true)
			{
				//�������� y���� 50�� ������Ų��. (�������� �Ʒ��� ����߸���.)
				g_myChar.y += 50;
			}

			//����14�� �浹�� ��, ����14�� �������� ����� ��� 
			if ((g_myChar.x < 270 || g_myChar.x > 520) && g_myleaf.check[13] == true)
			{
				//�������� y���� 50�� ������Ų��. (�������� �Ʒ��� ����߸���.)
				g_myChar.y += 50;
			}

			//����15�� �浹�� ��, ����15�� �������� ����� ��� 
			if ((g_myChar.x < 10 || g_myChar.x > 220) && g_myleaf.check[14] == true)
			{
				//�������� y���� 50�� ������Ų��. (�������� �Ʒ��� ����߸���.)
				g_myChar.y += 50;
			}

			//����16�� �浹�� ��, ����16�� �������� ����� ��� 
			if ((g_myChar.x < 300 || g_myChar.x > 500) && g_myleaf.check[15] == true)
			{
				//�������� y���� 50�� ������Ų��. (�������� �Ʒ��� ����߸���.)
				g_myChar.y += 50;
			}

			//����17�� �浹�� ��, ����17�� �������� ����� ��� //���ڸ�
			if ((g_myChar.x < 580 || g_myChar.x > 800) && g_myleaf.check[16] == true)
			{
				//�������� y���� 50�� ������Ų��. (�������� �Ʒ��� ����߸���.)
				g_myChar.y += 50;
			}

			//����18�� �浹�� ��, ����18�� �������� ����� ��� 
			if ((g_myChar.x < 300 || g_myChar.x > 520) && g_myleaf.check[17] == true)
			{
				//�������� y���� 50�� ������Ų��. (�������� �Ʒ��� ����߸���.)
				g_myChar.y += 50;
			}

			//����19�� �浹�� ��, ����19�� �������� ����� ��� 
			if ((g_myChar.x < 100 || g_myChar.x > 320) && g_myleaf.check[18] == true)
			{
				//�������� y���� 50�� ������Ų��. (�������� �Ʒ��� ����߸���.)
				g_myChar.y += 50;
			}


			/*==================================================================*/

			//���� �������� y���� 900���� Ŀ�����(�Ʒ��� ��������)
			if (g_myleaf.isHit[16] == true) //������ ������ �����ʾ�����
			{
				//�������� ��������
				if (g_myChar.y > 1200)
				{
					mciSendCommandW(soundplayCheck, MCI_CLOSE, 0, NULL); //���������� �����ϰ�
					sndPlaySoundA("./Data/sound/falling.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //����Ʈ���� - ������ ���� ���

					if (MessageBox(hWnd, "�������� �������� ���Ҿ��.. �ٽ��Ͻðھ��?", "������!", MB_RETRYCANCEL) == IDRETRY) //retry��ư�� ������
					{

						mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
							(DWORD)(LPVOID)& mciOpen);

						soundplayCheck = mciOpen.wDeviceID;

						mciSendCommand(soundplayCheck, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // ��ӹݺ� //��������
							(DWORD)(LPVOID)& m_mciPlayParms);

						isGameStart = false; //������Ʈ�η� ���ư���

											 //�ʱ�����
						Initial_content(); //�ʱ�ȭ��Ų��.
					}

					// cancle��ư�� ������
					else
					{
						//������ �����Ѵ�.
						exit(0);
					}


				}
			}

			//������ ������ �������
			else
			{
				//�������� x��ǥ�� 0���� �۰ų� ����, �������� ���������(�칰������)
				if (g_myChar.y > 1200 && g_myChar.x <= 0)
				{
					mciSendCommandW(soundplayCheck, MCI_CLOSE, 0, NULL); //���������� �����ϰ�
					sndPlaySoundA("./Data/sound/Clear.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //����Ʈ�������� ���


					if (MessageBox(hWnd, "�������� �칰�� Ż���߽��ϴ�! ������ �����̿���!!!", "����Ʈ ����!!!!", MB_OK) == IDOK) //ok��ư�� ������
					{

						/*�������� 2�� �Ѿ�� �κ�*/

						if (MessageBox(hWnd, "�������� 2�� �Ѿ�ϴ�", "����Ʈ ����!!!!", MB_OK) == IDOK) //ok��ư�� ������
						{
							//�������� 2�� �Ѿ�� bool���� true�� �ٲ���.
							g_myChar.isStage = true;

							g_myChar.x = 60;
							g_myChar.y = 745;
						}

					}


				}

				//�������� x��ǥ�� 0���� ũ��, �������� ���������(�칰������)
				else if (g_myChar.y > 1200 && g_myChar.x > 0)
				{
					mciSendCommandW(soundplayCheck, MCI_CLOSE, 0, NULL); //���������� �����ϰ�
					sndPlaySoundA("./Data/sound/falling.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //����Ʈ���� - ������ ���� ���


					if (MessageBox(hWnd, "���� �� �Դµ�,,, �Ʊ��׿�! �ٽ� �Ͻðھ��?", "����Ʈ ����!!!!", MB_RETRYCANCEL) == IDRETRY) //retry��ư�� �������
					{

						mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
							(DWORD)(LPVOID)& mciOpen);

						soundplayCheck = mciOpen.wDeviceID;

						mciSendCommand(soundplayCheck, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // ��ӹݺ� //��������
							(DWORD)(LPVOID)& m_mciPlayParms);


						isGameStart = false; //������Ʈ�η� ���ư���

											 //�ʱ�����
						Initial_content(); //������ �ʱ�ȭ�Ѵ�.
					}

					else //cancle��ư�� �������
					{
						//������ �����Ѵ�.
						exit(0);
					}
				}
			}
		}
		/*==================================================================*/

	}

	//�������� 2
	if (g_myChar.isStage == true)
	{
		/*======================== �簢����� ============================*/

		/*g_myanimal5.DrawCollisionRect_animal5(g_BackBuffer);
		g_myChar.DrawCollisionRect(g_BackBuffer);
		g_mykey.DrawCollisionRect_key(g_BackBuffer);
		g_mykey.DrawCollisionRect_key(g_BackBuffer);
		g_myenemy.DrawCollisionRect_enemy(g_BackBuffer);
		g_mywater.DrawCollisionRect_water(g_BackBuffer);*/

		/*=================================================================*/


		Rect bird2 = g_myanimal5.rcCollision_animal5[0]; // stage 2 ��
		Rect key1 = g_mykey.rcCollision_key[0]; // stage 2 ����1
		Rect key2 = g_mykey.rcCollision_key[1]; // stage 2 ����2
		Rect key3 = g_mykey.rcCollision_key[2]; // stage 2 ����3
		Rect enemy1 = g_myenemy.rcCollision_enemy[0]; // stage 2 ��1
		Rect enemy2 = g_myenemy.rcCollision_enemy[1]; // stage 2 ��2
		Rect enemy3 = g_myenemy.rcCollision_enemy[2]; // stage 2 ��3
		Rect enemy4 = g_myenemy.rcCollision_enemy[3]; // stage 2 ��4
		Rect enemy5 = g_myenemy.rcCollision_enemy[4]; // stage 2 ��5
		Rect water = g_mywater.rcCollision_water[0]; // stage 2 ��

		//�����浹
		if (bird2.Intersect(g_myChar.rcCollision) == TRUE)
		{
			mciSendCommandW(soundplayCheck2, MCI_CLOSE, 0, NULL); //���������� �����ϰ�
			sndPlaySoundA("./Data/sound/fail.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //����Ʈ����


			if (MessageBox(hWnd, "����! ������ �������ϴ�. �ٽ� �Ͻðھ��?", "�̷�!", MB_RETRYCANCEL) == IDRETRY)
			{
				mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
					(DWORD)(LPVOID)& mciOpen2);

				soundplayCheck2 = mciOpen2.wDeviceID;

				mciSendCommand(soundplayCheck2, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // ��ӹݺ� //��������
					(DWORD)(LPVOID)& m_mciPlayParms);

				isGameStart = false; //������Ʈ�η� ���ư���

				//�ʱ�����
				Initial_content(); //�ʱ�ȭ��Ų��.
			}

			else // cancle��ư�� ������
			{
				//������ �����Ѵ�.
				exit(0);
			}

		}


		//����1 �浹
		if (key1.Intersect(g_myChar.rcCollision) == TRUE)
		{
			g_mykey.keyscore = 1;
			if (g_mykey.isCreat[0] == true)
			{
				sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
			}
			g_mykey.isCreat[0] = false;
		}

		//����2 �浹
		if (key2.Intersect(g_myChar.rcCollision) == TRUE)
		{
			g_mykey.keyscore = 2;
			if (g_mykey.isCreat[1] == true)
			{
				sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
			}
			g_mykey.isCreat[1] = false;
		}

		//����3 �浹
		if (key3.Intersect(g_myChar.rcCollision) == TRUE)
		{
			g_mykey.keyscore = 3;
			if (g_mykey.isCreat[2] == true)
			{
				sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
			}
			g_mykey.isCreat[2] = false;
		}




		//��1 �浹
		if (enemy1.Intersect(g_myChar.rcCollision) == TRUE)
		{
			mciSendCommandW(soundplayCheck2, MCI_CLOSE, 0, NULL); //���������� �����ϰ�
			sndPlaySoundA("./Data/sound/fail.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //����Ʈ����

			if (MessageBox(hWnd, "�ٽ� �Ͻðھ��?", "�̷�! �������� �׾����", MB_RETRYCANCEL) == IDRETRY)
			{
				mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
					(DWORD)(LPVOID)& mciOpen2);

				soundplayCheck2 = mciOpen2.wDeviceID;

				mciSendCommand(soundplayCheck2, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // ��ӹݺ� //��������
					(DWORD)(LPVOID)& m_mciPlayParms);

				isGameStart = false; //������Ʈ�η� ���ư���

									 //�ʱ�����
				Initial_content(); //�ʱ�ȭ��Ų��.
			}

			else
			{
				exit(0);
			}
		}

		//��2 �浹
		if (enemy2.Intersect(g_myChar.rcCollision) == TRUE)
		{
			mciSendCommandW(soundplayCheck2, MCI_CLOSE, 0, NULL); //���������� �����ϰ�
			sndPlaySoundA("./Data/sound/fail.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //����Ʈ����

			if (MessageBox(hWnd, "�ٽ� �Ͻðھ��?", "�̷�! �������� �׾����", MB_RETRYCANCEL) == IDRETRY)
			{
				mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
					(DWORD)(LPVOID)& mciOpen2);

				soundplayCheck2 = mciOpen2.wDeviceID;

				mciSendCommand(soundplayCheck2, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // ��ӹݺ� //��������
					(DWORD)(LPVOID)& m_mciPlayParms);

				isGameStart = false; //������Ʈ�η� ���ư���

//�ʱ�����
Initial_content(); //�ʱ�ȭ��Ų��.
			}

			else
			{
				exit(0);
			}
		}

		//��3 �浹
		if (enemy3.Intersect(g_myChar.rcCollision) == TRUE)
		{
			mciSendCommandW(soundplayCheck2, MCI_CLOSE, 0, NULL); //���������� �����ϰ�
			sndPlaySoundA("./Data/sound/fail.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //����Ʈ����

			if (MessageBox(hWnd, "�ٽ� �Ͻðھ��?", "�̷�! �������� �׾����", MB_RETRYCANCEL) == IDRETRY)
			{
				mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
					(DWORD)(LPVOID)& mciOpen2);

				soundplayCheck2 = mciOpen2.wDeviceID;

				mciSendCommand(soundplayCheck2, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // ��ӹݺ� //��������
					(DWORD)(LPVOID)& m_mciPlayParms);

				isGameStart = false; //������Ʈ�η� ���ư���

									 //�ʱ�����
				Initial_content(); //�ʱ�ȭ��Ų��.
			}

			else
			{
				exit(0);
			}
		}

		//��4 �浹
		if (enemy4.Intersect(g_myChar.rcCollision) == TRUE)
		{
			mciSendCommandW(soundplayCheck2, MCI_CLOSE, 0, NULL); //���������� �����ϰ�
			sndPlaySoundA("./Data/sound/fail.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //����Ʈ����

			if (MessageBox(hWnd, "�ٽ� �Ͻðھ��?", "�̷�! �������� �׾����", MB_RETRYCANCEL) == IDRETRY)
			{
				mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
					(DWORD)(LPVOID)& mciOpen2);

				soundplayCheck2 = mciOpen2.wDeviceID;

				mciSendCommand(soundplayCheck2, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // ��ӹݺ� //��������
					(DWORD)(LPVOID)& m_mciPlayParms);

				isGameStart = false; //������Ʈ�η� ���ư���

									 //�ʱ�����
				Initial_content(); //�ʱ�ȭ��Ų��.
			}

			else
			{
				exit(0);
			}
		}

		//��5 �浹
		if (enemy5.Intersect(g_myChar.rcCollision) == TRUE)
		{
			mciSendCommandW(soundplayCheck2, MCI_CLOSE, 0, NULL); //���������� �����ϰ�
			sndPlaySoundA("./Data/sound/fail.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //����Ʈ����

			if (MessageBox(hWnd, "�ٽ� �Ͻðھ��?", "�̷�! �������� �׾����", MB_RETRYCANCEL) == IDRETRY)
			{
				mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
					(DWORD)(LPVOID)& mciOpen2);

				soundplayCheck2 = mciOpen2.wDeviceID;

				mciSendCommand(soundplayCheck2, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // ��ӹݺ� //��������
					(DWORD)(LPVOID)& m_mciPlayParms);

				isGameStart = false; //������Ʈ�η� ���ư���

									 //�ʱ�����
				Initial_content(); //�ʱ�ȭ��Ų��.
			}

			else
			{
				exit(0);
			}
		}


		//���� �浹
		if (water.Intersect(g_myChar.rcCollision) == TRUE)
		{
			mciSendCommandW(soundplayCheck2, MCI_CLOSE, 0, NULL); //���������� �����ϰ�
			sndPlaySoundA("./Data/sound/Clear.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //����Ʈ�������� ���

			if (MessageBox(hWnd, "�������� �������� ������ ���ư����!!", "����Ʈ ����!!!!", MB_OK) == IDOK)
			{
				exit(0);
			}
		}


	}

}

