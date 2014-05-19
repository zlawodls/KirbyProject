#pragma once

#include "../GameDev/GameDev.h"
#include <list>

enum Block1_id
{
	Block1_x_size = 255*3,
	Block1_y_size = 127*3,
};

class Stage1_1
{
public :
	~Stage1_1();
	void Load(HWND);
	void Draw(HDC);
	void Update(DWORD);

private :
	Image BackGround_1;
	Image BackGround_2;
	std::list<Image*> Block1;
	Rect ClientRect;
};