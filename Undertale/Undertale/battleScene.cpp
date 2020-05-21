#include "stdafx.h"
#include "battleScene.h"

HRESULT battleScene::init()
{
	_enemyArea = RectMake(80, 20, WINSIZEX - 160, 270);
	_playerArea = RectMakeCenter(WINSIZEX / 2, 450, 720, 300);
	_actButton[0] = RectMake(_playerArea.left, 667, 118, 88);
	_actButton[1] = RectMake(_playerArea.left + 200, 667, 118, 88);
	_actButton[2] = RectMake(_playerArea.left + 401, 667, 118, 88);
	_actButton[3] = RectMake(_playerArea.right - 118, 667, 118, 88);

	_isBattleEnd = FALSE;
	_isWin = TRUE;
	_isPAttack = FALSE;
	_isDialog = FALSE;

	_enemyManager = new enemyManager;
	_enemyManager->init(_playerArea);

	_player = new player;
	_player->init((_playerArea.left + _playerArea.right) / 2, (_playerArea.top + _playerArea.bottom) / 2);
	_player->setImage(IMAGEMANAGER->findImage("Red"));

	_player->getImage()->setFrameX(0);
	_player->getImage()->setFrameY(0);
	_player->getImage()->setX(_actButton[0].left + 10);
	_player->getImage()->setY(_actButton[0].top + 15);

	_player->setMHPRect(_playerArea.left + 235, 610, _player->getPlayerMaxHP() * 2, 30);

	_enemyNumber = RANDOM->Range(0, 2);
	//_enemyNumber = 1;
	_enemyManager->getEnemy(_enemyNumber);
	if (_enemyNumber == 0)
	{
		_imageWidth = _enemyManager->getEnemy(_enemyNumber)->getImage()->getWidth();
		_imageHeight = _enemyManager->getEnemy(_enemyNumber)->getImage()->getHeight();
	}
	else
	{
		_imageWidth = _enemyManager->getEnemy(_enemyNumber)->getImage()->getFrameWidth();
		_imageHeight = _enemyManager->getEnemy(_enemyNumber)->getImage()->getFrameHeight();
	}

	_x = WINSIZEX / 2 - _imageWidth;
	_y = _enemyArea.bottom - _imageHeight * 2;

	_bNumber = 0;
	_selectCount = 0;
	_battleCounter = 0;

	_count = 0;
	_frame = 0;

	_vDialogSize = 0;
	_dialogSize = 0;
	_dialogCount = 0;

	_player->setBattle(TRUE);
	_player->setBC(_battleCounter);
	_player->setPA(&_playerArea);
	_round = 0;

	_enemyManager->getEnemy(_enemyNumber)->setTurn(_round);
	//여기부터 폰트 설정
	_lf.lfHeight = 30;
	_lf.lfWidth = 30;
	_lf.lfEscapement = 0;
	_lf.lfOrientation = 0;
	_lf.lfWeight = 0;
	_lf.lfItalic = 0;
	_lf.lfUnderline = 0;
	_lf.lfStrikeOut = 0;
	_lf.lfCharSet = HANGEUL_CHARSET;
	_lf.lfOutPrecision = 0;
	_lf.lfClipPrecision = 0;
	_lf.lfQuality = 0;
	_lf.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	lstrcpy(_lf.lfFaceName, TEXT("DungGeunMo"));

	_enemyManager->setPlayerLink(_player);
	_player->setEMLink(_enemyManager);
	_player->setPlayerHP(PLAYERDATA->getHp());
	_dialogText = _enemyManager->getEnemy()[_enemyNumber]->getStart();
	_time = TIMEMANAGER->getWorldTime();
	return S_OK;
}

void battleScene::release()
{

}

