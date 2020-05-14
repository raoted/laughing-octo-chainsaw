#pragma once
#include "gameNode.h"
#include "Inventory.h"
#define PLAYERSIZE 40	//플레이어 Rect 크기

enum STATE
{
	IDLE = 0, 
	WALK,
	DEAD = 2
};
//상호참조용 클래스 전방선언
class enemyManager;

class player : public gameNode
{
private:
	enemyManager* _em;  //절대로 동적할당 놉!!
	//메인게임에서 생성한 EM과 플레이어에서 생성한 EM은
	//전혀 다른 메모리 주소라서 서로 참조할 수 없다!!!
	Inventory* _inven;
private:
	bool _isBattle;		//전투중인가?
	bool _isInfo;		//상태창을 열었는가?
	RECT _mapRect;		//플레이어 이동 불가 처리를 위한 맵 Rect

	int _count;
	int _frame;
private:
	image* _player;
	string _name;		//플레이어 이름
	int _LV;			//플레이어 LV
	int _gold;			//플레이어 소지금
	int _HP;			//플레이어 HP
	int _maxHP;			//플레이어 최대 HP
	int _playerAtk;		//플레이어 ATK
	int _playerSpeed;	//플레이어 이속
	POINT _pPoint;		//플레이어 (x, y) 좌표
	RECT _rcPlayer;		//플레이어 Rect
	STATE _pState;		//플레이어 상태
private:
	RECT _hpRect;
	RECT _mHPRect;
	RECT _krRect;
public:
	HRESULT init(long x, long y);
	void release();
	void update();
	void render();


	void setEMLink(enemyManager* em) { _em = em; }		//에너미 상호참조 함수
public:		//get, set
	//get함수
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
	//set함수
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

