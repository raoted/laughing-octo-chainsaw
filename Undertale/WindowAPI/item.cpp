#include "stdafx.h"
#include "item.h"
item::item() : _itemInfo(NULL), _weaponItem(NULL), _armorItem(NULL), _foodItem(NULL)
{
	
}
item::~item()
{

}


HRESULT item::init(ITEM_TYPE itemType, const char* itemDes)
{
	if (_itemInfo != NULL) { this->release(); }

	_itemInfo = new ITEM_INFO;
	_itemInfo->_itemType = itemType;
	int len = strlen(itemDes);
	_itemInfo->_itemDes = new char[len + 1];
	strcpy(_itemInfo->_itemDes, itemDes);

	return S_OK;
}

HRESULT item::init(const char* itemName, ITEM_TYPE itemType, const char* itemDes, int itemPrice, int itemAtk)
{
	if (_itemInfo != NULL) { this->release(); }
	int len;

	len = strlen(itemName);
	_itemName = new char[len + 1];
	strcpy(_itemName, itemName);

	_itemInfo = new ITEM_INFO;
	_itemInfo->_itemType = itemType;
	
	len = strlen(itemDes);
	_itemInfo->_itemDes = new char[len + 1];
	strcpy(_itemInfo->_itemDes, itemDes);

	_itemPrice = itemPrice;
	_itemAtk = itemAtk;


	return S_OK;
}

HRESULT item::init(const char* itemName, ITEM_TYPE itemType, const char* itemDes, int itemPrice, int itemDef, int itemAtk)
{
	if (_itemInfo != NULL) { this->release(); }
	int len;

	len = strlen(itemName);
	_itemName = new char[len + 1];
	strcpy(_itemName, itemName);

	_itemInfo = new ITEM_INFO;
	_itemInfo->_itemType = itemType;
	
	len = strlen(itemDes);
	_itemInfo->_itemDes = new char[len + 1];
	strcpy(_itemInfo->_itemDes, itemDes);

	_itemPrice = itemPrice;
	_itemDef = itemDef;
	_itemAtk = itemAtk;

	return S_OK;
}

HRESULT item::init(const char* itemName, ITEM_TYPE itemType, const char* itemDes, int itemPrice, int itemEffect, int itemAtk, int itemDef)
{
	if (_itemInfo != NULL) { this->release(); }
	int len;

	len = strlen(itemName);
	_itemName = new char[len + 1];
	strcpy(_itemName, itemName);

	_itemInfo = new ITEM_INFO;
	_itemInfo->_itemType = itemType;
	
	len = strlen(itemDes);
	_itemInfo->_itemDes = new char[len + 1];
	strcpy(_itemInfo->_itemDes, itemDes);

	_itemPrice = itemPrice;
	_itemEffect = itemEffect;
	_itemAtk = itemAtk;
	_itemDef = itemDef;

	return S_OK;
}



void item::release()
{
}

