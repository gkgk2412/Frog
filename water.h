#pragma once

#include "MyImage.h"
#include "myCharacter.h"


#define MAXWATER		1


struct WATERINFO
{
	int x;
	int y;
	int width;
	int height;
	DWORD water_time;
};

struct  WATER
{
	int x[MAXWATER];
	int y[MAXWATER];

	Rect rcCollision_water[MAXWATER];

		Pen *gPen_water;

	~WATER()
	{
		delete gPen_water;
	}

	WATERINFO water[100];
	int water_index;
	int water_nFrame;

	CMyImage water_image[MAXWATER];

	void LoadImage_water(char *filename, int k)
	{
		water_image[k].Load(filename);
	}

	void LoadAniInfo_water(char *filename)
	{
		FILE *fp;
		fopen_s(&fp, filename, "rt");
		fscanf_s(fp, "%d", &water_nFrame);

		for (int i = 0; i < water_nFrame; i++)
		{
			fscanf_s(fp, "%d %d %d %d %d",
				&water[i].x, &water[i].y,
				&water[i].width, &water[i].height,
				&water[i].water_time);
		}

		fclose(fp);
		gPen_water = new Pen(Color(255, 0, 0));
	}

	void BoxCol()
	{
		//충돌박스 범위조정
		for (int i = 0; i < MAXWATER; i++)
		{
			rcCollision_water[i].X = x[i];
			rcCollision_water[i].Y = y[i];
			rcCollision_water[i].Width = water[water_index].width;
			rcCollision_water[i].Height = water[water_index].height -40;
		}


	}



	void DrawCollisionRect_water(Graphics *g)
	{
		for (int i = 0; i < MAXWATER; i++)
		{
			g->DrawRectangle(gPen_water, rcCollision_water[i]);

		}
	}





};




