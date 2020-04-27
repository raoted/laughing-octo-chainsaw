#include "stdafx.h"
#include "maptoolScene.h"

HRESULT maptoolScene::init()
{
	//타일맵 이미지 초기화
	_rcScreen = RectMake(368, 0, WINSIZEX+32, WINSIZEY+32);
	_rcTool = RectMake(0, 0, WINSIZEX, 42);
	_rcNewMap = RectMake(_rcTool.left + 2, 2, ICONSIZE, ICONSIZE);
	_rcSaveMap = RectMake(_rcNewMap.right + 2, 2, ICONSIZE, ICONSIZE);
	_rcLayer[0] = RectMake(_rcSaveMap.right + 2, 2, ICONSIZE, ICONSIZE);
	_rcLayer[1] = RectMake(_rcLayer[0].right + 2, 2, ICONSIZE, ICONSIZE);
	_rcLayer[2] = RectMake(_rcLayer[1].right + 2, 2, ICONSIZE, ICONSIZE);
	_rcEvent = RectMake(_rcLayer[2].right + 2, 2, ICONSIZE, ICONSIZE);
	_rcEraser = RectMake(_rcEvent.right + 2, 2, ICONSIZE, ICONSIZE);
	_rcPen = RectMake(_rcEraser.right + 2, 2, ICONSIZE, ICONSIZE);
	_rcRectangle = RectMake(_rcPen.right + 2, 2, ICONSIZE, ICONSIZE);
	_rcPaint = RectMake(_rcRectangle.right + 2, 2, ICONSIZE, ICONSIZE);

	_rcPalette = RectMake(0, _rcTool.bottom, 40 * SAMPLETILEX, 40 * SAMPLETILEY);
	_rcMapList = RectMake(0, _rcPalette.bottom, 40 * SAMPLETILEX, WINSIZEY - _rcPalette.bottom);
	_rcTileScreen = RectMake(480, _rcTool.bottom, TILEX*TILESIZEX, TILEY*TILESIZEY);
	_mapAmount = PLAYERDATA->getMapAmount();
	_tileAmount = PLAYERDATA->getTileAmount();
	_editMode = false;
	_editMoveDirect = false;
	_setSaveLoad = false;
	_setSaveSlot = 3;
	//맵툴세팅
	this->maptoolSetup();
	
	_palettePage = 1;
	sprintf_s(_dataName, "Data/Tiles/tile%d.tiledata", _palettePage);
	//현재타일(0,0)좌표로 초기화
	_currentTile.pageNumber = _palettePage;
	_currentTile.x = 0;
	_currentTile.y = 0;

	//현재 드래그 좌표 초기화

	_drawMode = 0;
	_layer = 0;

	setTiles();

	return S_OK;
}

void maptoolScene::release()
{
	PLAYERDATA->setMapAmount(_mapAmount);
	PLAYERDATA->setTileAmount(_tileAmount);
}

