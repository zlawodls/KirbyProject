#include "Kirby.h"

Kirby::Kirby()
:KirbyPos(200, 200), PlayerPos(375, 200), update_dt(0),
state(0), BackPosition(false), MoveAcc(0), MoveAccCount(0)
{
}
Kirby::~Kirby()
{
}
void Kirby::Load(const Rect& rc)
{
	ClientRect = rc;
	KirbyRect.left = KirbyPos.x- KirbyWidth/2;
	KirbyRect.top = KirbyPos.y - KirbyHeight/2;
	KirbyRect.right = KirbyRect.left + KirbyWidth;
	KirbyRect.bottom = KirbyRect.top + KirbyHeight;

	KirbyState.Load(ClientRect, KirbyPos);

	for(int i = 0; i < 4; i++)
	{
		Zone[i] = ClientRect;
	}
	Zone[LeftZone].right = Zone[LeftZone].left + 385;
	Zone[RightZone].left = Zone[RightZone].right - 385;
}
void Kirby::Input(DWORD tick)
{
	if(update_dt > 20)
	{
		int count = update_dt / 20;

		for(int i = 0; i < count; i++)
		{
			Rect tmp;
			if(InputDevice[VK_RIGHT] && ::IntersectRect(&tmp, &Zone[RightZone], &KirbyRect))
			{
				if(BackPosition && MoveAcc >= 5)
				{
					MoveAcc = -MoveAcc*2;
					state = BreakState;
				}
				MoveAcc += 1;
				if(InputDevice[VK_SHIFT])
				{
					MoveAcc += 1;
					if(MoveAcc >= 8)
						MoveAcc = 8;

					if(MoveAcc > 0)
						state = RunState;
				}
				else
				{
					if(MoveAcc >= 4)
						MoveAcc = 4;
					if(MoveAcc > 0)
						state = MoveState;
				}
				if(PlayerPos.x < 390)
					PlayerPos.x = 390;

				PlayerPos.x += MoveAcc;
				BackPosition = false;
			}
			else if(InputDevice[VK_LEFT] && ::IntersectRect(&tmp, &Zone[LeftZone], &KirbyRect)
				&& !(PtInRect(&Zone[LeftZone], PlayerPos)))
			{
				if(!BackPosition && MoveAcc >= 5)
				{
					MoveAcc = -MoveAcc*2;
					state = BreakState;
				}
				MoveAcc += 1;

				if(InputDevice[VK_SHIFT])
				{
					MoveAcc += 1;
					if(MoveAcc >= 8)
						MoveAcc = 8;

					if(MoveAcc > 0)
						state = RunState;
				}
				else
				{
					if(MoveAcc >= 4)
						MoveAcc = 4;

					if(MoveAcc > 0)
						state = MoveState;
				}

				PlayerPos.x -= MoveAcc;
				if(PlayerPos.x < 0)
					PlayerPos.x = 0;

				BackPosition = true;
			}
			else if(InputDevice[VK_LEFT])
			{
				if(!BackPosition && MoveAcc >= 5)
				{
					MoveAcc = -MoveAcc*2;
					state = BreakState;
				}
				MoveAcc += 1;

				if(InputDevice[VK_SHIFT])
				{
					MoveAcc += 1;
					if(MoveAcc >= 8)
						MoveAcc = 8;

					if(MoveAcc > 0)
						state = RunState;
				}
				else
				{
					if(MoveAcc >= 4)
						MoveAcc = 4;

					if(MoveAcc > 0)
						state = MoveState;
				}

				KirbyPos.x -= MoveAcc;
				if(KirbyPos.x - KirbyWidth/2 < 0)
					KirbyPos.x = KirbyWidth/2;
				BackPosition = true;
			}
			else if(InputDevice[VK_RIGHT])
			{
				if(BackPosition && MoveAcc >= 5)
				{
					MoveAcc = -MoveAcc*2;
					state = BreakState;
				}
				MoveAcc += 1;

				if(InputDevice[VK_SHIFT])
				{
					MoveAcc += 1;
					if(MoveAcc >= 8)
						MoveAcc = 8;

					if(MoveAcc > 0)
						state = RunState;
				}
				else
				{
					if(MoveAcc >= 4)
						MoveAcc = 4;

					if(MoveAcc > 0)
						state = MoveState;
				}

				KirbyPos.x += MoveAcc;

				if(KirbyPos.x - KirbyWidth/2 < 0)
					KirbyPos.x = KirbyWidth/2;
				BackPosition = false;
			}
			else if(MoveAcc > 0 && state == MoveState || state == RunState || state == BreakState)
			{
				if(MoveAccCount > 2)
				{
					if(!(::IntersectRect(&tmp, &Zone[RightZone], &KirbyRect)))
					{
						if(BackPosition)
						{
							if(MoveAcc < 4)
								state = MoveState;
							MoveAcc -= 1;
							KirbyPos.x -= MoveAcc;
							if(KirbyPos.x - KirbyWidth/2 < 0)
								KirbyPos.x = KirbyWidth/2;
						}
						else
						{
							if(MoveAcc < 4)
								state = MoveState;
							MoveAcc -= 1;
							KirbyPos.x += MoveAcc;
						}
					}
					else
					{
						if(BackPosition)
						{
							if(MoveAcc < 4)
								state = MoveState;
							MoveAcc -= 1;
							PlayerPos.x -= MoveAcc;
							if(PlayerPos.x < 0)
								PlayerPos.x = 0;
						}
						else
						{
							if(MoveAcc < 4)
								state = MoveState;
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
				KirbyState.StateReset();
			}
			if(!(InputDevice[VK_RIGHT]) && !(InputDevice[VK_LEFT]) && state == RunState && MoveAcc == 0)
			{
				state = StdState;
				KirbyState.StateReset();
			}
			if(!(InputDevice[VK_RIGHT]) && !(InputDevice[VK_LEFT]) && state == MoveState && MoveAcc != 0)
			{
				if(MoveAcc < 0)
					MoveAcc += 1;
				else
					MoveAcc -= 1;
			}
		}

		update_dt = update_dt % 20;
	}

	update_dt += tick;
}
void Kirby::Draw(HDC hdc)
{
	KirbyState.Draw(hdc, state, BackPosition);

	::MoveToEx(hdc, KirbyRect.left, KirbyRect.top, NULL);
	::LineTo(hdc, KirbyRect.left, KirbyRect.bottom);
	::LineTo(hdc, KirbyRect.right, KirbyRect.bottom);
	::LineTo(hdc, KirbyRect.right, KirbyRect.top);
	::LineTo(hdc, KirbyRect.left, KirbyRect.top);
}
void Kirby::Update(DWORD tick)
{
	Rect tmp;
	tmp.left = KirbyPos.x - KirbyWidth/2;
	tmp.top = KirbyPos.y - KirbyHeight/2;
	tmp.right = KirbyWidth + tmp.left;
	tmp.bottom = KirbyHeight + tmp.top;

	KirbyRect.left = KirbyPos.x- KirbyWidth/2;
	KirbyRect.top = KirbyPos.y - KirbyHeight/2;
	KirbyRect.right = KirbyRect.left + KirbyWidth;
	KirbyRect.bottom = KirbyRect.top + KirbyHeight;

	KirbyState.Update(tick, tmp);
}

Point Kirby::RetrunKirbyPos() const
{
	return PlayerPos;
}