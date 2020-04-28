#include "stdafx.h"
#include "maptoolScene.h"

HRESULT maptoolScene::init()
{
	//Ÿ�ϸ� �̹��� �ʱ�ȭ
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
	//��������
	this->maptoolSetup();
	
	_palettePage = 1;
	sprintf_s(_dataName, "Data/Tiles/tile%d.tiledata", _palettePage);
	//����Ÿ��(0,0)��ǥ�� �ʱ�ȭ
	_currentTile.pageNumber = _palettePage;
	_currentTile.x = 0;
	_currentTile.y = 0;

	//���� �巡�� ��ǥ �ʱ�ȭ

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

	else if (_setSaveLoad) // ���̺� Ȯ��â�� ��������
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
			//���̺� �κ� ���� �ʿ�
			//if (_setSaveSlot == 0) { sprintf_s(_mapName, "Data/Map/save1.mapsave"); }	//����1���������� �̸�����
			//else if (_setSaveSlot == 1) { sprintf_s(_mapName, "save2.mapsave"); }	//����2���������� �̸�����
			//else if (_setSaveSlot == 2) { sprintf_s(_mapName, "save3.mapsave"); }	//����3���������� �̸�����
		}
		if (INPUT->GetKeyUp(VK_LBUTTON))
		{
			if (!PtInRect(&_rcSaveWindow, _ptMouse)) // ���̺� Ȯ��â ���� Ŭ���ϸ� ���̺�â �ݱ�
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
			//���̺�Ϸ�
			else if (PtInRect(&_rcLoad, _ptMouse))
			{
				this->load(_mapName);
				_setSaveLoad = false;
				return;
			}
			//�ε�Ϸ�
		}
	}
	
	else
	{
		_rcMouse = RectMakeCenter(_ptMouse.x, _ptMouse.y, TILESIZE, TILESIZE);

		//#====================================================================================================
		//#				�ءء� �ʵ� �ءء� ��� ���
		//#				�����͸� ����ϱ� ���� ĥ�� Ÿ���� �����͸� ����ϴ��� ������ ���� X
		//#				���Ŀ� ���� �ǵ��� �۾��� �����̸�
		//#				�� ���� ������ �� �����ϸ� �ΰ��ӿ��� �������ϴ�
		//#=====================================================================================================
		if (INPUT->GetKeyDown(VK_D))
		{
			//����Ÿ�� ������ ����Ʈ 
			loadMapData(_dataName);
			_editMode = true;
		}

		//ȭ�� �̵�
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

		//�����Ͱ� ����Ÿ�� ���� �ִٸ� ����Ÿ�� ����
		//�����Ͱ� ��Ÿ�� ���� �ִٸ� ���õǾ� �ִ� ����Ÿ���� ��Ÿ�Ͽ� ĥ���ش�.

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
			//�� �� �����
			if (PtInRect(&_rcNewMap, _ptMouse)) {}
			//��� �� ����
			if (PtInRect(&_rcSaveMap, _ptMouse)) {}
			//1�� ���̾� �����ϱ�
			if (PtInRect(&_rcLayer[0], _ptMouse))
			{
				_layer = 1;
			}
			//2�� ���̾� �����ϱ�
			if (PtInRect(&_rcLayer[1], _ptMouse)) 
			{
				_layer = 2; 
			}
			//3�� ���̾� �����ϱ�
			if (PtInRect(&_rcLayer[2], _ptMouse)) 
			{
				_layer = 3; 
			}
			//�̺�Ʈ ���̾� �����ϱ�
			if (PtInRect(&_rcEvent, _ptMouse)) 
			{
				_layer = 0; 
			}
			//�׸��� ����� ����� ���� ����
			if (PtInRect(&_rcEraser, _ptMouse))
			{
				if (_layer != 0)
				{
					_drawMode = 0;
				}
			}
			//�׸��� ����� ������ ����
			if (PtInRect(&_rcPen, _ptMouse))
			{
				if (_layer != 0)
				{
					_drawMode = 1;
				}
			}//�� ���
			if (PtInRect(&_rcRectangle, _ptMouse))
			{
				if (_layer != 0)
				{
					_drawMode = 2;
				}
			}	//�簢�� �׸���
			if (PtInRect(&_rcPaint, _ptMouse)) 
			{
				if (_layer != 0)
				{
					_drawMode = 3;
				}
			}		//Ÿ�� ��ü ĥ�ϱ�


			if (PtInRect(&_rcPalette, _ptMouse))
			{
				selectSampleTile();
			}

			if (PtInRect(&_rcDummy2, _ptMouse))
			{
				SCENEMANAGER->loadScene("����ȭ��");		//���� ȭ������ ���ư���
			}
			if (PtInRect(&_rcArrow5[0], _ptMouse))
			{
				if (_palettePage > 5)		//����Ÿ���� ��ȣ�� 5���� Ŭ ��
				{
					_palettePage -= 5;													//����Ÿ���� ��ȣ�� 5 ����
					sprintf_s(_dataName, "Data/Tiles/tile%d.tiledata", _palettePage);	//Ÿ���� �̸��� ���, Ȯ���� ���� _dataName�� �����ϰ�
					loadMapData(_dataName);												//�ش� Ÿ���� ������ �ȷ�Ʈ�� �ҷ��´�.
				}
			}
			if (PtInRect(&_rcArrow5[1], _ptMouse))
			{
				if (_palettePage <= _tileAmount - 5)		//����Ÿ���� ��ȣ�� ����Ÿ���� �� ������ ���̰� 5�� ��
				{
					_palettePage += 5;													//����Ÿ���� ��ȣ�� 5 ����
					sprintf_s(_dataName, "Data/Tiles/tile%d.tiledata", _palettePage);	//Ÿ���� �̸��� ���, Ȯ���� ���� _dataName�� �����ϰ�
					loadMapData(_dataName);												//�ش� ����Ÿ���� �ȷ�Ʈ�� �׷��ش�.
				}
				else if (_palettePage == _tileAmount) { return; }
				else
				{
					//���� Ÿ���� ��ȣ�� ���� Ÿ���� �� ������ ���̰� 5���� ���� ��
					_palettePage = _tileAmount;											//����Ÿ���� ��ȣ�� ����Ÿ���� �� ������ ����
					sprintf_s(_dataName, "Data/Tiles/tile%d.tiledata", _palettePage);	//Ÿ���� �̸��� ���, Ȯ���� ���� _dataName�� �����ϰ�
					loadMapData(_dataName);												//�ش� ����Ÿ���� �ȷ�Ʈ�� �׷��ش�.
				}
			}
			if (PtInRect(&_rcArrow[0], _ptMouse))
			{
				if (_palettePage > 1)		//����Ÿ���� ��ȣ�� 1���� Ŭ ��
				{
					_palettePage--;														//����Ÿ���� ��ȣ�� 1 ����
					sprintf_s(_dataName, "Data/Tiles/tile%d.tiledata", _palettePage);	//Ÿ���� �̸��� ���, Ȯ���� ���� _dataName�� �����ϰ�
					loadMapData(_dataName);												//�ش� ����Ÿ���� �ȷ�Ʈ�� �׷��ش�.
				}
				else { return; }
			}
			if (PtInRect(&_rcArrow[1], _ptMouse))
			{
				if (_palettePage < _tileAmount)		//����Ÿ���� ��ȣ�� �ִ밪���� ���� ��
				{
					_palettePage++;														//����Ÿ���� ��ȣ�� 1 ������Ű��
					sprintf_s(_dataName, "Data/Tiles/tile%d.tiledata", _palettePage);	//Ÿ���� �̸��� ���, Ȯ���� ���� _dataName�� �����ϰ�
					loadMapData(_dataName);												//�ش� ����Ÿ���� �ȷ�Ʈ�� �׷��ش�.
				}
			}
		}


		if (_rcPalette.top > WINSIZEY) // �ּ�ȭ�� ȭ������� �����鼭 ���ϸ�ŭ�������� ��ư����(�̵�) �ظ��� �¾����� �ؿ��־����
		{
			_rcSaveLoad = RectMake(_rcPalette.left, _rcScreen.bottom - TILESIZE, TILESIZE * 2, TILESIZE);		//���� UI
			_rcDummy2 = RectMake(_rcPalette.left + TILESIZE * 4, WINSIZEY - TILESIZE, TILESIZE * 2, TILESIZE);	//���� UI
			_rcArrow5[0] = RectMake(_rcPalette.left + TILESIZE * 8, WINSIZEY - TILESIZE, TILESIZE, TILESIZE);	//���� UI
			_rcArrow5[1] = RectMake(_rcPalette.left + TILESIZE * 9, WINSIZEY - TILESIZE, TILESIZE, TILESIZE);	//���� UI
			_rcArrow[0] = RectMake(_rcPalette.left + TILESIZE * 10, WINSIZEY - TILESIZE, TILESIZE, TILESIZE);	//���� UI
			_rcArrow[1] = RectMake(_rcPalette.left + TILESIZE * 11, WINSIZEY - TILESIZE, TILESIZE, TILESIZE);	//���� UI
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




	//���̾� �̹��� �׸���
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

		//�ϴ��� ����Ÿ�� ���� �����Ϸ����� 20->40�ȼ��� Ȯ��

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


	//����Ÿ���� ��ȣ�� 1���� ���� ���� �����Ƿ� 1���� ū ��쿡�� ���� Ÿ�Ϸ� �Ѿ�� ȭ��ǥ�� �����ش�.
	if (_palettePage > 1) { IMAGEMANAGER->findImage("LeftArrow")->render(getMemDC(), _rcArrow[0].left, _rcArrow[0].top); }
	if (_palettePage > 1) { IMAGEMANAGER->findImage("LeftArrow5")->render(getMemDC(), _rcArrow5[0].left, _rcArrow5[0].top); }
	//����Ÿ���� ��ȣ�� �ִ밪���� ū ���� �����Ƿ� �ִ밪���� ���� ��쿡�� ���� Ÿ�Ϸ� �Ѿ�� ȭ��ǥ�� �����ش�.
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
			}//���콺 �������� Ȱ��ȭ

			if (_setSaveSlot == i)
			{
				IMAGEMANAGER->findImage("activetextbox")->render(getMemDC(), _rcSaveSlot[i].left, _rcSaveSlot[i].top, 0, 0,
					_rcSaveSlot[i].right - _rcSaveSlot[i].left, _rcSaveSlot[i].bottom - _rcSaveSlot[i].top);
			}
		}


		FrameRect(getMemDC(), _rcSaveSlot[0], RGB(255, 255, 255));				 //���̺꽽�� �������ֱ�
		FrameRect(getMemDC(), _rcSaveSlot[1], RGB(255, 255, 255));				 //���̺꽽�� �������ֱ�
		FrameRect(getMemDC(), _rcSaveSlot[2], RGB(255, 255, 255));				 //���̺꽽�� �������ֱ�
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

	_rcSaveLoad = RectMake(_rcPalette.left + TILESIZE * 10, _rcPalette.top, TILESIZE * 2, TILESIZE);					//���� UI 1��ĭ
	_rcDummy2 =	RectMake(_rcPalette.left + TILESIZE * 10, _rcPalette.top + TILESIZE * 2, TILESIZE * 2, TILESIZE);		//���� UI 3��ĭ
	_rcArrow5[0] = RectMake(_rcPalette.left + TILESIZE * 10, _rcPalette.top + TILESIZE * 4, TILESIZE, TILESIZE);
	_rcArrow5[1] = RectMake(_rcPalette.left + TILESIZE * 11, _rcPalette.top + TILESIZE * 4, TILESIZE, TILESIZE);
	_rcArrow[0] = RectMake(_rcPalette.left + TILESIZE * 10, _rcPalette.top + TILESIZE * 5, TILESIZE, TILESIZE);			//���� UI �ǾƷ���ĭ
	_rcArrow[1] = RectMake(_rcPalette.left + TILESIZE * 11, _rcPalette.top + TILESIZE * 5, TILESIZE, TILESIZE);			//���� UI �ǾƷ���ĭ

	_rcSaveWindow = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2 - 50, 300, 250);					    //���̺�ε�UI
	for (int i = 0; i < _mapAmount; i++)																	    //���̺�ε�UI
	{																									//���̺�ε�UI
		_rcSaveSlot[i] = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2 - 125 + i * 49, 250, 50);			//���̺�ε�UI
	}																									//���̺�ε�UI
	_rcSave = RectMakeCenter(WINSIZEX / 2 - 70, WINSIZEY / 2 + 35, 80, 35);							//���̺�ε�UI
	_rcLoad = RectMakeCenter(WINSIZEX / 2 + 70, WINSIZEY / 2 + 35, 80, 35);							//���̺�ε�UI
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


	//�����ӹٵ�
	IMAGEMANAGER->findImage("FrameLT")->scaleRender(getMemDC(), left, top, scale);
	IMAGEMANAGER->findImage("FrameRT")->scaleRender(getMemDC(), left + width - (17 * scale), top, scale);
	IMAGEMANAGER->findImage("FrameLB")->scaleRender(getMemDC(), left, top + height - (17 * scale), scale);
	IMAGEMANAGER->findImage("FrameRB")->scaleRender(getMemDC(), left + width - (17 * scale), top + height - (17 * scale), scale);
	//�𼭸�

}
void maptoolScene::frameBoxRender(RECT rc, float scale)
{
	IMAGEMANAGER->findImage("FrameL")->scaleRender(getMemDC(), rc.left - 17 * scale, rc.top - 1, 0, 0, 17 * scale, rc.bottom + 1 - rc.top, scale);
	IMAGEMANAGER->findImage("FrameR")->scaleRender(getMemDC(), rc.right, rc.top - 1, 0, 0, 17 * scale, rc.bottom + 1 - rc.top, scale);
	IMAGEMANAGER->findImage("FrameT")->scaleRender(getMemDC(), rc.left - 1, rc.top - 17 * scale, 0, 0, rc.right + 1 - rc.left, 17 * scale, scale);
	IMAGEMANAGER->findImage("FrameB")->scaleRender(getMemDC(), rc.left - 1, rc.bottom, 0, 0, rc.right + 1 - rc.left, 17 * scale, scale);


	//�����ӹٵ�
	IMAGEMANAGER->findImage("FrameLT")->scaleRender(getMemDC(), rc.left - 17 * scale, rc.top - 17 * scale, scale);
	IMAGEMANAGER->findImage("FrameRT")->scaleRender(getMemDC(), rc.right, rc.top - 17 * scale, scale);
	IMAGEMANAGER->findImage("FrameLB")->scaleRender(getMemDC(), rc.left - 17 * scale, rc.bottom, scale);
	IMAGEMANAGER->findImage("FrameRB")->scaleRender(getMemDC(), rc.right, rc.bottom, scale);

	//�𼭸�

}

TERRAIN maptoolScene::terrainSelect(int frameX, int frameY)
{
	TERRAIN t;
	t.x = frameX;
	t.y = frameY;

	return t;
}
