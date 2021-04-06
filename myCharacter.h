#pragma once

#include "MyImage.h"

struct ANIINFO
{
	int x;
	int y;
	int width;
	int height;
	DWORD ani_time;
};

struct  CHARACTER
{
	int x;
	int y;

	bool isCreat = true; //개구리생존여부..
	bool isLeft = true;
	bool isMove = false;
	bool isQuest = false; //게임시작할때 false일때 메세지박스를 띄울수있게 false로 만들고, ok를 누르면 true로 만들기위한 bool형 변수
	bool isQuest2 = false; //스테이지2 시작할때 false일때 메세지박스를 띄울수있게 false로 만들고, ok를 누르면 true로 만들기위한 bool형 변수

   //스테이지 조절(false면 1스테이지  true면 2스테이지)
	bool isStage = false;

	//스테이지 2의 뒷배경좌표
	int myback_x[20] = { 0,0,0,0,930,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	int myback_y[20] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	CMyImage image[30];


	ANIINFO ani[100];
	int index;
	int nFrame;
	float speed;
	Rect rcCollision;
	Pen *gPen;

	~CHARACTER()
	{
		delete gPen;
	}


	void LoadImage(char *filename, int k)
	{
		image[k].Load(filename);

	}

	void LoadAniInfo(char *filename)
	{
		FILE *fp;
		fopen_s(&fp, filename, "rt");
		fscanf_s(fp, "%d", &nFrame);

		for (int i = 0; i < nFrame; i++)
		{
			fscanf_s(fp, "%d %d %d %d %d",
				&ani[i].x, &ani[i].y,
				&ani[i].width, &ani[i].height,
				&ani[i].ani_time);
		}
		fclose(fp);

		gPen = new Pen(Color(255, 0, 0));
	}

	void Move(int xDir, int yDir, DWORD deltaTime)
	{

			x += (xDir * speed * deltaTime / 1000);
			y += (yDir * speed * deltaTime / 1000);

	

		if (isStage == false)
		{
		//충돌박스 범위조정
		rcCollision.X = x+10;
		rcCollision.Y = y+10;
		rcCollision.Width = ani[index].width +10;
		rcCollision.Height = ani[index].height +5;

		}

		else
		{
		//충돌박스 범위조정
		rcCollision.X = x+190;
		rcCollision.Y = y-35 ;
		rcCollision.Width = ani[index].width -30;
		rcCollision.Height = ani[index].height -20;
		}
	}

	void DrawCollisionRect(Graphics *g)
	{
		g->DrawRectangle(gPen, rcCollision);
	}
};