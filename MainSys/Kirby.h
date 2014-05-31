#pragma once

#include "../GameDev/GameDev.h"
#include "KirbyState.h"
#include "Effect.h"
#include "BlockManager.h"
#include <list>

class Kirby : public singleton<Kirby>, public Object, public statemachine
{
	friend class singleton<Kirby>;
	friend class Object;
	friend class statemachine;

private :
	Kirby();
	~Kirby();
public :
	void Load(const Rect& rc);
	void Input(DWORD);
	void Draw(HDC hdc);
	void Update(DWORD);
	void Setposition(const Point& ppt, const Point& kpt, const bool& back);

	Point RetrunKirbyPos() const;

private :
	void init();

private :
	Point KirbyPos;
	Point PlayerPos;
	DWORD update_dt;
	Rect KirbyRect;

	LONG MoveAcc;
	int MoveAccCount;

	bool BackPosition;

	Rect ClientRect;

	BYTE state;

	BoundaryBox* BBox;
};

#define KirbyBase Kirby::getReference()