#pragma once

#include <Windows.h>
#include "BaseType.h"
#include "Utility.hpp"

struct GameVariable : public singleton<GameVariable>
{
	friend class singleton<GameVariable>;

	HINSTANCE hInstance;
	HWND hMainWnd;
	Rect rcClient;

private :
	GameVariable();
	~GameVariable();
};

#define GameGlobal GameVariable::getReference()