#include "stdafx.h"
#include "mainGame.h"

//=============================================================
//	## 초기화 ## init()
//=============================================================
HRESULT mainGame::init()
{
	gameNode::init();
	//클래스 초기화를 이곳에서 한다
	
	/*앞으로 메인게임은 가각의 씬들만 관리를 한다*/
	//각각의 씬들은 메인게임 역할을 한다

	/*씬추가*/
	SCENEMANAGER->addScene("시작화면", new startScene);
	SCENEMANAGER->addScene("게임화면", new inGameScene);
	SCENEMANAGER->addScene("로딩화면", new loadingScene);
	SCENEMANAGER->addScene("맵툴화면", new maptoolScene);
	SCENEMANAGER->addScene("전투화면", new battleScene);
	

	/*현재씬 설정*/
	SCENEMANAGER->loadScene("로딩화면");

	ITEMMANAGER->addFoodItem("버터스카치 파이", "버터스카치 파이", 99, "버터스카치-시나몬 파이 한 조각이다.", 180);
	ITEMMANAGER->addFoodItem("전설의 영웅", "전설의 영웅", 99, "검 모양 샌드위치.", 40);
	ITEMMANAGER->addFoodItem("눈사람 조각", "눈사람 조각", 45, "부디 이 땅의 끝까지 가져가줬으면 좋겠어.", 40);

	return S_OK;
}

//=============================================================
//	## 해제 ## release()
//=============================================================
void mainGame::release()
{
	gameNode::release();

}

//=============================================================
//	## 업데이트 ## update()
//=============================================================
void mainGame::update()
{
	gameNode::update();
	
	//씬매니져 업데이트
	SCENEMANAGER->update();
	//사운드매니져 업데이트(이놈안해주면 사운드가 정상작동안함)
	SOUNDMANAGER->update();
}

//=============================================================
//	## 렌더 ## render()
//=============================================================
void mainGame::render()
{
	//흰색 빈 비트맵
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, BLACKNESS);
//=============================================================	
	
	//글자색 배경 없애기
	SetBkMode(getMemDC(), TRANSPARENT);
	//씬매니져 렌더
	SCENEMANAGER->render();

	TIMEMANAGER->render(getMemDC());


	//글자배경모드
	SetBkMode(getMemDC(), TRANSPARENT);
	//텍스트 색상 변경
	SetTextColor(getMemDC(), RGB(0, 0, 0));

//=============================================================
	//백버퍼의 내용을 화면DC에 그린다
	this->getBackBuffer()->render(getHDC());
}
