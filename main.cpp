#include "MyImage.h"
#include "DSpriteManager.h"
#include "myCharacter.h"
#include "resource.h"
#include "meLeaf.h"
#include "MyKey.h"
#include "myFood.h"
#include "water.h"
#include "Enemy.h"
#include "myAnimal.h" //토끼
#include "myAnimal_2.h" //다람쥐
#include "myAnimal_3.h" //뱀
#include "myAnimal_4.h" //새
#include "myAnimal_5.h" // stage2 새
#include <MMSystem.h> //playsound함수사용위해
#include "Digitalv.h" //mci 사용위해


#define FROGY 70 //개구리가 연잎을 밟았을때의 y 위치 조정
#define FROGYSTAGE2_Y 50 //개구리가 스테이지 2에 들어갔을때 y 위치 조정
#define FROGYSTAGE2_X 180 //개구리가 스테이지 2에 들어갔을때 x 위치 조정


//사운드사용위해서(playsound, mci)
/*========================================*/
#pragma comment(lib, "winmm.lib" ) 

MCI_OPEN_PARMS m_mciOpenParms;
MCI_PLAY_PARMS m_mciPlayParms;
DWORD m_dwDeviceID;
MCI_OPEN_PARMS mciOpen;
MCI_PLAY_PARMS mciPlay;

MCI_OPEN_PARMS mciOpen2;
MCI_PLAY_PARMS mciPlay2;

//메인사운드 조절
int soundplayCheck; 
//스테이지2 사운드 조절
int soundplayCheck2;


/*===========================================================*/

Graphics *g_BackBuffer;
Graphics *g_MainBuffer;
Bitmap   *g_Bitmap;

CHARACTER g_myChar;
LEAF g_myleaf; //연잎
KEY g_mykey; //물고기
FOOD g_myfood; //음식
ENEMY g_myenemy; //적
WATER g_mywater; //물
ANIMAL g_myanimal; //토끼
ANIMAL2 g_myanimal2; //다람쥐
ANIMAL3 g_myanimal3; //뱀
ANIMAL4 g_myanimal4; //새
ANIMAL5 g_myanimal5; // stage 2 새
					 
//게임시작조절 (false면 인트로, true면 게임시작)
bool isGameStart = false;



/*===========================================================*/
//마지막 연잎에 닿지 않았을 때의 점프
#define MMAX_JUMP	16
POINT g_pt[] = {{ 0,-20 },{ 0,-40 },{ 0,-50 },{ 0,-60 }
			 ,{ 0,60 } ,{ 0,50 },{ 0,40 }, { 0,20 } , { 0,0 },{ 0,0 },{ 0,0 },{ 0,0 },{ 0,0 },{ 0,0 },{ 0,0 },{ 0,0 } };

int g_jmpIndex = 0;
bool g_isJump = false;
 

//마지막 연잎에 닿았을 때의 점프
#define MMAX_JUMP2	20
POINT g_pt2[] = { { 0,-20 },{ 0,-40 },{ 0,-50 },{ 0,-60 },{ 0,-80 },{ 0,-90 }
,{ 0,90 } ,{ 0,80 } ,{ 0,60 } ,{ 0,50 },{ 0,40 },{ 0,20 } ,{ 0,0 },{ 0,0 },{ 0,0 },{ 0,0 },{ 0,0 },{ 0,0 },{ 0,0 },{ 0,0 } };

int g_jmpIndex2 = 0;
bool g_isJump2 = false;
/*===========================================================*/

bool isScore[3] = { false,false,false };


DWORD g_duration = 0; //시간

