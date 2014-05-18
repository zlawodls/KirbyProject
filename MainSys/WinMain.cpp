#include "MainSystem.h"

INT WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE, LPTSTR, INT)
{
	Mainsys sys;

	sys.Setup(hInst);

	return sys.Run();
}