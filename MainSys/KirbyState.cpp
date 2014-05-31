#include "KirbyState.h"

/////////////////////////////////////////////////////////////
// 대기 동작
/////////////////////////////////////////////////////////////
KirbyIdle::KirbyIdle()
: update_dt(0), MoveAcc(0), update_delay(20)
{
	for(int i = 0; i < ZoneMax; i++)
	{
		Zone[i] = GameGlobal.rcClient;
	}
	Zone[LeftZone].right = Zone[LeftZone].left + 385;
	Zone[RightZone].left = Zone[RightZone].right - 385;
}
KirbyIdle::~KirbyIdle()
{
}
void KirbyIdle::Enter()
{
	srand((unsigned)time(NULL));

	ImgDepot["KirbyStd"]->SetRect(KirbyRect);
	ImgDepot["KirbyStdBack"]->SetRect(KirbyRect);
	AniDepot["KirbyStdEye"]->SetRect(KirbyRect);
	AniDepot["KirbyStdEyeBack"]->SetRect(KirbyRect);
}
void KirbyIdle::Input(DWORD tick)
{
	if(update_dt > 20)
	{
		int count = update_dt / 20;

		for(int i = 0; i < count; i++)
		{
			if(InputDevice[VK_RIGHT])
			{
				BackPosition = false;
				pMachine->transition(_T("Move"));
			}
			else if(InputDevice[VK_LEFT])
			{
				BackPosition = true;
				pMachine->transition(_T("Move"));
			}

			update_dt = update_dt % 20;
		}
	}
	update_dt += tick;
}
void KirbyIdle::Update(DWORD tick)
{
	ImgDepot["KirbyStd"]->SetRect(KirbyRect);
	AniDepot["KirbyStdEye"]->SetRect(KirbyRect);
	ImgDepot["KirbyStdBack"]->SetRect(KirbyRect);
	AniDepot["KirbyStdEyeBack"]->SetRect(KirbyRect);

	AniDepot["KirbyStdEye"]->Update(tick);
	AniDepot["KirbyStdEye"]->ResetAni(tick, rand()%5000 + 1000);
	AniDepot["KirbyStdEyeBack"]->Update(tick);
	AniDepot["KirbyStdEyeBack"]->ResetAni(tick, rand()%5000 + 1000);
}
void KirbyIdle::Draw(HDC hdc)
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
void KirbyIdle::Leave()
{
}
void KirbyIdle::SetPosition(const Point& ppt, const Point& kpt, const bool& back)
{
	PlayerPos = ppt;

	KirbyPos = kpt;

	BackPosition = back;

	KirbyRect.left = KirbyPos.x - KirbyWidth/2;
	KirbyRect.top = KirbyPos.y - KirbyHeight/2;
	KirbyRect.right = KirbyWidth + KirbyRect.left;
	KirbyRect.bottom = KirbyHeight + KirbyRect.top;
}


