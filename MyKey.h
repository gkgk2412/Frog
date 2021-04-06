#pragma once

#include "MyImage.h"
#include "myCharacter.h"


#define MAXKEY		3


struct keyINFO
{
	int x;
	int y;
	int width;
	int height;
	DWORD key_time;
};

struct  KEY
{
	int x[MAXKEY];
	int y[MAXKEY];

	Rect rcCollision_key[MAXKEY];

	int keyscore = 0;
	
	bool isCreat[3] = {true, true, true};
	Pen *gPen_key;

	~KEY()
	{
		delete gPen_key;
	}

	keyINFO key[100];
	int key_index;
	int key_nFrame;

	CMyImage key_image[MAXKEY];

	void LoadImage_key(char *filename, int k)
	{
		key_image[k].Load(filename);
	}

	void LoadAniInfo_key(char *filename)
	{
		FILE *fp;
		fopen_s(&fp, filename, "rt");
		fscanf_s(fp, "%d", &key_nFrame);

		for (int i = 0; i < key_nFrame; i++)
		{
			fscanf_s(fp, "%d %d %d %d %d",
				&key[i].x, &key[i].y,
				&key[i].width, &key[i].height,
				&key[i].key_time);
		}

		fclose(fp);
		gPen_key = new Pen(Color(255, 0, 0));
	}

	void BoxCol()
	{
		//충돌박스 범위조정
		for (int i = 0; i < MAXKEY; i++)
		{
			rcCollision_key[i].X = x[i];
			rcCollision_key[i].Y = y[i];
			rcCollision_key[i].Width = key[key_index].width;
			rcCollision_key[i].Height = key[key_index].height;
		}


	}



	void DrawCollisionRect_key(Graphics *g)
	{
		for (int i = 0; i < MAXKEY; i++)
		{
			g->DrawRectangle(gPen_key, rcCollision_key[i]);

		}
	}





};