void battleScene::update()
{

	_enemyManager->getEnemy()[_enemyNumber]->update();
	_player->update();
	if (_isDialog)
	{
		if (INPUT->GetKeyDown(0x5A))
		{
			if (_dialogCount >= _dialogText[_vDialogSize][_dialogSize].size())
			{
				_dialogCount = 0;
				_dialogSize++;
				if (_dialogSize >= _dialogText[_vDialogSize].size())
				{
					_dialogSize = 0;
					_vDialogSize++;
					if (_vDialogSize >= _dialogText.size())
					{
						_vDialogSize = 0;
						_isDialog = FALSE;
						_battleCounter++;
						_player->setBC(_battleCounter);
						_dialogText.clear();
					}
				}
			}
		}
		else
		{
			if (_dialogCount < _dialogText[_vDialogSize][_dialogSize].size())
			{
				if (TIMEMANAGER->getWorldTime() - _time >= 0.01f) 
				{
					_time = TIMEMANAGER->getWorldTime();
					_count++; 
				}
				if (_count == 3)
				{
					if ((_dialogText[_vDialogSize][_dialogSize][_dialogCount] & 0x80) == 0x80) { _dialogCount++; }
					_dialogCount++;
					_count = 0;
				}
			}
		}
	}
	else if (_isBattleEnd)
	{
		if (_isWin)
		{
			_bNumber = 0;
			_selectCount = 0;
			for (int i = 0; i < _playerAct.size(); i++)
			{
				_playerAct.pop();
			}

			if (INPUT->GetKeyDown(0x5A) && !_isDialog)
			{
				SCENEMANAGER->loadScene("게임화면");
			}
		}
		else
		{
			PLAYERDATA->setHp(-1);
			if (TIMEMANAGER->getWorldTime() - _time >= 0.01f)
			{
				_time = TIMEMANAGER->getWorldTime();
				_count++;
			}
			if (_count % 30 == 0)
			{
				_player->setPlayerPoint(_player->getImage()->getX(), _player->getImage()->getY());
				_player->setImage(IMAGEMANAGER->findImage("Break"));
			}
			if (INPUT->GetKeyDown(0x5A) && _count >= 600)
			{
				SCENEMANAGER->loadScene("시작화면");
			}
		}
	}
	else
	{
		if (_battleCounter == 0)
		{
			_dialogText = _enemyManager->getEnemy()[_enemyNumber]->getStart();

			if (INPUT->GetKeyDown(VK_LEFT))
			{
				if (_playerAct.size() == 0)
				{
					if (_bNumber > 0)
					{
						_bNumber--;
						_player->getImage()->setX(_actButton[_bNumber].left + 10);
						_player->getImage()->setY(_actButton[_bNumber].top + 15);
					}
				}
				if (_playerAct.size() == 1)
				{
					if (_selectCount > 0)
					{
						_selectCount--;
						_player->getImage()->setX(_antiEnemy[_selectCount]._eStringRect.left - 20);
						_player->getImage()->setY(_antiEnemy[_selectCount]._eStringRect.top + 25);
					}
				}
			}
			if (INPUT->GetKeyDown(VK_RIGHT))
			{
				if (_playerAct.size() == 0)
				{
					if (_bNumber < 3)
					{
						_bNumber++;
						_player->getImage()->setX(_actButton[_bNumber].left + 10);
						_player->getImage()->setY(_actButton[_bNumber].top + 15);
					}
				}
				if (_playerAct.size() == 1)
				{
					if (_selectCount < _antiEnemy.size() - 1)
					{
						_selectCount++;
						_player->getImage()->setX(_antiEnemy[_selectCount]._eStringRect.left - 20);
						_player->getImage()->setY(_antiEnemy[_selectCount]._eStringRect.top + 25);
					}
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

					_player->getImage()->setX(_actButton[_bNumber].left + 10);
					_player->getImage()->setY(_actButton[_bNumber].top + 15);
					_selectCount = 0;

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
						t._eStringRect = RectMake(_playerArea.left + 60, _playerArea.top + 5, 350, 40);
						_antiEnemy.push_back(t);
						_player->getImage()->setX(_antiEnemy[_selectCount]._eStringRect.left - 20);
						_player->getImage()->setY(_antiEnemy[_selectCount]._eStringRect.top + 25);
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
							t._eStringRect = RectMake(_playerArea.left + 60 + (i % 2) * 360, _playerArea.top + 5 + (i / 2) * 50, 350, 40);
							_antiEnemy.push_back(t);
						}
						_player->getImage()->setX(_antiEnemy[_selectCount]._eStringRect.left - 20);
						_player->getImage()->setY(_antiEnemy[_selectCount]._eStringRect.top + 25);
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
								t._eStringRect = RectMake(_playerArea.left + 60 + (i % 2) * 360, _playerArea.top + 5 + (i / 2) * 50, 350, 40);
								_antiEnemy.push_back(t);
							}
						}
						_player->getImage()->setX(_antiEnemy[_selectCount]._eStringRect.left - 20);
						_player->getImage()->setY(_antiEnemy[_selectCount]._eStringRect.top + 25);
						break;
					case 3:
						_playerAct.push("Marcy");
						{
							_antiEnemy.clear();
							TEXT t;

							for (int i = 0; i < _enemyManager->getEnemy()[_enemyNumber]->getMarcy().size(); i++)
							{
								t._eString = _enemyManager->getEnemy()[_enemyNumber]->getMarcy()[i];
								t._eStringRect = RectMake(_playerArea.left + 60 + (i % 2) * 360, _playerArea.top + 5 + (i / 2) * 50, 350, 40);
								_antiEnemy.push_back(t);
							}
						}
						_player->getImage()->setX(_antiEnemy[_selectCount]._eStringRect.left - 20);
						_player->getImage()->setY(_antiEnemy[_selectCount]._eStringRect.top + 25);
						break;
					}
				}
				else if (_playerAct.size() == 1)
				{
					if (_playerAct.top() == "Attack")
					{
						if (RANDOM->Range(0, 1) == 1)
						{
							_attBar.isLeft = TRUE;
							_attBar.pt.x = _playerArea.left;
							_attBar.pt.y = _playerArea.top;
						}
						else
						{
							_attBar.isLeft = FALSE;
							_attBar.pt.x = _playerArea.right;
							_attBar.pt.y = _playerArea.top;
						}
						_count = 0;
						_frame = 0;
						_playerAct.push(_enemyManager->getEnemy()[_enemyNumber]->getName());
					}
					else if (_playerAct.top() == "Act")
					{
						_count = 0;
						_playerAct.push(_enemyManager->getEnemy()[_enemyNumber]->getAct()[_selectCount]);
						_battleCounter++;
						_player->setBC(_battleCounter);
					}
					else if (_playerAct.top() == "Item")
					{
						if (_player->getInven()->getItem().size() != 0)
						{
							string str(_player->getInven()->getItem()[_selectCount]->getItemName());
							string t;
							vector<string> _vt;
							t = str + "를 사용했다.";
							_vt.push_back(t);
							t = "HP를 " + to_string(_player->getInven()->getItem()[_selectCount]->getItemEffect()) + "회복했다.";
							_vt.push_back(t);
							_dialogText.push_back(_vt);

							_player->useItem(_selectCount);
						}
						else
						{
							_playerAct.pop();
						}
					}
					else if (_playerAct.top() == "Marcy")
					{
						_playerAct.push(_enemyManager->getEnemy()[_enemyNumber]->getMarcy()[_selectCount]);
						_battleCounter++;
						_player->setBC(_battleCounter);
					}
				}

			}
			if (INPUT->GetKeyUp(0x5A))
			{
				if (_playerAct.size() == 2)
				{
					_battleCounter++;
					_player->setBC(_battleCounter);
				}
			}
		}
		else if (_battleCounter == 1)
		{
			if (_playerAct.top() == _enemyManager->getEnemy()[_enemyNumber]->getName())
			{
				if (TIMEMANAGER->getWorldTime() - _time >= 0.01f)
				{
					_time = TIMEMANAGER->getWorldTime();
					_count++;
				}
				if (_isPAttack)
				{
					attAnimation();
					if (_count == 100)
					{
						if (_enemyManager->getEnemy()[_enemyNumber]->getHP() <= 0)
						{
							_dialogText.clear();
							_isBattleEnd = TRUE;
							_isWin = TRUE;

							string s;
							vector<string> vs;

							s = "전투에서 승리했다.";

							vs.push_back(s);
							_dialogText.push_back(vs);
						}
						else
						{
							_dialogText = _enemyManager->getEnemy()[_enemyNumber]->getActText(RANDOM->Range(3, 4));
							_isPAttack = FALSE;
							_battleCounter++;
							_player->setBC(_battleCounter);
						}
					}
				}
				else
				{
					if (INPUT->GetKeyDown(0x5A))
					{
						_isPAttack = TRUE;
						_count = 0;
						_demage = RANDOM->Range(1, _player->getPlayerAtk());
						_enemyManager->getEnemy()[_enemyNumber]->
							setHP(_enemyManager->getEnemy()[_enemyNumber]->getHP() - _demage);
						_sDemage = to_string(_demage);
					}
					if (_attBar.isLeft)
					{
						_attBar.pt.x += _attBar.speed;
						if (_attBar.pt.x >= _playerArea.right)
						{
							_battleCounter++;
							_player->setBC(_battleCounter);
						}
					}
					else
					{
						_attBar.pt.x -= _attBar.speed;
						if (_attBar.pt.x <= _playerArea.left)
						{
							_battleCounter++;
							_player->setBC(_battleCounter);
						}
					}
				}
			}
			if (_playerAct.top() == _enemyManager->getEnemy()[_enemyNumber]->getAct()[_selectCount])
			{
				if (_playerAct.top() == "살펴보기")
				{
					_dialogText = _enemyManager->getEnemy()[_enemyNumber]->getExplain(_round);
					_isDialog = TRUE;
				}
				else
				{
					_enemyManager->getEnemy()[_enemyNumber]->setTurn(_round);
					_battleCounter++;
				}
			}
			if (_playerAct.top() == _enemyManager->getEnemy()[_enemyNumber]->getMarcy()[_selectCount])
			{
				if (_playerAct.top() == "도망가기")
				{
					_dialogText.clear();
					string s;
					vector<string> vs;
					s = "*(여기서 도망칠 수 없다.)";
					vs.push_back(s);
					_dialogText.push_back(vs);
				}
				else
				{
					_dialogText.clear();
					string s;
					vector<string> vs;
					s = "*(나는 저 괴물을 살려줄 생각이 없다.)";
					vs.push_back(s);
					_dialogText.push_back(vs);
				}
				if (INPUT->GetKeyDown(0x5A)) { _battleCounter++; }
			}
		}
		else if (_battleCounter == 2)
		{
			if (INPUT->GetKeyDown(0x5A))
			{
				_player->getImage()->setX((_playerArea.right + _playerArea.left) / 2);
				_player->getImage()->setY((_playerArea.top + _playerArea.bottom) / 2);

				_enemyManager->setAttackEnd(FALSE);
				_battleCounter++;
				_player->setBC(_battleCounter);
			}
		}
		else if (_battleCounter == 3)
		{
			if (TIMEMANAGER->getWorldTime() - _time >= 0.01f)
			{
				_time = TIMEMANAGER->getWorldTime();
				_count++;
			}
			if (_count > 600)
			{
				_enemyManager->setAttackEnd(TRUE);
				_dialogText = _enemyManager->getEnemy()[_enemyNumber]->getStart();
				_battleCounter = 0;
				_playerAct.pop();
				_playerAct.pop();
				_bNumber = 0;
				_player->getImage()->setX(_actButton[_bNumber].left + 10);
				_player->getImage()->setY(_actButton[_bNumber].top + 15);
				_player->setBC(_battleCounter);
			}
			else
			{
				if (_enemyNumber == 0)
				{
					_enemyManager->setAttackEnd(TRUE);
					_dialogText = _enemyManager->getEnemy()[_enemyNumber]->getStart();
					_enemyManager->getBone()->getBone().clear();
					_battleCounter = 0;
					_playerAct.pop();
					_playerAct.pop();
					_bNumber = 0;
					_player->getImage()->setX(_actButton[_bNumber].left + 10);
					_player->getImage()->setY(_actButton[_bNumber].top + 15);
					_player->setBC(_battleCounter);
				}
				_enemyManager->update(_enemyNumber);
				if (_player->getPlayerHP() <= 0) 
				{
					_count = 0;
					_player->setPlayerHP(0); 
					_isBattleEnd = TRUE;
					_isWin = FALSE;
					_x = _player->getImage()->getX();
					_y = _player->getImage()->getY();
				}
			}
		}
	}
}

