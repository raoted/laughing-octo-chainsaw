#pragma once
#include "gameNode.h"
#include "tileNode.h"
#include "player.h"
#include "enemyManager.h"
#include "astar.h"

struct WARPPOINT
{
	int mapNum;
	RECT rc;
};
class inGameScene : public gameNode
{
private:
	char mapName[128];		//������ �̸��� ������ char �迭
	char tileName[64];		//Ÿ�� ���� �̸��� ������ char �迭
	RECT rc;				//IntersectRect�� ����� �ӽ� Rect
private:
	player* _player;				//�÷��̾� Ŭ����
	enemyManager* _enemyManager;	//���Ŵ��� Ŭ����
	astar* _astar;					//A* Ŭ����
private:
	bool _walkCheck[4];				//�ش� �������� �ɾ �� �ִ��� Ȯ���ϴ� bool �迭
	POINT _walkPOINT[4];			//�� �������� �ɾ �� �ִ��� Ȯ���� �� ����ϴ� Rect �迭
	POINT _pPoint;					//�÷��̾��� ��ġ POINT
	POINT _mapPoint;				//���� ���� ��ǥ

	int _mapNumber;
	int _mapSize;					//map�� Ÿ�� ����
	int _selectCount;
	tagTile _map;					//map ������ ������ ��ü
	RECT _ingameScreen;				//ȭ�� ũ�⸸ŭ�� �簢������ �� RECT�� ������ ��쿡�� �׷��ش�.
	RECT _mapRect;					//���� �׸� Rect
	RECT _dialog;					//��ȭâ�� �׸� Rect
private:
	int _eSpawnerGoal;				//EnemySpawner�� ��ǥ�� �ϴ� Rect
	POINT _ePoint;					//EnemySpawner�� �߽���ǥ
	RECT _enemySpawner;				//�÷��̾�� �浹�ϸ� ����ó���� ��Ű�� ���� Rect
private:
	WARPPOINT _warp;
	vector<WARPPOINT> _vWarp;
private:
	bool _isMenu;		//�޴�â�� �����°�?
	bool _isInven;		//�κ��� �����ߴ°�?
	bool _isState;		//���¸� �����ߴ°�?
	bool _isCall;		//��ȭ�� �����ߴ°�?
	bool _isDialog;		//��ȭâ�� �����°�?

	int _count;
	int _dialogCount;
	int _dialogSize;
	int _menuCount;		//�޴� ��ȣ (0 ~ 2)
	vector<string> _dialogText;
	RECT _smallInfo;	//�޴�â ������ �� ���̴� ������ ����
	RECT _statInfo;		//���� ���� Rect
	RECT _inven;
	RECT _menuRect;		//�޴�â Rect
	RECT _menu[3];		//�� �޴� Rect
	string _text[3];	//�� �޴� Text
public:
	HRESULT init();
	void release();
	void update();
	void render();

public:
	void loadMap();
	void moveStartNode();
	void directCheck();
	void playerControl();

	void savePlayerInfo();

	void map1WarpPoint();
	void map2WarpPoint();
	void map3WarpPoint();

	void frameDraw(RECT rc);
	inGameScene() {}
	~inGameScene() {}
};

