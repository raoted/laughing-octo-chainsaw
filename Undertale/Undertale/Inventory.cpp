#include "stdafx.h"
#include "Inventory.h"

HRESULT Inventory::init()
{
	_item = ITEMMANAGER->findItem("���ͽ�īġ ����");
	_vItem.push_back(_item);
	_item = ITEMMANAGER->findItem("������ ����");
	_vItem.push_back(_item);
	_item = ITEMMANAGER->findItem("������ ����");
	_vItem.push_back(_item);
	_item = ITEMMANAGER->findItem("������ ����");
	_vItem.push_back(_item);
	_item = ITEMMANAGER->findItem("������ ����");
	_vItem.push_back(_item);
	_item = ITEMMANAGER->findItem("����� ����");
	_vItem.push_back(_item);
	_item = ITEMMANAGER->findItem("����� ����");
	_vItem.push_back(_item);
	_item = ITEMMANAGER->findItem("����� ����");
	_vItem.push_back(_item);
	_item = ITEMMANAGER->findItem("����� ����");
	_vItem.push_back(_item);
	_item = ITEMMANAGER->findItem("����� ����");
	_vItem.push_back(_item);

	return S_OK;
}

void Inventory::release()
{
}

void Inventory::update()
{
}

