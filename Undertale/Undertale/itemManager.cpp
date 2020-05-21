#include "stdafx.h"
#include "itemManager.h"

HRESULT itemManager::init()
{
	return E_NOTIMPL;
}

void itemManager::release()
{
}

item* itemManager::addItem(string strkey)
{
	item* _item = findItem(strkey);
	if (_item) { return _item; }

	_item = new item;
	if (FAILED(_item->init(_item->ITEM_EMPTY , "No Data")))
	{
		_item->release();
		SAFE_DELETE(_item);

		return nullptr;
	}

	_mItemList.insert(make_pair(strkey, _item));

	return _item;
}

item* itemManager::addWeaponItem(string strkey, const char* itemName, int itemAtk, const char* itemDes, int itemPrice)
{
	item* _item = findItem(strkey);
	if (_item) { return _item; }

	_item = new item;

	if (FAILED(_item->init(itemName,_item->WEAPON, itemDes, itemPrice, itemAtk)))
	{
		_item->release();
		SAFE_DELETE(_item);

		return nullptr;
	}

	_mItemList.insert(make_pair(strkey, _item));

	return _item;
}

item* itemManager::addArmorItem(string strkey, const char* itemName, int itemDef, const char* itemDes, int itemPrice)
{
	item* _item = findItem(strkey);
	if (_item) { return _item; }

	_item = new item;

	if (FAILED(_item->init(itemName, _item->ARMOR, itemDes, itemPrice, itemDef, 0)))
	{
		_item->release();
		SAFE_DELETE(_item);

		return nullptr;
	}

	_mItemList.insert(make_pair(strkey, _item));

	return _item;
}

item* itemManager::addFoodItem(string strkey, const char* itemName, int itemEffect, const char* itemDes, int itemPrice)
{
	item* _item = findItem(strkey);
	if (_item) { return _item; }

	_item = new item;

	if (FAILED(_item->init(itemName, _item->FOOD, itemDes, itemPrice, itemEffect, 0, 0)))
	{
		_item->release();
		SAFE_DELETE(_item);

		return nullptr;
	}

	_mItemList.insert(make_pair(strkey, _item));

	return _item;
}


item* itemManager::findItem(string strkey)
{
	mapItemiter key = _mItemList.find(strkey);
	
	//검색한 키를 찾았다면 아이템 클래스 리턴
	if (key != _mItemList.end())
	{
		//key->first = 해당 키
		return key->second; // = 아이템 클래스
	}

	//key가 end와 같다는 것은
	//해당 아이템이 없다는 말.
	//따라서 NULL을 return 하면 된다.
	return nullptr;
}

BOOL itemManager::deleteItem(string strkey)
{
	mapItemiter key = _mItemList.find(strkey);

	//검색한 키를 찾았다면
	if (key != _mItemList.end())
	{
		//아이템 해제
		key->second->release();
		//메모리 해제
		SAFE_DELETE(key->second);
		//맵의 반복자 해제
		_mItemList.erase(key);

		return TRUE;
	}

	//key가 end까지 도달했다면 해당 아이템이 없다는 뜻이다.
	//아이템이 없으므로 FALSE를 반환한다.
	return FALSE;
}

BOOL itemManager::deleteAll()
{
	mapItemiter iter = _mItemList.begin();

	for (; iter != _mItemList.end();)
	{
		if (iter->second != NULL)
		{
			iter->second->release();
			SAFE_DELETE(iter->second);
			iter = _mItemList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	_mItemList.clear();
	
	return FALSE;
}
