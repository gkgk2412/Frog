#pragma once

#include "MyImage.h"
#include "myCharacter.h"


#define MAXENEMY		5


struct enemyINFO
{
	int x;
	int y;
	int width;
	int height;
	DWORD enemy_time;
};

struct  ENEMY
{
	int x[MAXENEMY];
	int y[MAXENEMY];

	Rect rcCollision_enemy[MAXENEMY];
	Pen *gPen_enemy;

	~ENEMY()
	{
		delete gPen_enemy;
	}

	enemyINFO enemy[100];
	int enemy_index;
	int enemy_nFrame;

	CMyImage enemy_image[MAXENEMY];

	void LoadImage_enemy(char *filename, int k)
	{
		enemy_image[k].Load(filename);
	}

	void LoadAniInfo_enemy(char *filename)
	{
		FILE *fp;
		fopen_s(&fp, filename, "rt");
		fscanf_s(fp, "%d", &enemy_nFrame);

		for (int i = 0; i < enemy_nFrame; i++)
		{
			fscanf_s(fp, "%d %d %d %d %d",
				&enemy[i].x, &enemy[i].y,
				&enemy[i].width, &enemy[i].height,
				&enemy[i].enemy_time);
		}

		fclose(fp);
		gPen_enemy = new Pen(Color(255, 0, 0));
	}

	void BoxCol()
	{
		//충돌박스 범위조정
		for (int i = 0; i < MAXENEMY; i++)
		{
			rcCollision_enemy[i].X = x[i]+30;
			rcCollision_enemy[i].Y = y[i]+40;
			rcCollision_enemy[i].Width = enemy[enemy_index].width-70;
			rcCollision_enemy[i].Height = enemy[enemy_index].height-50;
		}


	}



	void DrawCollisionRect_enemy(Graphics *g)
	{
		for (int i = 0; i < MAXENEMY; i++)
		{
			g->DrawRectangle(gPen_enemy, rcCollision_enemy[i]);

		}
	}





};




