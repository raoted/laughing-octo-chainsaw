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
	
	//�˻��� Ű�� ã�Ҵٸ� ������ Ŭ���� ����
	if (key != _mItemList.end())
	{
		//key->first = �ش� Ű
		return key->second; // = ������ Ŭ����
	}

	//key�� end�� ���ٴ� ����
	//�ش� �������� ���ٴ� ��.
	//���� NULL�� return �ϸ� �ȴ�.
	return nullptr;
}

BOOL itemManager::deleteItem(string strkey)
{
	mapItemiter key = _mItemList.find(strkey);

	//�˻��� Ű�� ã�Ҵٸ�
	if (key != _mItemList.end())
	{
		//������ ����
		key->second->release();
		//�޸� ����
		SAFE_DELETE(key->second);
		//���� �ݺ��� ����
		_mItemList.erase(key);

		return TRUE;
	}

	//key�� end���� �����ߴٸ� �ش� �������� ���ٴ� ���̴�.
	//�������� �����Ƿ� FALSE�� ��ȯ�Ѵ�.
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
