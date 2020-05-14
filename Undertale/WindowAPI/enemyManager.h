#pragma once
#include "gameNode.h"
#include "minion.h"		//미니언 클래스 사용하기 위해
#include "bullet.h"		//공용총알 클래스 사용하기 위해

//상호참조용 클래스 전방선언
class player;

class enemyManager : public gameNode
{
private:
	int _enemyNumber;
	POINT _p;
private:
	vector<enemy*> _vEnemy;
	vector<enemy*>::iterator _viEnemy;
	player* _player;
	bullet* _bullet;

	int _count;
public:
	HRESULT init();
	void release();
	void update();
	void render();

	void setDummy();
	void setNapstablock();
	void setBoss();
	//미니언 벡터 가져오기
	vector<enemy*> getEnemy() { return _vEnemy; }

	//상호참조시 플레이어 포인터를 받을 셋터함수
	void setPlayerLink(player* player) { _player = player; }

	enemy* getEnemy(int i) { return _vEnemy[i]; }
	void setPoint(POINT p) { _p = p; }
	enemyManager() {}
	~enemyManager() {}
};