void battleScene::render()
{
	_lf.lfHeight = 30;
	_lf.lfWidth = 30;
	_font = CreateFontIndirect(&_lf);
	_oldFont = (HFONT)SelectObject(getMemDC(), _font);
	SetTextColor(getMemDC(), RGB(255, 255, 255));	//텍스트 컬러 = WHITE
	
	if (_isBattleEnd)
	{
		if (_isWin)
		{
			this->uiRender();
			this->playerInfo();
			_lf.lfHeight = 30;
			_lf.lfWidth = 30;
			_font = CreateFontIndirect(&_lf);
			_oldFont = (HFONT)SelectObject(getMemDC(), _font);
			SetTextColor(getMemDC(), RGB(255, 255, 255));	//텍스트 컬러 = WHITE
			TextOut(getMemDC(), _playerArea.left + 20, _playerArea.top + 20, _dialogText[0][0].c_str(), _dialogText[0][0].size());
			SelectObject(getMemDC(), _oldFont);
			DeleteObject(_font);
		}
		else
		{
			if (_count >= 500)
			{
				IMAGEMANAGER->scaleRender("GameOver", getMemDC(), WINSIZEX/2 - 422, 0, 2.0f);
			}
			else
			{
				IMAGEMANAGER->render("Break", getMemDC(), _x, _y);
			}
		}
	}
	else
	{
		_enemyManager->getEnemy()[_enemyNumber]->render(_x, _y);

		this->uiRender();

		switch (_battleCounter)
		{
		case 0:
		{
			drawPlayerArea();
			if (_playerAct.size() == 0)
			{
				switch (_enemyNumber)
				{
				case 0:
					IMAGEMANAGER->render("DummyStart", getMemDC(), _playerArea.left + 20, _playerArea.top + 20);
					break;
				case 1:
					IMAGEMANAGER->render("NapstablookStart", getMemDC(), _playerArea.left + 20, _playerArea.top + 20);
					break;
				case 2:
					IMAGEMANAGER->render("SansStart", getMemDC(), _playerArea.left + 20, _playerArea.top + 20);
				break;
				}
				_player->render();
			}
			break;

		}
		case 1:
		{
			if (_playerAct.top() == _enemyManager->getEnemy()[_enemyNumber]->getName())
			{
				IMAGEMANAGER->render("AttackBar", getMemDC(), _playerArea.left, _playerArea.top);
				IMAGEMANAGER->scaleFrameRender("AttackStick", getMemDC(), _attBar.pt.x - 28, _attBar.pt.y, _frame, 0, 2.0f);
				if (_isPAttack)
				{
					IMAGEMANAGER->scaleFrameRender("KnifeSlash", getMemDC(), WINSIZEX / 2 - 16, _y, _count / 5, 0, 2.0f);
					if (_count >= 50)
					{
						if (_count % 16 < 8)
						{
							for (int i = 0; i < _sDemage.size(); i++)
							{
								IMAGEMANAGER->frameRender("Number", getMemDC(),WINSIZEX / 2 - 14 * _sDemage.size(), _y - 28, _sDemage[i] - '0', 0);
							}
						}
					}
				}
			}
			else if (_isDialog)
			{
				if (_dialogSize == 0)
				{
					TextOut(getMemDC(), _playerArea.left + 20, _playerArea.top + 20, _dialogText[_vDialogSize][_dialogSize].c_str(), _dialogCount);
				}
				else
				{
					for (int i = 0; i < _dialogSize; i++)
					{
						TextOut(getMemDC(), _playerArea.left + 20, _playerArea.top + 20 + 30 * i,
							_dialogText[_vDialogSize][i].c_str(), _dialogText[_vDialogSize][i].size());
					}
					TextOut(getMemDC(), _playerArea.left + 20, _playerArea.top + 20 + 30 * _dialogSize, _dialogText[_vDialogSize][_dialogSize].c_str(), _dialogCount);
				}
			}
			else if (_playerAct.top() == _enemyManager->getEnemy()[_enemyNumber]->getMarcy()[_selectCount])
			{
				TextOut(getMemDC(), _playerArea.left + 20, _playerArea.top + 20, _dialogText[0][0].c_str(), _dialogText[0][0].size());
			}

			else
			{
				_player->getImage()->frameRender(getMemDC(), _player->getImage()->getX(), _player->getImage()->getY(),
					_player->getImage()->getFrameX(), _player->getImage()->getFrameY());
			}
			break;
		}
		case 2:
		{
			if (_playerAct.top() == "대화하기")
			{
				switch (_enemyNumber)
				{
				case 0:
					IMAGEMANAGER->render("TalkBubble0", getMemDC(), WINSIZEX / 2 + _imageWidth * 3, _y);
					break;
				case 1:
					IMAGEMANAGER->render("TalkBubble1", getMemDC(), WINSIZEX / 2 + _imageWidth * 3, _y);
					break;
				case 2:
					IMAGEMANAGER->render("TalkBubble2", getMemDC(), WINSIZEX / 2 + _imageWidth * 3, _y);
					break;
				}

			}
			else
			{
				switch (_enemyNumber)
				{

				}
			}
			IMAGEMANAGER->render("Bubble" + to_string(1), getMemDC(), _x + _imageWidth * 3, _y);
			break;
		}
		case 3:
			_player->getImage()->frameRender(getMemDC(), _player->getImage()->getX(), _player->getImage()->getY(),
				_player->getImage()->getFrameX(), _player->getImage()->getFrameY());
			switch (_enemyNumber)
			{
			case 0:
				break;
			case 1:
				_enemyManager->getTear()->render();
				break;
			case 2:
				_enemyManager->getBone()->render();
				break;
			}
			break;
		}

		this->playerInfo();
	}
	SelectObject(getMemDC(), _oldFont);
	DeleteObject(_font);
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
	_lf.lfWidth = 20;
	_lf.lfHeight = 20;
	_font = CreateFontIndirect(&_lf);
	_oldFont = (HFONT)SelectObject(getMemDC(), _font);

	if (_playerAct.size() == 0)
	{

	}
	else if (_playerAct.size() == 1)
	{
		_player->getImage()->frameRender(getMemDC(), _player->getImage()->getX(), _player->getImage()->getY(),
			_player->getImage()->getFrameX(), _player->getImage()->getFrameY());
		if (_playerAct.top() == "Attack")
		{
			TextOut(getMemDC(), _antiEnemy[0]._eStringRect.left + 20, _antiEnemy[0]._eStringRect.top + 20,
				_antiEnemy[0]._eString.c_str(), _antiEnemy[0]._eString.size());
		}
		else if (_playerAct.top() == "Act")
		{
			for (int i = 0; i < _antiEnemy.size(); i++)
			{
				TextOut(getMemDC(), _antiEnemy[i]._eStringRect.left + 20, _antiEnemy[i]._eStringRect.top + 20,
					_antiEnemy[i]._eString.c_str(), _antiEnemy[i]._eString.size());
			}
		}
		else if (_playerAct.top() == "Item")
		{
			for (int i = 0; i < _player->getInven()->getItem().size(); i++)
			{
				TextOut(getMemDC(), _antiEnemy[i]._eStringRect.left + 20, _antiEnemy[i]._eStringRect.top + 20,
					_antiEnemy[i]._eString.c_str(), _antiEnemy[i]._eString.size());
			}
		}
		else
		{
			for (int i = 0; i < _antiEnemy.size(); i++)
			{
				TextOut(getMemDC(), _antiEnemy[i]._eStringRect.left + 20, _antiEnemy[i]._eStringRect.top + 20,
					_antiEnemy[i]._eString.c_str(), _antiEnemy[i]._eString.size());
			}
		}
	}
	SelectObject(getMemDC(), _oldFont);
	DeleteObject(_font);
}