void maptoolScene::update()
{
	if (_editMode)
	{
		if (INPUT->GetKeyDown(VK_D))
		{
			_editMode = false;
			sprintf_s(_dataName, "Data/Tiles/tile%d.tiledata", _palettePage);
			saveMapData(_dataName);
		}
		if (INPUT->GetKeyDown(VK_1))
		{
			_editCanMove = true;
			_editMoveDirect = false;
		}
		if (INPUT->GetKeyDown(VK_2))
		{
			_editCanMove = false;
			_editMoveDirect = true;
		}

		if (_editCanMove)
		{
			if (INPUT->GetKeyDown(VK_LBUTTON) && _ptMouse.x < 535)
			{
				for (int i = 0; i < SAMPLETILEX * SAMPLETILEY; i++)
				{
					if (PtInRect(&_sampleTile[i].rc, _ptMouse))
					{
						_sampleTile[i].canMove = !_sampleTile[i].canMove;
						break;
					}
				}
			}
		}
		if (_editMoveDirect)
		{
			for (int i = 0; i < SAMPLETILEX * SAMPLETILEY; i++)
			{
				if (PtInRect(&_sampleTile[i].rc, _ptMouse))
				{
					if (INPUT->GetKeyUp(VK_UP)) { _sampleTile[i].direct[0] = !_sampleTile[i].direct[0]; }
					if (INPUT->GetKeyUp(VK_DOWN)) { _sampleTile[i].direct[1] = !_sampleTile[i].direct[1]; }
					if (INPUT->GetKeyUp(VK_LEFT)) { _sampleTile[i].direct[2] = !_sampleTile[i].direct[2]; }
					if (INPUT->GetKeyUp(VK_RIGHT)) { _sampleTile[i].direct[3] = !_sampleTile[i].direct[3]; }
					break;
				}
			}
		}
	}

	else if (_setSaveLoad) // 세이브 확인창이 켜졌을때
	{
		if (INPUT->GetKeyDown(VK_LBUTTON))
		{
			for (size_t i = 0; i < _rcSaveSlot.size(); i++)
			{
				if (PtInRect(&_rcSaveSlot[i], _ptMouse))
				{
					sprintf_s(_mapName, "Data/Map/save%d.mapsave", i);
				}
			}
			//세이브 부분 수정 필요
			//if (_setSaveSlot == 0) { sprintf_s(_mapName, "Data/Map/save1.mapsave"); }	//슬롯1번파일으로 이름변경
			//else if (_setSaveSlot == 1) { sprintf_s(_mapName, "save2.mapsave"); }	//슬롯2번파일으로 이름변경
			//else if (_setSaveSlot == 2) { sprintf_s(_mapName, "save3.mapsave"); }	//슬롯3번파일으로 이름변경
		}
		if (INPUT->GetKeyUp(VK_LBUTTON))
		{
			if (!PtInRect(&_rcSaveWindow, _ptMouse)) // 세이브 확인창 밖을 클릭하면 세이브창 닫기
			{
				_setSaveLoad = false;
				_setSaveSlot = 3;
				return;
			}
			else if (PtInRect(&_rcSave, _ptMouse))
			{
				this->save(_mapName);
				_setSaveLoad = false;
				return;
			}
			//세이브완료
			else if (PtInRect(&_rcLoad, _ptMouse))
			{
				this->load(_mapName);
				_setSaveLoad = false;
				return;
			}
			//로드완료
		}
	}
	
	else
	{
		_rcMouse = RectMakeCenter(_ptMouse.x, _ptMouse.y, TILESIZE, TILESIZE);

		//#====================================================================================================
		//#				※※※ 필독 ※※※ 사용 방법
		//#				에디터를 사용하기 전에 칠한 타일은 에디터를 사용하더라도 데이터 변경 X
		//#				추후에 적용 되도록 작업할 예정이며
		//#				그 전에 에디터 맵 저장하면 인게임에서 난리납니다
		//#=====================================================================================================
		if (INPUT->GetKeyDown(VK_D))
		{
			//샘플타일 데이터 에디트 
			loadMapData(_dataName);
			_editMode = true;
		}

		//화면 이동
		if (INPUT->GetKeyDown(VK_UP))
		{
			if (_rcTileScreen.top > 40)
			{
				_rcTileScreen.bottom -= (_rcTileScreen.top - 40);
				_rcTileScreen.top -= (_rcTileScreen.top - 40);
			}
			else if (_rcTileScreen.top == 40) { return; }
			else
			{
				_rcTileScreen.top += 10;
				_rcTileScreen.bottom += 10;
			}

			for (int i = 0; i < _tiles.xSize * _tiles.ySize; i++)
			{
				_tiles.tile[i].rc.top = _rcTileScreen.top + (i / _tiles.xSize) * 40;
				_tiles.tile[i].rc.bottom = _rcTileScreen.top + 40 + (i / _tiles.xSize) * 40;
			}
		}
		if (INPUT->GetKeyDown(VK_DOWN))
		{
			if (_tiles.tile[_tiles.xSize * _tiles.ySize - 1].rc.bottom < WINSIZEY)
			{
				for (int i = _tiles.xSize * _tiles.ySize - 1; i >= 0; i--)
				{
					_tiles.tile[i].rc.bottom = (i / _tiles.ySize) * 40;
					_tiles.tile[i].rc.top = _tiles.tile[i].rc.bottom - 40;
				}
			}
			else if (_tiles.tile[TILEX * TILEY - 1].rc.bottom == WINSIZEY) { return; }
			else
			{
				_rcTileScreen.top -= 10;
				_rcTileScreen.bottom -= 10;
			}
			for (int i = 0; i < _tiles.xSize * _tiles.ySize; i++)
			{
				_tiles.tile[i].rc.top = _rcTileScreen.top + (i / _tiles.xSize) * 40;
				_tiles.tile[i].rc.bottom = _rcTileScreen.top + 40 + (i / _tiles.xSize) * 40;
			}
		}
		if (INPUT->GetKeyDown(VK_RIGHT))
		{
			if (_tiles.tile[TILEX*TILEY-1].rc.right < WINSIZEX)
			{
				for (int i = TILEX * TILEY - 1; i >= 0; i--)
				{
					_tiles.tile[i].rc.right = (i % TILEX) * 40;
					_tiles.tile[i].rc.left = _tiles.tile[i].rc.right - 40;
				}
			}
			else if (_tiles.tile[TILEX * TILEY - 1].rc.right == WINSIZEX) { return; }
			else
			{
				_rcTileScreen.left -= 10;
				_rcTileScreen.right -= 10;
			}
		}
		if (INPUT->GetKeyDown(VK_LEFT))
		{
			if (_tiles.tile[0].rc.left > 480)
			{
				for (int i = 0; i < TILEX * TILEY; i++)
				{
					_tiles.tile[i].rc.left = 480 + (i % TILEX) * 40;
					_tiles.tile[i].rc.right = 520 + (i % TILEX) * 40;
				}
			}
			else if (_tiles.tile[0].rc.left == 480) { return; }
			else
			{
				_rcTileScreen.left += 10;
				_rcTileScreen.right += 10;
			}
		}

		//포인터가 샘플타일 위에 있다면 샘플타일 선택
		//포인터가 맵타일 위에 있다면 선택되어 있는 샘플타일을 맵타일에 칠해준다.

		if (PtInRect(&_rcTileScreen, _ptMouse))
		{
			if (INPUT->GetKey(VK_LBUTTON))
			{
				if (_drawMode == 3) { setAllMap(); }
				else if (_drawMode == 2) { setRectangle(); }
				else if (_drawMode == 1) { setMap(); }
				else if (_drawMode == 0) {}
				else { return; }
			}
		}

		if(INPUT->GetKeyUp(VK_LBUTTON))
		{ 
			//새 맵 만들기
			if (PtInRect(&_rcNewMap, _ptMouse)) {}
			//모든 맵 저장
			if (PtInRect(&_rcSaveMap, _ptMouse)) {}
			//1층 레이어 선택하기
			if (PtInRect(&_rcLayer[0], _ptMouse))
			{
				_layer = 1;
			}
			//2층 레이어 선택하기
			if (PtInRect(&_rcLayer[1], _ptMouse)) 
			{
				_layer = 2; 
			}
			//3층 레이어 선택하기
			if (PtInRect(&_rcLayer[2], _ptMouse)) 
			{
				_layer = 3; 
			}
			//이벤트 레이어 선택하기
			if (PtInRect(&_rcEvent, _ptMouse)) 
			{
				_layer = 0; 
			}
			//그리는 방식을 지우기 모드로 변경
			if (PtInRect(&_rcEraser, _ptMouse))
			{
				if (_layer != 0)
				{
					_drawMode = 0;
				}
			}
			//그리는 방식을 펜으로 변경
			if (PtInRect(&_rcPen, _ptMouse))
			{
				if (_layer != 0)
				{
					_drawMode = 1;
				}
			}//펜 모드
			if (PtInRect(&_rcRectangle, _ptMouse))
			{
				if (_layer != 0)
				{
					_drawMode = 2;
				}
			}	//사각형 그리기
			if (PtInRect(&_rcPaint, _ptMouse)) 
			{
				if (_layer != 0)
				{
					_drawMode = 3;
				}
			}		//타일 전체 칠하기


			if (PtInRect(&_rcPalette, _ptMouse))
			{
				selectSampleTile();
			}

			if (PtInRect(&_rcDummy2, _ptMouse))
			{
				SCENEMANAGER->loadScene("시작화면");		//게임 화면으로 돌아가기
			}
			if (PtInRect(&_rcArrow5[0], _ptMouse))
			{
				if (_palettePage > 5)		//샘플타일의 번호가 5보다 클 때
				{
					_palettePage -= 5;													//샘플타일의 번호를 5 감소
					sprintf_s(_dataName, "Data/Tiles/tile%d.tiledata", _palettePage);	//타일의 이름과 경로, 확장자 명을 _dataName에 저장하고
					loadMapData(_dataName);												//해당 타일의 정보를 팔레트에 불러온다.
				}
			}
			if (PtInRect(&_rcArrow5[1], _ptMouse))
			{
				if (_palettePage <= _tileAmount - 5)		//샘플타일의 번호와 샘플타일의 총 갯수의 차이가 5일 때
				{
					_palettePage += 5;													//샘플타일의 번호를 5 증가
					sprintf_s(_dataName, "Data/Tiles/tile%d.tiledata", _palettePage);	//타일의 이름과 경로, 확장자 명을 _dataName에 저장하고
					loadMapData(_dataName);												//해당 샘플타일을 팔레트에 그려준다.
				}
				else if (_palettePage == _tileAmount) { return; }
				else
				{
					//샘플 타일의 번호와 샘플 타일의 총 갯수의 차이가 5보다 작을 때
					_palettePage = _tileAmount;											//샘플타일의 번호를 샘플타일의 총 갯수로 변경
					sprintf_s(_dataName, "Data/Tiles/tile%d.tiledata", _palettePage);	//타일의 이름과 경로, 확장자 명을 _dataName에 저장하고
					loadMapData(_dataName);												//해당 샘플타일을 팔레트에 그려준다.
				}
			}
			if (PtInRect(&_rcArrow[0], _ptMouse))
			{
				if (_palettePage > 1)		//샘플타일의 번호가 1보다 클 때
				{
					_palettePage--;														//샘플타일의 번호를 1 감소
					sprintf_s(_dataName, "Data/Tiles/tile%d.tiledata", _palettePage);	//타일의 이름과 경로, 확장자 명을 _dataName에 저장하고
					loadMapData(_dataName);												//해당 샘플타일을 팔레트에 그려준다.
				}
				else { return; }
			}
			if (PtInRect(&_rcArrow[1], _ptMouse))
			{
				if (_palettePage < _tileAmount)		//샘플타일의 번호가 최대값보다 작을 때
				{
					_palettePage++;														//샘플타일의 번호를 1 증가시키고
					sprintf_s(_dataName, "Data/Tiles/tile%d.tiledata", _palettePage);	//타일의 이름과 경로, 확장자 명을 _dataName에 저장하고
					loadMapData(_dataName);												//해당 샘플타일을 팔레트에 그려준다.
				}
			}
		}


		if (_rcPalette.top > WINSIZEY) // 최소화시 화면밖으로 나가면서 이하만큼남앗을때 버튼생성(이동) ※맵툴 셋업보다 밑에있어야함
		{
			_rcSaveLoad = RectMake(_rcPalette.left, _rcScreen.bottom - TILESIZE, TILESIZE * 2, TILESIZE);		//맵툴 UI
			_rcDummy2 = RectMake(_rcPalette.left + TILESIZE * 4, WINSIZEY - TILESIZE, TILESIZE * 2, TILESIZE);	//맵툴 UI
			_rcArrow5[0] = RectMake(_rcPalette.left + TILESIZE * 8, WINSIZEY - TILESIZE, TILESIZE, TILESIZE);	//맵툴 UI
			_rcArrow5[1] = RectMake(_rcPalette.left + TILESIZE * 9, WINSIZEY - TILESIZE, TILESIZE, TILESIZE);	//맵툴 UI
			_rcArrow[0] = RectMake(_rcPalette.left + TILESIZE * 10, WINSIZEY - TILESIZE, TILESIZE, TILESIZE);	//맵툴 UI
			_rcArrow[1] = RectMake(_rcPalette.left + TILESIZE * 11, WINSIZEY - TILESIZE, TILESIZE, TILESIZE);	//맵툴 UI
		}

		
	}
}



