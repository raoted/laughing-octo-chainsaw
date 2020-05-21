#pragma once
#include "gameNode.h"
#include "item.h"
class Inventory : public gameNode
{
private:
	item* _item;
	vector<item*> _vItem;
public:
	HRESULT init();
	void release();
	void update();
public:
	vector<item*> getItem() { return _vItem; }
	void eraseItem(int i) { _vItem.erase(_vItem.begin() + i); }
	Inventory() {};
	~Inventory() {};
};

