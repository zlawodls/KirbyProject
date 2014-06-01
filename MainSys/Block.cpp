#include "Block.h"

Block::Block()
   : hBitmap(NULL)
   , clrTransparent(RGB(0,0,0))
   , bUseTransparent(false)
   , bUseAlpha(false), byteAlpha(127), bSrcAlpha(false)
   , bUseRotate(false), fAngle(0.f), ShowBox(false), Gap(0)
{
	BBox = new SquareBox();
}
Block::~Block()
{
	if (BBox)
		delete BBox;

   release();
}

// �̹��� ���ϸ����� �о���̰� �׸� �� ������ �κи� �׸����� �κе� ����.
BOOL Block::Load(LPCTSTR szFileName, const Rect& rc)
{
   // �̹� �о���� �̹����� �ִٸ�...
   if (hBitmap)
      // ������ �ϰ�..
      release();

   // ������ �̹����� �о���δ�.
   hBitmap = (HBITMAP)::LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_LOADTRANSPARENT | LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);
   if (hBitmap == NULL)
   {
      _checkSupportedFile(szFileName);
      DWORD dwError = ::GetLastError();
      ::ErrorMessageBox(dwError);

      return FALSE;
   }

   BITMAP bm;
   ::GetObject(hBitmap, sizeof(BITMAP), &bm);
   rcSrc = Rect(Point(0,0), Point(bm.bmWidth, bm.bmHeight));

   // �׷��� �κе� �����Ѵ�.
   rcSrc = rc;

   // rcDest �� �⺻������ rcSrc �� ������ ���� ������ �������� 0,0 ���� �ʱ�ȭ.
   rcDest = rcSrc << Size(rcSrc.left, rc.top);

   Size tmp(bm.bmWidth, bm.bmHeight);
   BitmapSize = tmp;

   return TRUE;
}
// �̹��� ���ϸ����� �̹����� �о���δ�.
// �׸��� �κ��� �̹��� ��ü.
BOOL Block::Load(LPCTSTR szFileName)
{
   // �̹� �о���� �̹����� �ִٸ�...
   if (hBitmap)
      // ������ �ϰ�...
      release();

   // ������ �̹����� �о���̰�...
   hBitmap = (HBITMAP)::LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);
   if (hBitmap == NULL)
      return FALSE;

   // ���� ������ �׸� �κ��� ������ �̹��� ��ü�� ����.
   BITMAP bm;
   ::GetObject(hBitmap, sizeof(BITMAP), &bm);
   rcSrc = Rect(Point(0,0), Point(bm.bmWidth, bm.bmHeight));

   // rcDest �� �⺻������ rcSrc �� ������ ���� �������� �ʱ�ȭ.
   rcDest = rcSrc;

   Size tmp(bm.bmWidth, bm.bmHeight);
   BitmapSize = tmp;

   return TRUE;
}
void Block::release()
{
   // �̹��� �ڵ��� ��ȿ�ϴٸ�...
   if (hBitmap)
   {
      // �����Ѵ�..
      ::DeleteObject(hBitmap);
      hBitmap = NULL;
   }
}
// �׸���..
void Block::Draw(HDC hdc)
{
   HBITMAP hUseBitmap = NULL;
   HBITMAP hRotateBitmap = NULL;

   ///////////////////////////////////////
   // ȸ����Ű�� �̹����� �����ϴ� ������ ũ�Ⱑ �޶�����.
   // �� �޶��� ������ ó���ϱ� ���� �簢��ũ�⸦ ����.
   Rect rcSrcTemp = rcSrc;
   Rect rcDestTemp = rcDest;
   if (bUseRotate)
   {
      // ȸ�� ���
      Matrix<float,2,2> rotMat;
      rotMat.m[0][0] = cos(fAngle*D2R);
      rotMat.m[0][1] = sin(fAngle*D2R);
      rotMat.m[1][0] = -sin(fAngle*D2R);
      rotMat.m[1][1] = cos(fAngle*D2R);

      // �̹����� �߽��� ��ǥ
      Matrix<float,2,1> _c;
      _c.m[0][0] = float(rcSrcTemp.width()/2);
      _c.m[1][0] = float(rcSrcTemp.height()/2);

      // ȸ�� �� ����� ��ǥ
      Matrix<float,2,1> _rc = rotMat*_c;

      // ȸ���� �̹����� �׷��� ������ ũ��.
      int cx = int(sqrt(float(rcSrc.width()*rcSrc.width() + rcSrc.height()*rcSrc.height())));
      int cy = cx;
      Size center(cx/2, cy/2);
      rcDestTemp = Rect(rcDestTemp.lefttop(), cx, cy);
      rcSrcTemp = Rect(Point(0,0), cx, cy);

      // ȸ���� �̹����� �׸� �� �ʿ��� HDC, HBITMAP ����.
      HDC hRotateDC = ::CreateCompatibleDC(hdc);
      hRotateBitmap = ::CreateCompatibleBitmap(hdc, cx,cy);
      HBITMAP hOldRotateBitmap = ::Select(hRotateDC, hRotateBitmap);

      // �⺻������ CreateCompatibleBitmap���� ������ ��Ʈ���� �������̶�
      // ����ó���� ������ ĥ�Ѵ�.
      HBRUSH bgBrush = ::CreateSolidBrush(clrTransparent);
      HBRUSH bgOldBrush = ::Select(hRotateDC, bgBrush);

      ::FillRect(hRotateDC, &rcSrcTemp, bgBrush);

      ::Select(hRotateDC, bgOldBrush);
      ::DeleteObject(bgBrush);

      // ȸ�� ��ȯ ��ų XFORM ����.
      XFORM xf;
      xf.eM11 = cos(fAngle*D2R);
      xf.eM12 = -sin(fAngle*D2R);
      xf.eM21 = sin(fAngle*D2R);
      xf.eM22 = cos(fAngle*D2R);
      xf.eDx = FLOAT(center.cx -_rc.m[0][0]);
      xf.eDy = FLOAT(center.cy -_rc.m[1][0]);

      // ��ȯ�� �ϰ�..
      ::SetGraphicsMode(hRotateDC, GM_ADVANCED);
      ::SetWorldTransform(hRotateDC, &xf);

      HDC hBitmapDC = ::CreateCompatibleDC(hdc);
      HBITMAP hOldBitmap = ::Select(hBitmapDC, hBitmap);

      // ���� �̹����� ��ȯ�� �� �������� ����.
      ::BitBlt(hRotateDC, 
         0,0,
         cx, cy,
         hBitmapDC, 
         rcSrc.left, rcSrc.top,
         SRCCOPY);

      // ��ȯ�� �̹����� ���� �ٸ� ����ߴ� �ڵ���� ���� �� ����.
      ::Select(hBitmapDC, hOldBitmap);
      ::DeleteDC(hBitmapDC);
      ::Select(hRotateDC, hOldRotateBitmap);
      ::DeleteDC(hRotateDC);

      hUseBitmap = hRotateBitmap;
   }
   else
   {
      hUseBitmap = hBitmap;
   }

   HDC hBitmapDC = ::CreateCompatibleDC(hdc);
   HBITMAP hOldBitmap = Select(hBitmapDC, hUseBitmap);

   // ȸ�� �Ҷ��� �⺻������ GdiTransparentBlt�� �̿�����.
   if (bUseRotate)
   {
      ::GdiTransparentBlt(hdc,							// �׷��� DC
         rcDestTemp.left, rcDestTemp.top,			// �׷��� ��ġ
         rcDestTemp.width(), rcDestTemp.height(),	// �׷��� ũ��
         hBitmapDC,									// �̹��� DC
         rcSrcTemp.left, rcSrcTemp.top,				// �̹��� �κ� ��ġ
         rcSrcTemp.width(), rcSrcTemp.height(),
         clrTransparent);							// �̹����� �׸��� ���.
   }
   // ����ó���� ����Ѵٸ�...
   else if (bUseAlpha)
   {
      BLENDFUNCTION bf;
      bf.BlendOp =  AC_SRC_OVER;
      bf.BlendFlags = 0;
      if (bSrcAlpha)
      {
         bf.AlphaFormat = AC_SRC_ALPHA;
         bf.SourceConstantAlpha = 0xFF;	// 0(��������:transparent) ~ 255(����������:opaque)
      }
      else
      {
         bf.AlphaFormat = 0;
         bf.SourceConstantAlpha = byteAlpha;	// 0(��������:transparent) ~ 255(����������:opaque)
      }

      ::GdiAlphaBlend(hdc, rcDest.left, rcDest.top,
         rcDest.width(), rcDest.height(),
         hBitmapDC,
         rcSrc.left, rcSrc.top,
         rcSrc.width(), rcSrc.height(),
         bf);
   }
   else if (bUseTransparent)
   {
      ::GdiTransparentBlt(hdc,									// �׷��� DC
         rcDest.left, rcDest.top,				// �׷��� ��ġ
         rcDest.width(), rcDest.height(),		// �׷��� ũ��
         hBitmapDC,								// �̹��� DC
         rcSrc.left, rcSrc.top,			// �̹��� �κ� ��ġ
         rcSrc.width(), rcSrc.height(),	// �̹��� �κ� ũ��
         clrTransparent);						// ����ó���� ��
   }
   else
   {
	   ::StretchBlt(hdc,								// �׷��� DC
         rcDest.left, rcDest.top,			// �׷��� ��ġ
         rcDest.width(), rcDest.height(),	// �׷��� ũ��
         hBitmapDC,							// �̹��� DC
         rcSrc.left, rcSrc.top,	
		 rcSrc.width(), rcSrc.height(),		// �̹��� �κ� ��ġ
         SRCCOPY);						// �̹����� �׸��� ���.

   }
   if(ShowBox)
   {
	   BBox->Draw(hdc);
   }

   Select(hBitmapDC, hOldBitmap);
   ::DeleteDC(hBitmapDC);

   // ȸ�����ÿ��� ȸ���� �̹����� �����Ѵ�.
   if (bUseRotate)
   {
      ::DeleteObject(hRotateBitmap);
   }

}
// ����ó���� �ϱ� ���� ����..
void Block::SetTransparent(COLORREF clr)
{
   clrTransparent = clr;
   bUseTransparent = true;
}
// ���ĺ������� �׸��� ���� ����.
void Block::SetAlpha(BYTE alpha, bool _bSrcAlpha/*=false*/)
{
   bUseAlpha = true;
   byteAlpha = alpha;
   bSrcAlpha = _bSrcAlpha;
}
// �׷��� �κ��� ũ�⸦ ��ȯ.
Rect Block::getRect()
{
   return rcSrc;
}
COLORREF Block::getTransparentColor() const
{
   return clrTransparent;
}
void Block::SetRect(const Rect& rc)
{
	rcDest = rc;
}
void Block::SetBBoxRect(const Rect& rc)
{
	Rect tmp;
	tmp = rcDest;
	tmp.top = tmp.top + Gap;

	dynamic_cast<SquareBox*>(BBox)->SetRect(tmp);
}
void Block::Move(const Rect& rc)
{
   rcDest = rc;
}
void Block::Move(const Point& pt)
{
   rcDest = Rect(pt, Size(rcDest.width(), rcDest.height()));
}
void Block::Rotate(const float& angle)
{
   bUseRotate = true;
   fAngle = angle;
}
// �̹��� �ڵ� ��ü�� �ʿ�� �ϴ� ��쿡 ȣ���.
Block::operator HBITMAP ()
{
   return hBitmap;
}
// getRect �� ���ϰ� ���� ���� ����...
Block::operator Rect ()
{
   return rcSrc;
}
void Block::_checkSupportedFile(LPCTSTR lpszFileName)
{
   BITMAPFILEHEADER bfh;

   std::fstream bmp(lpszFileName, std::ios_base::in | std::ios_base::binary);
   if (!bmp.good())
   {
      ::MessageBox(NULL, _T("File Open Failed."), _T("Error"), MB_OK);
      bmp.close();
      return ;
   }

   bmp.read((char*)&bfh, sizeof(bfh));
   if (::memcmp((void*)&bfh.bfType, "BM", 2) != 0)
   {
      ::MessageBox(NULL, _T("Not Supported File."), _T("Error"), MB_OK);
      bmp.close();
      return ;
   }

   DWORD dwSize;

   bmp.read((char*)&dwSize, sizeof(DWORD));
   /*
   12 : BITMAPCOREHEADER or OS21XBITMAPHEADER 
   64 : OS22XBITMAPHEADER (x)
   40 : BITMAPINFOHEADER
   52 : BITMAPV2INFOHEADER (x)
   56 : BITMAPV3INFOHEADER (x)
   108 : BITMAPV4HEADER
   124 : BITMAPV5HEADER
   */
   if (dwSize == 64 || dwSize == 52 || dwSize == 56)
   {
      ::MessageBox(NULL, _T("DIB Header is not Supported."), _T("Error"), MB_OK);
      bmp.close();
      return ;
   }

   bmp.close();
}
void Block::SetRectSrc(const Rect& rc)
{
	rcSrc = rc;
}
void Block::SetBlockPoint(const LONG& x, const LONG& y)
{
	rcDest.left -= x;
	rcDest.right -= x;
	rcDest.top -= y;
	rcDest.bottom -= y;

	Rect tmp;
	tmp = rcDest;
	tmp.top = tmp.top + Gap;
	dynamic_cast<SquareBox*>(BBox)->SetRect(tmp);
}
Rect Block::GetDrawRect() const
{
	return rcDest;
}
Size Block::GetBmSize() const
{
	return BitmapSize;
}
void Block::SetDrawPoint(const LONG& x)
{
	Rect tmp = rcDest;

	rcDest.left = x;
	rcDest.right = rcDest.left + tmp.width();
}
void Block::Input(DWORD)
{
}
void Block::Update(DWORD)
{
}
void Block::SetShowBox()
{
	ShowBox = true;
}
void Block::BoxGap(const LONG& lt)
{
	Gap = lt;
}
BoundaryBox* Block::getBBox()
{
	return BBox;
}
Rect Block::GetBBoxRect() const
{
	return dynamic_cast<SquareBox*>(BBox)->getBox();
}