#pragma once

#include "MyImage.h"
#include "myCharacter.h"


#define MAXFOOD		7

struct FOODINFO
{
	int x;
	int y;
	int width;
	int height;
	DWORD food_time;
};

struct FOOD
{
	int x[MAXFOOD];
	int y[MAXFOOD];
	
	Rect rcCollision_food[MAXFOOD];
	Pen *gPen_food;

	~FOOD()
	{
		delete gPen_food;
	}

	bool isCreat[MAXFOOD];
	bool isanimalEat;
	bool isanimalEat2;
	bool isanimalEat3;




	FOODINFO food[100];
	int food_index;
	int food_nFrame;

	CMyImage food_image[MAXFOOD];

	void LoadImage_food(char *filename, int k)
	{
		food_image[k].Load(filename);
	}

	void LoadAniInfo_food(char *filename)
	{
		FILE *fp;
		fopen_s(&fp, filename, "rt");
		fscanf_s(fp, "%d", &food_nFrame);

		for (int i = 0; i < food_nFrame; i++)
		{
			fscanf_s(fp, "%d %d %d %d %d",
				&food[i].x, &food[i].y,
				&food[i].width, &food[i].height,
				&food[i].food_time);
		}

		fclose(fp);
		gPen_food = new Pen(Color(255, 0, 0));
	}

	void BoxCol_food()
	{
		for (int i = 0; i < MAXFOOD; i++)
		{
			//충돌박스 범위조정
			rcCollision_food[i].X = x[i] + 10;
			rcCollision_food[i].Y = y[i] + 10;
			rcCollision_food[i].Width = food[food_index].width;
			rcCollision_food[i].Height = food[food_index].height;
		}
	}


	void DrawCollisionRect_food(Graphics *g)
	{
		for (int i = 0; i < MAXFOOD; i++)
		{
			g->DrawRectangle(gPen_food, rcCollision_food[i]);
		}
	}

};