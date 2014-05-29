#include "Animation.h"


Animation::Animation()
   : update_dt(0), delay(500)
   , bLoop(false), index(0)
   , bDoNotDraw(false), Reset_dt(0)
{
}
Animation::~Animation()
{
   for (depotiter it = shotlist.begin();
      it != shotlist.end();)
   {
      // 등록된 이미지를 메모리 해제한다.
      delete (*it);

      // 노드도 삭제한다.
      it = shotlist.erase(it);
   }
}

Animation* Animation::clone()
{
   Animation* pClone = new Animation(*this);
   return pClone;
}

void Animation::reset()
{
   bDoNotDraw = false;
   index = 0;
}
void Animation::SetImage(depot& imagelist)
{
   shotlist = imagelist;
}
void Animation::AddImage(Image* pImage)
{
   shotlist.push_back(pImage);

   rcImage = pImage->getRect();
}
void Animation::SetLoop(bool _loop/*=true*/)
{
   bLoop = _loop;
}
void Animation::SetDelay(DWORD _delay/*=500*/)
{
   delay = _delay;
}
void Animation::SetPosition(const Point& pt)
{
   ptDraw = pt;
}
void Animation::SetRect(const Rect& rc)
{
   rcDest = rc;
}
Rect Animation::getRect(void) const
{
   if (shotlist.empty()) return Rect();

   return (*shotlist.begin())->getRect();
}
void Animation::Update(DWORD tick)
{
   if (bDoNotDraw) return;

   if (update_dt >= delay)
   {
      index++;
      if (shotlist.size() == index)
      {
         if (bLoop)
         {
            index = 0;
         }
         else
         {
            bDoNotDraw = true;
         }
      }

      update_dt -= delay;
   }

   update_dt += tick;
}
void Animation::Draw(HDC hdc, const Rect& rc)
{
   if (bDoNotDraw) return;

   shotlist[index]->Move(rc);
   shotlist[index]->Draw(hdc);
}
void Animation::Draw(HDC hdc)
{
   if (bDoNotDraw) return;

   if (rcDest.width() == 0)
   {
      rcImage = Rect(ptDraw, Size(rcImage.width(), rcImage.height()));
   }
   else
   {
      rcImage = rcDest;
   }
   shotlist[index]->Move(rcImage);
   shotlist[index]->Draw(hdc);
}
void Animation::SetTransparent(COLORREF clr)
{
   for (depotiter it = shotlist.begin();
      it != shotlist.end();
      it++)
   {
      (*it)->SetTransparent(clr);
   }
}
bool Animation::IsPlaying() const
{
	return !bDoNotDraw;
}
// 알파블랜딩으로 그리기 위한 설정.
void Animation::SetAlpha(BYTE alpha)
{
   for (depotiter it = shotlist.begin();
      it != shotlist.end();
      it++)
   {
      (*it)->SetAlpha(alpha);
   }
}
// 개인적으로 만든 함수
void Animation::ResetAni(DWORD tick, DWORD delay)
{
	if(Reset_dt > delay)
	{
		int count = Reset_dt / delay;
		for(int i = 0; i < count; i++)
		{
			index = 0;
			bDoNotDraw = false;
		}
		Reset_dt = Reset_dt%delay;
	}
	Reset_dt += tick;
}