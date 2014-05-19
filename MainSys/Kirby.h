#pragma once

#include "../GameDev/GameDev.h"
#include <list>

class Kirby
{
public :
	Kirby();
	~Kirby();
	void Load();
	void Input();
	void Draw();
	void Update();

private :
	Point KirbyPos;
};