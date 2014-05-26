#pragma once

#include "Image.h"
#include "BaseType.h"
#include <list>

class Animation
{
public :
	Animation();
	~Animation();

	void AddImage(Image* pImage);
	void Update(DWORD tick);
	void Draw(HDC hdc);
	void SetDrawRect(const Rect& rc);
	void SetUpdateDelay(DWORD);
	void SetPlaying(const bool& set);
	void CheckIndex();
	void ResetAni(DWORD tick, DWORD delay);
	bool IsPlaying() const;

private :
	std::list<Image*> imgList;

	DWORD update_dt;
	DWORD update_delay;
	DWORD Reset_dt;

	int index;

	bool bDrawImage;
};