DWORD g_Interval = 0; //개구리
DWORD g_Interval2 = 0; //토끼
DWORD g_Interval3 = 0; //다람쥐
DWORD g_Interval4 = 0; //뱀
DWORD g_Interval5 = 0; //새
DWORD g_Interval6 = 0; //stage 2 새


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

	//사운드
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


		mciSendCommand(soundplayCheck, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // 계속반복 //메인음악
			(DWORD)(LPVOID)&m_mciPlayParms);


	/*=====================================================================*/



	/*이미지파일 불러오기*/
	/*=====================================================================*/

	//연잎
	for (int i = 0; i < MAXLEAF; i++)
	{
		g_myleaf.LoadImage_leaf("./Data/Image/LEAF.png", i);
		//g_myleaf.LoadImage_leaf("./Data/Image/lotusleaf.png", i);
	}
	
	//열쇠
	for (int i = 0; i < MAXKEY; i++)
	{
		g_mykey.LoadImage_key("./Data/Image/fish.png", i);
	}

	//적
	for (int i = 0; i < MAXENEMY; i++)
	{
		g_myenemy.LoadImage_enemy("./Data/Image/spike.png", i);
	}

	//음식
	for (int i = 0; i < MAXFOOD; i++)
	{
		g_myfood.LoadImage_food("./Data/Image/food.png", i);
	}

	//물
	for (int i = 0; i < MAXWATER; i++)
	{
		g_mywater.LoadImage_water("./Data/Image/water.png", i);
	}


	//동물
	g_myanimal.LoadImage_animal("./Data/Image/rabbit.png", 0);
	g_myanimal2.LoadImage_animal2("./Data/Image/squirrel.png", 0);
	g_myanimal3.LoadImage_animal3("./Data/Image/snake.png", 0);
	g_myanimal4.LoadImage_animal4("./Data/Image/bird1.png", 0);
	g_myanimal5.LoadImage_animal5("./Data/Image/birdstage2.png", 0);

	//오브젝트
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


	//데이터파일 읽어오기
	/*=====================================================================*/

	g_myChar.LoadAniInfo("ani_data_01.txt"); //개구리
	g_myleaf.LoadAniInfo_leaf("ani_data_02.txt"); //연잎
	g_myfood.LoadAniInfo_food("ani_data_03.txt"); //음식
	g_myanimal.LoadAniInfo_animal("ani_data_04.txt"); //토끼
	g_myanimal2.LoadAniInfo_animal2("ani_data_05.txt"); //다람쥐
	g_myanimal3.LoadAniInfo_animal3("ani_data_06.txt"); //뱀
	g_myanimal4.LoadAniInfo_animal4("ani_data_07.txt"); //새
	g_myanimal5.LoadAniInfo_animal5("ani_data_08.txt"); //stage 2 새
	g_mykey.LoadAniInfo_key("ani_data_09.txt"); //stage 2 열쇠
	g_myenemy.LoadAniInfo_enemy("ani_data_10.txt"); //stage 2 적
	g_mywater.LoadAniInfo_water("ani_data_11.txt"); //stage 2 물


	//초기정보
	Initial_content();

	MSG msg;
	DWORD tick = GetTickCount();
	while (1)
	{
		//윈도우 메세지가 있을경우 메세지를 처리한다.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		else //메세지가 없을 경우 게임 루프를 실행한다.
		{
			char buff[128];

			DWORD curTick = GetTickCount();
			DWORD deltatime = curTick - tick;

			OnUpdate(hwnd, deltatime);
			tick = curTick;

			g_duration += deltatime;
			//wsprintf(buff, "g_duration = %d", g_duration);
			wsprintf(buff, "우물안의 개구리");
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

	//게임시작을 안눌렀을때
	if (isGameStart == false)
	{
		MYImageDraw(hWnd, tick);

		BYTE  key[256];
		::GetKeyboardState(key);


		//ESC키 눌림
		if (key[VK_ESCAPE] & 0x80)
		{

			if (MessageBox(hWnd, "게임을 종료하시겠어요?", "종료", MB_YESNO) == IDYES)
			{
				exit(0);
			}

			else
			{}
		}


		//스페이스바 누르면 게임시작
		if (key[VK_SPACE] & 0x80)
		{
			isGameStart = true;
		}
	}
	
	
	//게임시작을 눌렀을때
	if (isGameStart == true)
	{
		if (hWnd == NULL)
			return;

		BYTE  key[256];
		::GetKeyboardState(key);

		//방향제어
		int xDir = 0, yDir = 0;

		//ESC키 눌림
		if (key[VK_ESCAPE] & 0x80)
		{

			if (MessageBox(hWnd, "게임을 종료하시겠어요?", "종료", MB_YESNO) == IDYES)
			{
				exit(0);
			}

			else
			{}
		}

		//오른쪽키 눌림
		if (key[VK_RIGHT] & 0x80)
		{
			if (g_myChar.isStage == false)
			{
				/*개구리와 오른쪽 벽의 충돌처리*/
				//마지막연잎에 닿지않았을때
				if (g_myleaf.isHit[16] == true)
				{
					//개구리의 x좌표가 750보다 크거나 같으면
					if (g_myChar.x >= 750)
					{
						//개구리의 x좌표를 749로 만들어준다.
						g_myChar.x = 749;
					}

				}

			}
	
			//개구리의 애니메이션을 오른쪽으로만들어준다.
			g_myChar.isLeft = false;
		}

		//왼쪽키 눌림
		if (key[VK_LEFT] & 0x80)
		{
			if (g_myChar.isStage == false)
			{
				/*개구리와 왼쪽벽과의 충돌처리*/
				//마지막연잎에 닿지않았을 경우
				if (g_myleaf.isHit[16] == true)
				{
					//개구리의 x좌표가 50보다 작거나 같을경우
					if (g_myChar.x <= 50)
					{
						//개구리의 x좌표를 51로 만들어준다.
						g_myChar.x = 51;
					}
				}
			}
			//개구리의 애니메이션을 왼쪽으로 만들어준다.
			g_myChar.isLeft = true;
		}

		//스페이스키 눌림
		if (key[VK_SPACE] & 0x80)
		{
			//점프사운드
			if (g_isJump == false)
			{
				sndPlaySoundA("./Data/sound/jumpsound.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
			}
			//개구리의 애니메이션을 재생해주고
			g_myChar.isMove = true;

			//점프를 뛰지않았을 경우
			if (g_isJump == false)
			{
				//점프를 뛴걸로만들어준다.
				g_isJump = true;
				g_jmpIndex = 0;
			}
		}

		//왼쪽 && 스페이스키 눌림
		if ((key[VK_LEFT] & 0x80) && (key[VK_SPACE] & 0x80))
		{
			//개구리의 애니메이션을 재생해주고
			g_myChar.isMove = true;

			//왼쪽모션을 재생
			g_myChar.isLeft = true;

			//개구리의 이동을 음수값(왼쪽)으로 만들어준다.
			xDir = -1;

			//개구리가 뛰지않았을경우
			if (g_isJump == false)
			{
				//뛴걸로 만들어준다
				g_isJump = true;
				g_jmpIndex = 0;
			}

			//개구리가 마지막 연잎에 충돌했을 경우 그리고 스테이지 1일 경우
			if (g_myleaf.isHit[16] == false && g_myChar.isStage == false)
			{
				//junp2의 높이로 점프를 한다.
				if (g_isJump2 == false)
				{
					g_isJump2 = true;
					g_jmpIndex2 = 0;
				}
			}


		}

		//오른쪽 && 스페이스 키 눌림
		if ((key[VK_RIGHT] & 0x80) && (key[VK_SPACE] & 0x80))
		{
			//개구리의 애니메이션을 재생해주고
			g_myChar.isMove = true;

			//개구리의 오른쪽애니메이션을 재생한다.
			g_myChar.isLeft = false;

			//개구리의 이동을 양수(오른쪽)으로 만들어준다.
			xDir = 1;

			//점프를 하지 않았을 경우
			if (g_isJump == false)
			{
				//점프한것으로 만들어준다
				g_isJump = true;
				g_jmpIndex = 0;
			}

			//개구리가 마지막연잎을 밟았을 경우 그리고 스테이지 1일 경우
			if (g_myleaf.isHit[16] == false && g_myChar.isStage == false)
			{
				//jump2의 높이로 점프한다.
				if (g_isJump2 == false)
				{
					g_isJump2 = true;
					g_jmpIndex2 = 0;
				}
			}

		}


		//오른쪽키와 컨트롤키(스테이지2의 슈퍼점프)
		if ((key[VK_CONTROL] & 0x80) && (key[VK_RIGHT] & 0x80))
		{	

			//점프사운드
			if (g_isJump == false)
			{
				sndPlaySoundA("./Data/sound/jumpsound.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
			}

			

			//개구리의 애니메이션을 재생해주고
			g_myChar.isMove = true;

			//개구리의 이동을 양수(오른쪽)으로 만들어준다.
			xDir = 1;

			if (g_myChar.isStage == true)     
			{
				//junp2의 높이로 점프를 한다.
				if (g_isJump2 == false)
				{
					g_isJump2 = true;
					g_jmpIndex2 = 0;
				}
			}

		}


		//왼쪽키와 컨트롤키(스테이지2의 슈퍼점프)
		if ((key[VK_CONTROL] & 0x80) && (key[VK_LEFT] & 0x80))
		{

			//점프사운드
			if (g_isJump == false)
			{
				sndPlaySoundA("./Data/sound/jumpsound.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
			}

			

			//개구리의 애니메이션을 재생해주고
			g_myChar.isMove = true;

			//개구리의 이동을 양수(오른쪽)으로 만들어준다.
			xDir = -1;

			if (g_myChar.isStage == true)
			{
				//junp2의 높이로 점프를 한다.
				if (g_isJump2 == false)
				{
					g_isJump2 = true;
					g_jmpIndex2 = 0;
				}
			}

		}


		g_myChar.Move(xDir, yDir, tick);

		//점프
		if (g_isJump)
		{
			g_myChar.x += g_pt[g_jmpIndex].x;
			g_myChar.y += g_pt[g_jmpIndex].y;
			g_jmpIndex++;

			if (g_jmpIndex >= MMAX_JUMP - 8) //애니메이션 딜레이를 줌 (8만큼)
			{
				g_isJump = false;
				g_myChar.isMove = false;
			}

		}


		//점프 (개구리가 마지막연잎에 닿았을 경우)
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

		MYImageDraw(hWnd, tick); //이미지하는 출력함수

		//퀘스트를 주는 부분
		if (g_myChar.isQuest == false)
		{
			if(MessageBox(hWnd, "개구리의 우물탈출을 도와주세요!! \n올라가는 도중 무서운 동물을 만날 수 있어요!\n그 무서운 동물은 토끼와 다람쥐, 뱀이에요\n그 동물들에게 개구리가 먹히지 않도록 음식을 주어야합니다.\n우물에서 나오는 음식을 모아주세요. 하지만 1개 이상은 개구리가 가져갈 수 없어요\n그럼 파이팅!!", "퀘스트가 도착했습니다.", MB_OK) == IDOK);
			{
				g_myChar.isQuest = true; // 메세지 박스가 더이상 출력되지않도록 isQuest를 true로 만들어준다.
			}			
		}

		MYImageHit(hWnd); //충돌처리함수
	}

	
}


//초기정보
void Initial_content()
{
	//캐릭터정보
	/*=====================================================================*/

	//개구리의 정보
	g_myChar.speed = 500;
	g_myChar.x = 60;
	g_myChar.y = 745;
	g_myChar.isCreat = true;


	//토끼의 정보
	g_myanimal.x[0] = 750;
	g_myanimal.y[0] = 0;
	g_myanimal.Check1 = false;
	g_myanimal.Check2 = false;


	//다람쥐의 정보
	g_myanimal2.x[0] = 640;
	g_myanimal2.y[0] = 90;
	g_myanimal2.Check1 = false;
	g_myanimal2.Check2 = false;


	//뱀의 정보
	g_myanimal3.x[0] = 740;
	g_myanimal3.y[0] = 120;
	g_myanimal3.Check1 = false;
	g_myanimal3.Check2 = false;

	//새의 정보
	g_myanimal4.x[0] = 100;
	g_myanimal4.y[0] = 100;
	g_myanimal4.speed = 300;
	g_myanimal4.xDir = -1;


	//stage 2 새의 정보
	g_myanimal5.x[0] = 100;
	g_myanimal5.y[0] = 300;
	g_myanimal5.speed = 50;
	g_myanimal5.xDir = 1;


	
	//연잎정보
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




	/*음식정보*/
	/*=====================================================================*/

	//옥수수
	g_myfood.x[0] = 180;
	g_myfood.y[0] = 260;
	g_myfood.isCreat[0] = true;

	//당근
	g_myfood.x[1] = 400;
	g_myfood.y[1] = 740;
	g_myfood.isCreat[1] = true;

	//뼈다귀
	g_myfood.isCreat[2] = false;

	//생선
	g_myfood.x[3] = 270;
	g_myfood.y[3] = 110;
	g_myfood.isCreat[3] = true;

	//도토리
	g_myfood.x[4] = 750;
	g_myfood.y[4] = 160;
	g_myfood.isCreat[4] = true;

	//고기
	g_myfood.x[5] = 750;
	g_myfood.y[5] = 80;
	g_myfood.isCreat[5] = true;

	//맥주
	g_myfood.x[6] = 110;
	g_myfood.y[6] = 80;
	g_myfood.isCreat[6] = true;
	/*=====================================================================*/

	//각각 토끼, 다람쥐, 뱀이 당근, 도토리, 고기를 먹었는지를 체크
	g_myfood.isanimalEat = false;
	g_myfood.isanimalEat2 = false;
	g_myfood.isanimalEat3 = false;

	
	/*==================================================================*/
}


//이미지그림, 움직임제어
void MYImageDraw(HWND hWnd, DWORD tick)
{
	Color color(255, 255, 255);
	g_BackBuffer->Clear(color);

	//게임시작을 누르지않았을때
	if (isGameStart == false)
	{
		g_myChar.image[0].Draw(g_BackBuffer, -1, 0); //뒷배경 출력
		g_myChar.image[1].Draw(g_BackBuffer, 0, 320);//바닥
		g_myChar.image[2].Draw(g_BackBuffer, 0, 0);//왼쪽벽
		g_myChar.image[3].Draw(g_BackBuffer, 460, 0);//오른쪽벽
		g_myChar.image[6].Draw(g_BackBuffer, -25, 300);//pressthespace
		g_myChar.image[7].Draw(g_BackBuffer, -20, -150);//타이틀
	}



	//게임시작을 눌렀을때
	if (isGameStart == true)
	{
		//스테이지 1일 경우
		if (g_myChar.isStage == false)
		{
			/*이미지출력*/

			//개구리가 마지막 연잎에 올라타지 않았을 때, 뒷배경 출력
			if (g_myleaf.isHit[16] == true)
			{
				g_myChar.image[0].Draw(g_BackBuffer, 0, 0);
			}

			//개구리가 마지막 연잎에 올라탔을 때, 뒷배경 출력
			if (g_myleaf.isHit[16] == false)
			{
				g_myChar.image[0].Draw(g_BackBuffer, 0, 200);
			}

			//개구리가 마지막 연잎에 올라탔을 때, 하늘, 우물윗벽 출력
			if (g_myleaf.isHit[16] == false)
			{
				g_myChar.image[5].Draw(g_BackBuffer, 0, -500);
				//g_myChar.image[6].Draw(g_BackBuffer, 0, -370); //없는게 낫나?

			}

			//개구리가 4번째 연잎에 올라타지않았을때, 바닥출력 => 올라타면 바닥 출력X
			if (g_myleaf.isHit[1] == true)
			{
				g_myChar.image[1].Draw(g_BackBuffer, 0, 320);//바닥
			}

			//개구리가 마지막 연잎에 올라타지 않았을 때, 벽 출력
			if (g_myleaf.isHit[16] == true)
			{
				g_myChar.image[2].Draw(g_BackBuffer, 0, 0);//왼쪽벽
				g_myChar.image[3].Draw(g_BackBuffer, 460, 0);//오른쪽벽
			}

			//개구리가 마지막 연잎에 올라탔을 때, 벽 출력
			if (g_myleaf.isHit[16] == false)
			{
				g_myChar.image[2].Draw(g_BackBuffer, 0, 150);//왼쪽벽
				g_myChar.image[3].Draw(g_BackBuffer, 460, 150);//오른쪽벽
			}

			/*스코어판*/
			if (g_myfood.isanimalEat == true && isScore[0] == false)
			{
				g_myChar.image[8].Draw(g_BackBuffer, 750, 10);//스코어판
			}

			if (g_myfood.isanimalEat2 == true && isScore[1] == false)
			{
				g_myChar.image[9].Draw(g_BackBuffer, 750, 10);//스코어판
			}

			if (g_myfood.isanimalEat3 == true && isScore[2] == false)
			{
				g_myChar.image[10].Draw(g_BackBuffer, 750, 10);//스코어판
			}

			/*==========================연잎================================*/
			//1
			if (g_myleaf.isHit[1] == true) //연잎1이 존재할때 출력
			{
				g_myleaf.leaf_image[0].Draw(g_BackBuffer, g_myleaf.x[0], g_myleaf.y[0],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//2
			if (g_myleaf.isHit[2] == true) //연잎2가 존재할때 출력
			{
				g_myleaf.leaf_image[1].Draw(g_BackBuffer, g_myleaf.x[1], g_myleaf.y[1],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//3
			if (g_myleaf.isHit[3] == true) //연잎3이 존재할때 출력
			{
				g_myleaf.leaf_image[2].Draw(g_BackBuffer, g_myleaf.x[2], g_myleaf.y[2],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//4
			if (g_myleaf.isHit[4] == true) //연잎4가 존재할때 출력
			{
				g_myleaf.leaf_image[3].Draw(g_BackBuffer, g_myleaf.x[3], g_myleaf.y[3],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//5
			if (g_myleaf.isHit[5] == true && g_myleaf.isHit[1] == false) //연잎5가 존재하고 연잎1이 존재하지않을때 출력
			{
				g_myleaf.leaf_image[4].Draw(g_BackBuffer, g_myleaf.x[4], g_myleaf.y[4],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//6
			if (g_myleaf.isHit[6] == true && g_myleaf.isHit[2] == false && g_myfood.isanimalEat == true) //여섯번째 연잎과 충돌했고, 두번째 연잎이 사라졌고, 토끼가 당근을 먹은 상태일때 나타남
			{
				g_myleaf.leaf_image[5].Draw(g_BackBuffer, g_myleaf.x[5], g_myleaf.y[5],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//7
			if (g_myleaf.isHit[7] == true && g_myleaf.isHit[3] == false) //연잎7이 존재하고 연잎3이 존재하지않을때 출력
			{
				g_myleaf.leaf_image[6].Draw(g_BackBuffer, g_myleaf.x[6], g_myleaf.y[6],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//8
			if (g_myleaf.isHit[8] == true && g_myleaf.isHit[4] == false) //연잎7이 존재하고 연잎4가 존재하지않을때 출력
			{
				g_myleaf.leaf_image[7].Draw(g_BackBuffer, g_myleaf.x[7], g_myleaf.y[7],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//9
			if (g_myleaf.isHit[9] == true && g_myleaf.isHit[5] == false) //연잎9가 존재하고 연잎5가 존재하지않을때 출력
			{
				g_myleaf.leaf_image[8].Draw(g_BackBuffer, g_myleaf.x[8], g_myleaf.y[8],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//10
			if (g_myleaf.isHit[10] == true && g_myleaf.isHit[6] == false)//연잎10 존재하고 연잎6 존재하지않을때 출력
			{
				g_myleaf.leaf_image[9].Draw(g_BackBuffer, g_myleaf.x[9], g_myleaf.y[9],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//11
			if (g_myleaf.isHit[11] == true && g_myleaf.isHit[7] == false)//연잎11 존재하고 연잎7 존재하지않을때 출력
			{
				g_myleaf.leaf_image[10].Draw(g_BackBuffer, g_myleaf.x[10], g_myleaf.y[10],
					g_myleaf.leaf[g_myleaf.leaf_index].x, g_myleaf.leaf[g_myleaf.leaf_index].y,
					g_myleaf.leaf[g_myleaf.leaf_index].width, g_myleaf.leaf[g_myleaf.leaf_index].height);
			}

			//12
			if (g_myleaf.isHit[12] == true && g_myleaf.isHit[8] == false && g_myfood.isanimalEat2 == true)//연잎12 존재하고 연잎8 존재하지않고 다람쥐가 음식을 먹었을때 출력
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


			//충돌박스범위
			g_myleaf.BoxCol();


			/*==========================음식===============================*/

			//옥수수
			if (g_myfood.isCreat[0] == true) //옥수수가 존재할때
			{
				g_myfood.food_index = 0; //받아올 줄 0번째

				g_myfood.food_image[0].Draw(g_BackBuffer, g_myfood.x[0], g_myfood.y[0],
					g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
					g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);
			}

			//당근
			if (g_myfood.isCreat[1] == true && g_myleaf.isHit[1] == true) //당근이 존재하고 땅이 사라지지 않았을 때(네번째 연잎 밟히지않았을때)
			{
				g_myfood.food_index = 1;//받아올 줄 1번째

				g_myfood.food_image[1].Draw(g_BackBuffer, g_myfood.x[1], g_myfood.y[1],
					g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
					g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);
			}

			//2는 뼈다귀

			//생선
			if (g_myfood.isCreat[3] == true && g_myleaf.isHit[2] == false && g_myfood.isanimalEat == true) //생선이 존재하고 다섯번째 연잎밟히고 토끼가 음식을 먹었을때
			{
				g_myfood.food_index = 3;//받아올 줄 3번째

				g_myfood.food_image[3].Draw(g_BackBuffer, g_myfood.x[3], g_myfood.y[3],
					g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
					g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);
			}

			//도토리
			if (g_myfood.isCreat[4] == true && g_myleaf.isHit[3] == false)
			{
				g_myfood.food_index = 4;//받아올 줄 4번째

				g_myfood.food_image[4].Draw(g_BackBuffer, g_myfood.x[4], g_myfood.y[4],
					g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
					g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);
			}

			//고기
			if (g_myfood.isCreat[5] == true && g_myleaf.isHit[9] == false)
			{
				g_myfood.food_index = 5;//받아올 줄 5번째

				g_myfood.food_image[5].Draw(g_BackBuffer, g_myfood.x[5], g_myfood.y[5],
					g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
					g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);
			}

			//맥주
			if (g_myfood.isCreat[6] == true && g_myleaf.isHit[11] == false)
			{
				g_myfood.food_index = 6;//받아올 줄 6번째

				g_myfood.food_image[6].Draw(g_BackBuffer, g_myfood.x[6], g_myfood.y[6],
					g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
					g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);
			}

			//충돌박스범위
			g_myfood.BoxCol_food();



			/*============================동물================================*/
			//토끼
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
				//충돌박스범위
				g_myanimal.BoxCol_animal();
			}


			//다람쥐
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
				//충돌박스범위
				g_myanimal2.BoxCol_animal2();

			}


			//뱀
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


				//충돌박스범위
				g_myanimal3.BoxCol_animal3();
			}


			//새
			if (g_myleaf.isHit[16] == false)
			{
				g_myanimal4.animal4_image[0].Draw(g_BackBuffer, g_myanimal4.x[0], g_myanimal4.y[0],
					g_myanimal4.animal4[g_myanimal4.animal4_index].x, g_myanimal4.animal4[g_myanimal4.animal4_index].y,
					g_myanimal4.animal4[g_myanimal4.animal4_index].width, g_myanimal4.animal4[g_myanimal4.animal4_index].height);

				g_Interval5 += tick;


				//왼쪽모션
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

				//오른쪽모션
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


				//충돌박스범위
				g_myanimal4.BoxCol_animal4();
			}


			//개구리
			if (g_myChar.isCreat == true)
			{
				if (g_myChar.isMove == true) //스페이스키가 눌림
				{
					//if (g_myChar.x == g_myfood.x[0] && g_myChar.y == g_myfood.y[0])
					//{

					//}

					//왼쪽(&& 스페이스)
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
							if (g_myChar.index >= 3) //왼쪽모션
								g_myChar.index = 0;
						}
					}

					//오른쪽(&& 스페이스)
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
							if (g_myChar.index >= 7) //오른쪽모션
								g_myChar.index = 4;
						}
					}
				}

				else //스페이스키가 안눌림
				{
					//왼쪽키가 눌림
					if (g_myChar.isLeft == true)
					{
						g_myChar.image[4].Draw(g_BackBuffer, g_myChar.x, g_myChar.y,
							g_myChar.ani[11].x, g_myChar.ani[11].y,
							g_myChar.ani[11].width, g_myChar.ani[11].height);

						g_Interval += tick;
						if (g_Interval >= g_myChar.ani[11].ani_time)
						{
							g_Interval = 0;
							g_myChar.index = 11; //방향을 완전히 왼쪽으로 만들기(흔들리게하지않기위해)

						}
					}

					//오른쪽키가 눌림
					else if (g_myChar.isLeft == false)
					{
						g_myChar.image[4].Draw(g_BackBuffer, g_myChar.x, g_myChar.y,
							g_myChar.ani[12].x, g_myChar.ani[12].y,
							g_myChar.ani[12].width, g_myChar.ani[12].height);

						g_Interval += tick;

						if (g_Interval >= g_myChar.ani[12].ani_time)
						{
							g_Interval = 0;
							g_myChar.index = 12;//방향을 완전히 오른쪽으로 만들기(흔들리게하지않기위해)

						}
					}
				}
			}

		}

		//스테이지 2일 경우
		else
		{
			/*=======================사운드===========================*/
			mciSendCommandW(soundplayCheck, MCI_CLOSE, 0, NULL); //메인음악을 종료하고
			mciSendCommand(soundplayCheck2, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // 계속반복 //메인음악
				(DWORD)(LPVOID)&m_mciPlayParms);



			/*================퀘스트를 주는 부분==================*/
			if (g_myChar.isQuest2 == false)
			{
				if (MessageBox(hWnd, "개구리가 우물에서 탈출했습니다!\n하지만 물에서 사는 개구리는 물이필요해요\n근처의 연못으로 개구리가 갈수있게 도와주세요!", "퀘스트가 도착했습니다.", MB_OK) == IDOK);
				{
					g_myChar.isQuest2 = true; // 메세지 박스가 더이상 출력되지않도록 isQuest를 true로 만들어준다.
				}
			}


			/*======================이미지출력==========================*/
			//뒷배경 출력
			g_myChar.image[11].Draw(g_BackBuffer, -1, -150);

			//구름 출력
			g_myChar.image[13].Draw(g_BackBuffer, g_myChar.myback_x[0] + 350, g_myChar.myback_y[0] + 60);

			g_myChar.myback_x[0] -= 5;

			if (g_myChar.myback_x[0] < -600)
			{
				g_myChar.myback_x[0] = 400;
			}

			//구름 출력 
			g_myChar.image[13].Draw(g_BackBuffer, g_myChar.myback_x[1] + 700, g_myChar.myback_y[1] + 280);

			g_myChar.myback_x[1] -= 5;

			if (g_myChar.myback_x[1] < -850)
			{
				g_myChar.myback_x[1] = 200;
			}

			//구름 출력
			g_myChar.image[13].Draw(g_BackBuffer, g_myChar.myback_x[2] + 120, g_myChar.myback_y[2] + 300);

			g_myChar.myback_x[2] -= 5;

			if (g_myChar.myback_x[2] < -320)

			{
				g_myChar.myback_x[2] = 700;
			}

			//열쇠를 3개 먹기전까지는 바닥을 계속 출력하고,
			if (g_mykey.keyscore < 3)
			{
				//만약 바닥2의 x좌표가 0보다작아지면
				if (g_myChar.myback_x[4] < 0)
				{
					//바닥1의 x좌표를 바닥 2의 x좌표 +930으로 만듦
					g_myChar.myback_x[3] = g_myChar.myback_x[4] + 930;

					//만약 바닥 1의 x좌표가 0보다 작아지면
					if (g_myChar.myback_x[3] < 0)
					{
						//바닥 2의 x좌표를 바닥 1의 x좌표 +930으로 만듦
						g_myChar.myback_x[4] = g_myChar.myback_x[3] + 930;
					}
				}

			}

			//열쇠를 3개먹으면 물을 출력
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

			//바닥1 출력
			g_myChar.image[15].Draw(g_BackBuffer, g_myChar.myback_x[3], g_myChar.myback_y[3] +600);


			//바닥2 출력
			g_myChar.image[15].Draw(g_BackBuffer, g_myChar.myback_x[4], g_myChar.myback_y[4] +600);


			/*================================= 적출력 ========================================*/

			//개구리의 움직임에 따라 적움직임 조정하기위해서 좌표지정
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



			/*===============================열쇠 출력=====================================*/

			//개구리의 움직임에 따라 열쇠움직임 조정하기위해서 좌표지정
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




			/*==========물고기를 먹으면 왼쪽상단에 출력===========*/
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




			//개구리의 x좌표가 200을 초과하면 object들을 14씩 뒤로 보냄
			if (g_myChar.x > 200)
			{
				
				for (int i = 3; i < 15; i++)
				{
					g_myChar.myback_x[i] -= 14;
				}
	
			}


			//개구리의 x좌표는 200으로 고정
			if (g_myChar.x >= 201)
			{
				g_myChar.x = 200;
			}

			






			/*======================동물 애니메이션 출력=================================*/
			
			//개구리
			if (g_myChar.isCreat == true)
			{
				if (g_myChar.isMove == true) //스페이스키가 눌림
				{
					//왼쪽(&& 스페이스)
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
							if (g_myChar.index >= 3) //왼쪽모션
								g_myChar.index = 0;
						}
					}

					//오른쪽(&& 스페이스)
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
							if (g_myChar.index >= 7) //오른쪽모션
								g_myChar.index = 4;
						}
					}
				}

				else //스페이스키가 안눌림
				{
					//왼쪽키가 눌림
					if (g_myChar.isLeft == true)
					{
						g_myChar.image[4].Draw(g_BackBuffer, g_myChar.x + FROGYSTAGE2_X, g_myChar.y - FROGYSTAGE2_Y,
							g_myChar.ani[11].x, g_myChar.ani[11].y,
							g_myChar.ani[11].width, g_myChar.ani[11].height);

						g_Interval += tick;
						if (g_Interval >= g_myChar.ani[11].ani_time)
						{
							g_Interval = 0;
							g_myChar.index = 11; //방향을 완전히 왼쪽으로 만들기(흔들리게하지않기위해)

						}
					}

					//오른쪽키가 눌림
					else if (g_myChar.isLeft == false)
					{
						g_myChar.image[4].Draw(g_BackBuffer, g_myChar.x + FROGYSTAGE2_X, g_myChar.y - FROGYSTAGE2_Y,
							g_myChar.ani[12].x, g_myChar.ani[12].y,
							g_myChar.ani[12].width, g_myChar.ani[12].height);

						g_Interval += tick;

						if (g_Interval >= g_myChar.ani[12].ani_time)
						{
							g_Interval = 0;
							g_myChar.index = 12;//방향을 완전히 오른쪽으로 만들기(흔들리게하지않기위해)

						}
					}
				}
			}

			

			//새
			if (g_myChar.isStage == true)
			{
				g_myanimal5.animal5_image[0].Draw(g_BackBuffer, g_myanimal5.x[0], g_myanimal5.y[0],
					g_myanimal5.animal5[g_myanimal5.animal5_index].x, g_myanimal5.animal5[g_myanimal5.animal5_index].y,
					g_myanimal5.animal5[g_myanimal5.animal5_index].width, g_myanimal5.animal5[g_myanimal5.animal5_index].height);

				g_Interval6 += tick;


				//왼쪽모션
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

				//오른쪽모션
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


				//충돌박스범위
				g_myanimal5.BoxCol_animal5();
			}
		}
	}
}


//충돌처리
void MYImageHit(HWND hWnd)
{
	//스테이지 1
	if (g_myChar.isStage == false)
	{
		/*사각형 출력*/
	/*	g_myChar.DrawCollisionRect(g_BackBuffer);
		g_myleaf.DrawCollisionRect_leaf(g_BackBuffer);
		g_myfood.DrawCollisionRect_food(g_BackBuffer);
		g_myanimal.DrawCollisionRect_animal(g_BackBuffer);
		g_myanimal2.DrawCollisionRect_animal2(g_BackBuffer);
		g_myanimal3.DrawCollisionRect_animal3(g_BackBuffer);
		g_myanimal4.DrawCollisionRect_animal4(g_BackBuffer);*/


		/*=====================================================================*/
		Rect laef = g_myleaf.rcCollision_leaf[0]; //연잎1
		Rect laef2 = g_myleaf.rcCollision_leaf[1];//연잎2
		Rect laef3 = g_myleaf.rcCollision_leaf[2];//연잎3
		Rect laef4 = g_myleaf.rcCollision_leaf[3];//연잎4
		Rect laef5 = g_myleaf.rcCollision_leaf[4];//연잎5
		Rect laef6 = g_myleaf.rcCollision_leaf[5];//연잎6
		Rect laef7 = g_myleaf.rcCollision_leaf[6];//연잎6
		Rect laef8 = g_myleaf.rcCollision_leaf[7];//연잎7
		Rect laef9 = g_myleaf.rcCollision_leaf[8];//연잎8
		Rect laef10 = g_myleaf.rcCollision_leaf[9];//연잎9
		Rect laef11 = g_myleaf.rcCollision_leaf[10];//연잎10
		Rect laef12 = g_myleaf.rcCollision_leaf[11];//연잎11
		Rect laef13 = g_myleaf.rcCollision_leaf[12];//연잎12
		Rect laef14 = g_myleaf.rcCollision_leaf[13];//연잎13
		Rect laef15 = g_myleaf.rcCollision_leaf[14];//연잎14
		Rect laef16 = g_myleaf.rcCollision_leaf[15];//연잎15
		Rect laef17 = g_myleaf.rcCollision_leaf[16];//연잎16
		Rect laef18 = g_myleaf.rcCollision_leaf[17];//연잎17
		Rect laef19 = g_myleaf.rcCollision_leaf[18];//연잎18


		/*=====================================================================*/
		Rect corn = g_myfood.rcCollision_food[0]; //옥수수
		Rect carrot = g_myfood.rcCollision_food[1]; //당근
		Rect born = g_myfood.rcCollision_food[2]; //뼈다귀
		Rect fish = g_myfood.rcCollision_food[3]; //생선
		Rect nut = g_myfood.rcCollision_food[4]; //도토리
		Rect meat = g_myfood.rcCollision_food[5]; //고기
		Rect beer = g_myfood.rcCollision_food[6]; //맥주


		/*=====================================================================*/
		Rect rabbit = g_myanimal.rcCollision_animal[0]; //토끼
		Rect squirrel = g_myanimal2.rcCollision_animal2[0]; //다람쥐
		Rect snake = g_myanimal3.rcCollision_animal3[0]; //뱀
		Rect bird = g_myanimal4.rcCollision_animal4[0]; //새



		/*연잎*/
		/*=====================================================================*/
		//연잎1과 개구리가 충돌함
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


		//연잎2과 개구리가 충돌함
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


		//연잎3과 개구리가 충돌함
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


		//연잎4과 개구리가 충돌함
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
					/*연잎*/
					g_myleaf.y[1] += 180;
					g_myleaf.y[2] += 180;
					g_myleaf.y[3] += 180;

					/*음식*/
					g_myfood.y[0] += 180;
				}


				g_myChar.y = g_myleaf.y[3] - FROGY;

				g_myleaf.isHit[1] = false; //충돌하면 false로 바뀜 (연잎1과 땅, 당근을 없앰, 연잎5를 출력)

			}
		}


		//연잎5과 개구리가 충돌함
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
					/*연잎*/
					g_myleaf.y[2] += 180;
					g_myleaf.y[3] += 180;
					g_myleaf.y[4] += 180;

					/*음식*/
					g_myfood.y[0] += 180;

					/*동물*/
					g_myanimal.y[0] += 180;
				}
				g_myChar.y = g_myleaf.y[4] - FROGY;

				g_myleaf.isHit[2] = false; //충돌하면 false로 바뀜 (연잎2 없앰, 연잎6를 출력)

			}
		}


		//연잎6과 개구리가 충돌함
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
					/*연잎*/
					g_myleaf.y[3] += 180;
					g_myleaf.y[4] += 180;
					g_myleaf.y[5] += 180;

					/*음식*/
					g_myfood.y[0] += 180;
					g_myfood.y[3] += 180;

					/*동물*/
					g_myanimal.y[0] += 180;
				}

				g_myChar.y = g_myleaf.y[5] - FROGY;

				g_myleaf.isHit[3] = false; //충돌하면 false로 바뀜 (연잎3 없앰, 연잎7를 출력)

			}

		}


		//연잎7과 개구리가 충돌함
		if (g_myleaf.isHit[7] == true && g_myleaf.isHit[3] == false) //연잎2가 사라져야만 충돌
		{
			if (laef7.Intersect(g_myChar.rcCollision) == TRUE)
			{

				g_myleaf.check[4] = false;
				g_myleaf.check[5] = false;
				g_myleaf.check[6] = true;
				g_myleaf.check[7] = false;



				if (g_myleaf.isHit[4] == true)
				{
					/*연잎*/
					g_myleaf.y[4] += 180;
					g_myleaf.y[5] += 180;
					g_myleaf.y[6] += 180;

					/*음식*/
					g_myfood.y[0] += 180;
					g_myfood.y[3] += 180;
					g_myfood.y[4] += 180;

					/*동물*/
					g_myanimal.y[0] += 180;
				}

				g_myChar.y = g_myleaf.y[6] - FROGY;

				g_myleaf.isHit[4] = false; //충돌하면 false로 바뀜 (연잎3 없앰, 연잎7를 출력)

			}

		}


		//연잎8과 개구리가 충돌함
		if (g_myleaf.isHit[8] == true && g_myleaf.isHit[4] == false) //연잎2가 사라져야만 충돌
		{
			if (laef8.Intersect(g_myChar.rcCollision) == TRUE)
			{

				g_myleaf.check[5] = false;
				g_myleaf.check[6] = false;
				g_myleaf.check[7] = true;
				g_myleaf.check[8] = false;


				if (g_myleaf.isHit[5] == true)
				{
					/*연잎*/
					g_myleaf.y[5] += 180;
					g_myleaf.y[6] += 180;
					g_myleaf.y[7] += 180;

					/*동물*/
					g_myanimal.y[0] += 180;

					/*음식*/
					g_myfood.y[3] += 180;
					g_myfood.y[4] += 180;

				}

				g_myChar.y = g_myleaf.y[7] - FROGY;

				g_myleaf.isHit[5] = false; //충돌하면 false로 바뀜 (연잎3 없앰, 연잎7를 출력)

			}

		}


		//연잎9과 개구리가 충돌함
		if (g_myleaf.isHit[9] == true && g_myleaf.isHit[5] == false) //연잎2가 사라져야만 충돌
		{
			if (laef9.Intersect(g_myChar.rcCollision) == TRUE)
			{

				g_myleaf.check[6] = false;
				g_myleaf.check[7] = false;
				g_myleaf.check[8] = true;
				g_myleaf.check[9] = false;



				if (g_myleaf.isHit[6] == true)
				{
					/*연잎*/
					g_myleaf.y[6] += 180;
					g_myleaf.y[7] += 180;
					g_myleaf.y[8] += 180;

					/*음식*/
					g_myfood.y[3] += 180;
					g_myfood.y[4] += 180;
				}

				g_myChar.y = g_myleaf.y[8] - FROGY;

				g_myleaf.isHit[6] = false; //충돌하면 false로 바뀜 (연잎3 없앰, 연잎7를 출력)

			}

		}


		//연잎10과 개구리가 충돌함
		if (g_myleaf.isHit[10] == true && g_myleaf.isHit[6] == false) //연잎2가 사라져야만 충돌
		{
			if (laef10.Intersect(g_myChar.rcCollision) == TRUE)
			{

				g_myleaf.check[7] = false;
				g_myleaf.check[8] = false;
				g_myleaf.check[9] = true;
				g_myleaf.check[10] = false;


				if (g_myleaf.isHit[7] == true)
				{
					/*연잎*/
					g_myleaf.y[7] += 180;
					g_myleaf.y[8] += 180;
					g_myleaf.y[9] += 180;

					/*음식*/
					g_myfood.y[3] += 180;
					g_myfood.y[4] += 180;
				}

				g_myChar.y = g_myleaf.y[9] - FROGY;

				g_myleaf.isHit[7] = false; //충돌하면 false로 바뀜 (연잎3 없앰, 연잎7를 출력)

			}

		}


		//연잎11과 개구리가 충돌함
		if (g_myleaf.isHit[11] == true && g_myleaf.isHit[7] == false) //연잎2가 사라져야만 충돌
		{
			if (laef11.Intersect(g_myChar.rcCollision) == TRUE)
			{

				g_myleaf.check[8] = false;
				g_myleaf.check[9] = false;
				g_myleaf.check[10] = true;
				g_myleaf.check[11] = false;



				if (g_myleaf.isHit[8] == true)
				{
					/*연잎*/
					g_myleaf.y[8] += 180;
					g_myleaf.y[9] += 180;
					g_myleaf.y[10] += 180;

					/*음식*/
					g_myfood.y[3] += 180;
					g_myfood.y[4] += 180;

					/*동물*/
					g_myanimal2.y[0] += 180;
				}

				g_myChar.y = g_myleaf.y[10] - FROGY;

				g_myleaf.isHit[8] = false; //충돌하면 false로 바뀜 (연잎3 없앰, 연잎7를 출력)

			}

		}


		//연잎12과 개구리가 충돌함
		if (g_myleaf.isHit[12] == true && g_myleaf.isHit[8] == false && g_myfood.isanimalEat2 == true) //연잎2가 사라져야만 충돌
		{
			if (laef12.Intersect(g_myChar.rcCollision) == TRUE)
			{

				g_myleaf.check[9] = false;
				g_myleaf.check[10] = false;
				g_myleaf.check[11] = true;
				g_myleaf.check[12] = false;




				if (g_myleaf.isHit[9] == true)
				{
					/*연잎*/
					g_myleaf.y[9] += 180;
					g_myleaf.y[10] += 180;
					g_myleaf.y[11] += 180;

					/*음식*/
					g_myfood.y[4] += 180;

					/*동물*/
					g_myanimal2.y[0] += 180;

				}

				g_myChar.y = g_myleaf.y[11] - FROGY;

				g_myleaf.isHit[9] = false; //충돌하면 false로 바뀜 (연잎3 없앰, 연잎7를 출력)

			}

		}


		//연잎13과 개구리가 충돌함
		if (g_myleaf.isHit[13] == true && g_myleaf.isHit[9] == false) //연잎2가 사라져야만 충돌
		{
			if (laef13.Intersect(g_myChar.rcCollision) == TRUE)
			{

				g_myleaf.check[10] = false;
				g_myleaf.check[11] = false;
				g_myleaf.check[12] = true;
				g_myleaf.check[13] = false;


				if (g_myleaf.isHit[10] == true)
				{
					/*연잎*/
					g_myleaf.y[10] += 180;
					g_myleaf.y[11] += 180;
					g_myleaf.y[12] += 180;

					/*동물*/
					g_myanimal2.y[0] += 180;

					/*음식*/
					g_myfood.y[5] += 180;



				}

				g_myChar.y = g_myleaf.y[12] - FROGY;

				g_myleaf.isHit[10] = false; //충돌하면 false로 바뀜 (연잎3 없앰, 연잎7를 출력)

			}

		}


		//연잎13과 개구리가 충돌함
		if (g_myleaf.isHit[14] == true && g_myleaf.isHit[10] == false) //연잎2가 사라져야만 충돌
		{
			if (laef14.Intersect(g_myChar.rcCollision) == TRUE)
			{

				g_myleaf.check[11] = false;
				g_myleaf.check[12] = false;
				g_myleaf.check[13] = true;
				g_myleaf.check[14] = false;


				if (g_myleaf.isHit[11] == true)
				{
					/*연잎*/
					g_myleaf.y[11] += 180;
					g_myleaf.y[12] += 180;
					g_myleaf.y[13] += 180;

					/*동물*/
					g_myanimal2.y[0] += 180;

					/*음식*/
					g_myfood.y[5] += 180;

				}

				g_myChar.y = g_myleaf.y[13] - FROGY;

				g_myleaf.isHit[11] = false; //충돌하면 false로 바뀜 (연잎3 없앰, 연잎7를 출력)

			}

		}


		//연잎14과 개구리가 충돌함
		if (g_myleaf.isHit[15] == true && g_myleaf.isHit[11] == false) //연잎2가 사라져야만 충돌
		{
			if (laef15.Intersect(g_myChar.rcCollision) == TRUE)
			{

				g_myleaf.check[12] = false;
				g_myleaf.check[13] = false;
				g_myleaf.check[14] = true;
				g_myleaf.check[15] = false;





				if (g_myleaf.isHit[12] == true)
				{
					/*연잎*/
					g_myleaf.y[12] += 180;
					g_myleaf.y[13] += 180;
					g_myleaf.y[14] += 180;

					/*동물*/
					g_myanimal2.y[0] += 180;

					/*음식*/
					g_myfood.y[5] += 180;
					g_myfood.y[6] += 180;
				}

				g_myChar.y = g_myleaf.y[14] - FROGY;

				g_myleaf.isHit[12] = false; //충돌하면 false로 바뀜 (연잎3 없앰, 연잎7를 출력)

			}

		}


		//연잎15과 개구리가 충돌함
		if (g_myleaf.isHit[16] == true && g_myleaf.isHit[12] == false) //연잎2가 사라져야만 충돌
		{
			if (laef16.Intersect(g_myChar.rcCollision) == TRUE)
			{

				g_myleaf.check[13] = false;
				g_myleaf.check[14] = false;
				g_myleaf.check[15] = true;
				g_myleaf.check[16] = false;



				if (g_myleaf.isHit[13] == true)
				{
					/*연잎*/
					g_myleaf.y[13] += 180;
					g_myleaf.y[14] += 180;
					g_myleaf.y[15] += 180;

					/*동물*/
					g_myanimal2.y[0] += 180;

					/*음식*/
					g_myfood.y[5] += 180;
					g_myfood.y[6] += 180;

				}

				g_myChar.y = g_myleaf.y[15] - FROGY;

				g_myleaf.isHit[13] = false; //충돌하면 false로 바뀜 (연잎3 없앰, 연잎7를 출력)

			}

		}


		//연잎16과 개구리가 충돌함
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
					/*연잎*/
					g_myleaf.y[14] += 180;
					g_myleaf.y[15] += 180;
					g_myleaf.y[16] += 180;

					/*음식*/
					g_myfood.y[5] += 180;
					g_myfood.y[6] += 180;


					/*동물*/
					g_myanimal3.y[0] += 180;
				}

				g_myChar.y = g_myleaf.y[16] - FROGY;

				g_myleaf.isHit[14] = false; //충돌하면 false로 바뀜 (연잎3 없앰, 연잎7를 출력)

			}

		}


		//연잎17과 개구리가 충돌함
		if (g_myleaf.isHit[18] == true && g_myleaf.isHit[14] == false && g_myfood.isanimalEat3 == true) //연잎2가 사라져야만 충돌
		{
			if (laef18.Intersect(g_myChar.rcCollision) == TRUE)
			{

				g_myleaf.check[15] = false;
				g_myleaf.check[16] = false;
				g_myleaf.check[17] = true;
				g_myleaf.check[18] = false;



				if (g_myleaf.isHit[15] == true)
				{
					/*연잎*/
					g_myleaf.y[15] += 180;
					g_myleaf.y[16] += 180;
					g_myleaf.y[17] += 180;

					/*동물*/
					g_myanimal3.y[0] += 180;

					/*음식*/
					g_myfood.y[6] += 180;
				}

				g_myChar.y = g_myleaf.y[17] - FROGY;

				g_myleaf.isHit[15] = false; //충돌하면 false로 바뀜 (연잎3 없앰, 연잎7를 출력)

			}

		}


		//연잎18과 개구리가 충돌함
		if (g_myleaf.isHit[15] == false) //연잎2가 사라져야만 충돌
		{
			if (laef19.Intersect(g_myChar.rcCollision) == TRUE)
			{

				g_myleaf.check[15] = false;
				g_myleaf.check[16] = false;
				g_myleaf.check[17] = false;
				g_myleaf.check[18] = true;


				if (g_myleaf.isHit[16] == true)
				{
					/*연잎*/
					g_myleaf.y[16] += 180;
					g_myleaf.y[17] += 180;
					g_myleaf.y[18] += 180;


					/*음식*/
					g_myfood.y[6] += 180;

					/*동물*/
					g_myanimal3.y[0] += 180;
				}

				g_myChar.y = g_myleaf.y[18] - FROGY;

				g_myleaf.isHit[16] = false; //충돌하면 false로 바뀜 (연잎3 없앰, 연잎7를 출력)

			}

		}

		/*=====================================================================*/




		/*음식*/

		/*토끼음식주기
		==================================================================*/

		//옥수수가 존재하고
		if (g_myfood.isCreat[0] == true)
		{
			//옥수수와 개구리가 충돌함
			if (corn.Intersect(g_myChar.rcCollision) == TRUE)
			{
				//당근이 존재하는 상태이면(개구리가 당근을 먹지않음)
				if (g_myfood.isCreat[1] == true)
				{
					sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					g_myfood.isCreat[0] = false; //옥수수를 지워준다.

				}

				//당근이 존재하지않는 상태이면(개구리가 당근을 먹음)
				else
				{
					sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					//당근을 존재하는 상태로 만들어준다.
					g_myfood.isCreat[1] = true;
					g_myfood.isCreat[0] = false; //옥수수를 지워준다.

				}
			}
		}

		//당근이 존재하고
		if (g_myfood.isCreat[1] == true)
		{
			//당근와 개구리가 충돌함
			if (carrot.Intersect(g_myChar.rcCollision) == TRUE)
			{
				//옥수수가 존재하는 상태이면(개구리가 옥수수를 먹지않음)
				if (g_myfood.isCreat[0] == true)
				{
					sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					g_myfood.isCreat[1] = false; //당근을 지워준다.		
				}

				//옥수수가 존재하지않는 상태이면(개구리가 옥수수를 먹음)
				else
				{
					sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					//옥수수를 존재하는 상태로 만들어준다.
					g_myfood.isCreat[0] = true;
					g_myfood.isCreat[1] = false; //당근을 지워준다.
				}
			}
		}


		//옥수수먹으면 위에 아이템으로 나타남
		if (g_myfood.isCreat[0] == false)
		{
			g_myfood.food_index = 0;

			g_myfood.food_image[0].Draw(g_BackBuffer, 0, 0,
				g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
				g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);
		}


		//당근먹으면 위에 아이템으로 나타남(당근이 존재할때, 동물이 먹지않았을때)
		if (g_myfood.isCreat[1] == false && g_myfood.isanimalEat == false)
		{
			g_myfood.food_index = 1;

			g_myfood.food_image[1].Draw(g_BackBuffer, 0, 0,
				g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
				g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);
		}


		//토끼와 개구리가 충돌함
		if (rabbit.Intersect(g_myChar.rcCollision) == TRUE)
		{
			if (g_myfood.isCreat[1] == false) //당근을 먹은 상태라면
			{
				if (g_myfood.isanimalEat == false)
				{
					sndPlaySoundA("./Data/sound/happy.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //퀘스트성공음악 재생
				}

				g_myfood.isanimalEat = true; //토끼가 먹었다고 바꿔줌
			}

			else //당근을 먹지않은 상태라면
			{
				//개구리를 없애주고
				g_myChar.isCreat = false;

				//뼈다귀가된다.
				g_myfood.food_index = 2;
				g_myfood.food_image[2].Draw(g_BackBuffer, g_myChar.x, g_myChar.y,
					g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
					g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);

				g_myfood.isCreat[2] = true;


			}
		}

		//개구리가 뼈다귀로 변했고 check1이 false이고, check2가 false일때
		if (g_myfood.isCreat[2] == true && g_myanimal.Check1 == false && g_myanimal.Check2 == false)
		{
			//check1을 true로 바꿔주고
			g_myanimal.Check1 = true;

			//check1이 true이면
			if (g_myanimal.Check1 == true)
			{
				g_duration = 0; //시간을 0으로 만들어주고
				g_myanimal.Check1 = false; //check1을 false로
				g_myanimal.Check2 = true; //check2를 true로 바꾸어준다. (이 if문이 다시 실행되지않게하기위해)
			}
		}


		//시간값이 1000을 넘어서고(1초가 지남), check2가 true일때
		if (g_duration >= 1000 && g_myanimal.Check2 == true)
		{
			mciSendCommandW(soundplayCheck, MCI_CLOSE, 0, NULL); //메인음악을 종료하고
			sndPlaySoundA("./Data/sound/fail.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //퀘스트실패

			if (MessageBox(hWnd, "개구리가 잡아먹혔습니다. 당신이 개구리를 죽였어요.", "퀘스트 실패!!!!!", MB_RETRYCANCEL) == IDRETRY) //RETRY버튼을 누르면
			{
				mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
					(DWORD)(LPVOID)& mciOpen);

				soundplayCheck = mciOpen.wDeviceID;

				mciSendCommand(soundplayCheck, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // 계속반복 //메인음악
					(DWORD)(LPVOID)& m_mciPlayParms);
				isGameStart = false; //게임인트로로 돌아가고

									 //초기정보
				Initial_content(); //초기화시킨다.


			}

			else // cancle버튼을 누르면
			{
				//게임을 종료한다.
				exit(0);
			}

		}


		/*////옥수수가 false(먹음)으로 변했고 isSound이 false이고, isSound_corn가 false일때
		//if (g_myfood.isCreat[0] == false && g_myChar.isSound == false && g_myChar.isSound_corn == false)
		//{
		//	//isSound을 true로 바꿔주고
		//	g_myChar.isSound = true;

		//	//isSound이 true이면
		//	if (g_myChar.isSound == true)
		//	{
		//		g_duration = 0; //시간을 0으로 만들어주고
		//		g_myChar.isSound = false; //isSound을 false로
		//		g_myChar.isSound_corn = true; //isSound_corn를 true로 바꾸어준다. (이 if문이 다시 실행되지않게하기위해)
		//	}
		//}

		////시간값이 1000을 넘어서고(1초가 지남), isSound_corn가 true일때
		//if (g_duration <= 400 && g_myChar.isSound_corn == true&& rabbit.Intersect(g_myChar.rcCollision) == FALSE)
		//{
		//	sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
		//}




		////옥수수가 false(먹음)으로 변했고 isSound이 false이고, isSound_corn가 false일때
		//if (g_myfood.isCreat[1] == false && g_myChar.isSound2 == false && g_myChar.isSound_carrot == false)
		//{
		//	//isSound을 true로 바꿔주고
		//	g_myChar.isSound2 = true;

		//	//isSound이 true이면
		//	if (g_myChar.isSound2 == true)
		//	{
		//		g_duration = 0; //시간을 0으로 만들어주고
		//		g_myChar.isSound2 = false; //isSound을 false로
		//		g_myChar.isSound_carrot = true; //isSound_corn를 true로 바꾸어준다. (이 if문이 다시 실행되지않게하기위해)
		//	}
		//}

		////시간값이 1000을 넘어서고(1초가 지남), isSound_corn가 true일때
		//if (g_duration <= 100 && g_myChar.isSound_carrot == true && rabbit.Intersect(g_myChar.rcCollision) == FALSE)
		//{
		//	sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
		//}*/

		/*==================================================================*/



		/*다람쥐음식주기
		==================================================================*/

		//생선이 존재하고
		if (g_myfood.isCreat[3] == true && g_myfood.isanimalEat == true)
		{
			//생선과 개구리가 충돌함
			if (fish.Intersect(g_myChar.rcCollision) == TRUE)
			{
				//생선이 존재하는 상태이면(개구리가 생선을 먹지않음)
				if (g_myfood.isCreat[4] == true)
				{
					sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					g_myfood.isCreat[3] = false; //도토리를 지워준다.
				}

				//생선이 존재하지않는 상태이면(개구리가 생선을 먹음)
				else
				{
					sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					//생선을 존재하는 상태로 만들어준다.
					g_myfood.isCreat[4] = true;
					g_myfood.isCreat[3] = false; //도토리를 지워준다.
				}
			}
		}

		//도토리가 존재하고 토끼가 당근을 먹었고, 7번째 나뭇잎을 밟았을 때
		if (g_myfood.isCreat[4] == true && g_myfood.isanimalEat == true && g_myleaf.isHit[4] == false)
		{
			//도토리와 개구리가 충돌함
			if (nut.Intersect(g_myChar.rcCollision) == TRUE)
			{
				//도토리가 존재하는 상태이면(개구리가 도토리를 먹지않음)
				if (g_myfood.isCreat[3] == true)
				{
					sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					g_myfood.isCreat[4] = false; //도토리를 지워준다.		
				}

				//도토리가 존재하지않는 상태이면(개구리가 도토리를 먹음)
				else
				{
					sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					//도토리를 존재하는 상태로 만들어준다.
					g_myfood.isCreat[3] = true;
					g_myfood.isCreat[4] = false; //생선을 지워준다.
				}
			}
		}


		//생선을먹으면 위에 아이템으로 나타남
		if (g_myfood.isCreat[3] == false)
		{
			g_myfood.food_index = 3;

			g_myfood.food_image[3].Draw(g_BackBuffer, 0, 0,
				g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
				g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);
		}

		//도토리먹으면 위에 아이템으로 나타남(도토리가 존재할때, 동물이 먹지않았을때)
		if (g_myfood.isCreat[4] == false && g_myfood.isanimalEat2 == false)
		{
			g_myfood.food_index = 4;

			g_myfood.food_image[4].Draw(g_BackBuffer, 0, 0,
				g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
				g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);
		}


		//다람쥐와 개구리가 충돌함
		if (squirrel.Intersect(g_myChar.rcCollision) == TRUE && g_myleaf.isHit[7] == false)
		{

			if (g_myfood.isCreat[4] == false) //도토리를 먹은 상태라면
			{
				if (g_myfood.isanimalEat2 == false)
				{
					sndPlaySoundA("./Data/sound/happy.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //퀘스트성공음악 재생
				}

				isScore[0] = true; //100점 스코어판 제거
				g_myfood.isanimalEat2 = true; //다람쥐가 먹었다고 바꿔줌
			}

			else //도토리를 먹지않은 상태라면
			{
				//개구리를 없애주고
				g_myChar.isCreat = false;

				//뼈다귀가된다.
				g_myfood.food_index = 2;
				g_myfood.food_image[2].Draw(g_BackBuffer, g_myChar.x, g_myChar.y,
					g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
					g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);

				g_myfood.isCreat[2] = true;


			}
		}


		//개구리가 뼈다귀로 변했고 check1이 false이고, check2가 false일때
		if (g_myfood.isCreat[2] == true && g_myanimal2.Check1 == false && g_myanimal2.Check2 == false)
		{
			//check1을 true로 바꿔주고
			g_myanimal2.Check1 = true;

			//check1이 true이면
			if (g_myanimal2.Check1 == true)
			{
				g_duration = 0; //시간을 0으로 만들어주고
				g_myanimal2.Check1 = false; //check1을 false로
				g_myanimal2.Check2 = true; //check2를 true로 바꾸어준다. (이 if문이 다시 실행되지않게하기위해)
			}
		}

		//시간값이 1000을 넘어서고(1초가 지남), check2가 true일때
		if (g_duration >= 1000 && g_myanimal2.Check2 == true)
		{
			mciSendCommandW(soundplayCheck, MCI_CLOSE, 0, NULL); //메인음악을 종료하고
			sndPlaySoundA("./Data/sound/fail.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //퀘스트실패
			if (MessageBox(hWnd, "개구리가 잡아먹혔습니다. 당신이 개구리를 죽였어요.", "퀘스트 실패!!!!!", MB_RETRYCANCEL) == IDRETRY) //RETRY버튼을 누르면
			{
				isGameStart = false; //게임인트로로 돌아가고

									 //초기정보
				Initial_content(); //초기화시킨다.
			}

			else // cancle버튼을 누르면
			{
				//게임을 종료한다.
				exit(0);
			}

		}

		/*==================================================================*/




		/*뱀 음식주기
		==================================================================*/

		//고기가 존재하고 다람쥐가 음식을 먹은상태이면
		if (g_myfood.isCreat[5] == true && g_myfood.isanimalEat2 == true)
		{
			//고기와 개구리가 충돌함
			if (meat.Intersect(g_myChar.rcCollision) == TRUE)
			{
				//맥주가 존재하는 상태이면
				if (g_myfood.isCreat[6] == true)
				{
					sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					g_myfood.isCreat[5] = false; //고기를 지워준다. => 고기아이템이 위에생김
				}

				//맥주가 존재하지않는 상태이면
				else
				{
					sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					//맥주를 존재하는 상태로 만들어준다.
					g_myfood.isCreat[6] = true;
					g_myfood.isCreat[5] = false; //고기를 지워준다.
				}
			}
		}

		//맥주가 존재하고
		if (g_myfood.isCreat[6] == true && g_myfood.isanimalEat2 == true)
		{
			//맥주와 개구리가 충돌함
			if (beer.Intersect(g_myChar.rcCollision) == TRUE)
			{
				//맥주가 존재하는 상태이면(개구리가 맥주를 먹지않음)
				if (g_myfood.isCreat[5] == true)
				{
					sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					g_myfood.isCreat[6] = false; //고기를 지워준다.
				}

				//맥주가 존재하지않는 상태이면(개구리가 맥주를 먹음)
				else
				{
					sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
					//맥주를 존재하는 상태로 만들어준다.
					g_myfood.isCreat[5] = true;
					g_myfood.isCreat[6] = false; //고기를 지워준다.
				}
			}
		}


		//고기를먹으면 위에 아이템으로 나타남
		if (g_myfood.isCreat[5] == false && g_myfood.isanimalEat3 == false)
		{
			g_myfood.food_index = 5;

			g_myfood.food_image[5].Draw(g_BackBuffer, 0, 0,
				g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
				g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);
		}


		//맥주를먹으면 위에 아이템으로 나타남
		if (g_myfood.isCreat[6] == false)
		{
			g_myfood.food_index = 6;

			g_myfood.food_image[6].Draw(g_BackBuffer, 0, 0,
				g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
				g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);
		}


		//뱀과 개구리가 충돌함
		if (snake.Intersect(g_myChar.rcCollision) == TRUE && g_myleaf.isHit[12] == false)
		{

			if (g_myfood.isCreat[5] == false) //고기를 먹은 상태라면
			{
				if (g_myfood.isanimalEat3 == false)
				{
					sndPlaySoundA("./Data/sound/happy.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //퀘스트성공음악 재생
				}

				isScore[1] = true; //200점 스코어판 제거
				g_myfood.isanimalEat3 = true; //뱀이 먹었다고 바꿔줌
			}

			else //고기를 먹지않은 상태라면
			{
				//개구리를 없애주고
				g_myChar.isCreat = false;

				//뼈다귀가된다.
				g_myfood.food_index = 2;
				g_myfood.food_image[2].Draw(g_BackBuffer, g_myChar.x, g_myChar.y,
					g_myfood.food[g_myfood.food_index].x, g_myfood.food[g_myfood.food_index].y,
					g_myfood.food[g_myfood.food_index].width, g_myfood.food[g_myfood.food_index].height);

				g_myfood.isCreat[2] = true;


			}
		}


		//개구리가 뼈다귀로 변했고 check1이 false이고, check2가 false일때
		if (g_myfood.isCreat[2] == true && g_myanimal3.Check1 == false && g_myanimal3.Check2 == false)
		{
			//check1을 true로 바꿔주고
			g_myanimal3.Check1 = true;

			//check1이 true이면
			if (g_myanimal3.Check1 == true)
			{
				g_duration = 0; //시간을 0으로 만들어주고
				g_myanimal3.Check1 = false; //check1을 false로
				g_myanimal3.Check2 = true; //check2를 true로 바꾸어준다. (이 if문이 다시 실행되지않게하기위해)
			}
		}


		//시간값이 1000을 넘어서고(1초가 지남), check2가 true일때
		if (g_duration >= 1000 && g_myanimal3.Check2 == true)
		{


			mciSendCommandW(soundplayCheck, MCI_CLOSE, 0, NULL); //메인음악을 종료하고
			sndPlaySoundA("./Data/sound/fail.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //퀘스트실패   

			if (MessageBox(hWnd, "개구리가 잡아먹혔습니다. 당신이 개구리를 죽였어요.", "퀘스트 실패!!!!!", MB_RETRYCANCEL) == IDRETRY) //RETRY버튼을 누르면
			{
				isGameStart = false; //게임인트로로 돌아가고

									 //초기정보
				Initial_content(); //초기화시킨다.
			}

			else // cancle버튼을 누르면
			{
				//게임을 종료한다.
				exit(0);
			}

		}

		/*==================================================================*/



		/*새와 부딪힘
		==================================================================*/
	
		if (bird.Intersect(g_myChar.rcCollision) == TRUE)
		{
			mciSendCommandW(soundplayCheck, MCI_CLOSE, 0, NULL); //메인음악을 종료하고
			sndPlaySoundA("./Data/sound/fail.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //퀘스트실패


			if (MessageBox(hWnd, "으악! 새에게 잡혔습니다. 다시 하시겠어요?", "이런!", MB_RETRYCANCEL) == IDRETRY)
			{
				mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
					(DWORD)(LPVOID)& mciOpen);

				soundplayCheck = mciOpen.wDeviceID;

				mciSendCommand(soundplayCheck, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // 계속반복 //메인음악
					(DWORD)(LPVOID)& m_mciPlayParms);

				isGameStart = false; //게임인트로로 돌아가고

									 //초기정보
				Initial_content(); //초기화시킨다.
			}

			else // cancle버튼을 누르면
			{
				//게임을 종료한다.
				exit(0);
			}

		}

		/*==================================================================*/




		/*개구리가 연잎을 밟지않으면 떨어짐
		==================================================================*/

		//스페이스바를 누르지않았을때
		if (g_myChar.isMove != true)
		{
			//연잎1와 한번 충돌한 후, 연잎1의 범위에서 벗어났을 경우 6
			if ((g_myChar.x < 110 || g_myChar.x > 320) && g_myleaf.check[0] == true)
			{
				//개구리의 y값을 50씩 증가시킨다. (개구리를 아래로 떨어뜨린다.)
				g_myChar.y += 50;

				//만약 개구리의 y값이 740보다 커진경우
				if (g_myChar.y > 740)
				{
					//연잎1과 충돌하지않은 상태로 만들어준다.
					g_myleaf.check[0] = false;
				}
			}

			//연잎2와 충돌한 후, 연잎2의 범위에서 벗어낫을 경우 
			if ((g_myChar.x < 470 || g_myChar.x > 660) && g_myleaf.check[1] == true)
			{
				//개구리의 y값을 50씩 증가시킨다. (개구리를 아래로 떨어뜨린다.)
				g_myChar.y += 50;

				//만약 개구리의 y값이 740보다 커지면서, 땅과 연잎1이 사라지지않은경우(연잎 4와 충돌하지않은 경우)
				if (g_myChar.y > 740 && g_myleaf.isHit[1] == true)
				{
					//연잎1과 충돌하지않은 상태로 만들어준다.
					g_myleaf.check[1] = false;
				}
			}

			//연잎3와 충돌한 후, 연잎3의 범위에서 벗어낫을 경우 
			if ((g_myChar.x < 130 || g_myChar.x > 310) && g_myleaf.check[2] == true)
			{
				//개구리의 y값을 50씩 증가시킨다. (개구리를 아래로 떨어뜨린다.)
				g_myChar.y += 50;

				//만약 개구리의 y값이 740보다 커지면서, 땅과 연잎1이 사라지지않은경우(연잎 4와 충돌하지않은 경우)
				if (g_myChar.y > 740 && g_myleaf.isHit[1] == true)
				{
					//연잎1과 충돌하지않은 상태로 만들어준다.
					g_myleaf.check[2] = false;
				}
			}

			//연잎4와 충돌한 후, 연잎4의 범위에서 벗어낫을 경우 
			if ((g_myChar.x < 380 || g_myChar.x > 600) && g_myleaf.check[3] == true)
			{
				//개구리의 y값을 50씩 증가시킨다. (개구리를 아래로 떨어뜨린다.)
				g_myChar.y += 50;
			}

			//연잎5와 충돌한 후, 연잎5의 범위에서 벗어낫을 경우 
			if ((g_myChar.x < 600 || g_myChar.x > 800) && g_myleaf.check[4] == true)
			{
				//개구리의 y값을 50씩 증가시킨다. (개구리를 아래로 떨어뜨린다.)
				g_myChar.y += 50;
			}

			//연잎6과 충돌한 후, 연잎6의 범위에서 벗어낫을 경우 
			if ((g_myChar.x < 200 || g_myChar.x > 400) && g_myleaf.check[5] == true)
			{
				//개구리의 y값을 50씩 증가시킨다. (개구리를 아래로 떨어뜨린다.)
				g_myChar.y += 50;
			}

			//연잎7과 충돌한 후, 연잎7의 범위에서 벗어낫을 경우 
			if ((g_myChar.x < 500 || g_myChar.x > 700) && g_myleaf.check[6] == true)
			{
				//개구리의 y값을 50씩 증가시킨다. (개구리를 아래로 떨어뜨린다.)
				g_myChar.y += 50;
			}

			//연잎8과 충돌한 후, 연잎8의 범위에서 벗어낫을 경우 
			if ((g_myChar.x < 200 || g_myChar.x > 420) && g_myleaf.check[7] == true)
			{
				//개구리의 y값을 50씩 증가시킨다. (개구리를 아래로 떨어뜨린다.)
				g_myChar.y += 50;
			}

			//연잎9과 충돌한 후, 연잎9의 범위에서 벗어낫을 경우 
			if ((g_myChar.x < 80 || g_myChar.x > 220) && g_myleaf.check[8] == true)
			{
				//개구리의 y값을 50씩 증가시킨다. (개구리를 아래로 떨어뜨린다.)
				g_myChar.y += 50;
			}

			//연잎10과 충돌한 후, 연잎10의 범위에서 벗어낫을 경우 
			if ((g_myChar.x < 310 || g_myChar.x > 530) && g_myleaf.check[9] == true)
			{
				//개구리의 y값을 50씩 증가시킨다. (개구리를 아래로 떨어뜨린다.)
				g_myChar.y += 50;
			}

			//연잎11과 충돌한 후, 연잎11의 범위에서 벗어낫을 경우 
			if ((g_myChar.x < 470 || g_myChar.x > 740) && g_myleaf.check[10] == true)
			{
				//개구리의 y값을 50씩 증가시킨다. (개구리를 아래로 떨어뜨린다.)
				g_myChar.y += 50;
			}

			//연잎12과 충돌한 후, 연잎12의 범위에서 벗어낫을 경우 
			if ((g_myChar.x < 330 || g_myChar.x > 540) && g_myleaf.check[11] == true)
			{
				//개구리의 y값을 50씩 증가시킨다. (개구리를 아래로 떨어뜨린다.)
				g_myChar.y += 50;
			}

			//연잎13과 충돌한 후, 연잎13의 범위에서 벗어낫을 경우 
			if ((g_myChar.x < 570 || g_myChar.x > 880) && g_myleaf.check[12] == true)
			{
				//개구리의 y값을 50씩 증가시킨다. (개구리를 아래로 떨어뜨린다.)
				g_myChar.y += 50;
			}

			//연잎14과 충돌한 후, 연잎14의 범위에서 벗어낫을 경우 
			if ((g_myChar.x < 270 || g_myChar.x > 520) && g_myleaf.check[13] == true)
			{
				//개구리의 y값을 50씩 증가시킨다. (개구리를 아래로 떨어뜨린다.)
				g_myChar.y += 50;
			}

			//연잎15과 충돌한 후, 연잎15의 범위에서 벗어낫을 경우 
			if ((g_myChar.x < 10 || g_myChar.x > 220) && g_myleaf.check[14] == true)
			{
				//개구리의 y값을 50씩 증가시킨다. (개구리를 아래로 떨어뜨린다.)
				g_myChar.y += 50;
			}

			//연잎16과 충돌한 후, 연잎16의 범위에서 벗어낫을 경우 
			if ((g_myChar.x < 300 || g_myChar.x > 500) && g_myleaf.check[15] == true)
			{
				//개구리의 y값을 50씩 증가시킨다. (개구리를 아래로 떨어뜨린다.)
				g_myChar.y += 50;
			}

			//연잎17과 충돌한 후, 연잎17의 범위에서 벗어낫을 경우 //뱀자리
			if ((g_myChar.x < 580 || g_myChar.x > 800) && g_myleaf.check[16] == true)
			{
				//개구리의 y값을 50씩 증가시킨다. (개구리를 아래로 떨어뜨린다.)
				g_myChar.y += 50;
			}

			//연잎18과 충돌한 후, 연잎18의 범위에서 벗어낫을 경우 
			if ((g_myChar.x < 300 || g_myChar.x > 520) && g_myleaf.check[17] == true)
			{
				//개구리의 y값을 50씩 증가시킨다. (개구리를 아래로 떨어뜨린다.)
				g_myChar.y += 50;
			}

			//연잎19과 충돌한 후, 연잎19의 범위에서 벗어낫을 경우 
			if ((g_myChar.x < 100 || g_myChar.x > 320) && g_myleaf.check[18] == true)
			{
				//개구리의 y값을 50씩 증가시킨다. (개구리를 아래로 떨어뜨린다.)
				g_myChar.y += 50;
			}


			/*==================================================================*/

			//만약 개구리의 y값이 900보다 커질경우(아래로 떨어져서)
			if (g_myleaf.isHit[16] == true) //마지막 연잎을 밟지않았을때
			{
				//개구리가 떨어지면
				if (g_myChar.y > 1200)
				{
					mciSendCommandW(soundplayCheck, MCI_CLOSE, 0, NULL); //메인음악을 종료하고
					sndPlaySoundA("./Data/sound/falling.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //퀘스트실패 - 떨어짐 음악 재생

					if (MessageBox(hWnd, "개구리가 떨어지고 말았어요.. 다시하시겠어요?", "아이쿠!", MB_RETRYCANCEL) == IDRETRY) //retry버튼을 누르면
					{

						mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
							(DWORD)(LPVOID)& mciOpen);

						soundplayCheck = mciOpen.wDeviceID;

						mciSendCommand(soundplayCheck, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // 계속반복 //메인음악
							(DWORD)(LPVOID)& m_mciPlayParms);

						isGameStart = false; //게임인트로로 돌아가고

											 //초기정보
						Initial_content(); //초기화시킨다.
					}

					// cancle버튼을 누르면
					else
					{
						//게임을 종료한다.
						exit(0);
					}


				}
			}

			//마지막 연잎을 밟았을때
			else
			{
				//개구리의 x좌표가 0보다 작거나 같고, 개구리가 떨어질경우(우물밖으로)
				if (g_myChar.y > 1200 && g_myChar.x <= 0)
				{
					mciSendCommandW(soundplayCheck, MCI_CLOSE, 0, NULL); //메인음악을 종료하고
					sndPlaySoundA("./Data/sound/Clear.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //퀘스트성공음악 재생


					if (MessageBox(hWnd, "개구리가 우물을 탈출했습니다! 여러분 덕분이에요!!!", "퀘스트 성공!!!!", MB_OK) == IDOK) //ok버튼을 누르면
					{

						/*스테이지 2로 넘어가는 부분*/

						if (MessageBox(hWnd, "스테이지 2로 넘어갑니다", "퀘스트 성공!!!!", MB_OK) == IDOK) //ok버튼을 누르면
						{
							//스테이지 2로 넘어가게 bool형을 true로 바꿔줌.
							g_myChar.isStage = true;

							g_myChar.x = 60;
							g_myChar.y = 745;
						}

					}


				}

				//개구리의 x좌표가 0보다 크고, 개구리가 떨어질경우(우물안으로)
				else if (g_myChar.y > 1200 && g_myChar.x > 0)
				{
					mciSendCommandW(soundplayCheck, MCI_CLOSE, 0, NULL); //메인음악을 종료하고
					sndPlaySoundA("./Data/sound/falling.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //퀘스트실패 - 떨어짐 음악 재생


					if (MessageBox(hWnd, "거의 다 왔는데,,, 아깝네요! 다시 하시겠어요?", "퀘스트 실패!!!!", MB_RETRYCANCEL) == IDRETRY) //retry버튼을 누를경우
					{

						mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
							(DWORD)(LPVOID)& mciOpen);

						soundplayCheck = mciOpen.wDeviceID;

						mciSendCommand(soundplayCheck, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // 계속반복 //메인음악
							(DWORD)(LPVOID)& m_mciPlayParms);


						isGameStart = false; //게임인트로로 돌아가고

											 //초기정보
						Initial_content(); //게임을 초기화한다.
					}

					else //cancle버튼을 누를경우
					{
						//게임을 종료한다.
						exit(0);
					}
				}
			}
		}
		/*==================================================================*/

	}

	//스테이지 2
	if (g_myChar.isStage == true)
	{
		/*======================== 사각형출력 ============================*/

		/*g_myanimal5.DrawCollisionRect_animal5(g_BackBuffer);
		g_myChar.DrawCollisionRect(g_BackBuffer);
		g_mykey.DrawCollisionRect_key(g_BackBuffer);
		g_mykey.DrawCollisionRect_key(g_BackBuffer);
		g_myenemy.DrawCollisionRect_enemy(g_BackBuffer);
		g_mywater.DrawCollisionRect_water(g_BackBuffer);*/

		/*=================================================================*/


		Rect bird2 = g_myanimal5.rcCollision_animal5[0]; // stage 2 새
		Rect key1 = g_mykey.rcCollision_key[0]; // stage 2 열쇠1
		Rect key2 = g_mykey.rcCollision_key[1]; // stage 2 열쇠2
		Rect key3 = g_mykey.rcCollision_key[2]; // stage 2 열쇠3
		Rect enemy1 = g_myenemy.rcCollision_enemy[0]; // stage 2 적1
		Rect enemy2 = g_myenemy.rcCollision_enemy[1]; // stage 2 적2
		Rect enemy3 = g_myenemy.rcCollision_enemy[2]; // stage 2 적3
		Rect enemy4 = g_myenemy.rcCollision_enemy[3]; // stage 2 적4
		Rect enemy5 = g_myenemy.rcCollision_enemy[4]; // stage 2 적5
		Rect water = g_mywater.rcCollision_water[0]; // stage 2 물

		//새와충돌
		if (bird2.Intersect(g_myChar.rcCollision) == TRUE)
		{
			mciSendCommandW(soundplayCheck2, MCI_CLOSE, 0, NULL); //메인음악을 종료하고
			sndPlaySoundA("./Data/sound/fail.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //퀘스트실패


			if (MessageBox(hWnd, "으악! 새에게 잡혔습니다. 다시 하시겠어요?", "이런!", MB_RETRYCANCEL) == IDRETRY)
			{
				mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
					(DWORD)(LPVOID)& mciOpen2);

				soundplayCheck2 = mciOpen2.wDeviceID;

				mciSendCommand(soundplayCheck2, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // 계속반복 //메인음악
					(DWORD)(LPVOID)& m_mciPlayParms);

				isGameStart = false; //게임인트로로 돌아가고

				//초기정보
				Initial_content(); //초기화시킨다.
			}

			else // cancle버튼을 누르면
			{
				//게임을 종료한다.
				exit(0);
			}

		}


		//열쇠1 충돌
		if (key1.Intersect(g_myChar.rcCollision) == TRUE)
		{
			g_mykey.keyscore = 1;
			if (g_mykey.isCreat[0] == true)
			{
				sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
			}
			g_mykey.isCreat[0] = false;
		}

		//열쇠2 충돌
		if (key2.Intersect(g_myChar.rcCollision) == TRUE)
		{
			g_mykey.keyscore = 2;
			if (g_mykey.isCreat[1] == true)
			{
				sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
			}
			g_mykey.isCreat[1] = false;
		}

		//열쇠3 충돌
		if (key3.Intersect(g_myChar.rcCollision) == TRUE)
		{
			g_mykey.keyscore = 3;
			if (g_mykey.isCreat[2] == true)
			{
				sndPlaySoundA("./Data/sound/Itemget.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC);
			}
			g_mykey.isCreat[2] = false;
		}




		//적1 충돌
		if (enemy1.Intersect(g_myChar.rcCollision) == TRUE)
		{
			mciSendCommandW(soundplayCheck2, MCI_CLOSE, 0, NULL); //메인음악을 종료하고
			sndPlaySoundA("./Data/sound/fail.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //퀘스트실패

			if (MessageBox(hWnd, "다시 하시겠어요?", "이런! 개구리가 죽었어요", MB_RETRYCANCEL) == IDRETRY)
			{
				mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
					(DWORD)(LPVOID)& mciOpen2);

				soundplayCheck2 = mciOpen2.wDeviceID;

				mciSendCommand(soundplayCheck2, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // 계속반복 //메인음악
					(DWORD)(LPVOID)& m_mciPlayParms);

				isGameStart = false; //게임인트로로 돌아가고

									 //초기정보
				Initial_content(); //초기화시킨다.
			}

			else
			{
				exit(0);
			}
		}

		//적2 충돌
		if (enemy2.Intersect(g_myChar.rcCollision) == TRUE)
		{
			mciSendCommandW(soundplayCheck2, MCI_CLOSE, 0, NULL); //메인음악을 종료하고
			sndPlaySoundA("./Data/sound/fail.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //퀘스트실패

			if (MessageBox(hWnd, "다시 하시겠어요?", "이런! 개구리가 죽었어요", MB_RETRYCANCEL) == IDRETRY)
			{
				mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
					(DWORD)(LPVOID)& mciOpen2);

				soundplayCheck2 = mciOpen2.wDeviceID;

				mciSendCommand(soundplayCheck2, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // 계속반복 //메인음악
					(DWORD)(LPVOID)& m_mciPlayParms);

				isGameStart = false; //게임인트로로 돌아가고

//초기정보
Initial_content(); //초기화시킨다.
			}

			else
			{
				exit(0);
			}
		}

		//적3 충돌
		if (enemy3.Intersect(g_myChar.rcCollision) == TRUE)
		{
			mciSendCommandW(soundplayCheck2, MCI_CLOSE, 0, NULL); //메인음악을 종료하고
			sndPlaySoundA("./Data/sound/fail.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //퀘스트실패

			if (MessageBox(hWnd, "다시 하시겠어요?", "이런! 개구리가 죽었어요", MB_RETRYCANCEL) == IDRETRY)
			{
				mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
					(DWORD)(LPVOID)& mciOpen2);

				soundplayCheck2 = mciOpen2.wDeviceID;

				mciSendCommand(soundplayCheck2, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // 계속반복 //메인음악
					(DWORD)(LPVOID)& m_mciPlayParms);

				isGameStart = false; //게임인트로로 돌아가고

									 //초기정보
				Initial_content(); //초기화시킨다.
			}

			else
			{
				exit(0);
			}
		}

		//적4 충돌
		if (enemy4.Intersect(g_myChar.rcCollision) == TRUE)
		{
			mciSendCommandW(soundplayCheck2, MCI_CLOSE, 0, NULL); //메인음악을 종료하고
			sndPlaySoundA("./Data/sound/fail.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //퀘스트실패

			if (MessageBox(hWnd, "다시 하시겠어요?", "이런! 개구리가 죽었어요", MB_RETRYCANCEL) == IDRETRY)
			{
				mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
					(DWORD)(LPVOID)& mciOpen2);

				soundplayCheck2 = mciOpen2.wDeviceID;

				mciSendCommand(soundplayCheck2, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // 계속반복 //메인음악
					(DWORD)(LPVOID)& m_mciPlayParms);

				isGameStart = false; //게임인트로로 돌아가고

									 //초기정보
				Initial_content(); //초기화시킨다.
			}

			else
			{
				exit(0);
			}
		}

		//적5 충돌
		if (enemy5.Intersect(g_myChar.rcCollision) == TRUE)
		{
			mciSendCommandW(soundplayCheck2, MCI_CLOSE, 0, NULL); //메인음악을 종료하고
			sndPlaySoundA("./Data/sound/fail.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //퀘스트실패

			if (MessageBox(hWnd, "다시 하시겠어요?", "이런! 개구리가 죽었어요", MB_RETRYCANCEL) == IDRETRY)
			{
				mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
					(DWORD)(LPVOID)& mciOpen2);

				soundplayCheck2 = mciOpen2.wDeviceID;

				mciSendCommand(soundplayCheck2, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // 계속반복 //메인음악
					(DWORD)(LPVOID)& m_mciPlayParms);

				isGameStart = false; //게임인트로로 돌아가고

									 //초기정보
				Initial_content(); //초기화시킨다.
			}

			else
			{
				exit(0);
			}
		}


		//물과 충돌
		if (water.Intersect(g_myChar.rcCollision) == TRUE)
		{
			mciSendCommandW(soundplayCheck2, MCI_CLOSE, 0, NULL); //메인음악을 종료하고
			sndPlaySoundA("./Data/sound/Clear.wav", SND_ASYNC | SND_NODEFAULT | SND_ASYNC); //퀘스트성공음악 재생

			if (MessageBox(hWnd, "개구리가 연못으로 무사히 돌아갔어요!!", "퀘스트 성공!!!!", MB_OK) == IDOK)
			{
				exit(0);
			}
		}


	}

}

