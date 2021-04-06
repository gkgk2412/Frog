#pragma once

#include "MyImage.h"
#include "myCharacter.h"


#define MAXLEAF		19


struct LEAFINFO
{
	int x;
	int y;
	int width;
	int height;
	DWORD leaf_time;
};

struct  LEAF
{
	int x[MAXLEAF];
	int y[MAXLEAF];
	
	Rect rcCollision_leaf[MAXLEAF];


	Pen *gPen_leaf;

	bool isHit[MAXLEAF];
	bool check[MAXLEAF];

	~LEAF()
	{
		delete gPen_leaf;
	}

	LEAFINFO leaf[100];
	int leaf_index;
	int leaf_nFrame;

	CMyImage leaf_image[MAXLEAF];

	void LoadImage_leaf(char *filename, int k)
	{
		leaf_image[k].Load(filename);
	}

	void LoadAniInfo_leaf(char *filename)
	{
		FILE *fp;
		fopen_s(&fp, filename, "rt");
		fscanf_s(fp, "%d", &leaf_nFrame);

		for (int i = 0; i < leaf_nFrame; i++)
		{
			fscanf_s(fp, "%d %d %d %d %d",
				&leaf[i].x, &leaf[i].y,
				&leaf[i].width, &leaf[i].height,
				&leaf[i].leaf_time);
		}

		fclose(fp);
		gPen_leaf = new Pen(Color(255, 0, 0));
	}

	void BoxCol()
	{
		//충돌박스 범위조정
		for (int i = 0; i < MAXLEAF; i++)
		{
		//rcCollision_leaf[i].X = x[i] + 45;
		//rcCollision_leaf[i].Y = y[i] + 80;
		//rcCollision_leaf[i].Width = leaf[leaf_index].width-40;
		//rcCollision_leaf[i].Height = leaf[leaf_index].height - 70;

			rcCollision_leaf[i].X = x[i] +10;
			rcCollision_leaf[i].Y = y[i] +40;
			rcCollision_leaf[i].Width = leaf[leaf_index].width;
			rcCollision_leaf[i].Height = leaf[leaf_index].height-50;
		}


	}
	


	void DrawCollisionRect_leaf(Graphics *g)
	{
		for (int i = 0; i < MAXLEAF; i++)
		{
			g->DrawRectangle(gPen_leaf, rcCollision_leaf[i]);

		}
	}




	
};




