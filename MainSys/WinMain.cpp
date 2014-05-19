#include "MainSystem.h"

INT WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE, LPTSTR, INT)
{
	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
	dwStyle = dwStyle & ~WS_THICKFRAME; 
	dwStyle = dwStyle & ~WS_MINIMIZEBOX;
	dwStyle = dwStyle & ~WS_MAXIMIZEBOX;

	Mainsys sys;
	sys.Start(dwStyle);

	sys.Setup(hInst);

	return sys.Run();
}