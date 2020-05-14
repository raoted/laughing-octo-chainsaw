#include "stdafx.h"
#include "inGameScene.h"

HRESULT inGameScene::init()
{
	_ingameScreen = { 0, 0, WINSIZEX, WINSIZEY };								//_ingameScreen Rect 위치와 크기 설정
	_statInfo = RectMake(341, 60, 800, 600);			//_statInfo Rect 위치와 크기 설정
	_inven = _statInfo;															//_inven Rect 위치와 크기는 _statInfo와 같다.
	_menuRect = RectMakeCenter(200, WINSIZEY / 2, 250, 250);					//_menuRect Rect 위치와 크기 설정
	//_menu Rect의 위치와 크기 설정
	_menu[0] = RectMake(_menuRect.left + 90, _menuRect.top + 10, 140, 70);		
	_menu[1] = RectMake(_menuRect.left + 90, _menu[0].bottom + 10, 140, 70);	
	_menu[2] = RectMake(_menuRect.left + 90, _menu[1].bottom + 10, 140, 70);	
	////_menu Rect 설정 종료
	_text[0] = "물품";
	_text[1] = "스탯";
	_text[2] = "전화";

	//플레이어 클래스 초기화
	_player = new player;
	_player->init(WINSIZEX / 2, WINSIZEY / 2);

	//적매니져 클래스 초기화
	_enemyManager = new enemyManager;
	_enemyManager->init();

	//_astar
	_astar = new astar;
	_astar->init();
	//상호참조
	_player->setEMLink(_enemyManager);
	_enemyManager->setPlayerLink(_player);
	_mapNumber = 1;
	//맵을 불러오기 위해 mapName에 맵 이름 저장
	sprintf_s(mapName, "Data/Map/MAP%d", _mapNumber);

	//map의 LT 좌표
	_mapPoint.x = 0;
	_mapPoint.y = 0;

	//맵 불러오기
	loadMap();

	//A*의 노드 갯수 설정(_map의 xSize, ySize);
	_astar->setMapData(_map.xSize, _map.ySize);

	//생성된 노드들의 Rect 초기화
	for (int i = 0; i < _map.tile.size(); i++)
	{
		_astar->setTotalNodeRc(i, _map.tile[i].rc);
	}
	//A* 시작 노트 설정
	_astar->setStartNode(0, 0);
	
	//EnemySpawner가 목표로 하는 A*의 경로
	_eSpawnerGoal = 0;
	_ePoint = { _astar->getStartNode()->idx + 20, _astar->getStartNode()->idy + 20 };
	_enemySpawner = RectMakeCenter(_ePoint.x, _ePoint.y, 40, 40);

	_isMenu = FALSE;
	_isInven = FALSE;
	_isState = FALSE;
	_isCall = FALSE;
	_isDialog = FALSE;

	_count = 0;
	_dialogCount = 0;
	_dialogSize = 0;
	_menuCount = 0;
	_selectCount = 0;
	
	return S_OK;
}

void inGameScene::release()
{
	//플레이어 클래스 해제
	_player->release();
	SAFE_DELETE(_player);

	//적매니져 클래스 해제
	_enemyManager->release();
	SAFE_DELETE(_enemyManager);

	_astar->release();
	SAFE_DELETE(_astar);
}

