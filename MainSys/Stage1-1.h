#pragma once

#include "../GameDev/GameDev.h"
#include "Kirby.h"
#include "BlockManager.h"
#include <list>
#include <sstream>

enum Block1_id
{
	Block1_x_size = 255*3,
	Block1_y_size = 127*3,

	Block2_x_size = 142*3,
	Block2_y_size = 180*3,

	Block3_x_size = 203*3,
	Block3_y_size = 176*3,

	Block4_x_size = 325*3,
	Block4_y_size = 176*3,

	Block6_x_size = 428*3,
	Block6_y_size = 135*3,

	Block7_x_size = 450*3,
	Block7_y_size = 220*3,

	Block7_1_x_size = 256*3,
	Block7_1_y_size = 220*3,

	Block7_2_x_size = 889*3,
	Block7_2_y_size = 220*3,

	Block8_x_size = 98*3,
	Block8_y_size = 73*3,

	Block8_1_x_size = 131*3,
	Block8_1_y_size = 157*3,

	Block9_x_size = 67*3,
	Block9_y_size = 90*3,

};

class Stage1_1
{
public :
	Stage1_1();
	~Stage1_1();
	void Load(HWND);
	void Draw(HDC);
	void Update(DWORD);

	void SetPlayerPos(const Point& pt);

private :
	Image BackGround[2];
	Rect ClientRect;
	Point MoveMap;
	Point MoveBlock;
	Point PlayerPos;

	bool Direction;
};