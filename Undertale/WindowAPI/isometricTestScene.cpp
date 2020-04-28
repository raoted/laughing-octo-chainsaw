#include "stdafx.h"
#include "isometricTestScene.h"

HRESULT isometricTestScene::init()
{
	//타일 구조체 초기화
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			_tile[i][j].x = STARTX + (j * TILESIZEX_HALF) - (i * TILESIZEX_HALF);
			_tile[i][j].y = STARTY + (j * TILESIZEY_HALF) + (i * TILESIZEY_HALF);
			_tile[i][j].rc = RectMakeCenter(_tile[i][j].x, _tile[i][j].y, TILESIZEX, TILESIZEY);
		}
	}

	//인덱스 초기화
	_index = { 0, 0 };

	_rc = RectMakeCenter(WINSIZEX / 2, WINSIZEY / 2, 500, 500);

	return S_OK;
}

void isometricTestScene::release()
{
}

void isometricTestScene::update()
{
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			if (PtInRect(&_tile[i][j].rc, _ptMouse))
			{
				if (_ptMouse.y > _tile[i][j].y) //3, 4분면
				{
					if (_ptMouse.x > _tile[i][j].x) //4분면
					{
						if (_ptMouse.x <= (_tile[i][j].x + TILESIZEX_HALF) - ((_ptMouse.y - _tile[i][j].y) * 2))
						{
							_index.x = j;
							_index.y = i;
						}
					}
					else //3사분면
					{
						if (_ptMouse.x < _tile[i][j].x) //2분면
						{
							if (_ptMouse.x >= (_tile[i][j].x - TILESIZEX_HALF) + ((_ptMouse.y - _tile[i][j].y) * 2))
							{
								_index.x = j;
								_index.y = i;
							}
						}
					}
				}
				else //1, 2분면
				{
					if (_ptMouse.x < _tile[i][j].x) //2분면
					{
						if (_ptMouse.x >= (_tile[i][j].x - TILESIZEX_HALF) + ((_tile[i][j].y - _ptMouse.y) * 2))
						{
							_index.x = j;
							_index.y = i;
						}
					}
					else //1분면
					{
						if (_ptMouse.x <= (_tile[i][j].x + TILESIZEX_HALF) - ((_tile[i][j].y - _ptMouse.y) * 2))
						{
							_index.x = j;
							_index.y = i;
						}
					}
				}

			}
		}
	}

	//카메라 움직이기
	if (INPUT->GetKey(VK_LEFT))
	{
		_rc.left -= 5;
		_rc.right -= 5;
	}
	if (INPUT->GetKey(VK_RIGHT))
	{
		_rc.left += 5;
		_rc.right += 5;
	}
	if (INPUT->GetKey(VK_UP))
	{
		_rc.top -= 5;
		_rc.bottom -= 5;
	}
	if (INPUT->GetKey(VK_DOWN))
	{
		_rc.top += 5;
		_rc.bottom += 5;
	}
}

void isometricTestScene::render()
{
	char str[128];
	for (int i = 0; i < TILEY; i++)
	{
		for (int j = 0; j < TILEX; j++)
		{
			//디버깅용 렉트 보여주기
			if (INPUT->GetToggleKey(VK_SPACE))
			{
				Rectangle(getMemDC(), _tile[i][j].rc);
				//FrameRect(getMemDC(), _tile[i][j].rc, RGB(255, 0, 0));
			}

			//카메라랑 충돌된 렉트만 보여주기
			RECT rc;
			if (IntersectRect(&rc, &_rc, &_tile[i][j].rc))
			{
				//마름모 그리기
				MoveToEx(getMemDC(), _tile[i][j].x - TILESIZEX_HALF, _tile[i][j].y, NULL);
				LineTo(getMemDC(), _tile[i][j].x, _tile[i][j].y - TILESIZEY_HALF);
				LineTo(getMemDC(), _tile[i][j].x + TILESIZEX_HALF, _tile[i][j].y);
				LineTo(getMemDC(), _tile[i][j].x, _tile[i][j].y + TILESIZEY_HALF);
				LineTo(getMemDC(), _tile[i][j].x - TILESIZEX_HALF, _tile[i][j].y);

				//타일안에 인덱스 표시
				sprintf_s(str, "[%d, %d]", j, i);
				TextOut(getMemDC(), _tile[i][j].x, _tile[i][j].y - 15, str, strlen(str));
			}
		
		}
	}

	//마우스 픽킹
	sprintf_s(str, "현재인덱스 : [%d, %d]", _index.x, _index.y);
	TextOut(getMemDC(), STARTX + 200, STARTY - 50, str, strlen(str));

	//카메라 프레임렉트 보여주기
	FrameRect(getMemDC(), _rc, RGB(255, 0, 0));
}