void inGameScene::update()
{
	if (_isDialog)
	{
		if(INPUT->GetKeyDown(0x43)) 
		{
			_dialogCount = _dialogText[_dialogSize].size();
		}
		if (INPUT->GetKeyDown(0x5A))
		{
			if (_dialogCount == _dialogText[_dialogSize].size())
			{
				if (_dialogSize+1 >= _dialogText.size())
				{
					_isDialog = FALSE;
					_isCall = FALSE;
					_isMenu = FALSE;
					_isState = FALSE;
					_isInven = FALSE;
					_dialogSize = 0;
					_dialogCount = 0;
					_dialogText.clear();
					return;
				}
				else
				{
					_dialogSize++; 
					_dialogCount = 0;
				}
			}
			
		}
		if (_dialogCount < _dialogText[_dialogSize].size())
		{
			_count++;
			if (_count == 3)
			{
				if ((_dialogText[_dialogSize][_dialogCount] & 0x80) == 0x80) { _dialogCount += 2; }
				else { _dialogCount++; }
				_count = 0;
			}
		}
	}
	if (_isMenu)
	{
		if (INPUT->GetKeyDown(0x43)) 
		{
			if (_isInven) { _isInven = FALSE; }
			else if (_isState) { _isState = FALSE; }
			else if (_isCall) { _isCall = FALSE; }
			else { _isMenu = FALSE; }
		}
		if (INPUT->GetKeyDown(0x5A))
		{
			if (!_isInven && !_isState && !_isCall && !_isDialog)
			{
				switch (_menuCount)
				{
				case 0:
					_isInven = TRUE;
					break;
				case 1:
					_isState = TRUE;
					break;
				case 2:
					_isCall = TRUE;
					_dialogText.push_back("* 이 전화기는 고장났다.");
					_dialogSize = 0;
					_isDialog = TRUE;
					break;
				}
			}
		}
		if (INPUT->GetKeyDown(VK_LEFT))
		{
			if (_player->getInven()->getItem().size() > 0)
			{
				if (_selectCount > 0) { _selectCount--; }
			}
		}
		if (INPUT->GetKeyDown(VK_RIGHT))
		{
			if (_player->getInven()->getItem().size() > 0)
			{
				if (_selectCount < _player->getInven()->getItem().size() - 1) { _selectCount++; }
			}
		}
		if (INPUT->GetKeyDown(VK_UP))
		{
			if (!_isInven && !_isState && !_isCall)
			{
				if (_menuCount > 0) { _menuCount--; }
			}
		}
		if (INPUT->GetKeyDown(VK_DOWN))
		{
			if (!_isInven && !_isState && !_isCall)
			{
				if (_menuCount < 2) { _menuCount++; }
			}
		}

		if (_player->getPlayerPoint().y < WINSIZEY / 2)
		{
			_smallInfo = RectMakeCenter(200, 140, 250, 160);
			_dialog = RectMake(_smallInfo.left, WINSIZEY - 220, _statInfo.right - _smallInfo.left, 160);
		}
		else
		{
			_smallInfo = RectMakeCenter(200, WINSIZEY - 140, 250, 160);
			_dialog = RectMake(_smallInfo.left, 60, _statInfo.right - _smallInfo.left, 160);
		}
	}
	else
	{
		if (!PtInRect(&_astar->getEndNode()->rc, _player->getPlayerPoint()))
		{
			for (int i = 0; i < _map.tile.size(); i++)
			{
				if (PtInRect(&_astar->getTotalNode()[i]->rc, _player->getPlayerPoint()))
				{
					_eSpawnerGoal = 0;

					_astar->setEndNode(i);
					moveStartNode();
					_astar->pathFinding();

					break;
				}
			}
		}

		if (_astar->getFinalList().size() != 0)
		{
			if (_ePoint.x == _astar->getFinalList()[_eSpawnerGoal]->rc.left + 20 
				&& _ePoint.y == _astar->getFinalList()[_eSpawnerGoal]->rc.top + 20)
			{
				if(_eSpawnerGoal != _astar->getFinalList().size() - 1)
				{
					_astar->eraseFinalList();
				}
			}
			else
			{
				if (_ePoint.x < _astar->getFinalList()[_eSpawnerGoal]->rc.left + 20) { _ePoint.x += 4; }
				if (_ePoint.x > _astar->getFinalList()[_eSpawnerGoal]->rc.left + 20) { _ePoint.x -= 4; }
				if (_ePoint.y < _astar->getFinalList()[_eSpawnerGoal]->rc.top + 20) { _ePoint.y += 4; }
				if (_ePoint.y > _astar->getFinalList()[_eSpawnerGoal]->rc.top + 20) { _ePoint.y -= 4; }
				_enemySpawner = RectMakeCenter(_ePoint.x, _ePoint.y, 50, 50);
			}
		}
		if (IntersectRect(&rc, &_enemySpawner, &_player->getPlayerRect()))
		{
			SCENEMANAGER->loadScene("전투화면");
		}
		

		//플레이어 클래스 업데이트
		_player->update();
		//적매니져 업데이트
		_enemyManager->update();
		_pPoint = _player->getPlayerPoint();
		_mapRect = RectMake(_mapPoint.x, _mapPoint.y, _map.xSize * 40, _map.ySize);
		_walkPOINT[0] = { _pPoint.x - 20, _pPoint.y - 20 };
		_walkPOINT[1] = { _pPoint.x + 20, _pPoint.y - 20 };
		_walkPOINT[2] = { _pPoint.x - 20, _pPoint.y + 20 };
		_walkPOINT[3] = { _pPoint.x + 20, _pPoint.y + 20 };

		this->playerControl();
		this->directCheck();

		_astar->update();
	}
}

