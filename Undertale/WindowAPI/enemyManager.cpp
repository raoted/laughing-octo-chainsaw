#include "stdafx.h"
#include "enemyManager.h"
#include "player.h"

HRESULT enemyManager::init()
{
	//미니언 생성, 보스, 일반몬스터
	//따로 함수로 빼서 처리하면 관리및 보기에 편하다

	this->setDummy();
	this->setNapstablock();
	this->setBoss();
	return S_OK;
}

void enemyManager::release()
{

}

void enemyManager::update()
{
	
}

void enemyManager::render()
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
