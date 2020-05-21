#include "stdafx.h"
#include "startScene.h"

HRESULT startScene::init()
{
	//렉트 위치 초기화
	for (int i = 0; i < 3; i++)
	{
		_menuButton[i].rc = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2 + 150 * i, 200, 100);
		_menuButton[i].imageRect 
			= RectMake(_menuButton[i].rc.left + 20, _menuButton[i].rc.left + 80, _menuButton[i].rc.top + 20, _menuButton[i].rc.left + 80);
	}
	
	_menuButton[0].sceneName = "게임화면";
	_menuButton[1].sceneName = "맵툴화면";
	_menuButton[2].sceneName = "종료";

	_count = 0;
	_frame = 0;
	_timer = TIMEMANAGER->getWorldTime();
	return S_OK;
}

void startScene::release()
{
}

void startScene::update()
{
	if (PtInRect(&_menuButton[0].rc, _ptMouse))
	{
		x = _menuButton[0].rc.left + 15;
		y = _menuButton[0].rc.top + 40;
	}
	if (PtInRect(&_menuButton[1].rc, _ptMouse))
	{
		x = _menuButton[1].rc.left + 15;
		y = _menuButton[1].rc.top + 40;
	}
	if (PtInRect(&_menuButton[2].rc, _ptMouse))
	{
		x = _menuButton[2].rc.left + 15;
		y = _menuButton[2].rc.top + 40;
	}
	if (INPUT->GetKeyDown(VK_LBUTTON))
	{
		if(PtInRect(&_menuButton[0].rc, _ptMouse))
		{
			SCENEMANAGER->loadScene(_menuButton[0].sceneName);
		}
		if (PtInRect(&_menuButton[1].rc, _ptMouse))
		{
			SCENEMANAGER->loadScene(_menuButton[1].sceneName);
		}
		if (PtInRect(&_menuButton[2].rc, _ptMouse))
		{
			this->release();
			PostQuitMessage(0);
		}
	}
}

void startScene::render()
{
	IMAGEMANAGER->scaleRender("Logo", getMemDC(), 0, 0, 2.0f);


	IMAGEMANAGER->frameRender("Start", getMemDC(), _menuButton[0].rc.left, _menuButton[0].rc.top, 0, 0);
	IMAGEMANAGER->frameRender("Start", getMemDC(), _menuButton[1].rc.left, _menuButton[1].rc.top, 0, 1);
	IMAGEMANAGER->frameRender("Start", getMemDC(), _menuButton[2].rc.left, _menuButton[2].rc.top, 0, 2);

	if (PtInRect(&_menuButton[0].rc, _ptMouse) || PtInRect(&_menuButton[1].rc, _ptMouse) || PtInRect(&_menuButton[2].rc, _ptMouse))
	{
		IMAGEMANAGER->frameRender("Red", getMemDC(), x, y, 0, 1);
	}
}
