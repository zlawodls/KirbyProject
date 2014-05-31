#pragma once

#include "../GameDev/GameDev.h"

#include <windows.h>
#include <fstream>
#include <sstream>
#include <math.h>

////////////////////////////////////////////////////////////////////////////
// �̹����� �о���̰� �׸��� �۾��� ���ϵ��� ���� Ŭ����.

#ifndef D2R
#define D2R float(M_PI/180)
#endif // D2R

class Block : public Object
{
public :
	Block();
	virtual ~Block();

public :

	// �̹��� ���ϸ����� �о���̰� �׸� �� ������ �κи� �׸����� �κе� ����.
	BOOL Load(LPCTSTR szFileName, const Rect& rc);

	// �̹��� ���ϸ����� �̹����� �о���δ�.
	// �׸��� �κ��� �̹��� ��ü.
	BOOL Load(LPCTSTR szFileName);

	// �׸���..
	void Draw(HDC);
	void Input(DWORD);
	void Update(DWORD);

	// ����ó���� �ϱ� ���� ����..
	void SetTransparent(COLORREF clr);

	// ���ĺ������� �׸��� ���� ����.
	void SetAlpha(BYTE alpha, bool _bSrcAlpha = false);

	// �׷��� �κ��� ũ�⸦ ��ȯ.
	Rect getRect();

	COLORREF getTransparentColor() const;

	void SetRectSrc(const Rect& rc);

	void SetRect(const Rect& rc);
	void SetBBoxRect(const Rect& rc);

	void Move(const Rect& rc);

	void Move(const Point& pt);

	void Rotate(const float& angle);

	void SetBlockPoint(const LONG& x, const LONG& y);

	void SetDrawPoint(const LONG& x);

	void SetShowBox();

	void BoxGap(const LONG& lt);

	Rect GetDrawRect() const;

	Size GetBmSize() const;

	// �̹��� �ڵ� ��ü�� �ʿ�� �ϴ� ��쿡 ȣ���.
	operator HBITMAP ();

	// getRect �� ���ϰ� ���� ���� ����...
	operator Rect ();

private :
	void release();

	void _checkSupportedFile(LPCTSTR lpszFileName);

private :
	HBITMAP hBitmap;
	Rect rcDest;
	Rect rcSrc;

	LONG Gap;

	Size BitmapSize;

	COLORREF clrTransparent;
	bool bUseTransparent;

	bool ShowBox;

	BoundaryBox* BBox;

	///////////////////////////
	// alpha
	bool bUseAlpha;
	BYTE byteAlpha;
	bool bSrcAlpha;

	//////////////////////////
	// rotate
	bool bUseRotate;
	float fAngle;
};