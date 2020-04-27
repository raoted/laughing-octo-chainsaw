#pragma once
#include "gameNode.h"
#include "tileNode.h"
#define VK_D	0x44
#define VK_1	0x31
#define VK_2	0x32
#define VK_3	0x33
#define ICONSIZE	38

class maptoolScene : public gameNode
{
private:
	bool _editMode;					//맵파일 속성 입력 모드.
	bool _editCanMove;				//해당 타일로 이동을 변경할 수 있는지 판단해주는 bool 변수
	bool _editMoveDirect;			//해당 타일에서 이동 방향을 변경할 수 있는지 판단해주는 bool 변수
	bool _canMove;					//현재 선택한 타일이 이동 가능한 타일인가?
	bool _direct[4] = { false, };	//현재 선택한 타일에서 어떤 방향으로 이동할 수 있는가?
									//0:상	1:하	  2:좌  3:우

	bool _setSaveLoad;				//세이브/로드창 띄워져있는가 없는가 판단

	short _drawMode;				//타일에 어떤 모드로 그릴 것인가?
									//0 : 지우개
									//1 : 펜
									//2 : 사각형
									//3 : 페인트
	short _layer;					//어떤 레이어를 선택했는가?
									//0 : 이벤트
									//1 : 1층 레이어
									//2 : 2층 레이어
									//3 : 3층 레이어

	int _setSaveSlot;				//세이브 슬롯 활성화
	char _palettePageNum[128];

	char _canMoveChar[8] = { 0, };		//에디트 모드에서 사용
										//각 샘플 타일로 이동할 수 있는지 표시.
										//0 : 이동 불가		1 : 이동 가능
	char _moveDirectChar[5] = { 0, };	//에디트 모드에서 사용
										//각 생플타일에서 어떤 방향으로 이동할 수 있는지를 표시.
										//0:상  1:하  2:좌  3:우


	char _fileName[128] = { 0, };	//맵 파일의 이름을 받아올 변수 
	char _mapName[128] = { 0, };
									//파일 이름(맵, 맵 데이터)을 받아올 변수
	char _imageName[128] = { 0, };	//맵 이미지 파일의 이름을 받아올 변수
	char _dataName[128] = { 0, };	//맵 데이터 파일의 이름을 받아올 변수

	unsigned short _mapAmount;
	unsigned short _tileAmount;

	int _startFile;			/*파일 명은 0번부터 시작
							ex) MAP0.map	MAP0.mapdata
							*/

	int _palettePage;		//지금 샘플 타일에 몇번째 샘플 이미지가 그려지는지를 저장한 변수.

private:
	TILE _addTile;	//_tiles를 초기화 할 때 사용
	tagTile _tiles;	//인게임화면에 사용되는 타일 총 400개
	vector<tagTile> _vTiles;
	tagSampleTile _sampleTile[60]; //샘플타일 총 (12-2)*6 = 60개
	drawField _drag;
	tagCurrentTile _currentTile;	//현재타일


private:
	RECT _rcTool;		//새 맵, 펜, 지우개 등 도구 버튼들이 위치할 공간.

	RECT _rcNewMap;		//새 맵을 생성하는 툴 버튼 Rect
	RECT _rcSaveMap;	//모든 맵의 변경사항을 저장하는 툴 버튼 Rect
	RECT _rcLayer[3];	//어떤 레이어에 그릴지 선택하는 툴 버튼 Rect
	RECT _rcEvent;		//이벤트를 관리하는 툴 버튼 Rect
	RECT _rcEraser;
	RECT _rcPen;		//그리기 모드를 펜 형태로 변경하는 툴 버튼 Rect
	RECT _rcRectangle;	//그리기 모드를 사각형으로 변경하는 툴 버튼 Rect
	RECT _rcPaint;		//그리기 모드를 전체로 변경하는 툴 버튼 Rect

	RECT _rcPalette;	//샘플타일을 그려줄 Rect;
	RECT _rcMapList;	//맵 목록을 그려주는 공간
	RECT _rcTileScreen;
	RECT _rcScreen;		//화면 카메라 Rect;
						//이 Rect와 충돌한 타일만 화면에 그려진다.
	RECT _rcArrow[2];	//샘플타일을 변경할 화살표를 그려주는 Rect
						//0번 : LeftArrow, 1번 : RightArrow
	RECT _rcArrow5[2];

	RECT _rcMouse;		//마우스 포인터를 따라다니는 Rect
						//중심점은 마우스 포인터의 좌표이며
						//이 Rect가 화면 끝에 닿으면 타일이 움직인다.

	RECT _rcSaveLoad;		//세이브/로드버튼
	RECT _rcSaveWindow;		//세이브 창
	vector<RECT>_rcSaveSlot;		//세이브슬롯3개
	
	RECT _rcSave;			//세이브로드 UI안에서의 세이브
	RECT _rcLoad;			//세이브로드 UI안에서의 로드

	RECT _rcDummy2;			//3번째칸	오브젝트
	RECT _rcDummy3;			//4번째칸	지우개(오브젝트만 지운다, 지형은 덮어씌운다)


	char _pageNum[100];
	char _pageName[100];

	char drag[1024];	//드래그 좌표 미리보기용
	char count[1042];

public:
	HRESULT init();
	void release();
	void update();
	void render();

	//맵툴세팅
	void maptoolSetup();
	void selectSampleTile();			//팔레트에서 칠해줄 타일 선택하기
	void setMap();						//일반 채우기
	void setRectangle();
	void setAllMap();					//전체 채우기
	void save(char* str);				//제작한 맵을 저장하는 함수
	void load(char* str);				//제작된 맵을 불러오는 함수
	void saveMapData(char *str);		//샘플타일의 속성을 프로젝트 폴더의 하위 폴더인 MapData에
										//.mapdata 확장자로 저장된다.
										//현재 버그로 미동작.
	void loadMapData(char *str);		//샘플타일의 속성을 프로젝트 폴더의 하위 폴더인 MapData에서
										//.mapdata 확장자를 불러온다.
	void drawToolLayer();				//툴 레이어에 아이콘들을 그려준다.
	void setTiles();



	void frameBoxRender(int left, int top, int width, int height, float scale);			//팝업창(텍스트등)에 프레임씌우기
	void frameBoxRender(RECT rc, float scale);											//프레임씌우기 사각형 넣어서 간편하게


	//지형, 오브젝트 세터
	TERRAIN terrainSelect(int frameX, int frameY);

	maptoolScene() {}
	~maptoolScene() {}
};

