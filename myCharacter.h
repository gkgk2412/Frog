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

	bool isCreat = true; //��������������..
	bool isLeft = true;
	bool isMove = false;
	bool isQuest = false; //���ӽ����Ҷ� false�϶� �޼����ڽ��� �����ְ� false�� �����, ok�� ������ true�� ��������� bool�� ����
	bool isQuest2 = false; //��������2 �����Ҷ� false�϶� �޼����ڽ��� �����ְ� false�� �����, ok�� ������ true�� ��������� bool�� ����

   //�������� ����(false�� 1��������  true�� 2��������)
	bool isStage = false;

	//�������� 2�� �޹����ǥ
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
		//�浹�ڽ� ��������
		rcCollision.X = x+10;
		rcCollision.Y = y+10;
		rcCollision.Width = ani[index].width +10;
		rcCollision.Height = ani[index].height +5;

		}

		else
		{
		//�浹�ڽ� ��������
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