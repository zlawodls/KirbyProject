#pragma once

#include "../GameDev/GameDev.h"
#include <list>

class Stage1_1
{
public :
	void Load(HWND);
	void Draw(HDC);
	void Update(DWORD);

private :
	Image BackGround_1;
	Image BackGround_2;
	std::list<Image*> Block1;
	Rect ClientRect;
};