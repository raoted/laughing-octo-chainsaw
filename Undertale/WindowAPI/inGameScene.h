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
	char mapName[128];		//맵파일 이름을 저장할 char 배열
	char tileName[64];		//타일 파일 이름을 저장할 char 배열
	RECT rc;				//IntersectRect에 사용할 임시 Rect
private:
	player* _player;				//플레이어 클래스
	enemyManager* _enemyManager;	//적매니져 클래스
	astar* _astar;					//A* 클래스
private:
	bool _walkCheck[4];				//해당 방향으로 걸어갈 수 있는지 확인하는 bool 배열
	POINT _walkPOINT[4];			//각 방향으로 걸어갈 수 있는지 확인할 때 사용하는 Rect 배열
	POINT _pPoint;					//플레이어의 위치 POINT
	POINT _mapPoint;				//맵의 시작 좌표

	int _mapNumber;
	int _mapSize;					//map의 타일 갯수
	int _selectCount;
	tagTile _map;					//map 파일을 저장할 객체
	RECT _ingameScreen;				//화면 크기만큼의 사각형으로 이 RECT에 곂쳐진 경우에만 그려준다.
	RECT _mapRect;					//맵을 그릴 Rect
	RECT _dialog;					//대화창을 그릴 Rect
private:
	int _eSpawnerGoal;				//EnemySpawner가 목표로 하는 Rect
	POINT _ePoint;					//EnemySpawner의 중심좌표
	RECT _enemySpawner;				//플레이어와 충돌하면 전투처리를 시키기 위한 Rect
private:
	WARPPOINT _warp;
	vector<WARPPOINT> _vWarp;
private:
	bool _isMenu;		//메뉴창을 열었는가?
	bool _isInven;		//인벤을 선택했는가?
	bool _isState;		//상태를 선택했는가?
	bool _isCall;		//전화를 선택했는가?
	bool _isDialog;		//대화창이 켜졌는가?

	int _count;
	int _dialogCount;
	int _dialogSize;
	int _menuCount;		//메뉴 번호 (0 ~ 2)
	vector<string> _dialogText;
	RECT _smallInfo;	//메뉴창 열었을 때 보이는 간단한 스탯
	RECT _statInfo;		//스탯 정보 Rect
	RECT _inven;
	RECT _menuRect;		//메뉴창 Rect
	RECT _menu[3];		//각 메뉴 Rect
	string _text[3];	//각 메뉴 Text
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

