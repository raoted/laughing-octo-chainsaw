#include "stdafx.h"
#include "battleScene.h"

HRESULT battleScene::init()
{
	_enemyArea = RectMake(80, 20, WINSIZEX - 160, 270);
	_playerArea = RectMakeCenter(WINSIZEX / 2, 450, 720, 300);
	/*_playerArea = RectMake(83, _enemyArea.bottom + 20, 1114, 264);*/
	_actButton[0] = RectMake(_playerArea.left, 667, 118, 88);
	_actButton[1] = RectMake(_playerArea.left + 200, 667, 118, 88);
	_actButton[2] = RectMake(_playerArea.left + 401, 667, 118, 88);
	_actButton[3] = RectMake(_playerArea.right - 118, 667, 118, 88);

	_isBattleEnd = false;
	_isWin = true;

	_enemyManager = new enemyManager;
	_enemyManager->init();

	_player = new player;
	_player->init((_playerArea.left + _playerArea.right) / 2, (_playerArea.top + _playerArea.bottom) / 2);
	_player->setMHPRect(_playerArea.left + 235, 610, _player->getPlayerMaxHP() * 2, 30);

	//_enemyNumber = RANDOM->Range(0, 2);
	_enemyNumber = 2;
	_enemyManager->getEnemy(_enemyNumber);

	_imageWidth = _enemyManager->getEnemy(_enemyNumber)->getImage()->getFrameWidth();
	_imageHeight = _enemyManager->getEnemy(_enemyNumber)->getImage()->getFrameHeight();

	_x = WINSIZEX / 2 - _imageWidth;
	_y = _enemyArea.bottom - _imageHeight * 2;

	_bNumber = 0;
	_selectCount = 0;
	_battleCounter = 0;
	
	//여기부터 폰트 설정
	_bigFont = CreateFont(30, 30, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "DungGeunMo");
	_nFont = CreateFont(20, 20, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "DungGeunMo");
	return S_OK;
}

void battleScene::release()
{
	_enemyManager->release();
	SAFE_DELETE(_enemyManager);
}

void battleScene::update()
{
	if (_isBattleEnd)
	{

	}
	else
	{
		if (INPUT->GetKeyDown(VK_LEFT))
		{
			if (_playerAct.size() == 0)
			{
				if (_bNumber > 0) { _bNumber--; }
			}
			if (_playerAct.size() == 1)
			{
				if (_selectCount > 0) { _selectCount--; }
			}
		}
		if (INPUT->GetKeyDown(VK_RIGHT))
		{
			if (_playerAct.size() == 0)
			{
				if (_bNumber < 3) { _bNumber++; }
			}
			if (_playerAct.size() == 1)
			{
				if (_selectCount < _antiEnemy.size() - 1) { _selectCount++; }
			}
		}
		if (INPUT->GetKeyDown(0x58))
		{
			if (_playerAct.size() == 1)
			{
				if (_playerAct.top() == "Attack") { _bNumber = 0; }
				if (_playerAct.top() == "Act") { _bNumber = 1; }
				if (_playerAct.top() == "Item") { _bNumber = 2; }
				if (_playerAct.top() == "Marcy") { _bNumber = 3; }
				_playerAct.pop();
			}
		}
		if (INPUT->GetKeyDown(0x5A))
		{
			if (_playerAct.size() == 0)
			{
				switch (_bNumber)
				{
				case 0:
				{
					_playerAct.push("Attack");
					_antiEnemy.clear();
					TEXT t;
					t._eString = _enemyManager->getEnemy()[_enemyNumber]->getName();
					t._eStringRect = RectMake(_playerArea.left + 20, _playerArea.top + 5, 350, 40);
					_antiEnemy.push_back(t);
				}
				break;
				case 1:
				{
					_playerAct.push("Act");
					_antiEnemy.clear();
					TEXT t;
					for (int i = 0; i < _enemyManager->getEnemy()[_enemyNumber]->getAct().size(); i++)
					{
						t._eString = _enemyManager->getEnemy()[_enemyNumber]->getAct()[i];
						t._eStringRect = RectMake(_playerArea.left + 5 + (i % 2) * 360, _playerArea.top + 5 + (i / 2) * 50, 350, 40);
						_antiEnemy.push_back(t);
					}
				}
					break;
				case 2:
					_playerAct.push("Item");
					{
						_antiEnemy.clear();
						TEXT t;

						for (int i = 0; i < _player->getInven()->getItem().size(); i++)
						{
							t._eString = _player->getInven()->getItem()[i]->getItemName();
							t._eStringRect = RectMake(_playerArea.left + 5 + (i % 2) * 360, _playerArea.top + 5 + (i / 2) * 50, 350, 40);
							_antiEnemy.push_back(t);
						}
					}
					break;
				case 3:
					_playerAct.push("Marcy");
					{
						_antiEnemy.clear();
						TEXT t;

						for (int i = 0; i < _enemyManager->getEnemy()[_enemyNumber]->getMarcy().size(); i++)
						{
							t._eString = _enemyManager->getEnemy()[_enemyNumber]->getMarcy()[i];
							t._eStringRect = RectMake(_playerArea.left + 5 + (i % 2) * 360, _playerArea.top + 5 + (i / 2) * 50, 350, 40);
							_antiEnemy.push_back(t);
						}
					}
				}
			}
			else if (_playerAct.size() == 1)
			{
				if (_playerAct.top() == "Attack")
				{

				}
				else if (_playerAct.top() == "Act")
				{
					
				}
				else if (_playerAct.top() == "Item")
				{
					
				}
				else if (_playerAct.top() == "Marcy")
				{
					
				}
			}
			else if (_playerAct.size() == 2)
			{
				if (_playerAct.top() == _enemyManager->getEnemy()[_enemyNumber]->getName())
				{

				}
				if (_playerAct.top() == _enemyManager->getEnemy()[_enemyNumber]->getAct()[_selectCount])
				{

				}
				if (_playerAct.top() == _player->getInven()->getItem()[_selectCount]->getItemName())
				{
					_player->useItem(_selectCount);
					_playerAct.pop();
					_playerAct.pop();
				}
				if (_playerAct.top() == _enemyManager->getEnemy()[_enemyNumber]->getMarcy()[_selectCount])
				{

				}
			}
		}
		_enemyManager->getEnemy()[_enemyNumber]->update();
	}
}

