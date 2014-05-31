#pragma once

#include <list>
#include "../GameDev/GameDev.h"
#include "Block.h"

class Block1Manager : public singleton<Block1Manager>
{
	friend class singleton<Block1Manager>;

private :
	Block1Manager();
	~Block1Manager();
public :
	void Input(DWORD);
	void Update(const LONG& x, const LONG& y);
	void Draw(HDC);

	void Push(Block* pObj);

	const std::list<Block*>& getList() const;

private :
	std::list<Block*> depot;

};

#define BlockDepot Block1Manager::getReference()