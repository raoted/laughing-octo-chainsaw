#pragma once
#include "gameNode.h"
#include "tileNode.h"
#define VK_D	0x44
#define VK_1	0x31
#define VK_2	0x32
#define VK_3	0x33
#define ICONSIZE	38

struct MAPLIST
{
	string mapListStr;
	RECT mapListRc;
};

class maptoolScene : public gameNode
{
private:
	char test[128];
private:
	bool _editMode;					//������ �Ӽ� �Է� ���.
	bool _resizeMode;				//�� ũ�� ���� ���

	bool _editCanMove;				//�ش� Ÿ�Ϸ� �̵��� ������ �� �ִ��� �Ǵ����ִ� bool ����
	bool _editMoveDirect;			//�ش� Ÿ�Ͽ��� �̵� ������ ������ �� �ִ��� �Ǵ����ִ� bool ����
	bool _canMove;					//���� ������ Ÿ���� �̵� ������ Ÿ���ΰ�?
	bool _direct[4] = { true, };	//���� ������ Ÿ�Ͽ��� � �������� �̵��� �� �ִ°�?
									//0:��	1:��	  2:��  3:��
	
	short _mapNumber;				//���° ���� �ۼ��� ���ΰ�?
	short _drawMode;				//Ÿ�Ͽ� � ���� �׸� ���ΰ�?
									//0 : ���찳
									//1 : ��
									//2 : �簢��
									//3 : ����Ʈ
	short _layer;					//� ���̾ �����ߴ°�?
									//0 : �̺�Ʈ
									//1 : 1�� ���̾�
									//2 : 2�� ���̾�
									//3 : 3�� ���̾�
	char _palettePageNum[128];

	char _canMoveChar[8] = { 0, };		//����Ʈ ��忡�� ���
										//�� ���� Ÿ�Ϸ� �̵��� �� �ִ��� ǥ��.
										//0 : �̵� �Ұ�		1 : �̵� ����
	char _moveDirectChar[5] = { 0, };	//����Ʈ ��忡�� ���
										//�� ����Ÿ�Ͽ��� � �������� �̵��� �� �ִ����� ǥ��.
										//0:��  1:��  2:��  3:��


	char _fileName[128] = { 0, };	//�� ������ �̸��� �޾ƿ� ���� 
	char _mapName[128] = { 0, };
									//���� �̸�(��, �� ������)�� �޾ƿ� ����
	char _imageName[128] = { 0, };	//�� �̹��� ������ �̸��� �޾ƿ� ����
	char _dataName[128] = { 0, };	//�� ������ ������ �̸��� �޾ƿ� ����

	unsigned short _mapAmount;
	unsigned short _tileAmount;

	int _palettePage;		//���� ���� Ÿ�Ͽ� ���° ���� �̹����� �׷��������� ������ ����.

private:
	TILE _addTile;					//_tiles�� �ʱ�ȭ �� �� ���
	tagTile _tiles;					//�ΰ���ȭ�鿡 ���Ǵ� Ÿ�� �� 400��
	vector<tagTile> _vTiles;
	MAPLIST _mapList;				//�� ����� ������ �� ����� ����ü
	vector<MAPLIST> _vMapList;				//�� ��ϵ��� ������ MAPLIST ����
	tagSampleTile _sampleTile[60];	//����Ÿ�� �� (12-2)*6 = 60��
	tagCurrentTile _currentTile;	//����Ÿ��
private:
	char section[128];
	char key[128];
	char value[128];
	unsigned int _mapCount;
private:
	RECT rc;			//intersectRect �񱳿� Rect
	RECT _rcTool;		//�� ��, ��, ���찳 �� ���� ��ư���� ��ġ�� ����.

	RECT _rcNewMap;		//�� ���� �����ϴ� �� ��ư Rect
	RECT _rcSaveMap;	//��� ���� ��������� �����ϴ� �� ��ư Rect
	RECT _rcLayer[3];	//� ���̾ �׸��� �����ϴ� �� ��ư Rect
	RECT _rcEraser;
	RECT _rcPen;		//�׸��� ��带 �� ���·� �����ϴ� �� ��ư Rect
	RECT _rcRectangle;	//�׸��� ��带 �簢������ �����ϴ� �� ��ư Rect
	RECT _rcPaint;		//�׸��� ��带 ��ü�� �����ϴ� �� ��ư Rect
	RECT _rcSetting;	//������ Ÿ���� ũ�⸦ �����ϴ� �� ��ư Rect
	RECT _rcHome;		//���� ȭ������ �̵���Ű�� �� ��ư Rect

	RECT _rcPalette;		//����Ÿ���� �׷��� Rect;
	RECT _rcMapList;		//�� ����� �׷��ִ� ����
	RECT _rcMapListSlide;	//�� ����� �����̵� ��ų Rect
	RECT _rcMapScreen;
	RECT _rcScreen;			//ȭ�� ī�޶� Rect;
							//�� Rect�� �浹�� Ÿ�ϸ� ȭ�鿡 �׷�����.

	RECT _rcMouse;		//���콺 �����͸� ����ٴϴ� Rect
						//�߽����� ���콺 �������� ��ǥ�̸�
						//�� Rect�� ȭ�� ���� ������ Ÿ���� �����δ�.

	RECT _rectangle;


	char _pageNum[100];
	char _pageName[100];
private:
	RECT _rcResize;		//�� ũ�� �缳�� â�� �׷��� Rect
	RECT _rcResizeX;		//���� x�� Ÿ�� ����
	RECT _rcResizeY;		//���� y�� Ÿ�� ����
	RECT _rcConfirm;
	RECT _rcCancle;

	unsigned short x;
	unsigned short y;


public:
	HRESULT init();
	void release();
	void update();
	void render();

	//��������
	void maptoolSetup();
	void selectSampleTile();			//�ȷ�Ʈ���� ĥ���� Ÿ�� �����ϱ�
	void setRemove();
	void setMap();						//�Ϲ� ä���
	void setRectangle();
	void setAllMap();					//��ü ä���
	void save(char* str, int i);				//������ ���� �����ϴ� �Լ�
	void load(char* str, int i);				//���۵� ���� �ҷ����� �Լ�
	void saveMapData(char *str);		//����Ÿ���� �Ӽ��� ������Ʈ ������ ���� ������ MapData��
										//.mapdata Ȯ���ڷ� ����ȴ�.
										//���� ���׷� �̵���.
	void loadMapData(char *str);		//����Ÿ���� �Ӽ��� ������Ʈ ������ ���� ������ MapData����
										//.mapdata Ȯ���ڸ� �ҷ��´�.
	void drawToolLayer();				//�� ���̾ �����ܵ��� �׷��ش�.
	void rcRectangleDraw(int startX, int startY, int endX, int endY);

	void addMap();						//�� �߰�
	void loadMap();

	void checkSampleTile(int wMouse);	
	void drawMapList();					//�� ����� �׷��ִ� �Լ�
	void mapResizing();
	void mapScreenResizing();


	//����, ������Ʈ ����
	TERRAIN terrainSelect(int frameX, int frameY);

	maptoolScene() {}
	~maptoolScene() {}
};

