#include "stdafx.h"
#include "mainGame.h"

//=============================================================
//	## �ʱ�ȭ ## init()
//=============================================================
HRESULT mainGame::init()
{
	gameNode::init();
	//Ŭ���� �ʱ�ȭ�� �̰����� �Ѵ�
	
	/*������ ���ΰ����� ������ ���鸸 ������ �Ѵ�*/
	//������ ������ ���ΰ��� ������ �Ѵ�

	/*���߰�*/
	SCENEMANAGER->addScene("����ȭ��", new startScene);
	SCENEMANAGER->addScene("����ȭ��", new inGameScene);
	SCENEMANAGER->addScene("�ε�ȭ��", new loadingScene);
	SCENEMANAGER->addScene("����ȭ��", new maptoolScene);
	SCENEMANAGER->addScene("����ȭ��", new battleScene);
	

	/*����� ����*/
	SCENEMANAGER->loadScene("�ε�ȭ��");

	ITEMMANAGER->addFoodItem("���ͽ�īġ ����", "���ͽ�īġ ����", 99, "���ͽ�īġ-�ó��� ���� �� �����̴�.", 180);
	ITEMMANAGER->addFoodItem("������ ����", "������ ����", 99, "�� ��� ������ġ.", 40);
	ITEMMANAGER->addFoodItem("����� ����", "����� ����", 45, "�ε� �� ���� ������ ������������ ���ھ�.", 40);

	return S_OK;
}

//=============================================================
//	## ���� ## release()
//=============================================================
void mainGame::release()
{
	gameNode::release();

}

//=============================================================
//	## ������Ʈ ## update()
//=============================================================
void mainGame::update()
{
	gameNode::update();
	
	//���Ŵ��� ������Ʈ
	SCENEMANAGER->update();
	//����Ŵ��� ������Ʈ(�̳�����ָ� ���尡 �����۵�����)
	SOUNDMANAGER->update();
}

//=============================================================
//	## ���� ## render()
//=============================================================
void mainGame::render()
{
	//��� �� ��Ʈ��
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, BLACKNESS);
//=============================================================	
	
	//���ڻ� ��� ���ֱ�
	SetBkMode(getMemDC(), TRANSPARENT);
	//���Ŵ��� ����
	SCENEMANAGER->render();

	TIMEMANAGER->render(getMemDC());


	//���ڹ����
	SetBkMode(getMemDC(), TRANSPARENT);
	//�ؽ�Ʈ ���� ����
	SetTextColor(getMemDC(), RGB(0, 0, 0));

//=============================================================
	//������� ������ ȭ��DC�� �׸���
	this->getBackBuffer()->render(getHDC());
}
