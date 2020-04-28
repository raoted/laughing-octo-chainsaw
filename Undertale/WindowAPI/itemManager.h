#pragma once
#include "singletonBase.h"
#include "item.h"

class itemManager : public singletonBase<itemManager>
{
private:
	typedef map<string, item*>mapItemList;
	typedef map<string, item*>::iterator mapItemiter;
private:
	mapItemList _mItemList;
public:
	HRESULT init();
	void release();

	item* addItem(string strkey);
	item* addWeaponItem(string strkey, const char* itemName, int itemAtk, const char* itemDes, int itemPrice);
	item* addArmorItem(string strkey, const char* itemName, int itemDef, const char* itemDes, int itemPrice);
	item* addFoodItem(string strkey, const char* itemName, int itemEffect, const char* itemDes, int itemPrice);


	item* findItem(string strkey);
	BOOL deleteItem(string strkey);
	BOOL deleteAll();
	
	itemManager() {};
	~itemManager() {};
};