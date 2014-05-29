#include "KirbyState.h"

void KirbyStateCon::Load(const Rect& rc, const Point& KirbyPos)
{
	srand((unsigned)time(NULL));
	ClientRect = rc;

	ImgLoader(_T("Resource//Kirby.image"));
	AniLoader(_T("Resource//Kirby.animation"));

	Rect tmp;
	tmp.left = KirbyPos.x - KirbyWidth/2;
	tmp.top = KirbyPos.y - KirbyHeight/2;
	tmp.right = KirbyWidth + tmp.left;
	tmp.bottom = KirbyHeight + tmp.top;

	ImgDepot["KirbyStd"]->SetRect(tmp);
	ImgDepot["KirbyStdBack"]->SetRect(tmp);
	ImgDepot["KirbyBreak"]->SetRect(tmp);
	ImgDepot["KirbyBreakBack"]->SetRect(tmp);

	AniDepot["KirbyMove"]->SetRect(tmp);
	AniDepot["KirbyMoveBack"]->SetRect(tmp);
	AniDepot["KirbyRun"]->SetRect(tmp);
	AniDepot["KirbyRunBack"]->SetRect(tmp);
	AniDepot["KirbyStdEye"]->SetRect(tmp);
	AniDepot["KirbyStdEyeBack"]->SetRect(tmp);
}
void KirbyStateCon::Input(DWORD)
{
}
void KirbyStateCon::Draw(HDC hdc, const BYTE& state, const bool& BackPosition)
{
	if(state == StdState)
	{
		if(!BackPosition)
		{
			if(!(AniDepot["KirbyStdEye"]->IsPlaying()))
				ImgDepot["KirbyStd"]->Draw(hdc);
			else
				AniDepot["KirbyStdEye"]->Draw(hdc);
		}
		else
		{
			if(!(AniDepot["KirbyStdEyeBack"]->IsPlaying()))
				ImgDepot["KirbyStdBack"]->Draw(hdc);
			else
				AniDepot["KirbyStdEyeBack"]->Draw(hdc);
		}
	}
	if(state == MoveState)
	{
		if(!BackPosition)
		{
			AniDepot["KirbyMove"]->Draw(hdc);
		}
		else
		{
			AniDepot["KirbyMoveBack"]->Draw(hdc);
		}
	}
	if(state == RunState)
	{
		if(!BackPosition)
		{
			AniDepot["KirbyRun"]->Draw(hdc);
		}
		else
		{
			AniDepot["KirbyRunBack"]->Draw(hdc);
		}
	}
	if(state == BreakState)
	{
		if(BackPosition)
		{
			ImgDepot["KirbyBreak"]->Draw(hdc);
		}
		else
		{
			ImgDepot["KirbyBreakBack"]->Draw(hdc);
		}
	}
}
void KirbyStateCon::Update(DWORD tick, const Rect& tmp)
{
	ImgDepot["KirbyStd"]->SetRect(tmp);
	AniDepot["KirbyStdEye"]->SetRect(tmp);
	ImgDepot["KirbyBreak"]->SetRect(Rect(tmp.left, tmp.top, tmp.right+5, tmp.bottom));
	AniDepot["KirbyMove"]->SetRect(Rect(tmp.left, tmp.top, tmp.right+5, tmp.bottom));
	AniDepot["KirbyRun"]->SetRect(Rect(tmp.left, tmp.top, tmp.right+5, tmp.bottom));

	ImgDepot["KirbyStdBack"]->SetRect(tmp);
	AniDepot["KirbyStdEyeBack"]->SetRect(tmp);
	ImgDepot["KirbyBreakBack"]->SetRect(Rect(tmp.left-5, tmp.top, tmp.right, tmp.bottom));
	AniDepot["KirbyMoveBack"]->SetRect(Rect(tmp.left-5, tmp.top, tmp.right, tmp.bottom));
	AniDepot["KirbyRunBack"]->SetRect(Rect(tmp.left-5, tmp.top, tmp.right, tmp.bottom));

	AniDepot["KirbyMove"]->Update(tick);
	AniDepot["KirbyStdEye"]->Update(tick);
	AniDepot["KirbyStdEye"]->ResetAni(tick, rand()%5000 + 1000);
	AniDepot["KirbyRun"]->Update(tick);

	AniDepot["KirbyMoveBack"]->Update(tick);
	AniDepot["KirbyStdEyeBack"]->Update(tick);
	AniDepot["KirbyStdEyeBack"]->ResetAni(tick, rand()%5000 + 1000);
	AniDepot["KirbyRunBack"]->Update(tick);
}
void KirbyStateCon::StateReset()
{
	
}