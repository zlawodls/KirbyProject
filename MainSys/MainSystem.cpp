#include "MainSystem.h"
Mainsys::Mainsys()
{
	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
	dwStyle = dwStyle & ~WS_THICKFRAME; 
	dwStyle = dwStyle & ~WS_MINIMIZEBOX;
	dwStyle = dwStyle & ~WS_MAXIMIZEBOX;

	SetWindowStyle(dwStyle);
	SetWindowSize(800, 600);	
}
void Mainsys::Enter()
{
	Stage1_1.Load(hMainWnd);
	KirbyBase.Load(rcClient);
}
void Mainsys::Input(DWORD tick)
{
	KirbyBase.Input(tick);
}
void Mainsys::Update(DWORD tick)
{
	KirbyBase.Update(tick);
	Stage1_1.Update(tick);
	Stage1_1.SetPlayerPos(KirbyBase.RetrunKirbyPos());
}
void Mainsys::Draw(DWORD)
{
	backbuffer << RGB(255,255,255);

	Stage1_1.Draw(backbuffer);
	KirbyBase.Draw(backbuffer);

	backbuffer.Draw();
}