#pragma once
#include "singletonBase.h"


class playerData : public singletonBase <playerData>
{
private:
	int _hp;
	int _mapNumber;				//플레이어가 있던 맵의 번호
	float _x, _y;				//플레이어가 있던 좌표

private:
	unsigned short _mapAmount = 0;		//전체 맵의 수
	unsigned short _tileAmount = 6;		//전체 데이터의 수

public:
	HRESULT init();
	void release();

	int getHp() { return _hp; }
	void setHp(int hp) { _hp = hp; }

	unsigned short getMapAmount() { return _mapAmount; }
	void setMapAmount(unsigned short mapAmount) { _mapAmount = mapAmount; }
	unsigned short getTileAmount() { return _tileAmount; }
	void setTileAmount(unsigned short tileAmount) { _tileAmount = tileAmount; }

	playerData() {}
	~playerData() {}
};

