#include "stdafx.h"
#include "player.h"
#include "enemyManager.h"

HRESULT player::init(long x, long y)
{
	_inven = new Inventory;
	_inven->init();

	_player = IMAGEMANAGER->findImage("Frisk");
	_name = "Frisk";
	_pPoint.x = x;
	_pPoint.y = y;
	_LV = 1;
	_maxHP = _HP = 20;
	_playerAtk = 5;
	_gold = 0;
	_pState = IDLE;
	_playerSpeed = 3;

	_frame = 0;
	_count = 0;

	_isBattle = FALSE;
	_isInfo = FALSE;
	return S_OK;
}

void player::release()
{

}

void player::update()
{
	if (_isBattle)
	{

	}
	else
	{
		this->playerAnimation();
		this->setPlayerRect(_pPoint);
	}
}

void player::render()
{
	Rectangle(getMemDC(), _rcPlayer);
	_player->scaleFrameRender(getMemDC(), 
		_rcPlayer.left, _rcPlayer.top-20, _player->getFrameX(), _player->getFrameY(), 2.0);
}

void player::playerAnimation()
{
	if (INPUT->GetKey(VK_UP))
	{
		_player->setFrameY(2);
		_count++;
		if (_count == 10)
		{
			_frame++;
			_count = 0;
			if (_frame > 3) { _frame = 0; }
		}
		_player->setFrameX(_frame);
	}
	if (INPUT->GetKeyUp(VK_UP))
	{
		_frame = 0;
		_player->setFrameX(_frame);
	}
	if (INPUT->GetKey(VK_DOWN))
	{
		_player->setFrameY(0);
		_count++;
		if (_count == 10)
		{
			_frame++;
			_count = 0;
			if (_frame > 3) { _frame = 0; }
		}
		_player->setFrameX(_frame);
	}
	if (INPUT->GetKeyUp(VK_DOWN))
	{
		_frame = 0;
		_player->setFrameX(_frame);
	}
	if (INPUT->GetKey(VK_LEFT))
	{
		_player->setFrameY(1);
		_count++;
		if (_count == 10)
		{
			_frame++;
			_count = 0;
			if (_frame > 3) { _frame = 0; }
		}
		_player->setFrameX(_frame);
	}
	if (INPUT->GetKeyUp(VK_LEFT))
	{
		_frame = 0;
		_player->setFrameX(_frame);
	}
	if (INPUT->GetKey(VK_RIGHT))
	{
		_player->setFrameY(3);
		_count++;
		if (_count == 10)
		{
			_frame++;
			_count = 0;
			if (_frame > 3) { _frame = 0; }
		}
		_player->setFrameX(_frame);
	}
	if (INPUT->GetKeyUp(VK_RIGHT))
	{
		_frame = 0;
		_player->setFrameX(_frame);
	}
}

void player::useItem(int i)
{
	if (_inven->getItem()[i]->getItemEffect() + _HP >= _maxHP) { _HP = _maxHP; }
	else { _HP + _inven->getItem()[i]->getItemEffect(); }
	_inven->eraseItem(i);
}
