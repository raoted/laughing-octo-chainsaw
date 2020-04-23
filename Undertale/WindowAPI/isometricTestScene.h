#pragma once
#include "gameNode.h"

//타일갯수
#define TILEX 10
#define TILEY 10
//타일크기
#define TILESIZEX 200
#define TILESIZEY (TILESIZEX / 2)
//타일절반크기
#define TILESIZEX_HALF (TILESIZEX /2)
#define TILESIZEY_HALF (TILESIZEY /2)
//아이소타일 위치 => 시작좌표(0, 0)
#define STARTX (WINSIZEX / 2)
#define STARTY 100

//타일 구조체
struct tagTile
{
	int x, y;
	RECT rc;
};

class isometricTestScene : public gameNode
{
private:
	tagTile _tile[TILEX][TILEY]; //타일구조체 10 * 10
	POINT _index;
	RECT _rc;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	isometricTestScene() {}
	~isometricTestScene() {}
};

