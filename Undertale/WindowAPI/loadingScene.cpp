#include "stdafx.h"
#include "loadingScene.h"

HRESULT loadingScene::init()
{
	//로딩클래스 초기화
	_loading = new loading;
	_loading->init();

	//로딩 이미지 및 사운드 초기화
	this->loadingImage();
	this->loadingSound();

	return S_OK;
}

void loadingScene::release()
{
	//로딩클래스 해제
	_loading->release();
	SAFE_DELETE(_loading);
}

void loadingScene::update()
{
	//로딩클래스 업데이트
	_loading->update();

	//로딩완료후 화면 변경
	if (_loading->loadingDone())
	{
		SCENEMANAGER->loadScene("시작화면");
	}
}

void loadingScene::render()
{
	//로딩클래스 렌더
	_loading->render();
}


//로딩이미지 함수(이곳에 이미지를 전부 넣어라)
void loadingScene::loadingImage()
{
	//_loading->loadImage();
	//_loading->loadFrameImage();

	
	//백그라운드 이미지 초기화
	IMAGEMANAGER->addImage("백그라운드", "Image/Picture", 640, 480);
	

	//======================================= 사 운 드 ==============================================================
	//#=============================================================================================================
	//#									BGM 불러오기
	//#=============================================================================================================

	//#=============================================================================================================
	//#									SE 불러오기
	//#=============================================================================================================


	//======================================= 이 미 지 ==============================================================
	//#=============================================================================================================
	//#									배경 이미지 불러오기
	//#=============================================================================================================

	//#=============================================================================================================
	//#									사진 불러오기
	//#=============================================================================================================
	_loading->loadImage("Intro1", "Image/UI/Intro1.bmp", 320, 240);
	_loading->loadImage("Intro2", "Image/UI/Intro2.bmp", 320, 240);
	_loading->loadImage("Intro3", "Image/UI/Intro3.bmp", 320, 240);
	_loading->loadImage("Intro4", "Image/UI/Intro4.bmp", 320, 240);
	_loading->loadImage("Intro5", "Image/UI/Intro5.bmp", 320, 240);
	_loading->loadImage("Intro6", "Image/UI/Intro6.bmp", 320, 240);
	_loading->loadImage("Intro7", "Image/UI/Intro7.bmp", 320, 240);
	_loading->loadImage("Intro8", "Image/UI/Intro8.bmp", 320, 240);
	_loading->loadImage("Intro9", "Image/UI/Intro9.bmp", 320, 350);
	_loading->loadImage("Logo", "Image/UI/Logo.bmp", 640, 320);
	//#=============================================================================================================
	//#									UI 불러오기
	//#=============================================================================================================
	_loading->loadImage("FrameT", "Image/UI/FrameT.bmp", 575, 17);
	_loading->loadImage("FrameB", "Image/UI/FrameB.bmp", 575, 17);
	_loading->loadImage("FrameL", "Image/UI/FrameL.bmp", 17, 275);
	_loading->loadImage("FrameR", "Image/UI/FrameR.bmp", 17, 275);

	_loading->loadImage("FrameLT", "Image/UI/FrameLT.bmp", 17, 17);
	_loading->loadImage("FrameLB", "Image/UI/FrameLB.bmp", 17, 17);
	_loading->loadImage("FrameRT", "Image/UI/FrameRT.bmp", 17, 17);
	_loading->loadImage("FrameRB", "Image/UI/FrameRB.bmp", 17, 17);

	_loading->loadImage("accountbox", "Image/UI/Button/accountbox.bmp", 742, 191);
	_loading->loadImage("activetextbox", "Image/UI/Button/activetextbox.bmp", 600, 800);
	_loading->loadImage("textbox", "Image/UI/Button/textbox.bmp", 600, 800);
	//#====================================== B a t t l e ==========================================================
	_loading->loadImage("AttackBar", "Image/UI/Battle/AttackBar.bmp", 546, 115);
	_loading->loadFrameImage("AttackStick", "Image/UI/Battle/AttackStick.bmp", 28, 128, 2, 1);

	//#====================================== B u t t o n ==========================================================
	_loading->loadImage("LeftArrow", "Image/UI/Button/arrow0.bmp", 40, 40);
	_loading->loadImage("RightArrow", "Image/UI/Button/arrow1.bmp", 40, 40);
	_loading->loadImage("LeftArrow5", "Image/UI/Button/arrow5_0.bmp", 40, 40);
	_loading->loadImage("RightArrow5", "Image/UI/Button/arrow5_1.bmp", 40, 40);
	_loading->loadImage("SaveLoad", "Image/UI/Button/saveLoad.bmp", 80, 40);
	_loading->loadImage("SaveLoadOff", "Image/UI/Button/saveLoad_off.bmp", 80, 40);
	_loading->loadImage("Home", "Image/UI/Button/home.bmp", 80, 40);
	_loading->loadImage("HomeOff", "Image/UI/Button/home_off.bmp", 80, 40);
	_loading->loadImage("Slide", "Image/UI/Button/slide.bmp", 80, 40);
	_loading->loadImage("SlideOff", "Image/UI/Button/slide_off.bmp", 80, 40);
	_loading->loadImage("Erase", "Image/UI/Button/erase.bmp", 80, 40);
	_loading->loadImage("EraseOff", "Image/UI/Button/erase_off.bmp", 80, 40);

	_loading->loadFrameImage("Act", "Image/UI/Button/Act.bmp", 112, 88, 1, 2);
	_loading->loadFrameImage("Fight", "Image/UI/Button/Fight.bmp", 112, 88, 1, 2);
	_loading->loadFrameImage("Item", "Image/UI/Button/Itme.bmp", 112, 88, 1, 2);
	_loading->loadFrameImage("Marcy", "Image/UI/Button/Marcy.bmp", 112, 88, 1, 2);

	//#======================================== F o n t ============================================================
	_loading->loadFrameImage("Number", "Image/UI/Font/Number.bmp", 280, 28, 10, 1);
	

	//#=============================================================================================================
	//#									타일 이미지 불러오기
	//#=============================================================================================================
	_loading->loadFrameImage("Tile1", "Image/Tileset/Home1.bmp", 160, 100, 8, 5);
	_loading->loadFrameImage("Tile2", "Image/Tileset/Ruin1.bmp", 160, 100, 8, 5);
	_loading->loadFrameImage("Tile3", "Image/Tileset/Ruin2.bmp", 160, 100, 8, 5);
	_loading->loadFrameImage("Tile4", "Image/Tileset/Ruin3.bmp", 160, 100, 8, 5);
	_loading->loadFrameImage("Tile5", "Image/Tileset/Ruin4.bmp", 160, 100, 8, 5);
	_loading->loadFrameImage("Tile6", "Image/Tileset/RuinObject.bmp", 160, 100, 8, 5);
	//#=============================================================================================================
	//#									캐릭터 이미지 불러오기
	//#=============================================================================================================

	//#=============================================================================================================
	//#									에너미 이미지 불러오기
	//#=============================================================================================================

	//#=============================================================================================================
	//#									에너미 공격 이미지 불러오기
	//#=============================================================================================================


	//로딩이 너무 빠르게 진행되서 천천히 돌아가도록 테스트용으로 만들기

}

//로딩사운드 함수(이곳에 사운드를 전부 넣어라)
void loadingScene::loadingSound()
{
}
