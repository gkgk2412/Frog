#pragma once

#include "MyImage.h"
#include "myCharacter.h"


#define MAXANIMAL3		1

struct ANIMALINFO3
{
	int x;
	int y;
	int width;
	int height;
	DWORD animal_time;
};

struct ANIMAL3
{
	int x[MAXANIMAL3];
	int y[MAXANIMAL3];

	Rect rcCollision_animal3[MAXANIMAL3];
	Pen *gPen_animal3;

	~ANIMAL3()
	{
		delete gPen_animal3;
	}


	ANIMALINFO3 animal3[100];

	int animal3_index;
	int animal3_nFrame;

	bool Check1 = false;
	bool Check2 = false;

	CMyImage animal3_image[MAXANIMAL3];

	void LoadImage_animal3(char *filename, int k)
	{
		animal3_image[k].Load(filename);
	}


	void LoadAniInfo_animal3(char *filename)
	{
		FILE *fp;
		fopen_s(&fp, filename, "rt");
		fscanf_s(fp, "%d", &animal3_nFrame);

		for (int i = 0; i < animal3_nFrame; i++)
		{
			fscanf_s(fp, "%d %d %d %d %d",
				&animal3[i].x, &animal3[i].y,
				&animal3[i].width, &animal3[i].height,
				&animal3[i].animal_time);
		}

		fclose(fp);
		gPen_animal3 = new Pen(Color(255, 0, 0));
	}


	void BoxCol_animal3()
	{
		for (int i = 0; i < MAXANIMAL3; i++)
		{
			//충돌박스 범위조정
			rcCollision_animal3[i].X = x[i] + 10;
			rcCollision_animal3[i].Y = y[i] + 10;
			rcCollision_animal3[i].Width = animal3[animal3_index].width -30;
			rcCollision_animal3[i].Height = animal3[animal3_index].height;
		}
	}


	void DrawCollisionRect_animal3(Graphics *g)
	{
		for (int i = 0; i < MAXANIMAL3; i++)
		{
			g->DrawRectangle(gPen_animal3, rcCollision_animal3[i]);
		}
	}

};