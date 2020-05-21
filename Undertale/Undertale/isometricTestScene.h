#pragma once
#include "gameNode.h"

//Ÿ�ϰ���
#define TILEX 10
#define TILEY 10
//Ÿ��ũ��
#define TILESIZEX 200
#define TILESIZEY (TILESIZEX / 2)
//Ÿ������ũ��
#define TILESIZEX_HALF (TILESIZEX /2)
#define TILESIZEY_HALF (TILESIZEY /2)
//���̼�Ÿ�� ��ġ => ������ǥ(0, 0)
#define STARTX (WINSIZEX / 2)
#define STARTY 100

//Ÿ�� ����ü
struct tagTile
{
	int x, y;
	RECT rc;
};

class isometricTestScene : public gameNode
{
private:
	tagTile _tile[TILEX][TILEY]; //Ÿ�ϱ���ü 10 * 10
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

