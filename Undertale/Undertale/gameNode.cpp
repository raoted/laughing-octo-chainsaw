#include "stdafx.h"
#include "gameNode.h"

//=============================================================
//	## �ʱ�ȭ ## init()
//=============================================================
HRESULT gameNode::init()
{
	AddFontResource("Data/System/Font/DungGeunMo.ttf");
	//AddFontResource("��������������.ttf");

	_hdc = GetDC(_hWnd);				//HDC �ʱ�ȭ
	SetTimer(_hWnd, 1, 10, NULL);		//Ÿ�̸� �ʱ�ȭ
	INPUT->init();						//�Է¸Ŵ��� �ʱ�ȭ
	RANDOM->init();						//�����Ŵ��� �ʱ�ȭ
	IMAGEMANAGER->init();				//�̹����Ŵ��� �ʱ�ȭ
	PLAYERDATA->init();					//�÷��̾���� �ʱ�ȭ
	SCENEMANAGER->init();				//���Ŵ��� �ʱ�ȭ
	INIDATA->init();					//INI������ �ʱ�ȭ
	SOUNDMANAGER->init();				//����Ŵ��� �ʱ�ȭ
	IMAGEMANAGER->init();				//�����۸Ŵ��� �ʱ�ȭ
	TIMEMANAGER->init();
	return S_OK;
}

//=============================================================
//	## ���� ## release()
//=============================================================
void gameNode::release()
{
	//Ÿ�̸� ����
	KillTimer(_hWnd, 1);
	//�Է¸Ŵ��� ����
	INPUT->release();
	INPUT->releaseSingleton();
	//�����Ŵ��� ����
	RANDOM->release();
	RANDOM->releaseSingleton();
	//�̹����Ŵ��� ����
	IMAGEMANAGER->release();
	IMAGEMANAGER->releaseSingleton();
	//�÷��̾���� ����
	PLAYERDATA->release();
	PLAYERDATA->releaseSingleton();
	//���Ŵ��� ����
	SCENEMANAGER->release();
	SCENEMANAGER->releaseSingleton();
	//INI������ ����
	INIDATA->release();
	INIDATA->releaseSingleton();
	//����Ŵ��� ����
	SOUNDMANAGER->release();
	SOUNDMANAGER->releaseSingleton();
	//�����۸Ŵ��� ����
	ITEMMANAGER->release();
	ITEMMANAGER->releaseSingleton();
	//Ÿ�ӸŴ��� ����
	TIMEMANAGER->release();
	TIMEMANAGER->releaseSingleton();

	//HDC ����
	ReleaseDC(_hWnd, _hdc);
	
	RemoveFontResource("Data/System/Font/DungGeunMo.ttf");
	//RemoveFontResource("��������������.ttf");
}

//=============================================================
//	## ������Ʈ ## update()
//=============================================================
void gameNode::update()
{
	//���ΰ�ħ (���߿� ���� Ÿ�̸Ӹ� �����Ŀ��� ������� �ʴ´�)
	//������۸� ���� ������� �ʴ´� (true => false)
	InvalidateRect(_hWnd, NULL, FALSE);
}

//=============================================================
//	## ���� ## render()
//=============================================================
void gameNode::render()
{
}

//=============================================================
//	## �������ν��� ## MainProc()
//=============================================================
LRESULT gameNode::MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;		
	PAINTSTRUCT ps; 
	switch (message)
	{
	case WM_TIMER:
		this->update();
		break;
	case WM_PAINT:			
		hdc = BeginPaint(hWnd, &ps);
		this->render();
		EndPaint(hWnd, &ps);
		break;
	case WM_MOUSEMOVE:
		_ptMouse.x = LOWORD(lParam);
		_ptMouse.y = HIWORD(lParam);
		break;
	case WM_MOUSEWHEEL:
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) 
		{
			_wMouse = 1; 
		}
		if (GET_WHEEL_DELTA_WPARAM(wParam) < 0) 
		{
			_wMouse = -1; 
		}
		if (GET_WHEEL_DELTA_WPARAM(wParam) == 0)
		{
			_wMouse = 0;
		}
		break;
	case WM_KEYDOWN:		
		switch (wParam)
		{
		case VK_ESCAPE:		
			PostMessage(hWnd, WM_DESTROY, 0, 0);
			break;
		}
		break;
	case WM_DESTROY:		
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, message, wParam, lParam));
}
