#include "stdafx.h"
#include "maptoolScene.h"

HRESULT maptoolScene::init()
{
	//Ÿ�ϸ� �̹��� �ʱ�ȭ
	_rcScreen = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, 600, 720);
	_rcPalette = RectMakeCenter((_rcScreen.left + _rcScreen.right) / 2, _rcScreen.bottom - 40 * 4, 40 * SAMPLETILEX, 40 * SAMPLETILEY);
	_mapAmount = PLAYERDATA->getMapAmount();
	_tileAmount = PLAYERDATA->getTileAmount();
	_editMode = false;
	_editMoveDirect = false;
	_setSaveLoad = false;
	_slideTool = true;
	_sliding = false;
	_setSaveSlot = 3;
	//��������
	this->maptoolSetup();

	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			_tiles[i * TILEX + j].rc = RectMake(TILESIZE * j, TILESIZE * i, TILESIZE, TILESIZE);
		}
	}

	_palettePage = 1;
	sprintf_s(_dataName, "Data/Tiles/tile%d.tiledata", _palettePage);
	//����Ÿ��(0,0)��ǥ�� �ʱ�ȭ
	_currentTile.pageNumber = _palettePage;
	_currentTile.x = 0;
	_currentTile.y = 0;

	//���� �巡�� ��ǥ �ʱ�ȭ

	_layer[0] = true;
	_layer[1] = false;
	_layer[2] = false;


	return S_OK;
}

