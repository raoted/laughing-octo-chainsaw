#include "stdafx.h"
#include "enemyManager.h"
#include "player.h"

HRESULT enemyManager::init(RECT rc)
{
	//미니언 생성, 보스, 일반몬스터
	//따로 함수로 빼서 처리하면 관리및 보기에 편하다rc;
	_bone = new bone;
	_bone->init(rc);

	_tear = new tear;
	_tear->init(rc);
	this->setDummy();
	this->setNapstablock();
	this->setBoss();

	_count = 0;

	_attackEnd = TRUE;
	return S_OK;
}

void enemyManager::release()
{

}

void enemyManager::update(int enemyNumber)
{
	if (!_attackEnd)
	{
		switch (enemyNumber)
		{
		case 0:
			break;
		case 1:
		{
			_tear->update(_player->getImage()->getX()+8, _player->getImage()->getX()+8);
			for (int i = 0; i < _tear->getTear().size(); i++)
			{
				if (PtInRect(&_tear->getTear()[i].rc, { (int)(_player->getImage()->getX() / 1), (int)(_player->getImage()->getY() / 1) }))
				{
					_player->setPlayerHP(_player->getPlayerHP() - 1);
					_tear->erase(i);
				}
			}
			break;
		}
		case 2:
		{
			_bone->update();
			for (int i = 0; i < _bone->getBone().size(); i++)
			{
				if (PtInRect(&_bone->getBone()[i].rc, { (int)(_player->getImage()->getX() / 1), (int)(_player->getImage()->getY() / 1) }))
				{
					_player->setPlayerHP(_player->getPlayerHP() - 1);
					_bone->erase(i);
				}
			}
		}
		break;
		}
	}
	else
	{
		_bone->release();
		return;
	}
}

void enemyManager::render(int x, int y)
{
	
	
}

void enemyManager::setDummy()
{
	enemy* dummy = new Dummy;
	dummy->init();
	_vEnemy.push_back(dummy);
}

void enemyManager::setNapstablock()
{
	enemy* napstablock = new Napstablook;
	napstablock->init();
	_vEnemy.push_back(napstablock);
}

void enemyManager::setBoss()
{
	enemy* boss = new Sans;
	boss->init();
	_vEnemy.push_back(boss);
}
