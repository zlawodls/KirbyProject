#include "Kirby.h"

Kirby::Kirby()
:KirbyPos(200, 0), PlayerPos(375, 0), update_dt(0),
BackPosition(false), MoveAcc(0), MoveAccCount(0)
, KirbyState(4), Wallcount(0)
{
	SquareBox* pBox = new SquareBox;

	KirbyRect.left = KirbyPos.x- KirbyWidth/2;
	KirbyRect.top = KirbyPos.y - KirbyHeight/2;
	KirbyRect.right = KirbyRect.left + KirbyWidth;
	KirbyRect.bottom = KirbyRect.top + KirbyHeight;

	pBox->SetRect(KirbyRect);

	BBox = pBox;
}
Kirby::~Kirby()
{
	if (BBox)
		delete BBox;
}
void Kirby::Load(const Rect& rc)
{
	ClientRect = rc;

	init();
}
void Kirby::Input(DWORD tick)
{
	if (_current)
		_current->Input(tick);	
}
void Kirby::Draw(HDC hdc)
{
	DWORD timeF = ::GetTickCount();

	if (_current)
		_current->Draw(hdc);

	if (BBox)
		BBox->Draw(hdc);

	DWORD timeE = ::GetTickCount() - timeF;

	std::wostringstream oss;
	oss << _T("KirbyDrawTime : ") << timeE << _T("\n");

	::OutputDebugString(oss.str().c_str());
}
void Kirby::Update(DWORD tick)
{
	DWORD timeF = ::GetTickCount();

	KirbyRect.left = KirbyPos.x- KirbyWidth/2;
	KirbyRect.top = KirbyPos.y - KirbyHeight/2;
	KirbyRect.right = KirbyRect.left + KirbyWidth;
	KirbyRect.bottom = KirbyRect.top + KirbyHeight;

	Point tmp;
	tmp = KirbyPos;
	tmp.x = tmp.x - KirbyWidth/2;
	tmp.y = tmp.y - KirbyHeight/2;

	int CollideCount = 0;

	std::list<Block*> bList = BlockDepot.getList();
	std::list<Block*>::iterator it;
	for (it = bList.begin(); it != bList.end(); it++)
	{
		Rect tmp;
		if(IntersectRect(&tmp, &KirbyRect, &(*it)->GetBBoxRect()))
		{
			BYTE tmpstate = BBox->BoxIsCollide((*it)->getBBox(), BackPosition);

			CollideCount++;
			if(tmpstate == 1)
			{
				_current->GetState(1);
				KirbyState = tmpstate;
			}
			else if(tmpstate == 2)
			{
				_current->GetState(2);
				KirbyState = tmpstate;
			}
			else if(tmpstate == 3)
			{
				_current->GetState(3);
				KirbyState = tmpstate;
			}
			else if(tmpstate == 0)
			{
				_current->GetStand(true);
				_current->GetState(0);
				KirbyState = tmpstate;
			}

			if(CollideCount >= 2)
			{
				break;
			}
		}
	}
	if(CollideCount == 0)
	{
		_current->GetStand(false);
		_current->GetState(0);
		KirbyState = 4;
	}

	if (_current)
		_current->Update(tick);

	if (BBox)
		dynamic_cast<SquareBox*>(BBox)->SetRect(KirbyRect);

	DWORD timeE = ::GetTickCount() - timeF;

	std::wostringstream oss;
	std::wostringstream sss;
	oss << _T("KirbyRect : ") << KirbyRect.bottom << _T("\n");
	sss << _T("KirbyState : ") << KirbyState << _T("\n");

	::OutputDebugString(oss.str().c_str());
	::OutputDebugString(sss.str().c_str());
}

Point Kirby::RetrunKirbyPos() const
{
	return PlayerPos;
}
void Kirby::Setposition(const Point& ppt, const Point& kpt, const bool& back)
{
	PlayerPos = ppt;
	KirbyPos = kpt;
	BackPosition = back;

	StateDepotType::iterator it;
	for (it = StateDepot.begin();
		it != StateDepot.end(); it++)
	{
		it->second->SetPosition(PlayerPos, KirbyPos, BackPosition);
	}
}
void Kirby::init()
{
	static KirbyIdle idle;
	static KirbyMove move;
	static KirbyRun run;
	static KirbyJump jump;
	static KirbyLanding landing;
	static KirbyFloat flo;

	idle.SetPosition(PlayerPos, KirbyPos, BackPosition);
	move.SetPosition(PlayerPos, KirbyPos, BackPosition);
	run.SetPosition(PlayerPos, KirbyPos, BackPosition);
	jump.SetPosition(PlayerPos, KirbyPos, BackPosition);
	landing.SetPosition(PlayerPos, KirbyPos, BackPosition);
	flo.SetPosition(PlayerPos, KirbyPos, BackPosition);

	AddEntry(_T("Idle"), &idle);
	AddEntry(_T("Move"), &move);
	AddEntry(_T("Run"), &run);
	AddEntry(_T("Jump"), &jump);
	AddEntry(_T("Landing"), &landing);
	AddEntry(_T("Float"), &flo);

	transition(_T("Landing"));
}