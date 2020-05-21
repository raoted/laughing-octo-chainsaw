#pragma once

#include "gameNode.h"
#include "enemyManager.h"
#include "player.h"
#include <stack>
struct ATTACKBAR
{
    POINT pt;
    bool isLeft;
    const int speed = 10;
};
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

    int _battleCounter;     //전투씬에서 순서를 저장한다.

    float _time;
    int _count;
    int _frame;             //Animation 프레임
    int _round;             //전투씬에서 턴 수
    int _vDialogSize;
    int _dialogSize;        //dialog에서 문장별 최대 글자수를 저장한다.
    int _dialogCount;       //dialog에서 몇번째 글자까지 출력했는지 나타낸다.
    int _demage;
    string _sDemage;
private:
    bool _isBattleEnd;      //전투가 끝났는지 판단해주는 bool 변수
    bool _isWin;            //승리로 끝났는지 판단해주는 bool 변수
    bool _isPAttack;        //플레이어가 공격을 했는지 판단해주는 bool 변수
    bool _isDialog;         //Dialog를 실행하는지 판단해주는 bool 변수
private:
    ATTACKBAR _attBar;
    stack<string> _playerAct;   //플레이어의 행동을 저장할 Stack
    vector<TEXT> _antiEnemy;    
    vector<vector<string>> _dialogText;

    RECT _enemyArea;    //적이 등장하는 공간
    RECT _playerArea;   //플레이어가 선택, 공격, 회피를 하는 공간
    RECT _actButton[4]; //4가지 행동 버튼. 0번: 공격, 1번: 행동, 2번: 아이템, 3번: 자비
private:
    enemyManager* _enemyManager;    //에너미의 데이터가 들어있는 클래스 객체
    player* _player;                //플레이어 데이터가 들어있는 클래스 객체
    HFONT _font, _oldFont;      //큰 글씨를 그릴 때 사용할 HFONT
    LOGFONT _lf;
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
    void playerInfo();
    void attAnimation();
    void dialog();
};

  