#include "Stage1-1.h"

Stage1_1::Stage1_1()
	:Direction(true), MoveBlock(0, 0)
{
	PlayerPos = KirbyBase.RetrunKirbyPos();
}
Stage1_1::~Stage1_1()
{
}
void Stage1_1::Draw(HDC hdc)
{

	BackGround[0].SetDrawPoint(MoveMap.x);
	BackGround[0].Draw(hdc);

	if(MoveMap.x < 0)
	{
		BackGround[1].SetDrawPoint((BackGround[1].GetBmSize().cx * 3) + MoveMap.x);
		BackGround[1].Draw(hdc);
	}
	else
	{
		BackGround[1].SetDrawPoint(-(BackGround[1].GetBmSize().cx * 3) + MoveMap.x);
		BackGround[1].Draw(hdc);
	}

	BlockDepot.Draw(hdc);
}
void Stage1_1::Update(DWORD tick)
{
	if(-MoveMap.x > (BackGround[1].GetBmSize().cx * 3) || MoveMap.x > (BackGround[1].GetBmSize().cx * 3))
		MoveMap.x = 0;

	if(PlayerPos.x < 0)
		MoveMap.x = 0;

	BlockDepot.Update(MoveBlock.x, MoveBlock.y);
}
void Stage1_1::Load(HWND hWnd)
{
	// 백그라운드 로드
	::GetClientRect(hWnd, &ClientRect);

	for(int i = 0; i < 2; i++)
	{
		BackGround[i].Load(_T("Resource//image//BackGrounds.bmp"));
		Rect tmpbm = ClientRect;
		Size tmpSize = BackGround[i].GetBmSize();
		tmpbm.right = tmpSize.cx*3;
		tmpbm.top = tmpbm.bottom - (tmpSize.cy*3);
		BackGround[i].SetRect(tmpbm);
	}

	// 블록1 로드

	Block* tmp = new Block;

	Rect tmpRc;

	tmpRc = ClientRect;
	tmpRc.top = tmpRc.height() - Block1_y_size + 100;
	tmpRc.bottom = tmpRc.top + Block1_y_size;
	tmpRc.right = Block1_x_size;

	tmp->Load(_T("Resource//image//Block_1.bmp"));
	tmp->SetTransparent(RGB(0,0,0));
	tmp->SetRect(tmpRc);
	tmp->SetBBoxRect(tmpRc);
	tmp->SetShowBox();
	tmp->BoxGap(30);

	BlockDepot.Push(tmp);


	tmp = new Block;

	tmpRc = ClientRect;
	tmpRc.top = tmpRc.height() - Block1_y_size -200;
	tmpRc.bottom = tmpRc.top + Block1_y_size;
	tmpRc.left = tmpRc.right +200;
	tmpRc.right = tmpRc.left + Block1_x_size;

	tmp->Load(_T("Resource//image//Block_1.bmp"));
	tmp->SetTransparent(RGB(0,0,0));
	tmp->SetRect(tmpRc);
	tmp->SetBBoxRect(tmpRc);
	tmp->SetShowBox();
	tmp->BoxGap(30);
	BlockDepot.Push(tmp);

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

	if(PlayerPos.x < pt.x)
	{
		MoveMap.x -= 1;
	}
	else if(PlayerPos.x > pt.x)
	{
		MoveMap.x += 1;
	}

	PlayerPos = pt;
}