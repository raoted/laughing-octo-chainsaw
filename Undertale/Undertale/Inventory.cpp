#include "stdafx.h"
#include "Inventory.h"

HRESULT Inventory::init()
{
	_item = ITEMMANAGER->findItem("버터스카치 파이");
	_vItem.push_back(_item);
	_item = ITEMMANAGER->findItem("전설의 영웅");
	_vItem.push_back(_item);
	_item = ITEMMANAGER->findItem("전설의 영웅");
	_vItem.push_back(_item);
	_item = ITEMMANAGER->findItem("전설의 영웅");
	_vItem.push_back(_item);
	_item = ITEMMANAGER->findItem("전설의 영웅");
	_vItem.push_back(_item);
	_item = ITEMMANAGER->findItem("눈사람 조각");
	_vItem.push_back(_item);
	_item = ITEMMANAGER->findItem("눈사람 조각");
	_vItem.push_back(_item);
	_item = ITEMMANAGER->findItem("눈사람 조각");
	_vItem.push_back(_item);
	_item = ITEMMANAGER->findItem("눈사람 조각");
	_vItem.push_back(_item);
	_item = ITEMMANAGER->findItem("눈사람 조각");
	_vItem.push_back(_item);

	return S_OK;
}

void Inventory::release()
{
}

void Inventory::update()
{
}

