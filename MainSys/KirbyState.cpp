#include "KirbyState.h"

/////////////////////////////////////////////////////////////
// 대기 동작
/////////////////////////////////////////////////////////////
KirbyIdle::KirbyIdle()
: update_dt(0), MoveAcc(0), update_delay(20)
, Stand(false), Wall(0)
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
	if(InputDevice.OnClick('C'))
	{
		pMachine->transition(_T("Jump"));
	}
}
void KirbyIdle::Update(DWORD tick)
{
	if (pMachine)
		dynamic_cast<Kirby*>(pMachine)->Setposition(PlayerPos, KirbyPos, BackPosition);

	if(!Stand)
	{
		pMachine->transition(_T("Landing"));
	}

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
void KirbyIdle::GetState(const BYTE& wl)
{
	Wall = wl;
}
void KirbyIdle::GetStand(const bool& st)
{
	Stand = st;
}


/////////////////////////////////////////////////////////////
// 걷기 동작
/////////////////////////////////////////////////////////////
KirbyMove::KirbyMove()
: update_dt(0), MoveAcc(0), update_delay(20)
, KeyPush(true), update_dt2(0), update_delay2(60)
, RightKey(false), Stand(false), Wall(0)
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
	if(InputDevice.OnClick('C'))
	{
		pMachine->transition(_T("Jump"));
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
					if(!(Wall == 2))
						PlayerPos.x += MoveAcc;
				}
				else if(BackPosition && ::IntersectRect(&tmp, &Zone[LeftZone], &KirbyRect) && !(PtInRect(&Zone[LeftZone], PlayerPos)))
				{
					MoveAcc += 1;

					if(MoveAcc >= 4)
						MoveAcc = 4;

					if(!(Wall == 1))
						PlayerPos.x -= MoveAcc;

					if(PlayerPos.x < 0)
						PlayerPos.x = 0;
				}
				else if(!BackPosition)
				{
					MoveAcc += 1;

					if(MoveAcc >= 4)
						MoveAcc = 4;

					if(!(Wall == 2))
						KirbyPos.x += MoveAcc;

					if(KirbyPos.x - KirbyWidth/2 < 0)
						KirbyPos.x = KirbyWidth/2;
				}
				else if(BackPosition)
				{
					MoveAcc += 1;

					if(MoveAcc >= 4)
						MoveAcc = 4;

					if(!(Wall == 1))
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
						if(!(Wall == 1))
							KirbyPos.x -= MoveAcc;
						if(KirbyPos.x - KirbyWidth/2 < 0)
							KirbyPos.x = KirbyWidth/2;
					}
					else
					{
						MoveAcc -= 1;
						if(!(Wall == 2))
							KirbyPos.x += MoveAcc;
					}
				}
				else
				{
					if(BackPosition)
					{
						MoveAcc -= 1;
						if(!(Wall == 1))
							PlayerPos.x -= MoveAcc;
						if(PlayerPos.x < 0)
							PlayerPos.x = 0;
					}
					else
					{
						MoveAcc -= 1;
						if(!(Wall == 2))
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

	if(!Stand)
	{
		pMachine->transition(_T("Landing"));
	}

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
void KirbyMove::GetState(const BYTE& wl)
{
	Wall = wl;
}
void KirbyMove::GetStand(const bool& st)
{
	Stand = st;
}


/////////////////////////////////////////////////////////////
// 달리는 동작
/////////////////////////////////////////////////////////////
KirbyRun::KirbyRun()
: Break(false), update_dt(0), update_delay(20), MoveAcc(4)
, KeyPush(true), update_dt2(0), update_delay2(60), Stand(false), Wall(0)
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
	if(InputDevice.OnClick('C'))
	{
		pMachine->transition(_T("Jump"));
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
				if(MoveAcc >= 0 && MoveAcc < 3)
				{
					Break = false;
					MoveAcc = 4;
					pMachine->transition(_T("Idle"));
				}
				Rect tmp;
				if(!BackPosition && ::IntersectRect(&tmp, &Zone[RightZone], &KirbyRect))
				{
					MoveAcc += 2;

					if(MoveAcc >= 8)
						MoveAcc = 8;

					if(PlayerPos.x < 375)
						PlayerPos.x = 375;

					if(!(Wall == 2))
						PlayerPos.x += MoveAcc;
				}
				else if(BackPosition && ::IntersectRect(&tmp, &Zone[LeftZone], &KirbyRect) && !(PtInRect(&Zone[LeftZone], PlayerPos)))
				{
					MoveAcc += 2;

					if(MoveAcc >= 8)
						MoveAcc = 8;

					if(!(Wall == 1))
						PlayerPos.x -= MoveAcc;

					if(PlayerPos.x < 0)
						PlayerPos.x = 0;
				}
				else if(!BackPosition)
				{
					MoveAcc += 2;

					if(MoveAcc >= 8)
						MoveAcc = 8;

					if(!(Wall == 2))
						KirbyPos.x += MoveAcc;

					if(KirbyPos.x - KirbyWidth/2 < 0)
						KirbyPos.x = KirbyWidth/2;
				}
				else if(BackPosition)
				{
					MoveAcc += 2;

					if(MoveAcc >= 8)
						MoveAcc = 8;

					if(!(Wall == 1))
						KirbyPos.x -= MoveAcc;

					if(KirbyPos.x - KirbyWidth/2 < 0)
						KirbyPos.x = KirbyWidth/2;
				}
			}
			update_dt = update_dt % update_delay;
		}
		update_dt += tick;
	}
	else if(MoveAcc > 0 || MoveAcc < 0)
	{
		update_dt = 0;
		if(update_dt2 > update_delay2)
		{
			int count = update_dt2 / update_delay2;
			for(int i = 0; i < count; i++)
			{
				if(MoveAcc > 0)
				{
					Rect tmp;
					if(!(::IntersectRect(&tmp, &Zone[RightZone], &KirbyRect)))
					{
						if(BackPosition)
						{
							MoveAcc -= 1;

							if(!(Wall == 1 || Wall == 2))
								KirbyPos.x -= MoveAcc;
							if(KirbyPos.x - KirbyWidth/2 < 0)
								KirbyPos.x = KirbyWidth/2;
						}
						else
						{
							MoveAcc -= 1;

							if(!(Wall == 1 || Wall == 2))
								KirbyPos.x += MoveAcc;
						}
					}
					else
					{
						if(BackPosition)
						{
							MoveAcc -= 1;

							if(!(Wall == 1 || Wall == 2))
								PlayerPos.x -= MoveAcc;
							if(PlayerPos.x < 0)
								PlayerPos.x = 0;
						}
						else
						{
							MoveAcc -= 1;

							if(!(Wall == 1 || Wall == 2))
								PlayerPos.x += MoveAcc;
						}
					}
				}
				else if(MoveAcc < 0)
				{
					Rect tmp;
					if(!(::IntersectRect(&tmp, &Zone[RightZone], &KirbyRect)))
					{
						if(BackPosition)
						{
							MoveAcc += 1;

							if(!(Wall == 1 || Wall == 2))
								KirbyPos.x -= MoveAcc;
							if(KirbyPos.x - KirbyWidth/2 < 0)
								KirbyPos.x = KirbyWidth/2;
						}
						else
						{
							MoveAcc += 1;

							if(!(Wall == 1 || Wall == 2))
								KirbyPos.x += MoveAcc;
							if(KirbyPos.x - KirbyWidth/2 < 0)
								KirbyPos.x = KirbyWidth/2;
						}
					}
					else
					{
						if(BackPosition)
						{
							MoveAcc += 1;

							if(!(Wall == 1 || Wall == 2))
								PlayerPos.x -= MoveAcc;
							if(PlayerPos.x < 0)
								PlayerPos.x = 0;
						}
						else
						{
							MoveAcc += 1;

							if(!(Wall == 1 || Wall == 2))
								PlayerPos.x += MoveAcc;
							if(PlayerPos.x < 375)
								PlayerPos.x = 375;
						}
					}
				}
			}
			update_dt2 = update_dt2 % update_delay2;
		}
		update_dt2 += tick;
	}
	if (pMachine)
		dynamic_cast<Kirby*>(pMachine)->Setposition(PlayerPos, KirbyPos, BackPosition);

	if(!Stand)
	{
		pMachine->transition(_T("Landing"));
	}

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
void KirbyRun::GetState(const BYTE& wl)
{
	Wall = wl;
}
void KirbyRun::GetStand(const bool& st)
{
	Stand = st;
}


