#pragma once
#include "singletonBase.h"


class playerData : public singletonBase <playerData>
{
private:
	char c[32] = { 0, };
private:
	int _hp;
	int _money;

	int _mapNumber;				//플레이어가 있던 맵의 번호
	POINT _playerPoint;				//플레이어가 있던 좌표
private:
	unsigned short _mapAmount = 0;		//전체 맵의 수
	unsigned short _tileAmount = 6;		//전체 데이터의 수
public:
	HRESULT init();
	void release();
	void saveMapToolData();
	void savePlayerData();

	int getHp() { return _hp; }
	void setHp(int hp) { _hp = hp; }
	int getMoney() { return _money; }
	void setMoney(int money) { _money = money; }

	int getMapNumber() { return _mapNumber; }
	void setMapNumber(int mapNumber) { _mapNumber = mapNumber; }
	POINT getPlayerPoint() { return _playerPoint; }
	void setPlayerPoint(POINT playerPoint) { _playerPoint = playerPoint; }

	unsigned short getMapAmount() { return _mapAmount; }
	void setMapAmount(unsigned short mapAmount) { _mapAmount = mapAmount; }
	unsigned short getTileAmount() { return _tileAmount; }
	void setTileAmount(unsigned short tileAmount) { _tileAmount = tileAmount; }

	playerData() {}
	~playerData() {}
};

