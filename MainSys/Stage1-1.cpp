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
	ImgDepot["BackGround"]->SetDrawPoint(MoveMap.x, MoveMap.y);
	ImgDepot["BackGround"]->Draw(hdc);

	Rect tmp = ImgDepot["BackGround"]->GetDrawRect();

	Point tmpPt = MoveMap;
	tmpPt.x = (ImgDepot["BackGround2"]->GetBmSize().cx * 3) + MoveMap.x;
	if(MoveMap.x < 0)
	{
		ImgDepot["BackGround2"]->SetDrawPoint(tmpPt.x, MoveMap.y);
		ImgDepot["BackGround2"]->Draw(hdc);
	}
	else
	{
		tmpPt.x = -(ImgDepot["BackGround2"]->GetBmSize().cx * 3) + MoveMap.x;
		ImgDepot["BackGround2"]->SetDrawPoint(tmpPt.x, MoveMap.y);
		ImgDepot["BackGround2"]->Draw(hdc);
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
	if(-MoveMap.x > (ImgDepot["BackGround2"]->GetBmSize().cx * 3) || MoveMap.x > (ImgDepot["BackGround2"]->GetBmSize().cx * 3))
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
	ImgLoader(_T("Resource//Stage1-1.image"));

	// 백그라운드 로드
	::GetClientRect(hWnd, &ClientRect);

	Rect tmpbm = ClientRect;
	Size tmpSize = ImgDepot["BackGround"]->GetBmSize();
	tmpbm.right = tmpSize.cx*3;
	tmpbm.top = tmpbm.bottom - (tmpSize.cy*3);

	ImgDepot["BackGround"]->SetRect(tmpbm);
	ImgDepot["BackGround2"]->SetRect(tmpbm);

	Rect tmpRc;

	tmpRc = ClientRect;
	tmpRc.top = tmpRc.height() - Block1_y_size + 100;
	tmpRc.bottom = tmpRc.top + Block1_y_size;
	tmpRc.right = Block1_x_size;

	ImgDepot["Block1"]->SetRect(tmpRc);
	Block1.push_back(ImgDepot["Block1"]);

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

	if(PlayerPos.x < pt.x && PlayerPos.x%5 == 1)
	{
		MoveMap.x -= 1;
	}
	else if(PlayerPos.x > pt.x && PlayerPos.x%5 == 1)
	{
		MoveMap.x += 1;
	}

	PlayerPos = pt;
}