/////////////////////////////////////////////////////////////
// 점프하는 동작
/////////////////////////////////////////////////////////////
KirbyJump::KirbyJump()
: Rolling(false), update_dt(0), update_delay(20), MoveAcc(4)
, Stand(false), Wall(0), JumpEnd(false)
,JumpAcc(18), KeyPush(false)
{
	for(int i = 0; i < ZoneMax; i++)
	{
		Zone[i] = GameGlobal.rcClient;
	}
	Zone[LeftZone].right = Zone[LeftZone].left + 385;
	Zone[RightZone].left = Zone[RightZone].right - 385;
}
KirbyJump::~KirbyJump()
{
}
void KirbyJump::Enter()
{
	ImgDepot["KirbyJumpStart"]->SetRect(KirbyRect);
	ImgDepot["KirbyJumpStartBack"]->SetRect(KirbyRect);

	AniDepot["KirbyRolling"]->SetRect(KirbyRect);
	AniDepot["KirbyRollingBack"]->SetRect(KirbyRect);
}
void KirbyJump::Input(DWORD)
{
	if (InputDevice[VK_RIGHT])
	{
		BackPosition = false;
		KeyPush = true;
	}
	else if (InputDevice[VK_LEFT])
	{
		BackPosition = true;
		KeyPush = true;
	}
	else
		KeyPush = false;

	if (InputDevice.OnClick('C'))
	{
		AniDepot["KirbyRolling"]->reset();
		AniDepot["KirbyRollingBack"]->reset();

		Rolling = false;
		JumpAcc = 18;
		pMachine->transition(_T("Float"));
	}
}
void KirbyJump::Update(DWORD tick)
{
	// 캐릭터 좌표 업데이트

	if(update_dt > update_delay)
	{
		int count = update_dt / update_delay;
		for(int i = 0; i < count; i++)
		{
			Rect tmp;
			if(KeyPush)
			{
				if(!BackPosition && ::IntersectRect(&tmp, &Zone[RightZone], &KirbyRect))
				{
					MoveAcc += 1;

					JumpAcc -= 1;

					if(MoveAcc >= 4)
						MoveAcc = 4;

					if(PlayerPos.x < 375)
						PlayerPos.x = 375;

					if(!(Wall == 2))
						PlayerPos.x += MoveAcc;
					if(!(Wall == 3))
						KirbyPos.y -= JumpAcc;

					if(KirbyPos.y < 0)
						KirbyPos.y = 0;
				}
				else if(BackPosition && ::IntersectRect(&tmp, &Zone[LeftZone], &KirbyRect) && !(PtInRect(&Zone[LeftZone], PlayerPos)))
				{
					MoveAcc += 1;

					JumpAcc -= 1;

					if(MoveAcc >= 4)
						MoveAcc = 4;

					if(!(Wall == 1))
						PlayerPos.x -= MoveAcc;
					if(!(Wall == 3))
						KirbyPos.y -= JumpAcc;

					if(PlayerPos.x < 0)
						PlayerPos.x = 0;

					if(KirbyPos.y < 0)
						KirbyPos.y = 0;
				}
				else if(!BackPosition)
				{
					MoveAcc += 1;

					JumpAcc -= 1;

					if(MoveAcc >= 4)
						MoveAcc = 4;

					if(!(Wall == 2))
						KirbyPos.x += MoveAcc;
					if(!(Wall == 3))
						KirbyPos.y -= JumpAcc;

					if(KirbyPos.x - KirbyWidth/2 < 0)
						KirbyPos.x = KirbyWidth/2;
					if(KirbyPos.y < 0)
						KirbyPos.y = 0;
				}
				else if(BackPosition)
				{
					MoveAcc += 1;

					JumpAcc -= 1;

					if(MoveAcc >= 4)
						MoveAcc = 4;

					if(!(Wall == 1))
						KirbyPos.x -= MoveAcc;
					if(!(Wall == 3))
						KirbyPos.y -= JumpAcc;

					if(KirbyPos.x - KirbyWidth/2 < 0)
						KirbyPos.x = KirbyWidth/2;
					if(KirbyPos.y < 0)
						KirbyPos.y = 0;
				}
				if(JumpAcc <= 0)
					Rolling = true;
			}
			else
			{
				if(!BackPosition)
				{
					JumpAcc -= 1;

					if(MoveAcc >= 4)
						MoveAcc = 4;

					if(!(Wall == 3))
						KirbyPos.y -= JumpAcc;

					if(KirbyPos.y < 0)
						KirbyPos.y = 0;
				}
				else if(BackPosition)
				{
					JumpAcc -= 1;

					if(!(Wall == 3))
						KirbyPos.y -= JumpAcc;

					if(KirbyPos.y < 0)
						KirbyPos.y = 0;
				}
				if(JumpAcc <= 0)
					Rolling = true;
			}
		}
		update_dt = update_dt % update_delay;
	} // if(update_dt > update_delay)
	update_dt += tick;

	if (pMachine)
		dynamic_cast<Kirby*>(pMachine)->Setposition(PlayerPos, KirbyPos, BackPosition);

	/*if(!Stand)
	{
	Rolling = false;
	JumpAcc = 15;
	pMachine->transition(_T("Landing"));
	}*/
	if(!(AniDepot["KirbyRolling"]->IsPlaying()) || !(AniDepot["KirbyRollingBack"]->IsPlaying()) && Rolling)
	{
		AniDepot["KirbyRolling"]->reset();
		AniDepot["KirbyRollingBack"]->reset();

		Rolling = false;
		JumpAcc = 18;
		pMachine->transition(_T("Landing"));
	}

	// 애니메이션 업데이트
	ImgDepot["KirbyJumpStart"]->SetRect(Rect(KirbyRect.left, KirbyRect.top, KirbyRect.right+5, KirbyRect.bottom));
	ImgDepot["KirbyJumpStartBack"]->SetRect(Rect(KirbyRect.left-5, KirbyRect.top, KirbyRect.right, KirbyRect.bottom));

	AniDepot["KirbyRolling"]->SetRect(Rect(KirbyRect.left, KirbyRect.top, KirbyRect.right+5, KirbyRect.bottom));
	AniDepot["KirbyRollingBack"]->SetRect(Rect(KirbyRect.left-5, KirbyRect.top, KirbyRect.right, KirbyRect.bottom));
	if(Rolling)
	{
		AniDepot["KirbyRolling"]->Update(tick);
		AniDepot["KirbyRollingBack"]->Update(tick);
	}
}
void KirbyJump::Draw(HDC hdc)
{
	if(!Rolling)
	{
		if(!BackPosition)
		{
			ImgDepot["KirbyJumpStart"]->Draw(hdc);
		}
		else
		{
			ImgDepot["KirbyJumpStartBack"]->Draw(hdc);
		}
	}
	if(Rolling)
	{
		if(!BackPosition)
		{
			AniDepot["KirbyRolling"]->Draw(hdc);
		}
		else
		{
			AniDepot["KirbyRollingBack"]->Draw(hdc);
		}
	}
}
void KirbyJump::Leave()
{
}
void KirbyJump::SetPosition(const Point& ppt, const Point& kpt, const bool& back)
{
	PlayerPos = ppt;

	KirbyPos = kpt;

	BackPosition = back;

	KirbyRect.left = KirbyPos.x - KirbyWidth/2;
	KirbyRect.top = KirbyPos.y - KirbyHeight/2;
	KirbyRect.right = KirbyWidth + KirbyRect.left;
	KirbyRect.bottom = KirbyHeight + KirbyRect.top;
}
void KirbyJump::GetState(const BYTE& wl)
{
	Wall = wl;
}
void KirbyJump::GetStand(const bool& st)
{
	Stand = st;
}