void maptoolScene::release()
{
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
		//���̾� ����
		if (INPUT->GetKeyDown(VK_1)) { selectLayer1(); }
		if (INPUT->GetKeyDown(VK_2)) { selectLayer2(); }
		if (INPUT->GetKeyDown(VK_3)) { selectLayer3(); }

		//ȭ�� �̵�
		if (INPUT->GetKey(VK_UP))
		{
			for (int i = 0; i < TILEX * TILEY; i++)
			{
				_tiles[i].rc.top += 10;
				_tiles[i].rc.bottom += 10;
			}
		}
		if (INPUT->GetKey(VK_DOWN))
		{
			for (int i = 0; i < TILEX * TILEY; i++)
			{
				_tiles[i].rc.top -= 10;
				_tiles[i].rc.bottom -= 10;
			}

		}
		if (INPUT->GetKey(VK_RIGHT))
		{
			for (int i = 0; i < TILEX * TILEY; i++)
			{
				_tiles[i].rc.left -= 10;
				_tiles[i].rc.right -= 10;
			}
		}
		if (INPUT->GetKey(VK_LEFT))
		{
			for (int i = 0; i < TILEX * TILEY; i++)
			{
				_tiles[i].rc.left += 10;
				_tiles[i].rc.right += 10;
			}
		}

		//�����Ͱ� ����Ÿ�� ���� �ִٸ� ����Ÿ�� ����
		//�����Ͱ� ��Ÿ�� ���� �ִٸ� ���õǾ� �ִ� ����Ÿ���� ��Ÿ�Ͽ� ĥ���ش�.

		if (INPUT->GetKey(VK_LBUTTON))
		{
			
			if (INPUT->GetKey('A'))
			{
				this->setAllMap();
			}
			else
			{
				this->setMap();//Ÿ�� ä���
			}

		}

		if(INPUT->GetKeyUp(VK_LBUTTON))
		{ 
			if (PtInRect(&_rcEraser, _ptMouse))
			{
				_currentTile.pageNumber = -1;
				//�̺κ� ����� ���찳 ����� ������ Ÿ�� �����ŭ �龦���������ϴ�
			}
			if (PtInRect(&_rcDummy2, _ptMouse))
			{
				SCENEMANAGER->loadScene("����ȭ��");		//���� ȭ������ ���ư���
			}
			if (PtInRect(&_rcslide, _ptMouse))
			{
				if (!_sliding)
				{
					if (_slideTool)
					{
						_slideTool = false;
						_sliding = true;
					}
					else
					{
						_slideTool = true;
						_sliding = true;
					}
				}
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


		if (_slideTool == true && _rcScreen.bottom - 280) //�ִ�ȭ��=>�ʱ���ġ���� �ø���
		{
			if (_rcPalette.top < (_rcScreen.bottom - 280)) 
			{
				_sliding = false; 
			}
			else 
			{
				if (_sliding)
				{
					_rcPalette.top -= 5;
					_rcPalette.bottom -= 5;
				}
			}
		}
		if (_slideTool == false) // �ּ�ȭ��=>ȭ��۱��� ������ (������ �����̻� ���������� ��ư�̵��ϴ� ����������)
		{
			if (_rcPalette.top > WINSIZEY + 17) { _sliding = false; }
			else
			{
				if (_sliding)
				{
					_rcPalette.top += 5;
					_rcPalette.bottom += 5;
				}
			}
		}


		if (_rcPalette.top > WINSIZEY) // �ּ�ȭ�� ȭ������� �����鼭 ���ϸ�ŭ�������� ��ư����(�̵�) �ظ��� �¾����� �ؿ��־����
		{
			_rcSaveLoad = RectMake(_rcPalette.left, _rcScreen.bottom - TILESIZE, TILESIZE * 2, TILESIZE);							//���� UI
			_rcEraser = RectMake(_rcPalette.left + TILESIZE * 2, WINSIZEY - TILESIZE, TILESIZE * 2, TILESIZE);				//���� UI
			_rcDummy2 = RectMake(_rcPalette.left + TILESIZE * 4, WINSIZEY - TILESIZE, TILESIZE * 2, TILESIZE);				//���� UI
			_rcslide = RectMake(_rcPalette.left + TILESIZE * 6, WINSIZEY - TILESIZE, TILESIZE * 2, TILESIZE);				//���� UI
			_rcArrow5[0] = RectMake(_rcPalette.left + TILESIZE * 8, WINSIZEY - TILESIZE, TILESIZE, TILESIZE);				//���� UI
			_rcArrow5[1] = RectMake(_rcPalette.left + TILESIZE * 9, WINSIZEY - TILESIZE, TILESIZE, TILESIZE);				//���� UI
			_rcArrow[0] = RectMake(_rcPalette.left + TILESIZE * 10, WINSIZEY - TILESIZE, TILESIZE, TILESIZE);				//���� UI			   ���� �����ǿ���
			_rcArrow[1] = RectMake(_rcPalette.left + TILESIZE * 11, WINSIZEY - TILESIZE, TILESIZE, TILESIZE);				//���� UI			   ���� �����ǿ���
		}


		
	}
}



void maptoolScene::render()
{
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		RECT rc;
		if (IntersectRect(&rc, &_rcScreen, &_tiles[i].rc))
		{
			if (_tiles[i].imagePage[0] == -1)
			{
				Rectangle(getMemDC(), _tiles[i].rc);
			}
			else
			{
				sprintf_s(_fileName, "Tile%d", _tiles[i].imagePage[0]);
				IMAGEMANAGER->scaleFrameRender(_fileName, getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top, _tiles[i].tileFrameX[0], _tiles[i].tileFrameY[0], 2.0f);
			}
		}
		else { continue; }
	}

	for (int i = 0; i < TILEX * TILEY; i++)
	{
		RECT rc;
		if (IntersectRect(&rc, &_rcScreen, &_tiles[i].rc))
		{
			if (_tiles[i].imagePage[0] == -1 && _tiles[i].imagePage[1] == -1) { continue; }
			else
			{
				sprintf_s(_fileName, "Tile%d", _tiles[i].imagePage[1]);
				IMAGEMANAGER->scaleFrameRender(_fileName, getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top, _tiles[i].tileFrameX[1], _tiles[i].tileFrameY[1], 2.0f);
			}
		}
		else { continue; }
	}

	for (int i = 0; i < TILEX * TILEY; i++)
	{
		RECT rc;
		if (IntersectRect(&rc, &_rcScreen, &_tiles[i].rc))
		{
			if (_tiles[i].imagePage[0] == -1 && _tiles[i].imagePage[1] == -1 && _tiles[i].imagePage[2] == -1) { continue; }
			else
			{
				sprintf_s(_fileName, "Tile%d", _tiles[i].imagePage[2]);
				IMAGEMANAGER->scaleFrameRender(_fileName, getMemDC(), _tiles[i].rc.left, _tiles[i].rc.top, _tiles[i].tileFrameX[2], _tiles[i].tileFrameY[2], 2.0f);
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

	IMAGEMANAGER->findImage("Erase")->render(getMemDC(), _rcEraser.left, _rcEraser.top);
	if (PtInRect(&_rcEraser, _ptMouse)) {
		IMAGEMANAGER->findImage("EraseOff")->render(getMemDC(), _rcEraser.left, _rcEraser.top);
	}

	IMAGEMANAGER->findImage("HomeOff")->render(getMemDC(), _rcDummy2.left, _rcDummy2.top);
	if (PtInRect(&_rcDummy2, _ptMouse)) {
		IMAGEMANAGER->findImage("Home")->render(getMemDC(), _rcDummy2.left, _rcDummy2.top);
	}

	IMAGEMANAGER->findImage("SlideOff")->render(getMemDC(), _rcslide.left, _rcslide.top);
	if (PtInRect(&_rcslide, _ptMouse)) {
		IMAGEMANAGER->findImage("Slide")->render(getMemDC(), _rcslide.left, _rcslide.top);
	}

	frameBoxRender(_rcPalette, 1.0f);

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
		frameBoxRender(_rcSaveWindow, 1.0f); //���̺�Ȯ��â������

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

		Rectangle(getMemDC(), _rcSave);
		frameBoxRender(_rcSave, 0.3f);															//���̺� ��ư �̹���..�̾�..
		IMAGEMANAGER->findImage("save")->render(getMemDC(), _rcSave.left - 1, _rcSave.top - 1);

		Rectangle(getMemDC(), _rcLoad);
		frameBoxRender(_rcLoad, 0.3f);															//�ε� ��ư ���������� �̹��� ������..
		IMAGEMANAGER->findImage("load")->render(getMemDC(), _rcLoad.left - 1, _rcLoad.top - 1);

	}

	
	

	if (_slideTool) { Rectangle(getMemDC(), 0, 0, 400, 400); }
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
	_rcEraser =	RectMake(_rcPalette.left + TILESIZE * 10, _rcPalette.top + TILESIZE, TILESIZE * 2, TILESIZE);			//���� UI 2��ĭ
	_rcDummy2 =	RectMake(_rcPalette.left + TILESIZE * 10, _rcPalette.top + TILESIZE * 2, TILESIZE * 2, TILESIZE);		//���� UI 3��ĭ
	_rcslide = RectMake(_rcPalette.left + TILESIZE * 10, _rcPalette.top + TILESIZE * 3, TILESIZE * 2, TILESIZE);		//���� UI 4��ĭ
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



void maptoolScene::setMap()
{
	if (PtInRect(&_rcPalette, _ptMouse))
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
	//�˾� �Ʒ����� ��ư�������� �ڿ� ĥ �ȵǰ� �Ѱ��ֱ�
	
	if (PtInRect(&_rcSaveLoad, _ptMouse)) { return; }
	else if (PtInRect(&_rcslide, _ptMouse)) { return; }
	else if (PtInRect(&_rcArrow5[0], _ptMouse)) { return; }
	else if (PtInRect(&_rcArrow5[1], _ptMouse)) { return; }
	else if (PtInRect(&_rcArrow[0], _ptMouse)) { return; }
	else if (PtInRect(&_rcArrow[1], _ptMouse)) { return; }
	else if (PtInRect(&_rcPalette, _ptMouse)) { return; }
		
	for (int i = 0; i < TILEX * TILEY; i++)
	{
		if (PtInRect(&_tiles[i].rc, _ptMouse))
		{
			//���� �Ʒ��� ���̾��ΰ�?
			if (_layer[0])
			{
				_tiles[i].canMove[0] = _canMove;
				_tiles[i].tileFrameX[0] = _currentTile.x;
				_tiles[i].tileFrameY[0] = _currentTile.y;
				_tiles[i].imagePage[0] = _currentTile.pageNumber;
				_tiles[i].terrain = terrainSelect(_currentTile.x, _currentTile.y);
			}
			//�߰� ���̾��ΰ�?
			if (_layer[1])
			{
				_tiles[i].canMove[1] = _canMove;
				_tiles[i].tileFrameX[1] = _currentTile.x;
				_tiles[i].tileFrameY[1] = _currentTile.y;
				_tiles[i].imagePage[1] = _currentTile.pageNumber;
				_tiles[i].terrain = terrainSelect(_currentTile.x, _currentTile.y);
			}
			//���� ������ ���̾��ΰ�?
			if (_layer[2])
			{
				_tiles[i].canMove[2] = _canMove;
				_tiles[i].tileFrameX[2] = _currentTile.x;
				_tiles[i].tileFrameY[2] = _currentTile.y;
				_tiles[i].imagePage[2] = _currentTile.pageNumber;
				_tiles[i].terrain = terrainSelect(_currentTile.x, _currentTile.y);
			}
		}
	}
}

void maptoolScene::setAllMap() {
	for (int i = 0; i < TILEX * TILEY; i++) {
		//���� �ϴ� ���̾�
		if (_layer[0])
		{
			_tiles[i].canMove[0] = _canMove;
			_tiles[i].imagePage[0] = _currentTile.pageNumber;
			_tiles[i].tileFrameX[0] = _currentTile.x;
			_tiles[i].tileFrameY[0] = _currentTile.y;
			_tiles[i].terrain = terrainSelect(_currentTile.x, _currentTile.y);
		}
		//�߰� ���̾�
		if (_layer[1])
		{
			_tiles[i].canMove[1] = _canMove;
			_tiles[i].tileFrameX[1] = _currentTile.x;
			_tiles[i].tileFrameY[1] = _currentTile.y;
			_tiles[i].imagePage[1] = _currentTile.pageNumber;
			_tiles[i].terrain = terrainSelect(_currentTile.x, _currentTile.y);
		}
		//���� �� ���̾�
		if (_layer[2])
		{
			_tiles[i].canMove[2] = _canMove;
			_tiles[i].tileFrameX[2] = _currentTile.x;
			_tiles[i].tileFrameY[2] = _currentTile.y;
			_tiles[i].imagePage[2] = _currentTile.pageNumber;
			_tiles[i].terrain = terrainSelect(_currentTile.x, _currentTile.y);
		}
	}
}

void maptoolScene::save(char* str)
{
	HANDLE file;
	DWORD write;

	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			_saveTile[i * TILEX + j] = _tiles[i * TILEX + j];
			_saveTile[i * TILEX + j].rc = RectMake(TILESIZE * j, TILESIZE * i, TILESIZE, TILESIZE);
		}
	}

	file = CreateFile(str, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(file, _saveTile, sizeof(tagTile) * TILEX * TILEY, &write, NULL);
	CloseHandle(file);
}
void maptoolScene::load(char* str)
{
	HANDLE file;
	DWORD read;

	file = CreateFile(str, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, _tiles, sizeof(tagTile) * TILEX * TILEY, &read, NULL);
	CloseHandle(file);
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

void maptoolScene::selectLayer1()
{
	_layer[0] = true;
	_layer[1] = false;
	_layer[2] = false;
}
void maptoolScene::selectLayer2()
{
	_layer[0] = false;
	_layer[1] = true;
	_layer[2] = false;
}
void maptoolScene::selectLayer3()
{
	_layer[0] = false;
	_layer[1] = false;
	_layer[2] = true;
}

TERRAIN maptoolScene::terrainSelect(int frameX, int frameY)
{
	TERRAIN t;
	t.x = frameX;
	t.y = frameY;

	return t;
}
