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
	RECT _rc;
private:
	vector<enemy*> _vEnemy;
	vector<enemy*>::iterator _viEnemy;
	player* _player;
	bullet* _bullet;
	bone* _bone;
	tear* _tear;
	int _count;
	bool _attackEnd;
public:
	HRESULT init(RECT rc);
	void release();
	void update(int enemyNumber);
	void render(int x, int y);

	void setDummy();
	void setNapstablock();
	void setBoss();
	//�̴Ͼ� ���� ��������
	vector<enemy*> getEnemy() { return _vEnemy; }

	//��ȣ������ �÷��̾� �����͸� ���� �����Լ�
	void setPlayerLink(player* player) { _player = player; }

	enemy* getEnemy(int i) { return _vEnemy[i]; }
	void setPoint(POINT p) { _p = p; }
	bone* getBone() { return _bone; }
	tear* getTear() { return _tear; }
	void setAttackEnd(bool b) 
	{
		switch (_enemyNumber)
		{
		case 0:
			break;
		case 1:
		{
			_tear->clear();
		}
			break;
		case 2:
		{
			_bone->clear();
		}
			break;
		}
		_attackEnd = b; 
	}
	enemyManager() {}
	~enemyManager() {}
};

