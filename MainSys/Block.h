#pragma once

#include "../GameDev/GameDev.h"

#include <windows.h>
#include <fstream>
#include <sstream>
#include <math.h>

////////////////////////////////////////////////////////////////////////////
// 이미지를 읽어들이고 그리는 작업을 편리하도록 만든 클래스.

#ifndef D2R
#define D2R float(M_PI/180)
#endif // D2R

class Block : public Object
{
public :
	Block();
	virtual ~Block();

public :

	// 이미지 파일명으로 읽어들이고 그릴 때 지정된 부분만 그리도록 부분도 설정.
	BOOL Load(LPCTSTR szFileName, const Rect& rc);

	// 이미지 파일명만으로 이미지를 읽어들인다.
	// 그리는 부분은 이미지 전체.
	BOOL Load(LPCTSTR szFileName);

	// 그리기..
	void Draw(HDC);
	void Input(DWORD);
	void Update(DWORD);

	// 투명처리를 하기 위한 설정..
	void SetTransparent(COLORREF clr);

	// 알파블랜딩으로 그리기 위한 설정.
	void SetAlpha(BYTE alpha, bool _bSrcAlpha = false);

	// 그려질 부분의 크기를 반환.
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

	// 이미지 핸들 자체를 필요로 하는 경우에 호출됨.
	operator HBITMAP ();

	// getRect 를 편하게 쓰기 위해 선언...
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