/////////////////////////////////////////////////////////////
// 걷기 동작
/////////////////////////////////////////////////////////////
KirbyMove::KirbyMove()
: update_dt(0), MoveAcc(0), update_delay(20)
, KeyPush(true), update_dt2(0), update_delay2(60)
, RightKey(false)
{
	for(int i = 0; i < ZoneMax; i++)
	{
		Zone[i] = GameGlobal.rcClient;
	}
	Zone[LeftZone].right = Zone[LeftZone].left + 385;
	Zone[RightZone].left = Zone[RightZone].right - 385;
}
KirbyMove::~KirbyMove()
{
}
void KirbyMove::Enter()
{
	AniDepot["KirbyMove"]->SetRect(KirbyRect);
	AniDepot["KirbyMoveBack"]->SetRect(KirbyRect);
}
void KirbyMove::Input(DWORD tick)
{	
	if(MoveAcc > 0 && !KeyPush)
	{
		if (InputDevice[VK_RIGHT] && RightKey)
		{
			BackPosition = false;
			pMachine->transition(_T("Run"));			
		}
		else if (InputDevice[VK_LEFT] && !RightKey)
		{
			BackPosition = true;
			pMachine->transition(_T("Run"));
		}		
	}

	if (InputDevice[VK_RIGHT])
	{
		BackPosition = false;
		KeyPush = true;
		RightKey = true;
	}
	else if (InputDevice[VK_LEFT])
	{
		BackPosition = true;
		KeyPush = true;
		RightKey = false;
	}
	else
	{
		KeyPush = false;
	}

	if(!KeyPush && MoveAcc == 0)
	{
		pMachine->transition(_T("Idle"));
	}
}
void KirbyMove::Update(DWORD tick)
{
	// 이동좌표 업데이트
	if(KeyPush)
	{
		update_dt2 = 0;
		if(update_dt > update_delay)
		{
			int count = update_dt / update_delay;
			for(int i = 0; i < count; i++)
			{
				Rect tmp;
				if(!BackPosition && ::IntersectRect(&tmp, &Zone[RightZone], &KirbyRect))
				{
					MoveAcc += 1;

					if(MoveAcc >= 4)
						MoveAcc = 4;

					if(PlayerPos.x < 390)
						PlayerPos.x = 390;

					PlayerPos.x += MoveAcc;
				}
				else if(BackPosition && ::IntersectRect(&tmp, &Zone[LeftZone], &KirbyRect) && !(PtInRect(&Zone[LeftZone], PlayerPos)))
				{
					MoveAcc += 1;

					if(MoveAcc >= 4)
						MoveAcc = 4;

					PlayerPos.x -= MoveAcc;

					if(PlayerPos.x < 0)
						PlayerPos.x = 0;
				}
				else if(!BackPosition)
				{
					MoveAcc += 1;

					if(MoveAcc >= 4)
						MoveAcc = 4;

					KirbyPos.x += MoveAcc;

					if(KirbyPos.x - KirbyWidth/2 < 0)
						KirbyPos.x = KirbyWidth/2;
				}
				else if(BackPosition)
				{
					MoveAcc += 1;

					if(MoveAcc >= 4)
						MoveAcc = 4;

					KirbyPos.x -= MoveAcc;

					if(KirbyPos.x - KirbyWidth/2 < 0)
						KirbyPos.x = KirbyWidth/2;
				}
			}
			update_dt = update_dt % update_delay;
		}
		update_dt += tick;
	}
	else if(MoveAcc > 0)
	{
		update_dt = 0;
		if(update_dt2 > update_delay2)
		{
			int count = update_dt2 / update_delay2;
			for(int i = 0; i < count; i++)
			{
				Rect tmp;
				if(!(::IntersectRect(&tmp, &Zone[RightZone], &KirbyRect)))
				{
					if(BackPosition)
					{
						MoveAcc -= 1;
						KirbyPos.x -= MoveAcc;
						if(KirbyPos.x - KirbyWidth/2 < 0)
							KirbyPos.x = KirbyWidth/2;
					}
					else
					{
						MoveAcc -= 1;
						KirbyPos.x += MoveAcc;
					}
				}
				else
				{
					if(BackPosition)
					{
						MoveAcc -= 1;
						PlayerPos.x -= MoveAcc;
						if(PlayerPos.x < 0)
							PlayerPos.x = 0;
					}
					else
					{
						MoveAcc -= 1;
						PlayerPos.x += MoveAcc;
					}
				}
			}
			update_dt2 = update_dt2 % update_delay2;
		}
		update_dt2 += tick;
	}
	if (pMachine)
		dynamic_cast<Kirby*>(pMachine)->Setposition(PlayerPos, KirbyPos, BackPosition);

	// 애니메이션 업데이트
	AniDepot["KirbyMove"]->SetRect(Rect(KirbyRect.left, KirbyRect.top, KirbyRect.right+5, KirbyRect.bottom));
	AniDepot["KirbyMoveBack"]->SetRect(Rect(KirbyRect.left-5, KirbyRect.top, KirbyRect.right, KirbyRect.bottom));
	AniDepot["KirbyMove"]->Update(tick);
	AniDepot["KirbyMoveBack"]->Update(tick);
}
void KirbyMove::Draw(HDC hdc)
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
void KirbyMove::Leave()
{
}
void KirbyMove::SetPosition(const Point& ppt, const Point& kpt, const bool& back)
{
	PlayerPos = ppt;

	KirbyPos = kpt;

	BackPosition = back;

	KirbyRect.left = KirbyPos.x - KirbyWidth/2;
	KirbyRect.top = KirbyPos.y - KirbyHeight/2;
	KirbyRect.right = KirbyWidth + KirbyRect.left;
	KirbyRect.bottom = KirbyHeight + KirbyRect.top;
}

