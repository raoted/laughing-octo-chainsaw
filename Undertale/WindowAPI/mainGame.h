#pragma once
#define gFont "DungGeunMo"
#include "gameNode.h"
/*������ ���ΰ����� ������ ���鸸 ������ �Ѵ�*/
/*������ϸ� ���ΰ��ӿ� �߰��ϱ�*/
#include "startScene.h"
#include "inGameScene.h"
#include "loadingScene.h"
#include "maptoolScene.h"
#include "battleScene.h"

class mainGame : public gameNode
{
public:
	HRESULT init();
	void release();
	void update();
	void render();

	mainGame() {}
	~mainGame() {}
};

