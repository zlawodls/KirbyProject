#include "BlockManager.h"

Block1Manager::Block1Manager()
{
}
Block1Manager::~Block1Manager()
{
	std::list<Block*>::iterator it;
	for (it = depot.begin(); it != depot.end(); )
	{
		delete *it;
		it = depot.erase(it);
	}
}
void Block1Manager::Input(DWORD tick)
{
	std::list<Block*>::iterator it;
	for (it = depot.begin(); it != depot.end(); it++)
	{
		(*it)->Input(tick);
	}
}
void Block1Manager::Update(const LONG& x, const LONG& y)
{
	std::list<Block*>::iterator it;
	for (it = depot.begin(); it != depot.end(); it++)
	{
		(*it)->SetBlockPoint(x, y);
	}
}
void Block1Manager::Draw(HDC hdc)
{
	std::list<Block*>::iterator it;
	for(it = depot.begin(); it != depot.end(); it++)
	{
		Rect tmp;
		if (::IntersectRect(&tmp, &GameGlobal.rcClient, &((*it)->GetDrawRect())))
		{
			(*it)->Draw(hdc);
		}
	}
}

void Block1Manager::Push(Block* pObj)
{
	depot.push_back(pObj);
}
const std::list<Block*>& Block1Manager::getList() const
{
	return depot;
}