void battleScene::render()
{
	if (_isBattleEnd)
	{
		if (_isWin)
		{

		}
		else
		{

		}
	}
	else
	{
		_enemyManager->getEnemy()[_enemyNumber]->render(_x, _y);
		uiRender();
		drawPlayerArea();
		
		SetTextColor(getMemDC(), RGB(255, 255, 255));	//텍스트 컬러 = WHITE
		SetBkColor(getMemDC(), RGB(0, 0, 0));			//텍스트 배경 = BLACK
		string s;

		_oBigFont = (HFONT)SelectObject(getMemDC(), _bigFont);
		s = _player->getPlayerName();
		TextOut(getMemDC(), _playerArea.left, 610, s.c_str(), s.size());

		s = "LV " + to_string(_player->getPlayerLV());
		TextOut(getMemDC(), _playerArea.left + 100, 610, s.c_str(), s.size());

		s = "HP";
		_nFont = CreateFont(20, 20, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "DungGeunMo");
		_oNFont = (HFONT)SelectObject(getMemDC(), _nFont);

		TextOut(getMemDC(), _playerArea.left + 205, 615, s.c_str(), s.size());

		
		_oBigFont = (HFONT)SelectObject(getMemDC(), _bigFont);

		Rectangle(getMemDC(), _player->getMHPRect());

		if (_player->getPlayerHP() < 10)
		{
			s = "0" + to_string(_player->getPlayerHP()) + "/" + to_string(_player->getPlayerMaxHP());
		}
		else
		{
			s = to_string(_player->getPlayerHP()) + "/" + to_string(_player->getPlayerMaxHP());
		}
		if (_enemyNumber == 2)
		{
			_oNFont = (HFONT)SelectObject(getMemDC(), _nFont);
			TextOut(getMemDC(), _player->getMHPRect().right + 10, 615, "KR", strlen("KR"));
			
			_oBigFont = (HFONT)SelectObject(getMemDC(), _bigFont);
			TextOut(getMemDC(), _player->getMHPRect().right + 40, 610, s.c_str(), s.size());
		}
		else
		{
			TextOut(getMemDC(), _player->getMHPRect().right + 10, 610, s.c_str(), s.size());
		}
		SetTextColor(getMemDC(), RGB(0, 0, 0));
	}
}

void battleScene::deadAnimation()
{
	//IMAGEMANAGER->render()
}

void battleScene::playerAreaFrame(RECT rc)
{
	FrameRect(getMemDC(), rc, RGB(255, 255, 255));
}