void battleScene::uiRender()
{
	if (_bNumber == 0 && _battleCounter == 0)
	{
		IMAGEMANAGER->frameRender("Fight", getMemDC(), _actButton[0].left, _actButton[0].top, 1, 1);
	}
	else
	{
		IMAGEMANAGER->frameRender("Fight", getMemDC(), _actButton[0].left, _actButton[0].top, 1, 0);
	}

	if (_bNumber == 1 && _battleCounter == 0)
	{
		IMAGEMANAGER->frameRender("Act", getMemDC(), _actButton[1].left, _actButton[1].top, 1, 1);
	}
	else
	{
		IMAGEMANAGER->frameRender("Act", getMemDC(), _actButton[1].left, _actButton[1].top, 1, 0);
	}

	if (_bNumber == 2 && _battleCounter == 0)
	{
		IMAGEMANAGER->frameRender("Item", getMemDC(), _actButton[2].left, _actButton[2].top, 1, 1);
	}
	else
	{
		IMAGEMANAGER->frameRender("Item", getMemDC(), _actButton[2].left, _actButton[2].top, 1, 0);
	}

	if (_bNumber == 3 && _battleCounter == 0)
	{
		IMAGEMANAGER->frameRender("Marcy", getMemDC(), _actButton[3].left, _actButton[3].top, 1, 1);
	}
	else
	{
		IMAGEMANAGER->frameRender("Marcy", getMemDC(), _actButton[3].left, _actButton[3].top, 1, 0);
	}

	playerAreaFrame(_playerArea);
}

