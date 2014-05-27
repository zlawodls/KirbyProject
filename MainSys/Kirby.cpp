#include "Kirby.h"

Kirby::Kirby()
:KirbyPos(200, 200), PlayerPos(400, 200), update_dt(0),
state(0), BackPosition(false), MoveAcc(0), MoveAccCount(0)
{
}
Kirby::~Kirby()
{
}
void Kirby::Load(const Rect& rc)
{
	ClientRect = rc;
	KirbyStd.Load(_T("Kirby_base.bmp"), Rect(6, 8, 26, 27));
	KirbyStd.SetTransparentColor(RGB(207, 176, 255));
	KirbyStdBack.Load(_T("Kirby_base_back.bmp"), Rect(760 - 26, 8, 760 - 6, 27));
	KirbyStdBack.SetTransparentColor(RGB(207, 176, 255));
	Rect tmp;
	tmp.left = KirbyPos.x - KirbyWidth/2;
	tmp.top = KirbyPos.y - KitbyHeight/2;
	tmp.right = KirbyWidth + tmp.left;
	tmp.bottom = KitbyHeight + tmp.top;
	KirbyStd.SetDrawRect(tmp);
	KirbyStdBack.SetDrawRect(tmp);

	Image* Anitmp = new Image;
	Anitmp->Load(_T("Kirby_base.bmp"), Rect(30, 8, 50, 27));
	Anitmp->SetTransparentColor(RGB(207, 176, 255));
	Anitmp->SetDrawRect(tmp);
	KirbyStdEye.AddImage(Anitmp);
	KirbyStdEye.SetUpdateDelay(100);
	Anitmp = new Image;
	Anitmp->Load(_T("Kirby_base_back.bmp"), Rect(760 - 50, 8, 760 - 30, 27));
	Anitmp->SetTransparentColor(RGB(207, 176, 255));
	Anitmp->SetDrawRect(tmp);
	KirbyStdEyeBack.AddImage(Anitmp);
	KirbyStdEyeBack.SetUpdateDelay(100);

	for(int i = 0; i < 10; i++)
	{
		Image* Anitmp = new Image;
		Anitmp->Load(_T("Kirby_base.bmp"), Rect(5+ (i * 23), 58, 29 + (i * 23), 77));
		Anitmp->SetTransparentColor(RGB(207, 176, 255));
		Anitmp->SetDrawRect(tmp);
		KirbyMove.AddImage(Anitmp);
		Anitmp = new Image;
		Anitmp->Load(_T("Kirby_base_back.bmp"), Rect(760 - (29+ (i * 23)), 58,760 - (5 + (i * 23)), 77));
		Anitmp->SetTransparentColor(RGB(207, 176, 255));
		Anitmp->SetDrawRect(tmp);
		KirbyMoveBack.AddImage(Anitmp);
	}
	KirbyMove.SetUpdateDelay(70);
	KirbyMoveBack.SetUpdateDelay(70);

	for(int i = 0; i < 4; i++)
	{
		Zone[i] = ClientRect;
	}
	Zone[LeftZone].right = Zone[LeftZone].left + 400;
	Zone[RightZone].left = Zone[RightZone].right - 400;
}
void Kirby::Input(DWORD tick)
{
	if(update_dt > 20)
	{
		int count = update_dt / 20;

		for(int i = 0; i < count; i++)
		{
			Rect tmp;
			if(InputDevice[VK_RIGHT] && ::IntersectRect(&tmp, &Zone[RightZone], &KirbyStd.GetDrawRect()))
			{
				if(BackPosition)
					MoveAcc = 0;
				MoveAcc += 1;
				if(MoveAcc >= 4)
					MoveAcc = 4;

				PlayerPos.x += MoveAcc;
				state = MoveState;
				BackPosition = false;
			}
			else if(InputDevice[VK_LEFT] && ::IntersectRect(&tmp, &Zone[LeftZone], &KirbyStd.GetDrawRect())
				    && !(PtInRect(&Zone[LeftZone], PlayerPos)))
			{
				if(!BackPosition)
					MoveAcc = 0;
				MoveAcc += 1;
				if(MoveAcc >= 4)
					MoveAcc = 4;

				PlayerPos.x -= MoveAcc;
				if(PlayerPos.x < 0)
					PlayerPos.x = 0;
				state = MoveState;
				BackPosition = true;
			}
			else if(InputDevice[VK_LEFT])
			{
				if(!BackPosition)
					MoveAcc = 0;
				MoveAcc += 1;
				if(MoveAcc >= 4)
					MoveAcc = 4;

				KirbyPos.x -= MoveAcc;
				if(KirbyPos.x - KirbyWidth/2 < 0)
					KirbyPos.x = KirbyWidth/2;
				state = MoveState;
				BackPosition = true;
			}
			else if(InputDevice[VK_RIGHT])
			{
				if(BackPosition)
					MoveAcc = 0;
				MoveAcc += 1;
				if(MoveAcc >= 4)
					MoveAcc = 4;

				KirbyPos.x += MoveAcc;
				state = MoveState;
				BackPosition = false;
			}
			else if(MoveAcc > 0 && state == MoveState)
			{
				if(MoveAccCount > 2)
				{
					if(!(::IntersectRect(&tmp, &Zone[LeftZone], &KirbyStd.GetDrawRect())))
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
					MoveAccCount = 0;
				}
				MoveAccCount += 1;
			}
			else if(MoveAcc == 0)
			{
				state = StdState;
				KirbyMove.SetPlaying(true);
			}
		}

		update_dt = update_dt % 20;
	}
	
	update_dt += tick;
}
void Kirby::Draw(HDC hdc)
{
	if(state == StdState)
	{
		if(!BackPosition)
		{
			if(!(KirbyStdEye.IsPlaying()))
				KirbyStd.Draw(hdc);
			else
				KirbyStdEye.Draw(hdc);
		}
		else
		{
			if(!(KirbyStdEyeBack.IsPlaying()))
				KirbyStdBack.Draw(hdc);
			else
				KirbyStdEyeBack.Draw(hdc);
		}
	}
	if(state == MoveState)
	{
		if(!BackPosition)
		{
			KirbyMove.Draw(hdc);
		}
		else
		{
			KirbyMoveBack.Draw(hdc);
		}
	}
}
void Kirby::Update(DWORD tick)
{
	Rect tmp;
	tmp.left = KirbyPos.x - KirbyWidth/2;
	tmp.top = KirbyPos.y - KitbyHeight/2;
	tmp.right = KirbyWidth + tmp.left;
	tmp.bottom = KitbyHeight + tmp.top;

	KirbyStd.SetDrawRect(tmp);
	KirbyStdEye.SetDrawRect(tmp);
	KirbyMove.SetDrawRect(Rect(tmp.left, tmp.top, tmp.right+5, tmp.bottom));

	KirbyStdBack.SetDrawRect(tmp);
	KirbyStdEyeBack.SetDrawRect(tmp);
	KirbyMoveBack.SetDrawRect(Rect(tmp.left, tmp.top, tmp.right+5, tmp.bottom));

	KirbyMove.Update(tick);
	KirbyMove.CheckIndex();
	KirbyStdEye.Update(tick);
	KirbyStdEye.ResetAni(tick, 5000);

	KirbyMoveBack.Update(tick);
	KirbyMoveBack.CheckIndex();
	KirbyStdEyeBack.Update(tick);
	KirbyStdEyeBack.ResetAni(tick, 5000);
}

Point Kirby::RetrunKirbyPos() const
{
	return PlayerPos;
}