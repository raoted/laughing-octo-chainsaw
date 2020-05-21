#pragma once
#define gFont "DungGeunMo"
#include "gameNode.h"
/*앞으로 메인게임은 각각의 씬들만 관리를 한다*/
/*헤더파일만 메인게임에 추가하기*/
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

