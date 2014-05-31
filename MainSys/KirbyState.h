#pragma once

#include "../GameDev/GameDev.h"
#include "Kirby.h"
#include <stdlib.h>
#include <time.h>

enum Zone_id
{
	LeftZone = 0,
	RightZone,
	ZoneMax,
};
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

class KirbyIdle : public state
{
public :
	KirbyIdle();
	virtual ~KirbyIdle();

	virtual void Enter();
	virtual void Input(DWORD);
	virtual void Update(DWORD);
	virtual void Draw(HDC);
	virtual void Leave();
	void SetPosition(const Point& kpt, const Point& ppt, const bool& back);

private :
	DWORD update_dt;
	DWORD update_delay;

	Point KirbyPos;
	Point PlayerPos;
	Rect KirbyRect;

	LONG MoveAcc;
	Rect Zone[ZoneMax];
	bool BackPosition;
};
class KirbyMove : public state
{
public :
	KirbyMove();
	virtual ~KirbyMove();

	virtual void Enter();
	virtual void Input(DWORD);
	virtual void Update(DWORD);
	virtual void Draw(HDC);
	virtual void Leave();
	void SetPosition(const Point& kpt, const Point& ppt, const bool& back);

private :
	DWORD update_dt;
	DWORD update_delay;
	DWORD update_dt2;
	DWORD update_delay2;

	Point KirbyPos;
	Point PlayerPos;
	Rect KirbyRect;
	LONG MoveAcc;
	Rect Zone[ZoneMax];
	bool BackPosition;
	bool KeyPush;
	bool RightKey;
};
class KirbyRun : public state
{
public :
	KirbyRun();
	virtual ~KirbyRun();

	virtual void Enter();
	virtual void Input(DWORD);
	virtual void Update(DWORD);
	virtual void Draw(HDC);
	virtual void Leave();
	void SetPosition(const Point& kpt, const Point& ppt, const bool& back);

private :
	DWORD update_dt;
	DWORD update_delay;
	DWORD update_dt2;
	DWORD update_delay2;

	Point KirbyPos;
	Point PlayerPos;
	Rect KirbyRect;
	bool Break;
	LONG MoveAcc;
	Rect Zone[ZoneMax];
	bool BackPosition;
	bool KeyPush;
};