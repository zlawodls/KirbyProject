#pragma once

#include "../GameDev/GameDev.h"
#include <stdlib.h>
#include <time.h>

enum Kirby_Size
{
	KirbyWidth = 60,
	KirbyHeight = 58,

};
enum Kirby_state
{
	StdState = 0,
	MoveState,
	RunState,
	BreakState,
};

class KirbyStateCon
{
public :
	void Load(const Rect& rc, const Point& KirbyPos);
	void Input(DWORD);
	void Draw(HDC hdc, const BYTE& state, const bool& BackPosition);
	void Update(DWORD tick, const Rect& tmp);
	void StateReset();

private :

	Rect ClientRect;
};