/////////////////////////////////////////////////////////////
// 달리는 동작
/////////////////////////////////////////////////////////////
KirbyRun::KirbyRun()
: Break(false), update_dt(0), update_delay(20), MoveAcc(4)
, KeyPush(true), update_dt2(0), update_delay2(60)
{
	for(int i = 0; i < ZoneMax; i++)
	{
		Zone[i] = GameGlobal.rcClient;
	}
	Zone[LeftZone].right = Zone[LeftZone].left + 385;
	Zone[RightZone].left = Zone[RightZone].right - 385;
}
KirbyRun::~KirbyRun()
{
}
void KirbyRun::Enter()
{
	ImgDepot["KirbyBreak"]->SetRect(KirbyRect);
	ImgDepot["KirbyBreakBack"]->SetRect(KirbyRect);
	AniDepot["KirbyRun"]->SetRect(KirbyRect);
	AniDepot["KirbyRunBack"]->SetRect(KirbyRect);
}
void KirbyRun::Input(DWORD)
{
	if (InputDevice[VK_RIGHT])
	{
		if(BackPosition && MoveAcc >= 5)
		{
			MoveAcc = -MoveAcc*2;
			Break = true;
		}

		BackPosition = false;
		KeyPush = true;
	}
	else if (InputDevice[VK_LEFT])
	{
		if(!BackPosition && MoveAcc >= 5)
		{
			MoveAcc = -MoveAcc*2;
			Break = true;
		}

		BackPosition = true;
		KeyPush = true;
	}
	else
	{
		KeyPush = false;
	}

	if(!KeyPush && MoveAcc == 0)
	{
		pMachine->transition(_T("Idle"));
	}
}
void KirbyRun::Update(DWORD tick)
{
	// 캐릭터 좌표 업데이트
	if(KeyPush)
	{
		update_dt2 = 0;
		if(update_dt > update_delay)
		{
			int count = update_dt / update_delay;
			for(int i = 0; i < count; i++)
			{
				if(MoveAcc >= 2)
					Break = false;
				Rect tmp;
				if(!BackPosition && ::IntersectRect(&tmp, &Zone[RightZone], &KirbyRect))
				{
					MoveAcc += 2;

					if(MoveAcc >= 8)
						MoveAcc = 8;

					if(PlayerPos.x < 390)
						PlayerPos.x = 390;

					PlayerPos.x += MoveAcc;
				}
				else if(BackPosition && ::IntersectRect(&tmp, &Zone[LeftZone], &KirbyRect) && !(PtInRect(&Zone[LeftZone], PlayerPos)))
				{
					MoveAcc += 2;

					if(MoveAcc >= 8)
						MoveAcc = 8;

					PlayerPos.x -= MoveAcc;

					if(PlayerPos.x < 0)
						PlayerPos.x = 0;
				}
				else if(!BackPosition)
				{
					MoveAcc += 2;

					if(MoveAcc >= 8)
						MoveAcc = 8;

					KirbyPos.x += MoveAcc;

					if(KirbyPos.x - KirbyWidth/2 < 0)
						KirbyPos.x = KirbyWidth/2;
				}
				else if(BackPosition)
				{
					MoveAcc += 2;

					if(MoveAcc >= 8)
						MoveAcc = 8;

					KirbyPos.x -= MoveAcc;

					if(KirbyPos.x - KirbyWidth/2 < 0)
						KirbyPos.x = KirbyWidth/2;
				}
			}
			update_dt = update_dt % update_delay;
		}
		update_dt += tick;
	}
	else if(MoveAcc > 0)
	{
		update_dt = 0;
		if(update_dt2 > update_delay2)
		{
			int count = update_dt2 / update_delay2;
			for(int i = 0; i < count; i++)
			{
				Rect tmp;
				if(!(::IntersectRect(&tmp, &Zone[RightZone], &KirbyRect)))
				{
					if(BackPosition)
					{
						MoveAcc -= 1;
						KirbyPos.x -= MoveAcc;
						if(KirbyPos.x - KirbyWidth/2 < 0)
							KirbyPos.x = KirbyWidth/2;
					}
					else
					{
						MoveAcc -= 1;
						KirbyPos.x += MoveAcc;
					}
				}
				else
				{
					if(BackPosition)
					{
						MoveAcc -= 1;
						PlayerPos.x -= MoveAcc;
						if(PlayerPos.x < 0)
							PlayerPos.x = 0;
					}
					else
					{
						MoveAcc -= 1;
						PlayerPos.x += MoveAcc;
					}
				}
			}
			update_dt2 = update_dt2 % update_delay2;
		}
		update_dt2 += tick;
	}
	if (pMachine)
		dynamic_cast<Kirby*>(pMachine)->Setposition(PlayerPos, KirbyPos, BackPosition);

	// 애니메이션 업데이트
	ImgDepot["KirbyBreak"]->SetRect(Rect(KirbyRect.left, KirbyRect.top, KirbyRect.right+5, KirbyRect.bottom));
	AniDepot["KirbyRun"]->SetRect(Rect(KirbyRect.left, KirbyRect.top, KirbyRect.right+5, KirbyRect.bottom));

	ImgDepot["KirbyBreakBack"]->SetRect(Rect(KirbyRect.left-5, KirbyRect.top, KirbyRect.right, KirbyRect.bottom));
	AniDepot["KirbyRunBack"]->SetRect(Rect(KirbyRect.left-5, KirbyRect.top, KirbyRect.right, KirbyRect.bottom));

	AniDepot["KirbyRun"]->Update(tick);

	AniDepot["KirbyRunBack"]->Update(tick);
}
void KirbyRun::Draw(HDC hdc)
{
	if(!Break)
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
	else
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
void KirbyRun::Leave()
{
}
void KirbyRun::SetPosition(const Point& ppt, const Point& kpt, const bool& back)
{
	PlayerPos = ppt;

	KirbyPos = kpt;

	BackPosition = back;

	KirbyRect.left = KirbyPos.x - KirbyWidth/2;
	KirbyRect.top = KirbyPos.y - KirbyHeight/2;
	KirbyRect.right = KirbyWidth + KirbyRect.left;
	KirbyRect.bottom = KirbyHeight + KirbyRect.top;
}