void maptoolScene::render()
{

	for (int i = 0; i < _tiles.xSize * _tiles.ySize; i++)
	{
		RECT rc;
		sprintf_s(_palettePageNum, "%d", i);
		if (IntersectRect(&rc, &_rcScreen, &_tiles.tile[i].rc))
		{
			Rectangle(getMemDC(), _tiles.tile[i].rc);
			if (_tiles.tile[i].imagePage[0] != -1)
			{
				sprintf_s(_fileName, "Tile%d", _tiles.tile[i].imagePage[0]);
				IMAGEMANAGER->scaleFrameRender(_fileName, getMemDC(), _tiles.tile[i].rc.left, _tiles.tile[i].rc.top,
					_tiles.tile[i].tileFrameX[0], _tiles.tile[i].tileFrameY[0], 2.0f);
			}
		}
		else { continue; }
		TextOut(getMemDC(), _tiles.tile[i].rc.left, _tiles.tile[i].rc.top, _palettePageNum, strlen(_palettePageNum));
	}

	for (int i = 0; i < _tiles.xSize * _tiles.ySize; i++)
	{
		RECT rc;
		if (IntersectRect(&rc, &_rcScreen, &_tiles.tile[i].rc))
		{
			if (_tiles.tile[i].imagePage[0] == -1 && _tiles.tile[i].imagePage[1] == -1) { continue; }
			else
			{
				sprintf_s(_fileName, "Tile%d", _tiles.tile[i].imagePage[1]);
				IMAGEMANAGER->scaleFrameRender(_fileName, getMemDC(), _tiles.tile[i].rc.left, _tiles.tile[i].rc.top, _tiles.tile[i].tileFrameX[1], _tiles.tile[i].tileFrameY[1], 2.0f);
			}
		}
		else { continue; }
	}

	for (int i = 0; i < _tiles.xSize * _tiles.ySize; i++)
	{
		RECT rc;
		if (IntersectRect(&rc, &_rcScreen, &_tiles.tile[i].rc))
		{
			if (_tiles.tile[i].imagePage[0] == -1 && _tiles.tile[i].imagePage[1] == -1 && _tiles.tile[i].imagePage[2] == -1) { continue; }
			else
			{
				sprintf_s(_fileName, "Tile%d", _tiles.tile[i].imagePage[2]);
				IMAGEMANAGER->scaleFrameRender(_fileName, getMemDC(), _tiles.tile[i].rc.left, _tiles.tile[i].rc.top, _tiles.tile[i].tileFrameX[2], _tiles.tile[i].tileFrameY[2], 2.0f);
			}
		}
		else { continue; }
	}




	//레이어 이미지 그리기
	//if (_layer[0]) { IMAGEMANAGER->findImage("Layer")->frameRender(getMemDC(), _rcScreen.right - 128, _rcScreen.top, 0, 0); }
	//else if (_layer[1]) { IMAGEMANAGER->findImage("Layer")->frameRender(getMemDC(), _rcScreen.right - 128, _rcScreen.top, 1, 0); }
	//else { IMAGEMANAGER->findImage("Layer")->frameRender(getMemDC(), _rcScreen.right - 128, _rcScreen.top, 2, 0); }


	for (int i = 0; i < 60; i++)
	{
		HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
		HBRUSH oldBrush = (HBRUSH)SelectObject(getMemDC(), brush);
		Rectangle(getMemDC(), _sampleTile[i].rc);
		SelectObject(getMemDC(), oldBrush);
		DeleteObject(brush);

		//하단의 샘플타일 랜더 스케일랜더로 20->40픽셀로 확대

		_sampleTile[i].tileFrameX = i % 10;
		_sampleTile[i].tileFrameY = i / 10;
		_sampleTile[i].imagePage = _palettePage;

		sprintf_s(_pageName, "Tile%d", _palettePage);
		IMAGEMANAGER->findImage(_pageName)->scaleFrameRender(getMemDC(), _sampleTile[i].rc.left, _sampleTile[i].rc.top, _sampleTile[i].tileFrameX, _sampleTile[i].tileFrameY, 2.0f);

		if (_editMode && _editCanMove)
		{
			if (_sampleTile[i].canMove == true) { sprintf_s(_canMoveChar, "%d", 1); }
			else { sprintf_s(_canMoveChar, "%d", 0); }

			TextOut(getMemDC(), _sampleTile[i].rc.left, _sampleTile[i].rc.top, _canMoveChar, strlen(_canMoveChar));
		}

		if (_editMode && _editMoveDirect)
		{
			for (int j = 0; j < 4; j++)
			{
				if (_sampleTile[i].direct[j] == true) { _moveDirectChar[j] = 'T'; }
				else { _moveDirectChar[j] = 'F'; }
			}
			TextOut(getMemDC(), _sampleTile[i].rc.left, _sampleTile[i].rc.top, _moveDirectChar, strlen(_moveDirectChar));
		}
	}

	IMAGEMANAGER->findImage("SaveLoadOff")->render(getMemDC(), _rcSaveLoad.left, _rcSaveLoad.top);
	if (PtInRect(&_rcSaveLoad, _ptMouse)) {
		IMAGEMANAGER->findImage("SaveLoad")->render(getMemDC(), _rcSaveLoad.left, _rcSaveLoad.top);
	}

	IMAGEMANAGER->findImage("HomeOff")->render(getMemDC(), _rcDummy2.left, _rcDummy2.top);
	if (PtInRect(&_rcDummy2, _ptMouse)) {
		IMAGEMANAGER->findImage("Home")->render(getMemDC(), _rcDummy2.left, _rcDummy2.top);
	}

	HBRUSH brush = CreateSolidBrush(RGB(16, 64, 168));
	HBRUSH oldBrush = (HBRUSH)SelectObject(getMemDC(), brush);
	Rectangle(getMemDC(), _rcArrow5[0].left, _rcArrow5[0].top, _rcArrow5[1].right, _rcArrow5[1].bottom);
	Rectangle(getMemDC(), _rcArrow[0].left, _rcArrow[0].top, _rcArrow[1].right, _rcArrow[1].bottom);
	SelectObject(getMemDC(), oldBrush);
	DeleteObject(brush);


	//샘플타일의 번호가 1보다 작은 경우는 없으므로 1보다 큰 경우에만 이전 타일로 넘어가는 화살표를 보여준다.
	if (_palettePage > 1) { IMAGEMANAGER->findImage("LeftArrow")->render(getMemDC(), _rcArrow[0].left, _rcArrow[0].top); }
	if (_palettePage > 1) { IMAGEMANAGER->findImage("LeftArrow5")->render(getMemDC(), _rcArrow5[0].left, _rcArrow5[0].top); }
	//샘플타일의 번호가 최대값보다 큰 경우는 없으므로 최대값보다 작은 경우에만 다음 타일로 넘어가는 화살표를 보여준다.
	if (_palettePage < _tileAmount) { IMAGEMANAGER->findImage("RightArrow")->render(getMemDC(), _rcArrow[1].left, _rcArrow[1].top); }
	if (_palettePage < _tileAmount) { IMAGEMANAGER->findImage("RightArrow5")->render(getMemDC(), _rcArrow5[1].left, _rcArrow5[1].top); }

	if (_setSaveLoad == true)
	{
		Rectangle(getMemDC(), _rcSaveWindow);
		IMAGEMANAGER->findImage("textbox")->render(getMemDC(), _rcSaveWindow.left, _rcSaveWindow.top, 0, 0,
			_rcSaveWindow.right - _rcSaveWindow.left, _rcSaveWindow.bottom - _rcSaveWindow.top);

		//Rectangle(getMemDC(), _rcSaveSlot[0]);
		//Rectangle(getMemDC(), _rcSaveSlot[1]);
		//Rectangle(getMemDC(), _rcSaveSlot[2]);	
		for (int i = 0; i < _tileAmount; i++)
		{
			if (PtInRect(&_rcSaveSlot[i], _ptMouse))
			{
				IMAGEMANAGER->findImage("activetextbox")->render(getMemDC(), _rcSaveSlot[i].left, _rcSaveSlot[i].top, 0, 0,
					_rcSaveSlot[i].right - _rcSaveSlot[i].left, _rcSaveSlot[i].bottom - _rcSaveSlot[i].top);
			}//마우스 가져가면 활성화

			if (_setSaveSlot == i)
			{
				IMAGEMANAGER->findImage("activetextbox")->render(getMemDC(), _rcSaveSlot[i].left, _rcSaveSlot[i].top, 0, 0,
					_rcSaveSlot[i].right - _rcSaveSlot[i].left, _rcSaveSlot[i].bottom - _rcSaveSlot[i].top);
			}
		}


		FrameRect(getMemDC(), _rcSaveSlot[0], RGB(255, 255, 255));				 //세이브슬롯 구분해주기
		FrameRect(getMemDC(), _rcSaveSlot[1], RGB(255, 255, 255));				 //세이브슬롯 구분해주기
		FrameRect(getMemDC(), _rcSaveSlot[2], RGB(255, 255, 255));				 //세이브슬롯 구분해주기
	}
	drawToolLayer();
	Rectangle(getMemDC(), _rcMapList);

}