/////////////////////////////////////////////////////////////
// 낙하하는 동작
/////////////////////////////////////////////////////////////
KirbyLanding::KirbyLanding()
: update_dt(0), update_delay(20), MoveAcc(4)
, Stand(false), Wall(0), Landing(false)
,JumpAcc(0), KeyPush(false)
{
	for(int i = 0; i < ZoneMax; i++)
	{
		Zone[i] = GameGlobal.rcClient;
	}
	Zone[LeftZone].right = Zone[LeftZone].left + 385;
	Zone[RightZone].left = Zone[RightZone].right - 385;
}
KirbyLanding::~KirbyLanding()
{
}
void KirbyLanding::Enter()
{
	AniDepot["KirbyLanding"]->SetRect(KirbyRect);
	AniDepot["KirbyLandingBack"]->SetRect(KirbyRect);

	AniDepot["KirbyJumpEnd"]->SetRect(KirbyRect);
	AniDepot["KirbyJumpEndBack"]->SetRect(KirbyRect);
}
void KirbyLanding::Input(DWORD)
{
	if (InputDevice[VK_RIGHT])
	{
		BackPosition = false;
		KeyPush = true;
	}
	else if (InputDevice[VK_LEFT])
	{
		BackPosition = true;
		KeyPush = true;
	}
	else
		KeyPush = false;

	if (InputDevice.OnClick('C'))
	{
		pMachine->transition(_T("Float"));
	}
}
void KirbyLanding::Update(DWORD tick)
{
	// 캐릭터 좌표 업데이트

	if(update_dt > update_delay)
	{
		int count = update_dt / update_delay;
		for(int i = 0; i < count; i++)
		{
			Rect tmp;
			if(KeyPush && !Landing)
			{
				if(!BackPosition && ::IntersectRect(&tmp, &Zone[RightZone], &KirbyRect))
				{
					MoveAcc += 1;

					JumpAcc += 2;

					if(JumpAcc >= 8)
						JumpAcc = 8;

					if(MoveAcc >= 4)
						MoveAcc = 4;

					if(PlayerPos.x < 375)
						PlayerPos.x = 375;

					if(!(Wall == 2))
						PlayerPos.x += MoveAcc;
					if(!(Wall == 3))
						KirbyPos.y += JumpAcc;
				}
				else if(BackPosition && ::IntersectRect(&tmp, &Zone[LeftZone], &KirbyRect) && !(PtInRect(&Zone[LeftZone], PlayerPos)))
				{
					MoveAcc += 1;

					JumpAcc += 2;

					if(JumpAcc >= 8)
						JumpAcc = 8;

					if(MoveAcc >= 4)
						MoveAcc = 4;

					if(!(Wall == 1))
						PlayerPos.x -= MoveAcc;
					if(!(Wall == 3))
						KirbyPos.y += JumpAcc;

					if(PlayerPos.x < 0)
						PlayerPos.x = 0;
				}
				else if(!BackPosition)
				{
					MoveAcc += 1;

					JumpAcc += 2;

					if(JumpAcc >= 8)
						JumpAcc = 8;

					if(MoveAcc >= 4)
						MoveAcc = 4;

					if(!(Wall == 2))
						KirbyPos.x += MoveAcc;
					if(!(Wall == 3))
						KirbyPos.y += JumpAcc;

					if(KirbyPos.x - KirbyWidth/2 < 0)
						KirbyPos.x = KirbyWidth/2;
				}
				else if(BackPosition)
				{
					MoveAcc += 1;

					JumpAcc += 2;

					if(JumpAcc >= 8)
						JumpAcc = 8;

					if(MoveAcc >= 4)
						MoveAcc = 4;

					if(!(Wall == 1))
						KirbyPos.x -= MoveAcc;
					if(!(Wall == 3))
						KirbyPos.y += JumpAcc;

					if(KirbyPos.x - KirbyWidth/2 < 0)
						KirbyPos.x = KirbyWidth/2;
				}
			}
			else if(!Landing)
			{
				if(!BackPosition)
				{
					JumpAcc += 2;

					if(JumpAcc >= 8)
						JumpAcc = 8;

					if(!(Wall == 3))
						KirbyPos.y += JumpAcc;
				}
				else if(BackPosition)
				{
					JumpAcc += 2;

					if(JumpAcc >= 8)
						JumpAcc = 8;

					if(!(Wall == 3))
						KirbyPos.y += JumpAcc;
				}
			}
		}
		update_dt = update_dt % update_delay;
	}
	update_dt += tick;

	if(Stand)
	{
		Landing = true;
	}
	if(!(AniDepot["KirbyLanding"]->IsPlaying()) && Landing)
	{
		JumpAcc = 0;
		Stand = false;
		Landing = false;
		AniDepot["KirbyLanding"]->reset();
		AniDepot["KirbyLandingBack"]->reset();
		pMachine->transition(_T("Idle"));
	}

	if (pMachine)
		dynamic_cast<Kirby*>(pMachine)->Setposition(PlayerPos, KirbyPos, BackPosition);

	// 애니메이션 업데이트
	AniDepot["KirbyLanding"]->SetRect(Rect(KirbyRect.left, KirbyRect.top, KirbyRect.right+5, KirbyRect.bottom));
	AniDepot["KirbyLandingBack"]->SetRect(Rect(KirbyRect.left-5, KirbyRect.top, KirbyRect.right, KirbyRect.bottom));

	AniDepot["KirbyJumpEnd"]->SetRect(Rect(KirbyRect.left-5, KirbyRect.top, KirbyRect.right+5, KirbyRect.bottom));
	AniDepot["KirbyJumpEndBack"]->SetRect(Rect(KirbyRect.left-5, KirbyRect.top, KirbyRect.right, KirbyRect.bottom));

	if(Landing)
	{
		AniDepot["KirbyLanding"]->Update(tick);
		AniDepot["KirbyLandingBack"]->Update(tick);
	}
	AniDepot["KirbyJumpEnd"]->Update(tick);
	AniDepot["KirbyJumpEndBack"]->Update(tick);
}
void KirbyLanding::Draw(HDC hdc)
{
	if(!Landing)
	{
		if(!BackPosition)
		{
			AniDepot["KirbyJumpEnd"]->Draw(hdc);
		}
		else
		{
			AniDepot["KirbyJumpEndBack"]->Draw(hdc);
		}
	}
	else
	{
		if(!BackPosition)
		{
			AniDepot["KirbyLanding"]->Draw(hdc);
		}
		else
		{
			AniDepot["KirbyLandingBack"]->Draw(hdc);
		}
	}
}
void KirbyLanding::Leave()
{
}
void KirbyLanding::SetPosition(const Point& ppt, const Point& kpt, const bool& back)
{
	PlayerPos = ppt;

	KirbyPos = kpt;

	BackPosition = back;

	KirbyRect.left = KirbyPos.x - KirbyWidth/2;
	KirbyRect.top = KirbyPos.y - KirbyHeight/2;
	KirbyRect.right = KirbyWidth + KirbyRect.left;
	KirbyRect.bottom = KirbyHeight + KirbyRect.top;
}
void KirbyLanding::GetState(const BYTE& wl)
{
	Wall = wl;
}
void KirbyLanding::GetStand(const bool& st)
{
	Stand = st;
}
/////////////////////////////////////////////////////////////
// 공중을 나는 동작
/////////////////////////////////////////////////////////////
KirbyFloat::KirbyFloat()
: update_dt(0), update_delay(20), MoveAcc(4)
, Stand(false), Wall(0), Landing(false)
,JumpAcc(0), KeyPush(false), Float(false), KirbyUp(false)
{
	for(int i = 0; i < ZoneMax; i++)
	{
		Zone[i] = GameGlobal.rcClient;
	}
	Zone[LeftZone].right = Zone[LeftZone].left + 385;
	Zone[RightZone].left = Zone[RightZone].right - 385;
}
KirbyFloat::~KirbyFloat()
{
}
void KirbyFloat::Enter()
{
	AniDepot["KirbyFloatStart"]->SetRect(KirbyRect);
	AniDepot["KirbyFloatStartBack"]->SetRect(KirbyRect);

	AniDepot["KirbyFloat"]->SetRect(KirbyRect);
	AniDepot["KirbyFloatBack"]->SetRect(KirbyRect);

	AniDepot["KirbyFloatUp"]->SetRect(KirbyRect);
	AniDepot["KirbyFloatUpBack"]->SetRect(KirbyRect);

	AniDepot["KirbyFloatEnd"]->SetRect(KirbyRect);
	AniDepot["KirbyFloatEndBack"]->SetRect(KirbyRect);
}
void KirbyFloat::Input(DWORD)
{
	if (InputDevice[VK_RIGHT])
	{
		BackPosition = false;
		KeyPush = true;
	}
	else if (InputDevice[VK_LEFT])
	{
		BackPosition = true;
		KeyPush = true;
	}
	else
		KeyPush = false;

	if (InputDevice.OnClick('C'))
	{
		if(!(AniDepot["KirbyFloatStart"]->IsPlaying()) || !(AniDepot["KirbyFloatStartBack"]->IsPlaying()))
		{
			if(AniDepot["KirbyFloatUp"]->IsPlaying() || AniDepot["KirbyFloatUpBack"]->IsPlaying())
			{
				AniDepot["KirbyFloatUp"]->reset();
				AniDepot["KirbyFloatUpBack"]->reset();

				KirbyUp = true;
			}
			KirbyUp = true;
		}
	}
	if(InputDevice.OnClick('X'))
	{
		Landing = true;
	}
}
void KirbyFloat::Update(DWORD tick)
{
	// 캐릭터 좌표 업데이트

	if(update_dt > update_delay)
	{
		int count = update_dt / update_delay;
		for(int i = 0; i < count; i++)
		{
			Rect tmp;

			if(AniDepot["KirbyFloatStart"]->IsPlaying() || AniDepot["KirbyFloatStartBack"]->IsPlaying())
			{
				if(KeyPush)
				{
					if(!BackPosition && ::IntersectRect(&tmp, &Zone[RightZone], &KirbyRect))
					{
						MoveAcc += 1;

						if(MoveAcc >= 4)
							MoveAcc = 4;

						if(PlayerPos.x < 375)
							PlayerPos.x = 375;

						if(!(Wall == 2))
							PlayerPos.x += MoveAcc;
						if(!(Wall == 3))
							KirbyPos.y -= 5;

						if(KirbyPos.y - KirbyHeight/2 < 0)
							KirbyPos.y = KirbyHeight/2;
					}
					else if(BackPosition && ::IntersectRect(&tmp, &Zone[LeftZone], &KirbyRect) && !(PtInRect(&Zone[LeftZone], PlayerPos)))
					{
						MoveAcc += 1;

						if(MoveAcc >= 4)
							MoveAcc = 4;

						if(!(Wall == 1))
							PlayerPos.x -= MoveAcc;
						if(!(Wall == 3))
							KirbyPos.y -= 5;

						if(PlayerPos.x < 0)
							PlayerPos.x = 0;

						if(KirbyPos.y - KirbyHeight/2 < 0)
							KirbyPos.y = KirbyHeight/2;
					}
					else if(!BackPosition)
					{
						MoveAcc += 1;

						if(MoveAcc >= 4)
							MoveAcc = 4;

						if(!(Wall == 2))
							KirbyPos.x += MoveAcc;
						if(!(Wall == 3))
							KirbyPos.y -= 5;

						if(KirbyPos.x - KirbyWidth/2 < 0)
							KirbyPos.x = KirbyWidth/2;

						if(KirbyPos.y - KirbyHeight/2 < 0)
							KirbyPos.y = KirbyHeight/2;
					}
					else if(BackPosition)
					{
						MoveAcc += 1;

						if(MoveAcc >= 4)
							MoveAcc = 4;

						if(!(Wall == 1))
							KirbyPos.x -= MoveAcc;
						if(!(Wall == 3))
							KirbyPos.y -= 5;

						if(KirbyPos.x - KirbyWidth/2 < 0)
							KirbyPos.x = KirbyWidth/2;

						if(KirbyPos.y - KirbyHeight/2 < 0)
							KirbyPos.y = KirbyHeight/2;
					}
				}
				else
				{
					if(!BackPosition)
					{
						if(!(Wall == 3))
							KirbyPos.y -= 5;

						if(KirbyPos.y - KirbyHeight/2 < 0)
							KirbyPos.y = KirbyHeight/2;
					}
					else if(BackPosition)
					{
						if(!(Wall == 3))
							KirbyPos.y -= 5;

						if(KirbyPos.y - KirbyHeight/2 < 0)
							KirbyPos.y = KirbyHeight/2;
					}
				}
			}
			if(KeyPush && Float)
			{
				if(!BackPosition && ::IntersectRect(&tmp, &Zone[RightZone], &KirbyRect))
				{
					MoveAcc += 1;

					JumpAcc += 1;

					if(JumpAcc >= 3)
						JumpAcc = 3;

					if(MoveAcc >= 4)
						MoveAcc = 4;

					if(PlayerPos.x < 375)
						PlayerPos.x = 375;

					if(!(Wall == 2))
						PlayerPos.x += MoveAcc;

						KirbyPos.y += JumpAcc;

					if(KirbyPos.y - KirbyHeight/2 < 0)
						KirbyPos.y = KirbyHeight/2;
				}
				else if(BackPosition && ::IntersectRect(&tmp, &Zone[LeftZone], &KirbyRect) && !(PtInRect(&Zone[LeftZone], PlayerPos)))
				{
					MoveAcc += 1;

					JumpAcc += 1;

					if(JumpAcc >= 3)
						JumpAcc = 3;

					if(MoveAcc >= 4)
						MoveAcc = 4;

					if(!(Wall == 1))
						PlayerPos.x -= MoveAcc;

						KirbyPos.y += JumpAcc;

					if(PlayerPos.x < 0)
						PlayerPos.x = 0;

					if(KirbyPos.y - KirbyHeight/2 < 0)
						KirbyPos.y = KirbyHeight/2;
				}
				else if(!BackPosition)
				{
					MoveAcc += 1;

					JumpAcc += 1;

					if(JumpAcc >= 3)
						JumpAcc = 3;

					if(MoveAcc >= 4)
						MoveAcc = 4;

					if(!(Wall == 2))
						KirbyPos.x += MoveAcc;

						KirbyPos.y += JumpAcc;

					if(KirbyPos.x - KirbyWidth/2 < 0)
						KirbyPos.x = KirbyWidth/2;

					if(KirbyPos.y - KirbyHeight/2 < 0)
						KirbyPos.y = KirbyHeight/2;
				}
				else if(BackPosition)
				{
					MoveAcc += 1;

					JumpAcc += 1;

					if(JumpAcc >= 3)
						JumpAcc = 3;

					if(MoveAcc >= 4)
						MoveAcc = 4;

					if(!(Wall == 1))
						KirbyPos.x -= MoveAcc;
	
						KirbyPos.y += JumpAcc;

					if(KirbyPos.x - KirbyWidth/2 < 0)
						KirbyPos.x = KirbyWidth/2;

					if(KirbyPos.y - KirbyHeight/2 < 0)
						KirbyPos.y = KirbyHeight/2;
				}
			}
			else if(Float)
			{
				if(!BackPosition)
				{
					JumpAcc += 1;

					if(JumpAcc >= 3)
						JumpAcc = 3;


						KirbyPos.y += JumpAcc;

					if(KirbyPos.y - KirbyHeight/2 < 0)
						KirbyPos.y = KirbyHeight/2;
				}
				else if(BackPosition)
				{
					JumpAcc += 1;

					if(JumpAcc >= 3)
						JumpAcc = 3;


						KirbyPos.y += JumpAcc;

					if(KirbyPos.y - KirbyHeight/2 < 0)
						KirbyPos.y = KirbyHeight/2;
				}
			} // if(KeyPush && Float)
			if(KeyPush && KirbyUp)
			{
				if(!BackPosition && ::IntersectRect(&tmp, &Zone[RightZone], &KirbyRect))
				{
					MoveAcc += 1;

					if(MoveAcc >= 4)
						MoveAcc = 4;

					if(PlayerPos.x < 375)
						PlayerPos.x = 375;

					if(!(Wall == 2))
						PlayerPos.x += MoveAcc;
					if(!(Wall == 3))
						KirbyPos.y -= 5;

					if(KirbyPos.y - KirbyHeight/2 < 0)
						KirbyPos.y = KirbyHeight/2;
				}
				else if(BackPosition && ::IntersectRect(&tmp, &Zone[LeftZone], &KirbyRect) && !(PtInRect(&Zone[LeftZone], PlayerPos)))
				{
					MoveAcc += 1;

					if(MoveAcc >= 4)
						MoveAcc = 4;

					if(!(Wall == 1))
						PlayerPos.x -= MoveAcc;
					if(!(Wall == 3))
						KirbyPos.y -= 5;

					if(PlayerPos.x < 0)
						PlayerPos.x = 0;

					if(KirbyPos.y - KirbyHeight/2 < 0)
						KirbyPos.y = KirbyHeight/2;
				}
				else if(!BackPosition)
				{
					MoveAcc += 1;

					if(MoveAcc >= 4)
						MoveAcc = 4;

					if(!(Wall == 2))
						KirbyPos.x += MoveAcc;
					if(!(Wall == 3))
						KirbyPos.y -= 5;

					if(KirbyPos.x - KirbyWidth/2 < 0)
						KirbyPos.x = KirbyWidth/2;

					if(KirbyPos.y - KirbyHeight/2 < 0)
						KirbyPos.y = KirbyHeight/2;
				}
				else if(BackPosition)
				{
					MoveAcc += 1;

					if(MoveAcc >= 4)
						MoveAcc = 4;

					if(!(Wall == 1))
						KirbyPos.x -= MoveAcc;
					if(!(Wall == 3))
						KirbyPos.y -= 5;

					if(KirbyPos.x - KirbyWidth/2 < 0)
						KirbyPos.x = KirbyWidth/2;

					if(KirbyPos.y - KirbyHeight/2 < 0)
						KirbyPos.y = KirbyHeight/2;
				}
			}
			else if(KirbyUp)
			{
				if(!BackPosition)
				{
					if(!(Wall == 3))
						KirbyPos.y -= 5;

					if(KirbyPos.y - KirbyHeight/2 < 0)
							KirbyPos.y = KirbyHeight/2;
				}
				else if(BackPosition)
				{
					if(!(Wall == 3))
						KirbyPos.y -= 5;

					if(KirbyPos.y - KirbyHeight/2 < 0)
							KirbyPos.y = KirbyHeight/2;
				}
			}
		} // for(int i = 0; i < count; i++)
		update_dt = update_dt % update_delay;
	}
	update_dt += tick;

	if(!(AniDepot["KirbyFloatUp"]->IsPlaying()) || !(AniDepot["KirbyFloatUpBack"]->IsPlaying()) && KirbyUp)
	{
		KirbyUp = false;
		AniDepot["KirbyFloatUp"]->reset();
		AniDepot["KirbyFloatUpBack"]->reset();
		Float = true;
	}
	if(!(AniDepot["KirbyFloatStart"]->IsPlaying()) || !(AniDepot["KirbyFloatStartBack"]->IsPlaying()) && !KirbyUp && !Float)
	{
		Float = true;
	}
	if(Stand)
	{
		Float = false;
		Landing = true;
	}
	if(Float && KirbyUp)
	{
		Float = false;
	}

	if(Landing && !(AniDepot["KirbyFloatEnd"]->IsPlaying()) || !(AniDepot["KirbyFloatEndBack"]->IsPlaying()))
	{
		AniDepot["KirbyFloatStart"]->reset();
		AniDepot["KirbyFloatStartBack"]->reset();
		AniDepot["KirbyFloatUp"]->reset();
		AniDepot["KirbyFloatUpBack"]->reset();
		AniDepot["KirbyFloatEnd"]->reset();
		AniDepot["KirbyFloatEndBack"]->reset();

		Landing = false;
		Stand = false;
		KirbyUp = false;
		Float = false;

		pMachine->transition(_T("Idle"));
	}
	if (pMachine)
		dynamic_cast<Kirby*>(pMachine)->Setposition(PlayerPos, KirbyPos, BackPosition);

	AniDepot["KirbyFloatStart"]->SetRect(Rect(KirbyRect.left, KirbyRect.top - 5, KirbyRect.right+5, KirbyRect.bottom));
	AniDepot["KirbyFloatStartBack"]->SetRect(Rect(KirbyRect.left-5, KirbyRect.top - 5, KirbyRect.right, KirbyRect.bottom));

	AniDepot["KirbyFloat"]->SetRect(Rect(KirbyRect.left, KirbyRect.top - 5, KirbyRect.right+15, KirbyRect.bottom));
	AniDepot["KirbyFloatBack"]->SetRect(Rect(KirbyRect.left-15, KirbyRect.top - 5, KirbyRect.right, KirbyRect.bottom));

	AniDepot["KirbyFloatUp"]->SetRect(Rect(KirbyRect.left, KirbyRect.top - 5, KirbyRect.right+15, KirbyRect.bottom));
	AniDepot["KirbyFloatUpBack"]->SetRect(Rect(KirbyRect.left-15, KirbyRect.top - 5, KirbyRect.right, KirbyRect.bottom));

	AniDepot["KirbyFloatEnd"]->SetRect(Rect(KirbyRect.left, KirbyRect.top - 5, KirbyRect.right+5, KirbyRect.bottom));
	AniDepot["KirbyFloatEndBack"]->SetRect(Rect(KirbyRect.left-5, KirbyRect.top - 5, KirbyRect.right, KirbyRect.bottom));

	if(!Float && !KirbyUp && !Stand && !Landing)
	{
		AniDepot["KirbyFloatStart"]->Update(tick);
		AniDepot["KirbyFloatStartBack"]->Update(tick);
	}
	if(Float && !Landing)
	{
		AniDepot["KirbyFloat"]->Update(tick);
		AniDepot["KirbyFloatBack"]->Update(tick);
	}
	if(KirbyUp && !Landing)
	{
		AniDepot["KirbyFloatUp"]->Update(tick);
		AniDepot["KirbyFloatUpBack"]->Update(tick);
	}
	if(Landing)
	{
		AniDepot["KirbyFloatEnd"]->Update(tick);
		AniDepot["KirbyFloatEndBack"]->Update(tick);
	}
}
void KirbyFloat::Draw(HDC hdc)
{
	if(!Float && !KirbyUp && !Stand && !Landing)
	{
		if(!BackPosition)
		{
			AniDepot["KirbyFloatStart"]->Draw(hdc);
		}
		else
		{
			AniDepot["KirbyFloatStartBack"]->Draw(hdc);
		}
	}
	else if(Float && !Landing)
	{
		if(!BackPosition)
		{
			AniDepot["KirbyFloat"]->Draw(hdc);
		}
		else
		{
			AniDepot["KirbyFloatBack"]->Draw(hdc);
		}
	}
	else if(KirbyUp && !Landing)
	{
		if(!BackPosition)
		{
			AniDepot["KirbyFloatUp"]->Draw(hdc);
		}
		else
		{
			AniDepot["KirbyFloatUpBack"]->Draw(hdc);
		}
	}
	else if(Landing)
	{
		if(!BackPosition)
		{
			AniDepot["KirbyFloatEnd"]->Draw(hdc);
		}
		else
		{
			AniDepot["KirbyFloatEndBack"]->Draw(hdc);
		}
	}
}
void KirbyFloat::Leave()
{
}
void KirbyFloat::SetPosition(const Point& ppt, const Point& kpt, const bool& back)
{
	PlayerPos = ppt;

	KirbyPos = kpt;

	BackPosition = back;

	KirbyRect.left = KirbyPos.x - KirbyWidth/2;
	KirbyRect.top = KirbyPos.y - KirbyHeight/2;
	KirbyRect.right = KirbyWidth + KirbyRect.left;
	KirbyRect.bottom = KirbyHeight + KirbyRect.top;
}
void KirbyFloat::GetState(const BYTE& wl)
{
	Wall = wl;
}
void KirbyFloat::GetStand(const bool& st)
{
	Stand = st;
}
///////////////////////////////////////////////////////////////
//// 흡수하는 동작
///////////////////////////////////////////////////////////////
//KirbyDrain::KirbyDrain()
//: update_dt(0), update_delay(20), MoveAcc(4)
//, Stand(false), Wall(0), Landing(false)
//,JumpAcc(0), KeyPush(false), Float(false), KirbyUp(false)
//{
//	for(int i = 0; i < ZoneMax; i++)
//	{
//		Zone[i] = GameGlobal.rcClient;
//	}
//	Zone[LeftZone].right = Zone[LeftZone].left + 385;
//	Zone[RightZone].left = Zone[RightZone].right - 385;
//}
//KirbyDrain::~KirbyDrain()
//{
//}
//void KirbyDrain::Enter()
//{
//	AniDepot["KirbyDrainStart"]->SetRect(KirbyRect);
//	AniDepot["KirbyDrainStartBack"]->SetRect(KirbyRect);
//
//	AniDepot["KirbyDrain"]->SetRect(KirbyRect);
//	AniDepot["KirbyDrainBack"]->SetRect(KirbyRect);
//
//	AniDepot["KirbyDrainUp"]->SetRect(KirbyRect);
//	AniDepot["KirbyDrainUpBack"]->SetRect(KirbyRect);
//
//	AniDepot["KirbyDrainEnd"]->SetRect(KirbyRect);
//	AniDepot["KirbyDrainEndBack"]->SetRect(KirbyRect);
//}
//void KirbyDrain::Input(DWORD)
//{
//	if (InputDevice[VK_RIGHT])
//	{
//		BackPosition = false;
//		KeyPush = true;
//	}
//	else if (InputDevice[VK_LEFT])
//	{
//		BackPosition = true;
//		KeyPush = true;
//	}
//	else
//		KeyPush = false;
//
//	if (InputDevice.OnClick('C'))
//	{
//		if(!(AniDepot["KirbyDrainStart"]->IsPlaying()) || !(AniDepot["KirbyDrainStartBack"]->IsPlaying()))
//		{
//			if(AniDepot["KirbyDrainUp"]->IsPlaying() || AniDepot["KirbyDrainUpBack"]->IsPlaying())
//			{
//				AniDepot["KirbyDrainUp"]->reset();
//				AniDepot["KirbyDrainUpBack"]->reset();
//
//				KirbyUp = true;
//			}
//			KirbyUp = true;
//		}
//	}
//	if(InputDevice.OnClick('X'))
//	{
//		Landing = true;
//	}
//}
//void KirbyDrain::Update(DWORD tick)
//{
//	// 캐릭터 좌표 업데이트
//
//	if(update_dt > update_delay)
//	{
//		int count = update_dt / update_delay;
//		for(int i = 0; i < count; i++)
//		{
//			Rect tmp;
//
//			if(AniDepot["KirbyDrainStart"]->IsPlaying() || AniDepot["KirbyDrainStartBack"]->IsPlaying())
//			{
//				if(KeyPush)
//				{
//					if(!BackPosition && ::IntersectRect(&tmp, &Zone[RightZone], &KirbyRect))
//					{
//						MoveAcc += 1;
//
//						if(MoveAcc >= 4)
//							MoveAcc = 4;
//
//						if(PlayerPos.x < 375)
//							PlayerPos.x = 375;
//
//						if(!(Wall == 2))
//							PlayerPos.x += MoveAcc;
//						if(!(Wall == 3))
//							KirbyPos.y -= 5;
//
//						if(KirbyPos.y - KirbyHeight/2 < 0)
//							KirbyPos.y = KirbyHeight/2;
//					}
//					else if(BackPosition && ::IntersectRect(&tmp, &Zone[LeftZone], &KirbyRect) && !(PtInRect(&Zone[LeftZone], PlayerPos)))
//					{
//						MoveAcc += 1;
//
//						if(MoveAcc >= 4)
//							MoveAcc = 4;
//
//						if(!(Wall == 1))
//							PlayerPos.x -= MoveAcc;
//						if(!(Wall == 3))
//							KirbyPos.y -= 5;
//
//						if(PlayerPos.x < 0)
//							PlayerPos.x = 0;
//
//						if(KirbyPos.y - KirbyHeight/2 < 0)
//							KirbyPos.y = KirbyHeight/2;
//					}
//					else if(!BackPosition)
//					{
//						MoveAcc += 1;
//
//						if(MoveAcc >= 4)
//							MoveAcc = 4;
//
//						if(!(Wall == 2))
//							KirbyPos.x += MoveAcc;
//						if(!(Wall == 3))
//							KirbyPos.y -= 5;
//
//						if(KirbyPos.x - KirbyWidth/2 < 0)
//							KirbyPos.x = KirbyWidth/2;
//
//						if(KirbyPos.y - KirbyHeight/2 < 0)
//							KirbyPos.y = KirbyHeight/2;
//					}
//					else if(BackPosition)
//					{
//						MoveAcc += 1;
//
//						if(MoveAcc >= 4)
//							MoveAcc = 4;
//
//						if(!(Wall == 1))
//							KirbyPos.x -= MoveAcc;
//						if(!(Wall == 3))
//							KirbyPos.y -= 5;
//
//						if(KirbyPos.x - KirbyWidth/2 < 0)
//							KirbyPos.x = KirbyWidth/2;
//
//						if(KirbyPos.y - KirbyHeight/2 < 0)
//							KirbyPos.y = KirbyHeight/2;
//					}
//				}
//				else
//				{
//					if(!BackPosition)
//					{
//						if(!(Wall == 3))
//							KirbyPos.y -= 5;
//
//						if(KirbyPos.y - KirbyHeight/2 < 0)
//							KirbyPos.y = KirbyHeight/2;
//					}
//					else if(BackPosition)
//					{
//						if(!(Wall == 3))
//							KirbyPos.y -= 5;
//
//						if(KirbyPos.y - KirbyHeight/2 < 0)
//							KirbyPos.y = KirbyHeight/2;
//					}
//				}
//			}
//			if(KeyPush && Float)
//			{
//				if(!BackPosition && ::IntersectRect(&tmp, &Zone[RightZone], &KirbyRect))
//				{
//					MoveAcc += 1;
//
//					JumpAcc += 1;
//
//					if(JumpAcc >= 3)
//						JumpAcc = 3;
//
//					if(MoveAcc >= 4)
//						MoveAcc = 4;
//
//					if(PlayerPos.x < 375)
//						PlayerPos.x = 375;
//
//					if(!(Wall == 2))
//						PlayerPos.x += MoveAcc;
//
//						KirbyPos.y += JumpAcc;
//
//					if(KirbyPos.y - KirbyHeight/2 < 0)
//						KirbyPos.y = KirbyHeight/2;
//				}
//				else if(BackPosition && ::IntersectRect(&tmp, &Zone[LeftZone], &KirbyRect) && !(PtInRect(&Zone[LeftZone], PlayerPos)))
//				{
//					MoveAcc += 1;
//
//					JumpAcc += 1;
//
//					if(JumpAcc >= 3)
//						JumpAcc = 3;
//
//					if(MoveAcc >= 4)
//						MoveAcc = 4;
//
//					if(!(Wall == 1))
//						PlayerPos.x -= MoveAcc;
//
//						KirbyPos.y += JumpAcc;
//
//					if(PlayerPos.x < 0)
//						PlayerPos.x = 0;
//
//					if(KirbyPos.y - KirbyHeight/2 < 0)
//						KirbyPos.y = KirbyHeight/2;
//				}
//				else if(!BackPosition)
//				{
//					MoveAcc += 1;
//
//					JumpAcc += 1;
//
//					if(JumpAcc >= 3)
//						JumpAcc = 3;
//
//					if(MoveAcc >= 4)
//						MoveAcc = 4;
//
//					if(!(Wall == 2))
//						KirbyPos.x += MoveAcc;
//
//						KirbyPos.y += JumpAcc;
//
//					if(KirbyPos.x - KirbyWidth/2 < 0)
//						KirbyPos.x = KirbyWidth/2;
//
//					if(KirbyPos.y - KirbyHeight/2 < 0)
//						KirbyPos.y = KirbyHeight/2;
//				}
//				else if(BackPosition)
//				{
//					MoveAcc += 1;
//
//					JumpAcc += 1;
//
//					if(JumpAcc >= 3)
//						JumpAcc = 3;
//
//					if(MoveAcc >= 4)
//						MoveAcc = 4;
//
//					if(!(Wall == 1))
//						KirbyPos.x -= MoveAcc;
//	
//						KirbyPos.y += JumpAcc;
//
//					if(KirbyPos.x - KirbyWidth/2 < 0)
//						KirbyPos.x = KirbyWidth/2;
//
//					if(KirbyPos.y - KirbyHeight/2 < 0)
//						KirbyPos.y = KirbyHeight/2;
//				}
//			}
//			else if(Float)
//			{
//				if(!BackPosition)
//				{
//					JumpAcc += 1;
//
//					if(JumpAcc >= 3)
//						JumpAcc = 3;
//
//
//						KirbyPos.y += JumpAcc;
//
//					if(KirbyPos.y - KirbyHeight/2 < 0)
//						KirbyPos.y = KirbyHeight/2;
//				}
//				else if(BackPosition)
//				{
//					JumpAcc += 1;
//
//					if(JumpAcc >= 3)
//						JumpAcc = 3;
//
//
//						KirbyPos.y += JumpAcc;
//
//					if(KirbyPos.y - KirbyHeight/2 < 0)
//						KirbyPos.y = KirbyHeight/2;
//				}
//			} // if(KeyPush && Float)
//			if(KeyPush && KirbyUp)
//			{
//				if(!BackPosition && ::IntersectRect(&tmp, &Zone[RightZone], &KirbyRect))
//				{
//					MoveAcc += 1;
//
//					if(MoveAcc >= 4)
//						MoveAcc = 4;
//
//					if(PlayerPos.x < 375)
//						PlayerPos.x = 375;
//
//					if(!(Wall == 2))
//						PlayerPos.x += MoveAcc;
//					if(!(Wall == 3))
//						KirbyPos.y -= 5;
//
//					if(KirbyPos.y - KirbyHeight/2 < 0)
//						KirbyPos.y = KirbyHeight/2;
//				}
//				else if(BackPosition && ::IntersectRect(&tmp, &Zone[LeftZone], &KirbyRect) && !(PtInRect(&Zone[LeftZone], PlayerPos)))
//				{
//					MoveAcc += 1;
//
//					if(MoveAcc >= 4)
//						MoveAcc = 4;
//
//					if(!(Wall == 1))
//						PlayerPos.x -= MoveAcc;
//					if(!(Wall == 3))
//						KirbyPos.y -= 5;
//
//					if(PlayerPos.x < 0)
//						PlayerPos.x = 0;
//
//					if(KirbyPos.y - KirbyHeight/2 < 0)
//						KirbyPos.y = KirbyHeight/2;
//				}
//				else if(!BackPosition)
//				{
//					MoveAcc += 1;
//
//					if(MoveAcc >= 4)
//						MoveAcc = 4;
//
//					if(!(Wall == 2))
//						KirbyPos.x += MoveAcc;
//					if(!(Wall == 3))
//						KirbyPos.y -= 5;
//
//					if(KirbyPos.x - KirbyWidth/2 < 0)
//						KirbyPos.x = KirbyWidth/2;
//
//					if(KirbyPos.y - KirbyHeight/2 < 0)
//						KirbyPos.y = KirbyHeight/2;
//				}
//				else if(BackPosition)
//				{
//					MoveAcc += 1;
//
//					if(MoveAcc >= 4)
//						MoveAcc = 4;
//
//					if(!(Wall == 1))
//						KirbyPos.x -= MoveAcc;
//					if(!(Wall == 3))
//						KirbyPos.y -= 5;
//
//					if(KirbyPos.x - KirbyWidth/2 < 0)
//						KirbyPos.x = KirbyWidth/2;
//
//					if(KirbyPos.y - KirbyHeight/2 < 0)
//						KirbyPos.y = KirbyHeight/2;
//				}
//			}
//			else if(KirbyUp)
//			{
//				if(!BackPosition)
//				{
//					if(!(Wall == 3))
//						KirbyPos.y -= 5;
//
//					if(KirbyPos.y - KirbyHeight/2 < 0)
//							KirbyPos.y = KirbyHeight/2;
//				}
//				else if(BackPosition)
//				{
//					if(!(Wall == 3))
//						KirbyPos.y -= 5;
//
//					if(KirbyPos.y - KirbyHeight/2 < 0)
//							KirbyPos.y = KirbyHeight/2;
//				}
//			}
//		} // for(int i = 0; i < count; i++)
//		update_dt = update_dt % update_delay;
//	}
//	update_dt += tick;
//
//	if(!(AniDepot["KirbyDrainUp"]->IsPlaying()) || !(AniDepot["KirbyDrainUpBack"]->IsPlaying()) && KirbyUp)
//	{
//		KirbyUp = false;
//		AniDepot["KirbyDrainUp"]->reset();
//		AniDepot["KirbyDrainUpBack"]->reset();
//		Float = true;
//	}
//	if(!(AniDepot["KirbyDrainStart"]->IsPlaying()) || !(AniDepot["KirbyDrainStartBack"]->IsPlaying()) && !KirbyUp && !Float)
//	{
//		Float = true;
//	}
//	if(Stand)
//	{
//		Float = false;
//		Landing = true;
//	}
//	if(Float && KirbyUp)
//	{
//		Float = false;
//	}
//
//	if(Landing && !(AniDepot["KirbyDrainEnd"]->IsPlaying()) || !(AniDepot["KirbyDrainEndBack"]->IsPlaying()))
//	{
//		AniDepot["KirbyDrainStart"]->reset();
//		AniDepot["KirbyDrainStartBack"]->reset();
//		AniDepot["KirbyDrainUp"]->reset();
//		AniDepot["KirbyDrainUpBack"]->reset();
//		AniDepot["KirbyDrainEnd"]->reset();
//		AniDepot["KirbyDrainEndBack"]->reset();
//
//		Landing = false;
//		Stand = false;
//		KirbyUp = false;
//		Float = false;
//
//		pMachine->transition(_T("Idle"));
//	}
//	if (pMachine)
//		dynamic_cast<Kirby*>(pMachine)->Setposition(PlayerPos, KirbyPos, BackPosition);
//
//	AniDepot["KirbyDrainStart"]->SetRect(Rect(KirbyRect.left, KirbyRect.top - 5, KirbyRect.right+5, KirbyRect.bottom));
//	AniDepot["KirbyDrainStartBack"]->SetRect(Rect(KirbyRect.left-5, KirbyRect.top - 5, KirbyRect.right, KirbyRect.bottom));
//
//	AniDepot["KirbyDrain"]->SetRect(Rect(KirbyRect.left, KirbyRect.top - 5, KirbyRect.right+15, KirbyRect.bottom));
//	AniDepot["KirbyDrainBack"]->SetRect(Rect(KirbyRect.left-15, KirbyRect.top - 5, KirbyRect.right, KirbyRect.bottom));
//
//	AniDepot["KirbyDrainUp"]->SetRect(Rect(KirbyRect.left, KirbyRect.top - 5, KirbyRect.right+15, KirbyRect.bottom));
//	AniDepot["KirbyDrainUpBack"]->SetRect(Rect(KirbyRect.left-15, KirbyRect.top - 5, KirbyRect.right, KirbyRect.bottom));
//
//	AniDepot["KirbyDrainEnd"]->SetRect(Rect(KirbyRect.left, KirbyRect.top - 5, KirbyRect.right+5, KirbyRect.bottom));
//	AniDepot["KirbyDrainEndBack"]->SetRect(Rect(KirbyRect.left-5, KirbyRect.top - 5, KirbyRect.right, KirbyRect.bottom));
//
//	if(!Float && !KirbyUp && !Stand && !Landing)
//	{
//		AniDepot["KirbyDrainStart"]->Update(tick);
//		AniDepot["KirbyDrainStartBack"]->Update(tick);
//	}
//	if(Float && !Landing)
//	{
//		AniDepot["KirbyDrain"]->Update(tick);
//		AniDepot["KirbyDrainBack"]->Update(tick);
//	}
//	if(KirbyUp && !Landing)
//	{
//		AniDepot["KirbyDrainUp"]->Update(tick);
//		AniDepot["KirbyDrainUpBack"]->Update(tick);
//	}
//	if(Landing)
//	{
//		AniDepot["KirbyDrainEnd"]->Update(tick);
//		AniDepot["KirbyDrainEndBack"]->Update(tick);
//	}
//}
//void KirbyDrain::Draw(HDC hdc)
//{
//	if(!Float && !KirbyUp && !Stand && !Landing)
//	{
//		if(!BackPosition)
//		{
//			AniDepot["KirbyDrainStart"]->Draw(hdc);
//		}
//		else
//		{
//			AniDepot["KirbyDrainStartBack"]->Draw(hdc);
//		}
//	}
//	else if(Float && !Landing)
//	{
//		if(!BackPosition)
//		{
//			AniDepot["KirbyDrain"]->Draw(hdc);
//		}
//		else
//		{
//			AniDepot["KirbyDrainBack"]->Draw(hdc);
//		}
//	}
//	else if(KirbyUp && !Landing)
//	{
//		if(!BackPosition)
//		{
//			AniDepot["KirbyDrainUp"]->Draw(hdc);
//		}
//		else
//		{
//			AniDepot["KirbyDrainUpBack"]->Draw(hdc);
//		}
//	}
//	else if(Landing)
//	{
//		if(!BackPosition)
//		{
//			AniDepot["KirbyDrainEnd"]->Draw(hdc);
//		}
//		else
//		{
//			AniDepot["KirbyDrainEndBack"]->Draw(hdc);
//		}
//	}
//}
//void KirbyDrain::Leave()
//{
//}
//void KirbyDrain::SetPosition(const Point& ppt, const Point& kpt, const bool& back)
//{
//	PlayerPos = ppt;
//
//	KirbyPos = kpt;
//
//	BackPosition = back;
//
//	KirbyRect.left = KirbyPos.x - KirbyWidth/2;
//	KirbyRect.top = KirbyPos.y - KirbyHeight/2;
//	KirbyRect.right = KirbyWidth + KirbyRect.left;
//	KirbyRect.bottom = KirbyHeight + KirbyRect.top;
//}
//void KirbyDrain::GetState(const BYTE& wl)
//{
//	Wall = wl;
//}
//void KirbyDrain::GetStand(const bool& st)
//{
//	Stand = st;
//}
