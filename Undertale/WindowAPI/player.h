#pragma once
#include "gameNode.h"
#include "Inventory.h"
#define PLAYERSIZE 40	//�÷��̾� Rect ũ��

enum STATE
{
	IDLE = 0, 
	WALK,
	DEAD = 2
};
//��ȣ������ Ŭ���� ���漱��
class enemyManager;

class player : public gameNode
{
private:
	enemyManager* _em;  //����� �����Ҵ� ��!!
	//���ΰ��ӿ��� ������ EM�� �÷��̾�� ������ EM��
	//���� �ٸ� �޸� �ּҶ� ���� ������ �� ����!!!
	Inventory* _inven;
private:
	bool _isBattle;		//�������ΰ�?
	bool _isInfo;		//����â�� �����°�?
	RECT _mapRect;		//�÷��̾� �̵� �Ұ� ó���� ���� �� Rect

	int _count;
	int _frame;
private:
	image* _player;
	string _name;		//�÷��̾� �̸�
	int _LV;			//�÷��̾� LV
	int _gold;			//�÷��̾� ������
	int _HP;			//�÷��̾� HP
	int _maxHP;			//�÷��̾� �ִ� HP
	int _playerAtk;		//�÷��̾� ATK
	int _playerSpeed;	//�÷��̾� �̼�
	POINT _pPoint;		//�÷��̾� (x, y) ��ǥ
	RECT _rcPlayer;		//�÷��̾� Rect
	STATE _pState;		//�÷��̾� ����
private:
	RECT _hpRect;
	RECT _mHPRect;
	RECT _krRect;
public:
	HRESULT init(long x, long y);
	void release();
	void update();
	void render();


	void setEMLink(enemyManager* em) { _em = em; }		//���ʹ� ��ȣ���� �Լ�
public:		//get, set
	//get�Լ�
	int getPlayerLV() { return _LV; }
	int getPlayerGold() { return _gold; }
	int getPlayerHP() { return _HP; }
	int getPlayerMaxHP() { return _maxHP; }
	int getPlayerAtk() { return _playerAtk; }
	int getPlayerSpeed() { return _playerSpeed; }
	RECT getPlayerRect() { return _rcPlayer; }
	RECT getHPRect() { return _hpRect; }
	RECT getMHPRect() { return _mHPRect; }
	RECT getKrRect() { return _krRect; }
	POINT getPlayerPoint() { return _pPoint; }
	STATE getplayerState() { return _pState; }
	string getPlayerName() { return _name; }
	//set�Լ�
	void setPlayerLV(int LV) { _LV = LV; }
	void setPlayerGold(int gold) { _gold = gold; }
	void setPlayerHP(int HP) { _HP = HP; }
	void setPlayerMaxHP(int maxHP) { _maxHP = maxHP; }
	void setPlayerAtk(int playerAtk) { _playerAtk = playerAtk; }
	void setPlayerSpeed(int playerSpeed) { _playerSpeed = playerSpeed; }
	void setPlayerPoint(long x, long y) { _pPoint = { x, y }; }
	void setPlayerRect(POINT point) { _rcPlayer = RectMakeCenter(point.x, point.y, PLAYERSIZE, PLAYERSIZE); }
	void setPlayerState(STATE pState) { _pState = pState; }
	void setMHPRect(int _x, int _y, int _width, int _height) { _mHPRect = RectMake(_x, _y, _width, _height); }
	void setHPRect(int _width) { _hpRect = RectMake(_mHPRect.left, _mHPRect.top, _width, _mHPRect.bottom-_mHPRect.top); }
	void setKrRect(int _width) { _krRect = RectMake(_hpRect.right, _hpRect.top, _width, _hpRect.bottom - _hpRect.top); }

	void playerAnimation();

	Inventory* getInven() { return _inven; }
	void useItem(int i);
	player() {}
	~player() {}
};

