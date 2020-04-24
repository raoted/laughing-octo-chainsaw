#pragma once

class item
{
public:
	enum ITEM_TYPE
	{
		ITEM_EMPTY,
		WEAPON,
		ARMOR,
		FOOD, 
		END
	}type;

	typedef struct tagItem
	{
		ITEM_TYPE _itemType;
		char* _itemDes;
		tagItem()
		{
			_itemType = ITEM_EMPTY;
			_itemDes = { 0, };
		}

	}ITEM_INFO, *LPITEM_INFO;
private:
	char* _itemName = { 0, };
	int _itemEffect = 0;
	int _itemAtk = 0;
	int _itemDef = 0;
	int _itemPrice = 0;

	LPITEM_INFO _itemInfo;
	LPITEM_INFO _weaponItem;
	LPITEM_INFO _armorItem;
	LPITEM_INFO _foodItem;

public:
	item();
	~item();

	HRESULT init(ITEM_TYPE itemType, const char*itemDes);
	HRESULT init(const char* itemName, ITEM_TYPE itemType, const char* itemDes, int itemPrice, int itemAtk);
	HRESULT init(const char* itemName, ITEM_TYPE itemType, const char* itemDes, int itemPrice, int itemDef, int itemAtk);
	HRESULT init(const char* itemName, ITEM_TYPE itemType, const char* itemDes, int itemPrice, int itemEffect, int itemAtk, int itemDef);
	
	char* getItemName() { return _itemName; }
	ITEM_TYPE getItemType() { return _itemInfo->_itemType; }
	char* getItemDes() { return _itemInfo->_itemDes; }
	int getItemPrice() { return _itemPrice; }
	int getItemAtk() { return _itemAtk; }
	int getItemDef() { return _itemDef; }
	int getItemEffect() { return _itemEffect; }

	void release();
};