void inGameScene::render()
{
	for (int i = 0; i < _mapSize; i++)
	{
		if (IntersectRect(&rc, &_map.tile[i].rc, &_ingameScreen))
		{
			for (int layer = 0; layer < 3; layer++)
			{
				if (_map.tile[i].imagePage[layer] != -1)
				{
					sprintf_s(tileName, "Tile%d", _map.tile[i].imagePage[layer]);
					IMAGEMANAGER->scaleFrameRender(tileName, getMemDC(), _map.tile[i].rc.left, _map.tile[i].rc.top, _map.tile[i].tileFrameX[layer],
						_map.tile[i].tileFrameY[layer], 2.0f);
				}
				else
				{
					Rectangle(getMemDC(), _map.tile[i].rc);
				}
			}
		}
	}
	_player->render();
	_enemyManager->render();
	//_astar->render();

	HBRUSH brush, oldBrush;
	brush = CreateSolidBrush(RGB(128, 0, 128));
	oldBrush = (HBRUSH)SelectObject(getMemDC(), brush);
	Rectangle(getMemDC(), _enemySpawner);
	SelectObject(getMemDC(), oldBrush);
	DeleteObject(brush);

	if (_isMenu)
	{
		string s;
	
		SetTextColor(getMemDC(), RGB(255, 255, 255));	//텍스트 컬러 = WHITE
		SetBkColor(getMemDC(), RGB(0, 0, 0));			//텍스트 배경 = BLACK
		HFONT font, oldFont;				//폰트 설정
		font = CreateFont(50, 30, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "DungGeunMo");
		oldFont = (HFONT)SelectObject(getMemDC(), font);

		brush = CreateSolidBrush(RGB(0, 0, 0));			//도형 컬러 = BLACK
		oldBrush = (HBRUSH)SelectObject(getMemDC(), brush);

		//간략한 정보 그리기 시작
		Rectangle(getMemDC(), _smallInfo);
		TextOut(getMemDC(), _smallInfo.left, _smallInfo.top, _player->getPlayerName().c_str(), _player->getPlayerName().size());

		font = CreateFont(30, 25, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "DungGeunMo");
		oldFont = (HFONT)SelectObject(getMemDC(), font);

		TextOut(getMemDC(), _smallInfo.left + 20, _smallInfo.top+20, "LV", strlen("LV"));
		s = to_string(_player->getPlayerLV());
		TextOut(getMemDC(), _smallInfo.left + 100, _smallInfo.top+20, s.c_str(), s.size());

		TextOut(getMemDC(), _smallInfo.left + 20, _smallInfo.top + 60, "HP", strlen("HP"));
		s = to_string(_player->getPlayerHP()) + "/" + to_string(_player->getPlayerMaxHP());
		TextOut(getMemDC(), _smallInfo.left + 100, _smallInfo.top + 60, s.c_str(), s.size());

		TextOut(getMemDC(), _smallInfo.left + 20, _smallInfo.top + 100, "G", strlen("G"));
		s = to_string(_player->getPlayerGold());
		TextOut(getMemDC(), _smallInfo.left + 100, _smallInfo.top + 100, s.c_str(), s.size());

		frameDraw(_smallInfo);
		//간략한 정보 출력 종료

		//메뉴창 그리기 시작
		Rectangle(getMemDC(), _menuRect);
		frameDraw(_menuRect);
		//메뉴창 그리기 종료

		SetTextColor(getMemDC(), RGB(255, 255, 255));
		SetBkColor(getMemDC(), RGB(0, 0, 0));
		font = CreateFont(50, 30, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "DungGeunMo");
		oldFont = (HFONT)SelectObject(getMemDC(), font);
		for (int i = 0; i < 3; i++)
		{
			TextOut(getMemDC(), _menu[i].left, _menu[i].top + 10, _text[i].c_str(), _text[i].size());
		}
		//어떤 메뉴를 선택하게 될지 그리기 시작
		if (!_isState && !_isInven && !_isCall)
		{
			IMAGEMANAGER->frameRender("Red", getMemDC(), _menu[_menuCount].left - 30, _menu[_menuCount].top + 25);
		}
		//그리기 종료
		//상태창 그리기 시작
		if (_isState)
		{
			Rectangle(getMemDC(), _statInfo);
			frameDraw(_statInfo);
		}
		//그리기 종료
		//인벤토리 내용물 그리기 시작
		if (_isInven)
		{
			font = CreateFont(50, 25, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "DungGeunMo");
			oldFont = (HFONT)SelectObject(getMemDC(), font);

			Rectangle(getMemDC(), _inven);
			for (int i = 0; i < _player->getInven()->getItem().size(); i++)
			{
				TextOut(getMemDC(), _inven.left + 40 + 430 * (i % 2), _inven.top + 65 * ((i / 2) + 1),
					_player->getInven()->getItem()[i]->getItemName(), strlen(_player->getInven()->getItem()[i]->getItemName()));
				if (_selectCount == i)
				{
					IMAGEMANAGER->scaleFrameRender("Red", getMemDC(), _inven.left + 7 + 430 * (i % 2), (_inven.top + 12) + 65 * ((i / 2) + 1), 0, 0, 1.6f);
				}
			}
			frameDraw(_inven);

			font = CreateFont(50, 30, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "DungGeunMo");
			oldFont = (HFONT)SelectObject(getMemDC(), font);

		}
		//그리기 종료
		//대화창 그리기 시작
		if (_isDialog)
		{
			Rectangle(getMemDC(), _dialog);
			TextOut(getMemDC(), _dialog.left, _dialog.top, _dialogText[_dialogSize].c_str(), _dialogCount);
			frameDraw(_dialog);
		}
		//대화창 그리기 종료
		SelectObject(getMemDC(), oldFont);
		DeleteObject(font);
		SetTextColor(getMemDC(), RGB(0, 0, 0));

		SelectObject(getMemDC(), oldBrush);
		DeleteObject(brush);
	}
}

