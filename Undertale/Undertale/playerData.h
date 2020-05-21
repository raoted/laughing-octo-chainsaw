#pragma once
#include "singletonBase.h"


class playerData : public singletonBase <playerData>
{
private:
	char c[32] = { 0, };
private:
	int _HP = -1;
	int _maxHP;
	int _money;

	int _mapNumber;				//플레이어가 있던 맵의 번호
	POINT _playerPoint;				//플레이어가 있던 좌표
	POINT _ePoint;
	POINT _mapPoint;
private:
	unsigned short _mapAmount = 0;		//전체 맵의 수
	unsigned short _tileAmount = 6;		//전체 데이터의 수
public:
	HRESULT init();
	void release();
	void saveMapToolData();
	void savePlayerData();

	int getHp() { return _HP; }
	void setHp(int hp) { _HP = hp; }
	int getMaxHP() { return _maxHP; }
	void setMaxHP(int maxHP) { _maxHP = maxHP; }
	int getMoney() { return _money; }
	void setMoney(int money) { _money = money; }
	int getGold() { return _money; }
	void setGold(int gold) { _money = gold; }


	int getMapNumber() { return _mapNumber; }
	void setMapNumber(int mapNumber) { _mapNumber = mapNumber; }
	POINT getPlayerPoint() { return _playerPoint; }
	void setPlayerPoint(POINT playerPoint) { _playerPoint = playerPoint; }
	void setPlayerPoint(long x, long y) 
	{
		_playerPoint.x = x; 
		_playerPoint.y = y;
	}
	POINT getMapPoint() { return _mapPoint; }
	void setMapPoint(long x, long y)
	{
		_mapPoint.x = x;
		_mapPoint.y = y;
	}
	POINT getEPoint() { return _ePoint; }
	void setEPoint(long x, long y)
	{
		_ePoint.x = x;
		_ePoint.y = y;
	}

	unsigned short getMapAmount() { return _mapAmount; }
	void setMapAmount(unsigned short mapAmount) { _mapAmount = mapAmount; }
	unsigned short getTileAmount() { return _tileAmount; }
	void setTileAmount(unsigned short tileAmount) { _tileAmount = tileAmount; }

	playerData() {}
	~playerData() {}
};