void maptoolScene::maptoolSetup()
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			_sampleTile[i * 10 + j].rc = RectMake(_rcPalette.left + (j * TILESIZE), _rcPalette.top + (i * TILESIZE), TILESIZE, TILESIZE);
			_sampleTile[i * 10 + j].tileFrameX = j;
			_sampleTile[i * 10 + j].tileFrameY = i;
		}
	}

	_rcSaveLoad = RectMake(_rcPalette.left + TILESIZE * 10, _rcPalette.top, TILESIZE * 2, TILESIZE);					//맵툴 UI 1번칸
	_rcDummy2 =	RectMake(_rcPalette.left + TILESIZE * 10, _rcPalette.top + TILESIZE * 2, TILESIZE * 2, TILESIZE);		//맵툴 UI 3번칸
	_rcArrow5[0] = RectMake(_rcPalette.left + TILESIZE * 10, _rcPalette.top + TILESIZE * 4, TILESIZE, TILESIZE);
	_rcArrow5[1] = RectMake(_rcPalette.left + TILESIZE * 11, _rcPalette.top + TILESIZE * 4, TILESIZE, TILESIZE);
	_rcArrow[0] = RectMake(_rcPalette.left + TILESIZE * 10, _rcPalette.top + TILESIZE * 5, TILESIZE, TILESIZE);			//맵툴 UI 맨아래번칸
	_rcArrow[1] = RectMake(_rcPalette.left + TILESIZE * 11, _rcPalette.top + TILESIZE * 5, TILESIZE, TILESIZE);			//맵툴 UI 맨아래번칸

	_rcSaveWindow = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2 - 50, 300, 250);					    //세이브로드UI
	for (int i = 0; i < _mapAmount; i++)																	    //세이브로드UI
	{																									//세이브로드UI
		_rcSaveSlot[i] = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2 - 125 + i * 49, 250, 50);			//세이브로드UI
	}																									//세이브로드UI
	_rcSave = RectMakeCenter(WINSIZEX / 2 - 70, WINSIZEY / 2 + 35, 80, 35);							//세이브로드UI
	_rcLoad = RectMakeCenter(WINSIZEX / 2 + 70, WINSIZEY / 2 + 35, 80, 35);							//세이브로드UI
}

