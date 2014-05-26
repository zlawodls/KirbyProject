#pragma once

#include "../GameDev/GameDev.h"
#include <list>

enum Zone_id
{
	LeftZone = 0,
	RightZone,
	UpZone,
	DownZone,
	ZoneMax,
};
enum Kirby_Size
{
	KirbyWidth = 60,
	KitbyHeight = 58,

};
enum Kirby_state
{
	StdState = 0,
	MoveState,

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

	Image KirbyStd;
	Image KirbyStdBack;
	Animation KirbyStdEye;
	Animation KirbyStdEyeBack;
	Animation KirbyMove;
	Animation KirbyMoveBack;

	bool BackPosition;

	Rect ClientRect;
	Rect Zone[ZoneMax];

	BYTE state;
};

#define KirbyBase Kirby::getReference()