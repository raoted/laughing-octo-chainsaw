#include "stdafx.h"
#include "inGameScene.h"

HRESULT inGameScene::init()
{
	//��׶��� �̹��� �ʱ�ȭ
	//IMAGEMANAGER->addImage("��׶���", "Images/background.bmp", WINSIZEX, WINSIZEY);
	//UFO ������ �̹��� �ʱ�ȭ
	//IMAGEMANAGER->addFrameImage("ufo", "Images/ufo.bmp", 530, 32, 10, 1);
	//�Ѿ� �̹���
	//IMAGEMANAGER->addImage("bullet", "Images/bullet.bmp", 21, 21);


	////�÷��̾� Ŭ���� �ʱ�ȭ
	//_player = new player;
	//_player->init();

	////���Ŵ��� Ŭ���� �ʱ�ȭ
	//_enemyManager = new enemyManager;
	//_enemyManager->init();

	////��ȣ����
	//_player->setEMLink(_enemyManager);
	//_enemyManager->setPlayerLink(_player);
	ITEMMANAGER->addFoodItem("��", "�ٰ�Ʈ", 50, "�ٰ�Ʈ ���̴�.", 10);
	ITEMMANAGER->addFoodItem("��", "�ٰ�Ʈ", 50, "�ٰ�Ʈ ���̴�.", 10);

	return S_OK;
}

void inGameScene::release()
{
	////�÷��̾� Ŭ���� ����
	//_player->release();
	//SAFE_DELETE(_player);

	////���Ŵ��� Ŭ���� ����
	//_enemyManager->release();
	//SAFE_DELETE(_enemyManager);
}

void inGameScene::update()
{
	////�÷��̾� Ŭ���� ������Ʈ
	//_player->update();

	////���Ŵ��� ������Ʈ
	//_enemyManager->update();

	//if (INPUT->GetKeyDown(VK_RETURN))
	//{
	//	SCENEMANAGER->loadScene("����ȭ��");
	//}
}

void inGameScene::render()
{
	Rectangle(getMemDC(), RectMake(0, 0, WINSIZEX, WINSIZEY));
	string str = to_string(ITEMMANAGER->findItem("��")->getItemType());
	TextOut(getMemDC(), 30, 30, str.c_str(), strlen(str.c_str()));
}
