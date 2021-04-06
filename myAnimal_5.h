#pragma once

#include "MyImage.h"
#include "myCharacter.h"


#define MAXANIMAL5		1

struct ANIMALINFO5
{
	int x;
	int y;
	int width;
	int height;
	DWORD animal_time;
};

struct ANIMAL5
{
	int x[MAXANIMAL5];
	int y[MAXANIMAL5];
	int xDir; //���� �̵� ����


	Rect rcCollision_animal5[MAXANIMAL5];
	Pen* gPen_animal5;

	~ANIMAL5()
	{
		delete gPen_animal5;
	}


	ANIMALINFO5 animal5[100];

	int animal5_index;
	int animal5_nFrame;

	float speed;//x�ӵ�

	CMyImage animal5_image[MAXANIMAL5];

	void LoadImage_animal5(char* filename, int k)
	{
		animal5_image[k].Load(filename);
	}


	void LoadAniInfo_animal5(char* filename)
	{
		FILE* fp;
		fopen_s(&fp, filename, "rt");
		fscanf_s(fp, "%d", &animal5_nFrame);

		for (int i = 0; i < animal5_nFrame; i++)
		{
			fscanf_s(fp, "%d %d %d %d %d",
				&animal5[i].x, &animal5[i].y,
				&animal5[i].width, &animal5[i].height,
				&animal5[i].animal_time);
		}

		fclose(fp);
		gPen_animal5 = new Pen(Color(255, 0, 0));
	}


	void BoxCol_animal5()
	{
		for (int i = 0; i < MAXANIMAL5; i++)
		{
			//�浹�ڽ� ��������
			rcCollision_animal5[i].X = x[i] + 20;
			rcCollision_animal5[i].Y = y[i];
			rcCollision_animal5[i].Width = animal5[animal5_index].width - 40;
			rcCollision_animal5[i].Height = animal5[animal5_index].height -10;
		}
	}


	void DrawCollisionRect_animal5(Graphics* g)
	{
		for (int i = 0; i < MAXANIMAL5; i++)
		{
			g->DrawRectangle(gPen_animal5, rcCollision_animal5[i]);
		}
	}


	void Move(int xDir, DWORD deltaTime)
	{
		speed = 300; // ���� x���� ����
		x[0] += (xDir * speed * deltaTime / 1000);
	}

};