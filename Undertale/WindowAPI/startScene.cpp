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

	return S_OK;
}

void startScene::release()
{
}

void startScene::update()
{
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
	for (int i = 0; i < 3; i++)
	{
		if (PtInRect(&_menuButton[i].rc, _ptMouse))
		{
			HBRUSH brush = CreateSolidBrush(RGB(128, 128, 128));
			HBRUSH oldBrush = (HBRUSH)SelectObject(getMemDC(), brush);
			Rectangle(getMemDC(), _menuButton[i].rc);
			SelectObject(getMemDC(), oldBrush);
			DeleteObject(brush);
		}
		else
		{
			Rectangle(getMemDC(), _menuButton[i].rc);
		}
	}
}
