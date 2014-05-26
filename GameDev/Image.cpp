#include "Image.h"

Image::Image()
: hBitmap(NULL)
, alpha(255)
, clrTransparent(RGB(0,0,0))
, bUseAlpha(false)
, bUseTransparent(false)
, BitmapSize(0, 0)
{
}
Image::~Image()
{
	if (hBitmap)
		::DeleteObject(hBitmap);
}

bool Image::Load(LPCTSTR lpFileName)
{
	hBitmap = (HBITMAP)::LoadImage(NULL, 
								lpFileName, 
								IMAGE_BITMAP,
								0, 0, 
								LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);
	if (hBitmap == NULL)
		return false;

	BITMAP bm;
	::GetObject(hBitmap, sizeof(bm), &bm);

	Size tmp(bm.bmWidth, bm.bmHeight);

	BitmapSize = tmp;

	rcSrc = Rect(Point(0,0), Point(bm.bmWidth, bm.bmHeight));

	return true;
}
bool Image::Load(LPCTSTR lpFileName, const Rect& rc)
{
	hBitmap = (HBITMAP)::LoadImage(NULL, lpFileName, IMAGE_BITMAP,
		0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);
	if (hBitmap == NULL)
		return false;

	rcSrc = rc;

	BITMAP bm;
	::GetObject(hBitmap, sizeof(bm), &bm);

	Size tmp(bm.bmWidth, bm.bmHeight);

	BitmapSize = tmp;

	return true;
}
void Image::Draw(HDC hdc)
{
	if (!hBitmap) return;

	HDC hBitmapDC = ::CreateCompatibleDC(hdc);
	HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hBitmapDC, hBitmap);

	if (bUseAlpha)
	{
		BLENDFUNCTION bf;
		bf.AlphaFormat = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.BlendOp = 0;
		bf.SourceConstantAlpha = alpha;

		::GdiAlphaBlend(hdc,
			rcDraw.left, rcDraw.top,
			rcDraw.width(), rcDraw.height(),
			hBitmapDC,
			rcSrc.left, rcSrc.top,
			rcSrc.width(), rcSrc.height(),
			bf);
	}
	else if (bUseTransparent)
	{
		::GdiTransparentBlt(hdc,
			rcDraw.left, rcDraw.top,
			rcDraw.width(), rcDraw.height(),
			hBitmapDC,
			rcSrc.left, rcSrc.top,
			rcSrc.width(), rcSrc.height(),
			clrTransparent);
	}
	else
	{
		::StretchBlt(hdc,
			rcDraw.left, rcDraw.top,
			rcDraw.width(), rcDraw.height(),
			hBitmapDC,
			rcSrc.left, rcSrc.top,
			rcSrc.width(), rcSrc.height(),
			SRCCOPY);
	}

	::SelectObject(hBitmapDC, hOldBitmap);
	::DeleteDC(hBitmapDC);
}
void Image::SetDrawRect(const Rect& rc)
{
	rcDraw = rc;
}
void Image::SetAlphaValue(const BYTE& _alpha)
{
	bUseAlpha = true;
	bUseTransparent = false;
	alpha = _alpha;
}
void Image::SetTransparentColor(const COLORREF& clr)
{
	bUseTransparent = true;
	bUseAlpha = false;
	clrTransparent = clr;
}
void Image::SetDrawRectBitmap()
{
	rcDraw.right = BitmapSize.cx;
	rcDraw.bottom = BitmapSize.cy;
}
void Image::SetDrawPoint(const LONG& x, const LONG& y)
{
	Rect tmp = rcDraw;

	rcDraw.left = x;
	rcDraw.right = rcDraw.left + tmp.width();
	rcDraw.top = y;
	rcDraw.bottom = rcDraw.top + tmp.height();
}
void Image::SetBlockPoint(const LONG& x, const LONG& y)
{
	rcDraw.left -= x;
	rcDraw.right -= x;
	rcDraw.top -= y;
	rcDraw.bottom -= y;
}
Rect Image::GetDrawRect() const
{
	return rcDraw;
}
Rect Image::GetSrcRect() const
{
	return rcSrc;
}
Size Image::GetBmSize() const
{
	return BitmapSize;
}
HBITMAP Image::GetBitmap() const
{
	return hBitmap;
}