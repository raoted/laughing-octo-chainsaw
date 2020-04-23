#pragma once
#include "singletonBase.h"


class playerData : public singletonBase <playerData>
{
private:
	int _hp;
	int _mapNumber;				//�÷��̾ �ִ� ���� ��ȣ
	float _x, _y;				//�÷��̾ �ִ� ��ǥ

private:
	unsigned short _mapAmount = 0;		//��ü ���� ��
	unsigned short _tileAmount = 6;		//��ü �������� ��

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

