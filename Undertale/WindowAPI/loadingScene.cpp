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
	_loading->loadImage("Logo", "Image/Picture/Logo.bmp", 640, 320);
	_loading->loadImage("GameOver", "Image/GameOver/GameOver.bmp", 422, 182);
	_loading->loadImage("Black", "Image/Picture/Black.bmp", 1280, 720);
	_loading->loadImage("White", "Image/Picture/White.bmp", 1280, 720);
	//#=============================================================================================================
	//#									UI 불러오기
	//#=============================================================================================================
	_loading->loadImage("tileMouse", "Image/UI/tileMouse.bmp", 40, 40);
	//#====================================== B a t t l e ==========================================================
	_loading->loadImage("AttackBar", "Image/UI/Battle/AttackBar.bmp", 546, 115);
	_loading->loadFrameImage("AttackStick", "Image/UI/Battle/AttackStick.bmp", 28, 128, 2, 1);

	_loading->loadImage("TextSide", "Image/UI/Battle/TextSide.bmp", 3, 3);
	_loading->loadImage("TextFloar", "Image/UI/Battle/TextFloar.bmp", 283, 3);
	_loading->loadImage("TextWall", "Image/UI/Battle/TextWall.bmp", 3, 283);
	//#====================================== B u t t o n ==========================================================
	_loading->loadFrameImage("Act", "Image/UI/Button/Act.bmp", 112, 88, 1, 2);
	_loading->loadFrameImage("Fight", "Image/UI/Button/Fight.bmp", 112, 88, 1, 2);
	_loading->loadFrameImage("Item", "Image/UI/Button/Item.bmp", 112, 88, 1, 2);
	_loading->loadFrameImage("Marcy", "Image/UI/Button/Marcy.bmp", 112, 88, 1, 2);
	//#======================================== T o o l ============================================================
	_loading->loadImage("NewMap", "Image/UI/Tool/map.bmp", 32, 32);
	_loading->loadImage("SaveMap", "Image/UI/Tool/save.bmp", 32, 32);
	_loading->loadFrameImage("MapLayer", "Image/UI/Tool/layer.bmp", 96, 32, 3, 1);
	_loading->loadImage("Event", "Image/UI/Tool/event.bmp", 32, 32);
	_loading->loadImage("Eraser", "Image/UI/Tool/eraser.bmp", 32, 32);
	_loading->loadImage("Pencle", "Image/UI/Tool/pencle.bmp", 32, 32);
	_loading->loadImage("Rectangle", "Image/UI/Tool/rectangle.bmp", 32, 32);
	_loading->loadImage("Paint", "Image/UI/Tool/paint.bmp", 32, 32);
	_loading->loadImage("Setting", "Image/UI/Tool/setting.bmp", 32, 32);
	_loading->loadImage("Home", "Image/UI/Tool/home.bmp", 32, 32);
	//#======================================== F o n t ============================================================
	_loading->loadFrameImage("Number", "Image/UI/Font/Number.bmp", 280, 28, 10, 1);
	

	//#=============================================================================================================
	//#									타일 이미지 불러오기
	//#=============================================================================================================
	_loading->loadImage("null", "Image/Tileset/nullTile.bmp", 40, 40);
	_loading->loadFrameImage("Tile1", "Image/Tileset/Home1.bmp", 160, 100, 8, 5);
	_loading->loadFrameImage("Tile2", "Image/Tileset/Ruin1.bmp", 160, 100, 8, 5);
	_loading->loadFrameImage("Tile3", "Image/Tileset/Ruin2.bmp", 160, 100, 8, 5);
	_loading->loadFrameImage("Tile4", "Image/Tileset/Ruin3.bmp", 160, 100, 8, 5);
	_loading->loadFrameImage("Tile5", "Image/Tileset/Ruin4.bmp", 160, 100, 8, 5);
	_loading->loadFrameImage("Tile6", "Image/Tileset/RuinObject.bmp", 160, 100, 8, 5);
	//#=============================================================================================================
	//#									캐릭터 이미지 불러오기
	//#=============================================================================================================
	_loading->loadFrameImage("Frisk", "Image/Character/Frisk.bmp", 76, 116, 4, 4);
	_loading->loadFrameImage("Red", "Image/Character/Red.bmp", 16, 32, 1, 2);
	_loading->loadFrameImage("Flee", "Image/Character/Flee.bmp", 32, 16, 2, 1);
	_loading->loadFrameImage("Blue", "Image/Character/Blue.bmp", 64, 32, 4, 2);
	_loading->loadFrameImage("LightBlue", "Image/Character/LightBlue.bmp", 16, 32, 1, 2);
	_loading->loadFrameImage("Orange", "Image/Character/Orange.bmp", 16, 32, 1, 2);
	_loading->loadFrameImage("Yellow", "Image/Character/Yellow.bmp", 32, 32, 2, 2);
	//#=============================================================================================================
	//#									에너미 이미지 불러오기
	//#=============================================================================================================
	_loading->loadImage("Dummy", "Image/Enemy/Dummy/Dummy.bmp", 39, 52);
	_loading->loadFrameImage("Napstablook", "Image/Enemy/Napstablook/Napstablook.bmp", 104, 83, 2, 1);
	_loading->loadFrameImage("SansFace", "Image/Enemy/Sans/Face/face.bmp", 160, 90, 5, 3);
	_loading->loadImage("SansBody", "Image/Enemy/Sans/Body/body.bmp", 54, 72);
	_loading->loadFrameImage("SansHandDown", "Image/Enemy/Sans/Body/HandDown.bmp", 310, 72, 4, 1);
	_loading->loadFrameImage("SansHandRight", "Image/Enemy/Sans/Body/HandRight.bmp", 310, 72, 4, 1);
	//#=============================================================================================================
	//#									에너미 공격 이미지 불러오기
	//#=============================================================================================================
	_loading->loadImage("Tear", "Image/AttackEffect/Tear.bmp", 12, 13);
	_loading->loadFrameImage("GasterBlaster", "Image/AttackEffect/GasterBlaster.bmp", 252, 57, 6, 1);
	_loading->loadFrameImage("KnifeSlash", "Image/AttackEffect/KnifeSlash.bmp", 96, 66, 6, 1);
	_loading->loadFrameImage("Bone", "Image/AttackEffect/Bone.bmp", 10, 18, 1, 3);
	_loading->loadFrameImage("Bone", "Image/AttackEffect/BlueBone.bmp", 10, 18, 1, 3);
	//로딩이 너무 빠르게 진행되서 천천히 돌아가도록 테스트용으로 만들기

}

//로딩사운드 함수(이곳에 사운드를 전부 넣어라)
void loadingScene::loadingSound()
{
}
