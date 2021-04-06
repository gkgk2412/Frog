#pragma once

#include "MyImage.h"
#include "myCharacter.h"


#define MAXANIMAL4		1

struct ANIMALINFO4
{
	int x;
	int y;
	int width;
	int height;
	DWORD animal_time;
};

struct ANIMAL4
{
	int x[MAXANIMAL4];
	int y[MAXANIMAL4];
	int xDir; //새의 이동 조절
	int yDir; //새의 이동 조절

	Rect rcCollision_animal4[MAXANIMAL4];
	Pen* gPen_animal4;

	~ANIMAL4()
	{
		delete gPen_animal4;
	}


	ANIMALINFO4 animal4[100];

	int animal4_index;
	int animal4_nFrame;

	float speed;//x속도
	float speedy;//y속도

	bool Check1 = false;
	bool Check2 = false;

	CMyImage animal4_image[MAXANIMAL4];

	void LoadImage_animal4(char* filename, int k)
	{
		animal4_image[k].Load(filename);
	}


	void LoadAniInfo_animal4(char* filename)
	{
		FILE* fp;
		fopen_s(&fp, filename, "rt");
		fscanf_s(fp, "%d", &animal4_nFrame);

		for (int i = 0; i < animal4_nFrame; i++)
		{
			fscanf_s(fp, "%d %d %d %d %d",
				&animal4[i].x, &animal4[i].y,
				&animal4[i].width, &animal4[i].height,
				&animal4[i].animal_time);
		}

		fclose(fp);
		gPen_animal4 = new Pen(Color(255, 0, 0));
	}


	void BoxCol_animal4()
	{
		for (int i = 0; i < MAXANIMAL4; i++)
		{
			//충돌박스 범위조정
			rcCollision_animal4[i].X = x[i] +20;
			rcCollision_animal4[i].Y = y[i];
			rcCollision_animal4[i].Width = animal4[animal4_index].width - 80;
			rcCollision_animal4[i].Height = animal4[animal4_index].height -40;
		}
	}


	void DrawCollisionRect_animal4(Graphics* g)
	{
		for (int i = 0; i < MAXANIMAL4; i++)
		{
			g->DrawRectangle(gPen_animal4, rcCollision_animal4[i]);
		}
	}


	void Move(int xDir, int yDir, DWORD deltaTime)
	{
		if (xDir == -1)
		{
			speed = 500; // 새의 x값을 조절
			speedy = 60;
			y[0] += (yDir * speedy * deltaTime / 1000);
		}
			
		else
		{
			speed = 300;// 새의 x값을 조절
			speedy = 30;
			y[0] += (yDir * speedy * deltaTime / 1000);
		}



		x[0] += (xDir * speed * deltaTime / 1000);

	}

};