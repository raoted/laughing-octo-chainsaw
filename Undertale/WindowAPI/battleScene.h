#pragma once

#include "gameNode.h"
#include "enemyManager.h"
#include "player.h"
#include <stack>
struct TEXT
{
    string _eString;
    RECT _eStringRect;
};

class battleScene : public gameNode
{
private:
    int _bNumber;       //버튼 번호
    int _enemyNumber;   //적 번호
    int _x, _y;
    int _imageWidth, _imageHeight;
    int _selectCount;

    int _battleCounter;     
private:
    bool _isBattleEnd;      //전투가 끝났는지 판단해주는 bool 변수
    bool _isWin;            //승리로 끝났는지 판단해주는 bool 변수
private:
    stack<string> _playerAct;   //플레이어의 행동을 저장할 Stack
    vector<TEXT> _antiEnemy;  

    RECT _enemyArea;    //적이 등장하는 공간
    RECT _playerArea;   //플레이어가 선택, 공격, 회피를 하는 공간
    RECT _actButton[4]; //4가지 행동 버튼. 0번: 공격, 1번: 행동, 2번: 아이템, 3번: 자비
private:
    enemyManager* _enemyManager;    //에너미의 데이터가 들어있는 클래스 객체
    player* _player;                //플레이어 데이터가 들어있는 클래스 객체
    HFONT _bigFont, _oBigFont;      //큰 글씨를 그릴 때 사용할 HFONT
    HFONT _nFont, _oNFont;          //중간 크기의 글씨를 그릴 떄 사용할 HFONT
    HFONT _sFont, _oSFont;          //작은 글씨를 그릴 때 사용할 HFONT
public:
    HRESULT init();
    void release();
    void update();
    void render();
public:
    void deadAnimation();
    void playerAreaFrame(RECT rc);
    void drawPlayerArea();
    void uiRender();
};

  