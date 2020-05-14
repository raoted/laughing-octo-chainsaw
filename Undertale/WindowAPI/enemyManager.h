#pragma once
#include "gameNode.h"
#include "minion.h"		//�̴Ͼ� Ŭ���� ����ϱ� ����
#include "bullet.h"		//�����Ѿ� Ŭ���� ����ϱ� ����

//��ȣ������ Ŭ���� ���漱��
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
	//�̴Ͼ� ���� ��������
	vector<enemy*> getEnemy() { return _vEnemy; }

	//��ȣ������ �÷��̾� �����͸� ���� �����Լ�
	void setPlayerLink(player* player) { _player = player; }

	enemy* getEnemy(int i) { return _vEnemy[i]; }
	void setPoint(POINT p) { _p = p; }
	enemyManager() {}
	~enemyManager() {}
};