void battleScene::drawPlayerArea()
{
	SetTextColor(getMemDC(), RGB(255, 255, 255));
	HFONT font, oldFont;
	font = CreateFont(40, 20, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "DungGeunMo");
	oldFont = (HFONT)SelectObject(getMemDC(), font);
	if (_playerAct.size() == 0)
	{

	}
	else if (_playerAct.size() == 1)
	{
		if (_playerAct.top() == "Attack")
		{
			TextOut(getMemDC(), _antiEnemy[0]._eStringRect.left + 20, _antiEnemy[0]._eStringRect.top + 20,
				_antiEnemy[0]._eString.c_str(), _antiEnemy[0]._eString.size());
			if (_selectCount == 0)
			{
				IMAGEMANAGER->frameRender("Red", getMemDC(), _antiEnemy[0]._eStringRect.left, _antiEnemy[0]._eStringRect.top + 15, 1, 0);
			}
		}
		else if (_playerAct.top() == "Act")
		{
			for (int i = 0; i < _antiEnemy.size(); i++)
			{
				TextOut(getMemDC(), _antiEnemy[i]._eStringRect.left + 20, _antiEnemy[i]._eStringRect.top + 20,
					_antiEnemy[i]._eString.c_str(), _antiEnemy[i]._eString.size());

				if (_selectCount == i)
				{
					IMAGEMANAGER->frameRender("Red", getMemDC(), _antiEnemy[i]._eStringRect.left, _antiEnemy[i]._eStringRect.top + 15, 1, 0);
				}
			}
		}
		else if (_playerAct.top() == "Item")
		{
			for (int i = 0; i < _player->getInven()->getItem().size(); i++)
			{
				TextOut(getMemDC(), _antiEnemy[i]._eStringRect.left + 20, _antiEnemy[i]._eStringRect.top + 20,
					_antiEnemy[i]._eString.c_str(), _antiEnemy[i]._eString.size());

				if (_selectCount == i)
				{
					IMAGEMANAGER->frameRender("Red", getMemDC(), _antiEnemy[i]._eStringRect.left, _antiEnemy[i]._eStringRect.top + 15, 1, 0);
				}
			}
		}
		else
		{
			for (int i = 0; i < _antiEnemy.size(); i++)
			{
				TextOut(getMemDC(), _antiEnemy[i]._eStringRect.left + 20, _antiEnemy[i]._eStringRect.top + 20,
					_antiEnemy[i]._eString.c_str(), _antiEnemy[i]._eString.size());

				if (_selectCount == i)
				{
					IMAGEMANAGER->frameRender("Red", getMemDC(), _antiEnemy[i]._eStringRect.left, _antiEnemy[i]._eStringRect.top + 15, 1, 0);
				}
			}
		}
	}
	SelectObject(getMemDC(), oldFont);
	DeleteObject(font);
	SetTextColor(getMemDC(), RGB(0, 0, 0));
}

void battleScene::uiRender()
{
	if (_bNumber == 0)
	{
		IMAGEMANAGER->frameRender("Fight", getMemDC(), _actButton[0].left, _actButton[0].top, 1, 1);
		IMAGEMANAGER->frameRender("Red", getMemDC(), _actButton[0].left + 10, _actButton[0].top + 15, 1, 0);
	}
	else
	{
		IMAGEMANAGER->frameRender("Fight", getMemDC(), _actButton[0].left, _actButton[0].top, 1, 0);
	}

	if (_bNumber == 1)
	{
		IMAGEMANAGER->frameRender("Act", getMemDC(), _actButton[1].left, _actButton[1].top, 1, 1);
		IMAGEMANAGER->frameRender("Red", getMemDC(), _actButton[1].left + 10, _actButton[1].top + 15, 1, 0);
	}
	else
	{
		IMAGEMANAGER->frameRender("Act", getMemDC(), _actButton[1].left, _actButton[1].top, 1, 0);
	}

	if (_bNumber == 2)
	{
		IMAGEMANAGER->frameRender("Item", getMemDC(), _actButton[2].left, _actButton[2].top, 1, 1);
		IMAGEMANAGER->frameRender("Red", getMemDC(), _actButton[2].left + 10, _actButton[2].top + 15, 1, 0);
	}
	else
	{
		IMAGEMANAGER->frameRender("Item", getMemDC(), _actButton[2].left, _actButton[2].top, 1, 0);
	}

	if (_bNumber == 3)
	{
		IMAGEMANAGER->frameRender("Marcy", getMemDC(), _actButton[3].left, _actButton[3].top, 1, 1);
		IMAGEMANAGER->frameRender("Red", getMemDC(), _actButton[3].left + 10, _actButton[3].top + 15, 1, 0);
	}
	else
	{
		IMAGEMANAGER->frameRender("Marcy", getMemDC(), _actButton[3].left, _actButton[3].top, 1, 0);
	}
	playerAreaFrame(_playerArea);
}
