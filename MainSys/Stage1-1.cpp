#include "Stage1-1.h"

Stage1_1::~Stage1_1()
{
	std::list<Image*>::iterator it;
	for(it = Block1.begin(); it != Block1.end();)
	{
		delete (*it);
		it = Block1.erase(it);
	}
}

void Stage1_1::Load(HWND hWnd)
{
	::GetClientRect(hWnd, &ClientRect);

	BackGround_1.Load(_T("BackGrounds.bmp"));
	BackGround_1.SetDrawRect(ClientRect);

	BackGround_2.Load(_T("BackGrounds.bmp"));
	BackGround_1.SetDrawRect(ClientRect);

	Image* tmp = new Image;

	Rect tmpRc;

	tmpRc = ClientRect;
	tmpRc.top = tmpRc.height() - Block1_y_size + 100;
	tmpRc.bottom = tmpRc.top + Block1_y_size;
	tmpRc.right = Block1_x_size;

	tmp->Load(_T("Block_1.bmp"));
	tmp->SetTransparentColor(RGB(0,0,0));
	tmp->SetDrawRect(tmpRc);
	Block1.push_back(tmp);
}
void Stage1_1::Draw(HDC hdc)
{
	BackGround_1.Draw(hdc);
	BackGround_2.Draw(hdc);
	
	std::list<Image*>::iterator it;
	for(it = Block1.begin(); it != Block1.end(); it++)
	{
		Rect tmp;
		if (::IntersectRect(&tmp, &ClientRect, &((*it)->GetDrawRect())))
		{
			(*it)->Draw(hdc);
		}
	}
}
void Stage1_1::Update(DWORD)
{
	
}