#pragma once

#pragma comment(lib, "../GameDev/GameDev.lib")

#include "../GameDev/GameDev.h"
#include "Stage1-1.h"

class Mainsys : public MainWindow<Mainsys>
{
public :
	Mainsys();
	void Input(DWORD);
	void Update(DWORD tick);
	void Draw(DWORD);

protected :
	void Enter();

private :
	Point KirbyPos;
	Stage1_1 Stage1_1;
};