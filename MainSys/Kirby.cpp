#include "Kirby.h"

Kirby::Kirby()
:KirbyPos(200, 200), PlayerPos(375, 200), update_dt(0),
state(0), BackPosition(false), MoveAcc(0), MoveAccCount(0)
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

	if (_current)
		_current->Update(tick);

	Point tmp;
	tmp = KirbyPos;
	tmp.x = tmp.x - KirbyWidth/2;
	tmp.y = tmp.y - KirbyHeight/2;

	if (BBox)
		dynamic_cast<SquareBox*>(BBox)->SetRect(KirbyRect);

	DWORD timeE = ::GetTickCount() - timeF;

	std::wostringstream oss;
	oss << _T("KirbyUpdateTime : ") << timeE << _T("\n");

	::OutputDebugString(oss.str().c_str());
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

	idle.SetPosition(PlayerPos, KirbyPos, BackPosition);
	move.SetPosition(PlayerPos, KirbyPos, BackPosition);
	run.SetPosition(PlayerPos, KirbyPos, BackPosition);

	AddEntry(_T("Idle"), &idle);
	AddEntry(_T("Move"), &move);
	AddEntry(_T("Run"), &run);

	transition(_T("Idle"));
}