void inGameScene::loadMap()
{
	ifstream in(mapName, ios::in | ios::binary);

	in >> _map.xSize >> _map.ySize;

	_mapSize = _map.xSize * _map.ySize;
	_map.tile.resize(_mapSize);

	for (int y = 0; y < _map.xSize; y++)
	{
		for (int x = 0; x < _map.ySize; x++)
		{
			for (int layer = 0; layer < 3; layer++)
			{
				in >> _map.tile[y * _map.xSize + x].canMove[layer] >> _map.tile[y * _map.xSize + x].imagePage[layer] >>
					_map.tile[y * _map.xSize + x].tileFrameX[layer] >> _map.tile[y * _map.xSize + x].tileFrameY[layer];
			}
			in >> _map.tile[y * _map.xSize + x].rc.left >> _map.tile[y * _map.xSize + x].rc.top >> _map.tile[y * _map.xSize + x].rc.right >> _map.tile[y * _map.xSize + x].rc.bottom;
			in >> _map.tile[y * _map.xSize + x].terrain.x >> _map.tile[y * _map.xSize + x].terrain.y;
			_map.tile[y * _map.xSize + x].rc.left = x * 40;
			_map.tile[y * _map.xSize + x].rc.right = _map.tile[y * _map.xSize + x].rc.left + 40;
			_map.tile[y * _map.xSize + x].rc.top = y * 40;
			_map.tile[y * _map.xSize + x].rc.bottom = _map.tile[y * _map.xSize + x].rc.top + 40;
		}
	}
}