void maptoolScene::selectSampleTile()
{
	for (int i = 0; i < SAMPLETILEX * SAMPLETILEY; i++)
	{
		if (PtInRect(&_sampleTile[i].rc, _ptMouse))
		{

			_currentTile.pageNumber = _palettePage;
			_currentTile.x = _sampleTile[i].tileFrameX;
			_currentTile.y = _sampleTile[i].tileFrameY;
			_canMove = _sampleTile[i].canMove;
			break;
		}
	}
}



void maptoolScene::setMap()
{
	if (_layer != 0) 
	{
		for (int i = 0; i < _tiles.xSize * _tiles.ySize; i++)
		{
			if (PtInRect(&_tiles.tile[i].rc, _ptMouse))
			{
				_tiles.tile[i].canMove[_layer - 1] = _canMove;
				_tiles.tile[i].tileFrameX[_layer - 1] = _currentTile.x;
				_tiles.tile[i].tileFrameY[_layer - 1] = _currentTile.y;
				if (_drawMode == 0) { _tiles.tile[i].imagePage[_layer -1] = -1; }
				else { _tiles.tile[i].imagePage[_layer - 1] = _currentTile.pageNumber; }
				_tiles.tile[i].terrain = terrainSelect(_currentTile.x, _currentTile.y);

			}
		}
	}
}

