#include "Stage1-1.h"

Stage1_1::Stage1_1()
	:Direction(true), MoveBlock(0, 0)
{
	PlayerPos = KirbyBase.RetrunKirbyPos();
}
Stage1_1::~Stage1_1()
{
	std::list<Image*>::iterator it;
	for(it = Block1.begin(); it != Block1.end();)
	{
		delete (*it);
		it = Block1.erase(it);
	}
}
void Stage1_1::Draw(HDC hdc)
{
	BackGround[0].SetDrawPoint(MoveMap.x, MoveMap.y);
	BackGround[0].Draw(hdc);

	if(MoveMap.x < 0)
	{
		BackGround[1].SetDrawPoint((BackGround[1].GetBmSize().cx * 3) + MoveMap.x, MoveMap.y);
		BackGround[1].Draw(hdc);
	}
	else
	{
		BackGround[1].SetDrawPoint(-(BackGround[1].GetBmSize().cx * 3) + MoveMap.x, MoveMap.y);
		BackGround[1].Draw(hdc);
	}

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
void Stage1_1::Update(DWORD tick)
{
	if(-MoveMap.x > (BackGround[1].GetBmSize().cx * 3) || MoveMap.x > (BackGround[1].GetBmSize().cx * 3))
		MoveMap.x = 0;

	if(PlayerPos.x < 0)
		MoveMap.x = 0;

	std::list<Image*>::iterator it;
	for(it = Block1.begin(); it != Block1.end(); it++)
	{
			(*it)->SetBlockPoint(MoveBlock.x, MoveBlock.y);
	}

}
void Stage1_1::Load(HWND hWnd)
{
	// 백그라운드 로드
	::GetClientRect(hWnd, &ClientRect);

	for(int i = 0; i < 2; i++)
	{
		BackGround[i].Load(_T("BackGrounds.bmp"));
		Rect tmpbm = ClientRect;
		Size tmpSize = BackGround[i].GetBmSize();
		tmpbm.right = tmpSize.cx*3;
		tmpbm.top = tmpbm.bottom - (tmpSize.cy*3);
		BackGround[i].SetDrawRect(tmpbm);
	}

	// 블록1 로드
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
void Stage1_1::SetPlayerPos(const Point& pt)
{
	if(PlayerPos.x < pt.x)
	{
		MoveBlock.x = pt.x - PlayerPos.x;
	}
	else if(PlayerPos.x > pt.x)
	{
		MoveBlock.x = -(PlayerPos.x - pt.x);
	}
	else
		MoveBlock.x = 0;

	if(PlayerPos.x < pt.x && PlayerPos.x%4 == 1)
	{
		MoveMap.x -= 1;
	}
	else if(PlayerPos.x > pt.x && PlayerPos.x%4 == 1)
	{
		MoveMap.x += 1;
	}

	PlayerPos = pt;
}