void inGameScene::moveStartNode()
{
	for (int i = 0; i < _mapSize; i++)
	{
		if (PtInRect(&_map.tile[i].rc, _ePoint))
		{
			_astar->setStartNode(i % _map.xSize, i / _map.xSize);
			break;
		}
	}
}
void inGameScene::directCheck()
{
	for (int i = 0; i < _mapSize; i++)
	{
		if (PtInRect(&_map.tile[i].rc, _walkPOINT[0]) || PtInRect(&_map.tile[i].rc, _walkPOINT[1]))
		{
			for (int layer = 0; layer < 3; layer++)
			{
				if (_map.tile[i].canMove[layer] == FALSE)
				{
					_walkCheck[0] = FALSE;
					break;
				}
				else { _walkCheck[0] = TRUE; }
			}
		}
		if (PtInRect(&_map.tile[i].rc, _walkPOINT[2]) || PtInRect(&_map.tile[i].rc, _walkPOINT[3]))
		{
			for (int layer = 0; layer < 3; layer++)
			{
				if (_map.tile[i].canMove[layer] == FALSE)
				{
					_walkCheck[1] = FALSE;
					break;
				}
				else { _walkCheck[1] = TRUE; }
			}
		}
		if (PtInRect(&_map.tile[i].rc, _walkPOINT[0]) || PtInRect(&_map.tile[i].rc, _walkPOINT[2]))
		{
			for (int layer = 0; layer < 3; layer++)
			{
				if (_map.tile[i].canMove[layer] == FALSE)
				{
					_walkCheck[2] = FALSE;
					break;
				}
				else { _walkCheck[2] = TRUE; }
			}
		}
		if (PtInRect(&_map.tile[i].rc, _walkPOINT[1]) || PtInRect(&_map.tile[i].rc, _walkPOINT[3]))
		{
			for (int layer = 0; layer < 3; layer++)
			{
				if (_map.tile[i].canMove[layer] == FALSE)
				{
					_walkCheck[3] = FALSE;
					break;
				}
				else { _walkCheck[3] = TRUE; }
			}
		}
	}
}

void inGameScene::playerControl()
{
	if (INPUT->GetKeyDown(0x43)) { _isMenu = TRUE; }
	if (INPUT->GetKey(VK_UP) && _walkCheck[0])
	{
		_pPoint.y -= _player->getPlayerSpeed();
		_player->setPlayerPoint(_pPoint.x, _pPoint.y);
	}
	if (INPUT->GetKey(VK_DOWN) && _walkCheck[1])
	{
		_pPoint.y += _player->getPlayerSpeed();
		_player->setPlayerPoint(_pPoint.x, _pPoint.y);
	}
	if (INPUT->GetKey(VK_LEFT) && _walkCheck[2])
	{
		_pPoint.x -= _player->getPlayerSpeed();
		_player->setPlayerPoint(_pPoint.x, _pPoint.y);
	}
	if (INPUT->GetKey(VK_RIGHT) && _walkCheck[3])
	{
		_pPoint.x += _player->getPlayerSpeed();
		_player->setPlayerPoint(_pPoint.x, _pPoint.y);
	}
}

void inGameScene::savePlayerInfo()
{
	PLAYERDATA->setPlayerPoint(_pPoint);
	PLAYERDATA->setMapNumber(_mapNumber);
}

void inGameScene::map1WarpPoint()
{
	_vWarp.clear();
	_warp.mapNum = 2;
	//맵 나오면 Rect 적용시키기
	//_warp.rc = ;
	_vWarp.push_back(_warp);
}

void inGameScene::map2WarpPoint()
{
	_vWarp.clear();
	_warp.mapNum = 1;
	//_warp.rc = ;
	_vWarp.push_back(_warp);
	_warp.mapNum = 3;
	//_warp.rc = ;
	_vWarp.push_back(_warp);
}

void inGameScene::map3WarpPoint()
{
	_vWarp.clear();
	_warp.mapNum = 2;
	//_warp.rc = ;
	_vWarp.push_back(_warp);
}

void inGameScene::frameDraw(RECT rc)
{
	FrameRect(getMemDC(), rc, (RGB(255, 255, 255)));
}
