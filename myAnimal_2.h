#pragma once

#include "MyImage.h"
#include "myCharacter.h"


#define MAXANIMAL2		1

struct ANIMALINFO2
{
	int x;
	int y;
	int width;
	int height;
	DWORD animal_time;
};

struct ANIMAL2
{
	int x[MAXANIMAL2];
	int y[MAXANIMAL2];

	Rect rcCollision_animal2[MAXANIMAL2];
	Pen *gPen_animal2;

	~ANIMAL2()
	{
		delete gPen_animal2;
	}


	ANIMALINFO2 animal2[100];

	int animal2_index;
	int animal2_nFrame;

	bool Check1 = false;
	bool Check2 = false;


	CMyImage animal2_image[MAXANIMAL2];

	void LoadImage_animal2(char *filename, int k)
	{
		animal2_image[k].Load(filename);
	}


	void LoadAniInfo_animal2(char *filename)
	{
		FILE *fp;
		fopen_s(&fp, filename, "rt");
		fscanf_s(fp, "%d", &animal2_nFrame);

		for (int i = 0; i < animal2_nFrame; i++)
		{
			fscanf_s(fp, "%d %d %d %d %d",
				&animal2[i].x, &animal2[i].y,
				&animal2[i].width, &animal2[i].height,
				&animal2[i].animal_time);
		}

		fclose(fp);
		gPen_animal2 = new Pen(Color(255, 0, 0));
	}


	void BoxCol_animal2()
	{
		for (int i = 0; i < MAXANIMAL2; i++)
		{
			//충돌박스 범위조정
			rcCollision_animal2[i].X = x[i] + 10;
			rcCollision_animal2[i].Y = y[i] + 10;
			rcCollision_animal2[i].Width = animal2[animal2_index].width;
			rcCollision_animal2[i].Height = animal2[animal2_index].height;
		}
	}


	void DrawCollisionRect_animal2(Graphics *g)
	{
		for (int i = 0; i < MAXANIMAL2; i++)
		{
			g->DrawRectangle(gPen_animal2, rcCollision_animal2[i]);
		}
	}

};