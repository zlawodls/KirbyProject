#pragma once

#include "../GameDev/GameDev.h"

class Effect
{
public :
	void Load();
	void Update(DWORD, const Point& pt);
	void Draw(HDC);

private :
	Animation Dusty;
};