void maptoolScene::setRectangle()
{
	for (int i = 0; i < _tiles.xSize * _tiles.ySize; i++)
	{
		if (PtInRect(&_tiles.tile[i].rc, _ptMouse))
		{
			if (_drag.startX == -1 && _drag.startY == -1)
			{
				_drag.startX = i % _tiles.xSize;
				_drag.startY = i / _tiles.xSize;
			}
			else
			{
				_drag.endX = i % _tiles.xSize;
				_drag.endY = i % _tiles.ySize;
				break;
			}
		}
	}
	if (_drag.startX > _drag.endX)
	{
		int tmp;
		tmp = _drag.startX;
		_drag.startX = _drag.endX;
		_drag.endX = tmp;
	}
	if (_drag.startY > _drag.endY)
	{
		int tmp;
		tmp = _drag.startY;
		_drag.startY = _drag.endY;
		_drag.endY = tmp;
	}
}

void maptoolScene::setAllMap() 
{
	if (_layer != 0)
	{
		for (int i = 0; i < _tiles.xSize * _tiles.ySize; i++) 
		{
			if (_layer == 1)
			{
				_tiles.tile[i].canMove[_layer - 1] = _canMove;
				_tiles.tile[i].imagePage[_layer - 1] = _currentTile.pageNumber;
				_tiles.tile[i].tileFrameX[_layer - 1] = _currentTile.x;
				_tiles.tile[i].tileFrameY[_layer - 1] = _currentTile.y;
				_tiles.tile[i].terrain = terrainSelect(_currentTile.x, _currentTile.y);
			}
		}
	}
}