void battleScene::playerInfo()
{
	string s;
	SetTextColor(getMemDC(), RGB(255, 255, 255));	//텍스트 컬러 = WHITE
	SetBkColor(getMemDC(), RGB(0, 0, 0));			//텍스트 배경 = BLACK

	_lf.lfHeight = 30;
	_lf.lfWidth = 30;
	_font = CreateFontIndirect(&_lf);
	_oldFont = (HFONT)SelectObject(getMemDC(), _font);
	s = _player->getPlayerName();
	TextOut(getMemDC(), _playerArea.left, 610, s.c_str(), s.size());

	s = "LV " + to_string(_player->getPlayerLV());
	TextOut(getMemDC(), _playerArea.left + 100, 610, s.c_str(), s.size());

	
	IMAGEMANAGER->render("HP", getMemDC(), _playerArea.left + 205, 620);

	Rectangle(getMemDC(), _player->getMHPRect());
	IMAGEMANAGER->scaleRender("maxHP", getMemDC(), _player->getMHPRect().left, _player->getMHPRect().top, _player->getMHPRect().right - _player->getMHPRect().left, 0);
	IMAGEMANAGER->scaleRender("curHP", getMemDC(), _player->getMHPRect().left, _player->getMHPRect().top, _player->getPlayerHP()*2, 0);

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
		IMAGEMANAGER->render("KR", getMemDC(), _player->getMHPRect().right + 10, 620);

		_lf.lfHeight = 30;
		_lf.lfWidth = 30;
		_font = CreateFontIndirect(&_lf);
		_oldFont = (HFONT)SelectObject(getMemDC(), _font);
		TextOut(getMemDC(), _player->getMHPRect().right + 40, 610, s.c_str(), s.size());
	}
	else
	{
		TextOut(getMemDC(), _player->getMHPRect().right + 10, 610, s.c_str(), s.size());
	}
	SetTextColor(getMemDC(), RGB(0, 0, 0));

	SelectObject(getMemDC(), _oldFont);
	DeleteObject(_font);
}

void battleScene::attAnimation()
{
	if (_count % 10 == 0)
	{
		_frame++;
		if (_frame > 1) { _frame = 0; }
	}
}

void battleScene::dialog()
{

}

