#pragma once
#include "singletonBase.h"


class playerData : public singletonBase <playerData>
{
private:
	char c[32] = { 0, };
private:
	int _hp;
	int _money;

	int _mapNumber;				//�÷��̾ �ִ� ���� ��ȣ
	POINT _playerPoint;				//�÷��̾ �ִ� ��ǥ
private:
	unsigned short _mapAmount = 0;		//��ü ���� ��
	unsigned short _tileAmount = 6;		//��ü �������� ��
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

