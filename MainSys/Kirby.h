#pragma once

#include "../GameDev/GameDev.h"
#include "KirbyState.h"
#include "Effect.h"
#include <list>

enum Zone_id
{
	LeftZone = 0,
	RightZone,
	ZoneMax,
};

class Kirby : public singleton<Kirby>
{
	friend class singleton<Kirby>;

private :
	Kirby();
	~Kirby();
public :
	void Load(const Rect& rc);
	void Input(DWORD);
	void Draw(HDC hdc);
	void Update(DWORD);

	Point RetrunKirbyPos() const;

private :
	Point KirbyPos;
	Point PlayerPos;
	DWORD update_dt;

	KirbyStateCon KirbyState;

	Rect KirbyRect;

	LONG MoveAcc;
	int MoveAccCount;

	bool BackPosition;

	Rect ClientRect;
	Rect Zone[ZoneMax];

	BYTE state;
};

#define KirbyBase Kirby::getReference()