#include "Animation.h"

Animation::Animation()
: update_dt(0), update_delay(100)
, bDrawImage(true), Reset_dt(0)
, index(0)
{}
Animation::~Animation()
{
	std::list<Image*>::iterator it;
	for (it = imgList.begin(); it != imgList.end(); it++)
	{
		delete (*it);
	}
}

void Animation::AddImage(Image* pImage)
{
	imgList.push_back(pImage);
}

void Animation::Update(DWORD tick)
{
	if (!bDrawImage) return;

	if (update_dt > update_delay)
	{
		index++;
		if (index == imgList.size())
			bDrawImage = false;

		update_dt -= update_delay;
	}

	update_dt += tick;
}
void Animation::Draw(HDC hdc)
{
	if (!bDrawImage) return;

	std::list<Image*>::iterator it = imgList.begin();
	std::advance(it, index);

	(*it)->Draw(hdc);
}
void Animation::SetDrawRect(const Rect& rc)
{
	std::list<Image*>::iterator it;
	for (it = imgList.begin(); it != imgList.end(); it++)
	{
		(*it)->SetDrawRect(rc);
	}
}
void Animation::SetUpdateDelay(DWORD delay)
{
	update_delay = delay;
}
void Animation::SetPlaying(const bool& set)
{
	index = 0;
	bDrawImage = set;
}
void Animation::ResetAni(DWORD tick, DWORD delay)
{
	if(Reset_dt > delay)
	{
		int count = Reset_dt / delay;
		for(int i = 0; i < count; i++)
		{
			SetPlaying(true);
		}
		Reset_dt = Reset_dt%delay;
	}
	Reset_dt += tick;
}
void Animation::CheckIndex()
{
	if (index == imgList.size())
		SetPlaying(true);
}
bool Animation::IsPlaying() const
{
	return bDrawImage;
}