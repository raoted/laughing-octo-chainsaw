#include "stdafx.h"
#include "enemyManager.h"
#include "player.h"

HRESULT enemyManager::init()
{
	//�̴Ͼ� ����, ����, �Ϲݸ���
	//���� �Լ��� ���� ó���ϸ� ������ ���⿡ ���ϴ�

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
