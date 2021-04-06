#pragma once

#include "MyImage.h"
#include "myCharacter.h"


#define MAXANIMAL		1

struct ANIMALINFO
{
	int x;
	int y;
	int width;
	int height;
	DWORD animal_time;
};

struct ANIMAL
{
	int x[MAXANIMAL];
	int y[MAXANIMAL];

	Rect rcCollision_animal[MAXANIMAL];
	Pen *gPen_animal;

	~ANIMAL()
	{
		delete gPen_animal;
	}


	ANIMALINFO animal[100];

	int animal_index;
	int animal_nFrame;

	bool Check1 = false;
	bool Check2 = false;


	CMyImage animal_image[MAXANIMAL];

	void LoadImage_animal(char *filename, int k)
	{
		animal_image[k].Load(filename);
	}

	//토끼용
	void LoadAniInfo_animal(char *filename)
	{
		FILE *fp;
		fopen_s(&fp, filename, "rt");
		fscanf_s(fp, "%d", &animal_nFrame);

		for (int i = 0; i < animal_nFrame; i++)
		{
			fscanf_s(fp, "%d %d %d %d %d",
				&animal[i].x, &animal[i].y,
				&animal[i].width, &animal[i].height,
				&animal[i].animal_time);
		}

		fclose(fp);
		gPen_animal = new Pen(Color(255, 0, 0));
	}


	void BoxCol_animal()
	{
		for (int i = 0; i < MAXANIMAL; i++)
		{
			//충돌박스 범위조정
			rcCollision_animal[i].X = x[i] + 10;
			rcCollision_animal[i].Y = y[i] + 10;
			rcCollision_animal[i].Width = animal[animal_index].width;
			rcCollision_animal[i].Height = animal[animal_index].height;
		}
	}


	void DrawCollisionRect_animal(Graphics *g)
	{
		for (int i = 0; i < MAXANIMAL; i++)
		{
			g->DrawRectangle(gPen_animal, rcCollision_animal[i]);
		}
	}

};