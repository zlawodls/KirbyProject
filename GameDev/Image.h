#pragma once

#include <windows.h>
#include <tchar.h>
#include "BaseType.h"

class Image
{
public :
	Image();
	virtual ~Image();

public :
	bool Load(LPCTSTR lpFileName);
	bool Load(LPCTSTR lpFileName, const Rect& rc);
	void Draw(HDC hdc);

	void SetDrawRect(const Rect& rc);
	void SetAlphaValue(const BYTE& _alpha);
	void SetTransparentColor(const COLORREF& clr);
	void SetDrawRectBitmap();
	void SetDrawPoint(const LONG& x, const LONG& y);
	void SetBlockPoint(const LONG& x, const LONG& y);

	Rect GetDrawRect() const;
	Rect GetSrcRect() const;
	Size GetBmSize() const;
	HBITMAP GetBitmap() const;

private :
	HBITMAP hBitmap;
	Rect rcDraw;
	Rect rcSrc;
	Size BitmapSize;

	BYTE alpha;
	bool bUseAlpha;

	COLORREF clrTransparent;
	bool bUseTransparent;
};