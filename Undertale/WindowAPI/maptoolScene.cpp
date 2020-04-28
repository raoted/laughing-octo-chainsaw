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
	_rcEraser = RectMake(_rcLayer[2].right + 2, 2, ICONSIZE, ICONSIZE);
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
	_count = 0;

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
	else
	{
		if (INPUT->GetKeyDown(VK_D))
		{
			//샘플타일 데이터 에디트 
			loadMapData(_dataName);
			_editMode = true;
		}

		//화면 이동
		if (INPUT->GetKey(VK_UP))
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

			for (int i = 0; i < (int)_tiles.tile.size(); i++)
			{
				_tiles.tile[i].rc.top = _rcTileScreen.top + (i / _tiles.xSize) * 40;
				_tiles.tile[i].rc.bottom = _rcTileScreen.top + 40 + (i / _tiles.xSize) * 40;
			}

		}
		if (INPUT->GetKey(VK_DOWN))
		{
			if (_tiles.tile[_tiles.tile.size() - 1].rc.bottom < WINSIZEY)
			{
				for (int i = _tiles.tile.size() - 1; i >= 0; i--)
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
			for (int i = 0; i < (int)_tiles.tile.size(); i++)
			{
				_tiles.tile[i].rc.top = _rcTileScreen.top + (i / _tiles.xSize) * 40;
				_tiles.tile[i].rc.bottom = _rcTileScreen.top + 40 + (i / _tiles.xSize) * 40;
			}
		}
		if (INPUT->GetKey(VK_RIGHT))
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
		if (INPUT->GetKey(VK_LEFT))
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

		
		if (INPUT->GetKeyDown(VK_LBUTTON))
		{
			if (PtInRect(&_rcPalette, _ptMouse))
			{
				selectSampleTile();
			}
			if (PtInRect(&_rcTool, _ptMouse))
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
			}
		}
		if (PtInRect(&_rcTileScreen, _ptMouse))
		{
			if (INPUT->GetKeyUp(VK_LBUTTON) && _drawMode == 2)
			{
				for (int i = 0; i < (int)_tiles.tile.size(); i++)
				{
					if (IntersectRect(&rc, &_rectangle, &_tiles.tile[i].rc))
					{
						_tiles.tile[i].canMove[_layer - 1] = _canMove;
						_tiles.tile[i].tileFrameX[_layer - 1] = _currentTile.x;
						_tiles.tile[i].tileFrameY[_layer - 1] = _currentTile.y;
						_tiles.tile[i].imagePage[_layer - 1] = _currentTile.pageNumber;
						_tiles.tile[i].terrain = terrainSelect(_currentTile.x, _currentTile.y);
					}
					if (_tiles.tile[i].rc.right > _rectangle.right && _tiles.tile[i].rc.bottom >_rectangle.bottom)
					{
						break;
					}
				}
			}
			
			if (INPUT->GetKey(VK_LBUTTON))
			{
				if (_drawMode == 2) { setRectangle(); }
				else if (_drawMode == 3) { setAllMap(); }
				else if (_drawMode == 1) { setMap(); }
				else if (_drawMode == 0) { setRemove(); }
				else { return; }
			}
			else
			{
				for (int i = 0; i < (int)_tiles.tile.size(); i++)
				{
					if (PtInRect(&_tiles.tile[i].rc, _ptMouse))
					{
						_rcMouse = _tiles.tile[i].rc;
						_rectangle = _tiles.tile[i].rc;
						break;
					}
				}
			}
		}
	}
}



