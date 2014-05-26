#pragma once

#include "../GameDev/GameDev.h"
#include "Kirby.h"
#include <list>

enum Block1_id
{
	Block1_x_size = 255*3,
	Block1_y_size = 127*3,
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
	std::list<Image*> Block1;
	Rect ClientRect;
	Point MoveMap;
	Point MoveBlock;
	Point PlayerPos;

	bool Direction;
};