void maptoolScene::save(char* str)
{
	HANDLE file;
	DWORD write;

	/*for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			_saveTile[i * TILEX + j] = _tiles[i * TILEX + j];
			_saveTile[i * TILEX + j].rc = RectMake(TILESIZE * j, TILESIZE * i, TILESIZE, TILESIZE);
		}
	}

	file = CreateFile(str, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(file, _saveTile, sizeof(tagTile) * TILEX * TILEY, &write, NULL);
	CloseHandle(file);*/
}
void maptoolScene::load(char* str)
{
	HANDLE file;
	DWORD read;

	//file = CreateFile(str, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	//ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	//CloseHandle(file);
}

void maptoolScene::saveMapData(char* str)
{
	HANDLE file;
	DWORD write;

	file = CreateFile(str, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (file == INVALID_HANDLE_VALUE)
	{
		file = CreateFile(str, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		WriteFile(file, _sampleTile, sizeof(tagSampleTile) * (SAMPLETILEX - 2) * SAMPLETILEY, &write, NULL);
	}
	else
	{
		WriteFile(file, _sampleTile, sizeof(tagSampleTile) * (SAMPLETILEX - 2) * SAMPLETILEY, &write, NULL);
	}
	CloseHandle(file);
}

void maptoolScene::loadMapData(char* str)
{
	HANDLE file;
	DWORD read;

	file = CreateFile(str, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (file == INVALID_HANDLE_VALUE)
	{
		saveMapData(str);
	}
	else
	{
		ReadFile(file, _sampleTile, sizeof(tagSampleTile) * (SAMPLETILEX - 2) * SAMPLETILEY, &read, NULL);
	}
	CloseHandle(file);
}

void maptoolScene::drawToolLayer()
{
	Rectangle(getMemDC(), _rcTool);
	if (PtInRect(&_rcNewMap, _ptMouse))
	{
		HBRUSH brush = CreateSolidBrush(RGB(172, 172, 172));
		HBRUSH oldBrush = (HBRUSH)SelectObject(getMemDC(), brush);
		Rectangle(getMemDC(), _rcNewMap);
		SelectObject(getMemDC(), oldBrush);
		DeleteObject(brush);
	}
	else
	{
		Rectangle(getMemDC(), _rcNewMap);
	}
	IMAGEMANAGER->render("NewMap", getMemDC(), _rcNewMap.left + 3, _rcNewMap.top + 3);
	if (PtInRect(&_rcSaveMap, _ptMouse))
	{
		HBRUSH brush = CreateSolidBrush(RGB(172, 172, 172));
		HBRUSH oldBrush = (HBRUSH)SelectObject(getMemDC(), brush);
		Rectangle(getMemDC(), _rcSaveMap);
		SelectObject(getMemDC(), oldBrush);
		DeleteObject(brush);
	}
	else
	{
		Rectangle(getMemDC(), _rcSaveMap);
	}
	IMAGEMANAGER->render("SaveMap", getMemDC(), _rcSaveMap.left + 3, _rcSaveMap.top + 3);
	for (int i = 0; i < 3; i++)
	{
		if(PtInRect(&_rcLayer[i], _ptMouse))
		{
			HBRUSH brush = CreateSolidBrush(RGB(172, 172, 172));
			HBRUSH oldBrush = (HBRUSH)SelectObject(getMemDC(), brush);
			Rectangle(getMemDC(), _rcLayer[i]);
			SelectObject(getMemDC(), oldBrush);
			DeleteObject(brush);
		}
		else
		{
			Rectangle(getMemDC(), _rcLayer[i]);
		}
		IMAGEMANAGER->frameRender("MapLayer", getMemDC(), _rcLayer[i].left + 3, _rcLayer[i].top + 3, i, 1);
	}
	if(PtInRect(&_rcEvent, _ptMouse))
	{
		HBRUSH brush = CreateSolidBrush(RGB(172, 172, 172));
		HBRUSH oldBrush = (HBRUSH)SelectObject(getMemDC(), brush);
		Rectangle(getMemDC(), _rcEvent);
		SelectObject(getMemDC(), oldBrush);
		DeleteObject(brush);
	}
	else
	{
	Rectangle(getMemDC(), _rcEvent);
	}
	IMAGEMANAGER->render("Event", getMemDC(), _rcEvent.left + 3, _rcEvent.top + 3);
	if (PtInRect(&_rcEraser, _ptMouse))
	{
		HBRUSH brush = CreateSolidBrush(RGB(172, 172, 172));
		HBRUSH oldBrush = (HBRUSH)SelectObject(getMemDC(), brush);
		Rectangle(getMemDC(), _rcEraser);
		SelectObject(getMemDC(), oldBrush);
		DeleteObject(brush);
	}
	else
	{
		Rectangle(getMemDC(), _rcEraser);
	}
	IMAGEMANAGER->render("Eraser", getMemDC(), _rcEraser.left + 3, _rcEraser.top + 3);
	if (PtInRect(&_rcPen, _ptMouse))
	{
		HBRUSH brush = CreateSolidBrush(RGB(172, 172, 172));
		HBRUSH oldBrush = (HBRUSH)SelectObject(getMemDC(), brush);
		Rectangle(getMemDC(), _rcPen);
		SelectObject(getMemDC(), oldBrush);
		DeleteObject(brush);
	}
	else
	{
		Rectangle(getMemDC(), _rcPen);
	}
	IMAGEMANAGER->render("Pencle", getMemDC(), _rcPen.left + 3, _rcPen.top + 3);
	if (PtInRect(&_rcRectangle, _ptMouse))
	{
		HBRUSH brush = CreateSolidBrush(RGB(172, 172, 172));
		HBRUSH oldBrush = (HBRUSH)SelectObject(getMemDC(), brush);
		Rectangle(getMemDC(), _rcRectangle);
		SelectObject(getMemDC(), oldBrush);
		DeleteObject(brush);
	}
	else
	{
		Rectangle(getMemDC(), _rcRectangle);
	}
	IMAGEMANAGER->render("Rectangle", getMemDC(), _rcRectangle.left + 3, _rcRectangle.top + 3);
	if(PtInRect(&_rcPaint, _ptMouse))
	{
		HBRUSH brush = CreateSolidBrush(RGB(172, 172, 172));
		HBRUSH oldBrush = (HBRUSH)SelectObject(getMemDC(), brush);
		Rectangle(getMemDC(), _rcPaint);
		SelectObject(getMemDC(), oldBrush);
		DeleteObject(brush);
	}
	else
	{
	Rectangle(getMemDC(), _rcPaint);
	}
	IMAGEMANAGER->render("Paint", getMemDC(), _rcPaint.left + 3, _rcPaint.top + 3);
}

void maptoolScene::setTiles()
{
	for (int i = 0; i < _tiles.xSize; i++)
	{
		for (int j = 0; j < _tiles.ySize; j++)
		{
			_addTile.rc = RectMake(_rcTileScreen.left + (j * 40), _rcTileScreen.top + (i * 40), TILESIZE, TILESIZE);
			_tiles.tile.push_back(_addTile);
		}
	}
}

void maptoolScene::frameBoxRender(int left, int top, int width, int height, float scale)
{
	IMAGEMANAGER->findImage("FrameL")->scaleRender(getMemDC(), left, top, 0, 0, 17 * scale, height, scale);
	IMAGEMANAGER->findImage("FrameR")->scaleRender(getMemDC(), left + width - (17 * scale), top, 0, 0, 17 * scale, height, scale);
	IMAGEMANAGER->findImage("FrameT")->scaleRender(getMemDC(), left, top, 0, 0, width, 17 * scale, scale);
	IMAGEMANAGER->findImage("FrameB")->scaleRender(getMemDC(), left, top + height - (17 * scale), 0, 0, width, 17 * scale, scale);


	//프레임바디
	IMAGEMANAGER->findImage("FrameLT")->scaleRender(getMemDC(), left, top, scale);
	IMAGEMANAGER->findImage("FrameRT")->scaleRender(getMemDC(), left + width - (17 * scale), top, scale);
	IMAGEMANAGER->findImage("FrameLB")->scaleRender(getMemDC(), left, top + height - (17 * scale), scale);
	IMAGEMANAGER->findImage("FrameRB")->scaleRender(getMemDC(), left + width - (17 * scale), top + height - (17 * scale), scale);
	//모서리

}
void maptoolScene::frameBoxRender(RECT rc, float scale)
{
	IMAGEMANAGER->findImage("FrameL")->scaleRender(getMemDC(), rc.left - 17 * scale, rc.top - 1, 0, 0, 17 * scale, rc.bottom + 1 - rc.top, scale);
	IMAGEMANAGER->findImage("FrameR")->scaleRender(getMemDC(), rc.right, rc.top - 1, 0, 0, 17 * scale, rc.bottom + 1 - rc.top, scale);
	IMAGEMANAGER->findImage("FrameT")->scaleRender(getMemDC(), rc.left - 1, rc.top - 17 * scale, 0, 0, rc.right + 1 - rc.left, 17 * scale, scale);
	IMAGEMANAGER->findImage("FrameB")->scaleRender(getMemDC(), rc.left - 1, rc.bottom, 0, 0, rc.right + 1 - rc.left, 17 * scale, scale);


	//프레임바디
	IMAGEMANAGER->findImage("FrameLT")->scaleRender(getMemDC(), rc.left - 17 * scale, rc.top - 17 * scale, scale);
	IMAGEMANAGER->findImage("FrameRT")->scaleRender(getMemDC(), rc.right, rc.top - 17 * scale, scale);
	IMAGEMANAGER->findImage("FrameLB")->scaleRender(getMemDC(), rc.left - 17 * scale, rc.bottom, scale);
	IMAGEMANAGER->findImage("FrameRB")->scaleRender(getMemDC(), rc.right, rc.bottom, scale);

	//모서리

}

TERRAIN maptoolScene::terrainSelect(int frameX, int frameY)
{
	TERRAIN t;
	t.x = frameX;
	t.y = frameY;

	return t;
}
