#pragma once

#include "pch.h"

enum State_Rect
{
	Start,
	End,
	Block,
	Open,
	Close,
	Path
};

enum Draw_State
{
	DRAW,
	ERASE,
	EXCEPT,
};

enum DirectionPath
{
	N,
	NE,
	E,
	SE,
	S,
	SW,
	W,
	NW
};

const int RowDirection[DIRECTION] =
{
	1, 1, 0, -1, -1, -1, 0, 1
};
const int ColDirection[DIRECTION] =
{
	0, 1, 1, 1, 0, -1, -1, -1
};

inline vector<Vec2> pathListPaser(string& path, Vec2 target)
{
	vector<Vec2> pathlist;
	Vec2 temp = target;

	for (int i = 0; i < path.length(); i++)
	{
		switch (path.at(i))
		{
		case '0':
			temp.x += 1;
			temp.y += 0;
			pathlist.push_back(temp);
			break;
		case '1':
			temp.x += 1;
			temp.y += 1;
			pathlist.push_back(temp);
			break;
		case '2':
			temp.x -= 0;
			temp.y += 1;
			pathlist.push_back(temp);
			break;
		case '3':
			temp.x -= 1;
			temp.y += 1;
			pathlist.push_back(temp);
			break;
		case '4':
			temp.x -= 1;
			temp.y -= 0;
			pathlist.push_back(temp);
			break;
		case '5':
			temp.x -= 1;
			temp.y -= 1;
			pathlist.push_back(temp);
			break;
		case '6':
			temp.x += 0;
			temp.y -= 1;
			pathlist.push_back(temp);
			break;
		case '7':
			temp.x += 1;
			temp.y -= 1;
			pathlist.push_back(temp);
			break;
		}

	}

	return pathlist;
}

class Util
{
private:
protected:
public:
	//atic Vec2 convertRectToPoint(int row, int col)
	//
	//loat y = row * ROW_GAP + (ROW_GAP * 0.5);
	//loat x = col * COL_GAP + (COL_GAP * 0.5);
	//
	//eturn Vec2(x, y);
	//
	//
	//atic Matrix convertPointToRect(const Point& pt)	// 터치한 위치의 배열 좌표
	//
	//nt row = (int)(pt.y / ROW_GAP);
	//nt col = (int)(pt.x / COL_GAP);
	//
	//f (row < 0)
	//row = 0;
	//lse if (row>ROW_SIZE)
	//row = ROW_SIZE - 1;
	//
	//f (col < 0)
	//col = 0;
	//lse if (col >= COL_SIZE)
	//col = COL_SIZE - 1;
	//
	//
	//atic int convertRectIndex(int row, int col)	// 고유 index
	//
	//eturn COL_SIZE * row + col;
	//
};