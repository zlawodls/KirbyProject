#include "MainSystem.h"
Mainsys::Mainsys()
{

	
}
void Mainsys::Start(DWORD dwStyle)
{
	SetWindowStyle(dwStyle);
}
void Mainsys::Input(DWORD)
{
}
void Mainsys::Update(DWORD tick)
{
	
}
void Mainsys::Draw(DWORD)
{
	backbuffer << RGB(255,255,255);

	Stage1_1.Load(hMainWnd);
	Stage1_1.Draw(backbuffer);

	backbuffer.Draw();
}