void maptoolScene::render()
{
	for (int i = 0; i < (int)_tiles.tile.size(); i++)
	{
		sprintf_s(_palettePageNum, "%d", i);
		if (IntersectRect(&rc, &_rcScreen, &_tiles.tile[i].rc))
		{
			if (_tiles.tile[i].imagePage[0] == -1 && _tiles.tile[i].imagePage[1] == -1 && _tiles.tile[i].imagePage[2] == -1)
			{
				IMAGEMANAGER->findImage("null")->render(getMemDC(), _tiles.tile[i].rc.left, _tiles.tile[i].rc.top);
			}
			else
			{
				if (_tiles.tile[i].imagePage[0] != -1)
				{
					sprintf_s(_fileName, "Tile%d", _tiles.tile[i].imagePage[0]);
					IMAGEMANAGER->scaleFrameRender(_fileName, getMemDC(), _tiles.tile[i].rc.left, _tiles.tile[i].rc.top,
						_tiles.tile[i].tileFrameX[0], _tiles.tile[i].tileFrameY[0], 2.0f);
				}
				if (_tiles.tile[i].imagePage[1] != -1)
				{
					sprintf_s(_fileName, "Tile%d", _tiles.tile[i].imagePage[1]);
					IMAGEMANAGER->scaleFrameRender(_fileName, getMemDC(), _tiles.tile[i].rc.left, _tiles.tile[i].rc.top, _tiles.tile[i].tileFrameX[1], _tiles.tile[i].tileFrameY[1], 2.0f);
				}
				if (_tiles.tile[i].imagePage[2] != -1)
				{
					sprintf_s(_fileName, "Tile%d", _tiles.tile[i].imagePage[2]);
					IMAGEMANAGER->scaleFrameRender(_fileName, getMemDC(), _tiles.tile[i].rc.left, _tiles.tile[i].rc.top, _tiles.tile[i].tileFrameX[2], _tiles.tile[i].tileFrameY[2], 2.0f);
				}
			}
			TextOut(getMemDC(), _tiles.tile[i].rc.left, _tiles.tile[i].rc.top, _palettePageNum, strlen(_palettePageNum));
		}
	}

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

	drawToolLayer();
	Rectangle(getMemDC(), _rcMapList);
	drawMapList();
	if (PtInRect(&_rcScreen, _ptMouse))
	{
		if (INPUT->GetKey(VK_LBUTTON) && _drawMode == 2)
		{
			Rectangle(getMemDC(), _rectangle);
		}
		IMAGEMANAGER->render("tileMouse", getMemDC(), _rcMouse.left, _rcMouse.top);
	}
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

void maptoolScene::setRemove()
{
	if (_layer != 0)
	{
		for (int i = 0; i < (int)_tiles.tile.size(); i++)
		{
			if (PtInRect(&_tiles.tile[i].rc, _ptMouse))
			{
				_rcMouse = _tiles.tile[i].rc;
				_rectangle = _tiles.tile[i].rc;
				break;
			}
		}
		for (int i = 0; i < (int)_tiles.tile.size(); i++)
		{
			if (IntersectRect(&rc, &_tiles.tile[i].rc, &_rcMouse))
			{
				_tiles.tile[i].imagePage[_layer - 1] = -1;
				break;
			}
		}

	}
}

void maptoolScene::setMap()
{
	if (_layer != 0) 
	{
		for (int i = 0; i < (int)_tiles.tile.size(); i++)
		{
			if (PtInRect(&_tiles.tile[i].rc, _ptMouse))
			{
				_rcMouse = _tiles.tile[i].rc;
				_rectangle = _tiles.tile[i].rc;
				break;
			}
		}
		for (int i = 0; i < (int)_tiles.tile.size(); i++)
		{
			if (IntersectRect(&rc, &_tiles.tile[i].rc, &_rcMouse))
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

void maptoolScene::setRectangle()
{
	if (_layer != 0)
	{
		for (int i = 0; i < (int)_tiles.tile.size(); i++)
		{
			if (PtInRect(&_tiles.tile[i].rc, _ptMouse))
			{
				if (_rcMouse.right <= _tiles.tile[i].rc.left)
				{
					_rectangle.left = _rcMouse.left;
					_rectangle.right = _tiles.tile[i].rc.right;
				}
				else
				{
					_rectangle.left = _tiles.tile[i].rc.left;
					_rectangle.right = _rcMouse.right;
				}
				if (_rcMouse.bottom > _tiles.tile[i].rc.top)
				{
					_rectangle.top = _tiles.tile[i].rc.top;
					_rectangle.bottom = _rcMouse.bottom;
				}
				else
				{
					_rectangle.top = _rcMouse.top;
					_rectangle.bottom = _tiles.tile[i].rc.bottom;
				}
			}
		}
	}
}

void maptoolScene::setAllMap() 
{
	if (_layer != 0)
	{
		for (int i = 0; i < (int)_tiles.tile.size(); i++) 
		{
			if (_layer)
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

void maptoolScene::rcRectangleDraw(int startX, int startY, int endX, int endY)
{
	int tmp;
	if (startX > endX)
	{
		tmp = startX;
		startX = endX;
		endX = tmp;
	}
	if (startY > endY)
	{
		tmp = startY;
		startY = endY;
		endY = tmp;
	}
	Rectangle(getMemDC(), startX, startY, endX, endY);
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
	_vTiles.push_back(_tiles);
	_mapList.mapListStr = "MAP" + to_string(_vTiles.size());
	_mapList.mapListRc = RectMake(_rcMapList.left + 20, _rcMapList.top + 20 * _vTiles.size(), _rcMapList.right - 40, 20);
	_vMapList.push_back(_mapList);
}

void maptoolScene::drawMapList()
{
	for (int i = 0; i < (int)_vMapList.size(); i++)
	{
		Rectangle(getMemDC(), _vMapList[i].mapListRc);
		TextOut(getMemDC(), _vMapList[i].mapListRc.left+5, _vMapList[i].mapListRc.top+2, _vMapList[i].mapListStr.c_str(), strlen(_vMapList[i].mapListStr.c_str()));
	}
}


TERRAIN maptoolScene::terrainSelect(int frameX, int frameY)
{
	TERRAIN t;
	t.x = frameX;
	t.y = frameY;

	return t;
}
