#pragma once
#include "gameNode.h"
struct MENUBUTTON
{
	RECT rc;
	string sceneName;
	RECT imageRect;
};
class startScene : public gameNode
{
private:
	MENUBUTTON _menuButton[3];

public:
	HRESULT init();
	void release();
	void update();
	void render();

	startScene() {}
	~startScene() {}
};

