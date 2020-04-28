#include "stdafx.h"
#include "inGameScene.h"

HRESULT inGameScene::init()
{
	//백그라운드 이미지 초기화
	//IMAGEMANAGER->addImage("백그라운드", "Images/background.bmp", WINSIZEX, WINSIZEY);
	//UFO 프레임 이미지 초기화
	//IMAGEMANAGER->addFrameImage("ufo", "Images/ufo.bmp", 530, 32, 10, 1);
	//총알 이미지
	//IMAGEMANAGER->addImage("bullet", "Images/bullet.bmp", 21, 21);


	////플레이어 클래스 초기화
	//_player = new player;
	//_player->init();

	////적매니져 클래스 초기화
	//_enemyManager = new enemyManager;
	//_enemyManager->init();

	////상호참조
	//_player->setEMLink(_enemyManager);
	//_enemyManager->setPlayerLink(_player);
	ITEMMANAGER->addFoodItem("빵", "바게트", 50, "바게트 빵이다.", 10);
	ITEMMANAGER->addFoodItem("빵", "바게트", 50, "바게트 빵이다.", 10);

	return S_OK;
}

void inGameScene::release()
{
	////플레이어 클래스 해제
	//_player->release();
	//SAFE_DELETE(_player);

	////적매니져 클래스 해제
	//_enemyManager->release();
	//SAFE_DELETE(_enemyManager);
}

void inGameScene::update()
{
	////플레이어 클래스 업데이트
	//_player->update();

	////적매니져 업데이트
	//_enemyManager->update();

	//if (INPUT->GetKeyDown(VK_RETURN))
	//{
	//	SCENEMANAGER->loadScene("시작화면");
	//}
}

void inGameScene::render()
{
	Rectangle(getMemDC(), RectMake(0, 0, WINSIZEX, WINSIZEY));
	string str = to_string(ITEMMANAGER->findItem("빵")->getItemType());
	TextOut(getMemDC(), 30, 30, str.c_str(), strlen(str.c_str()));
}
