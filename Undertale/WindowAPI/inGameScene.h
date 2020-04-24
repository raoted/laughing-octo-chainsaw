#pragma once
#include "gameNode.h"
#include "player.h"
#include "enemyManager.h"

class inGameScene : public gameNode
{
private:
	char* str;
private:
	player* _player; //�÷��̾� Ŭ����
	enemyManager* _enemyManager;	//���Ŵ��� Ŭ����

	RECT _RcMenuIndex;
	RECT _InfoIndex;
	RECT _PlayerState;

	unsigned short _menuIndex;
public:
	HRESULT init();
	void release();
	void update();
	void render();

	inGameScene() {}
	~inGameScene() {}
};

