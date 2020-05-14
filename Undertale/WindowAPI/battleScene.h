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
    int _bNumber;       //��ư ��ȣ
    int _enemyNumber;   //�� ��ȣ
    int _x, _y;
    int _imageWidth, _imageHeight;
    int _selectCount;

    int _battleCounter;     
private:
    bool _isBattleEnd;      //������ �������� �Ǵ����ִ� bool ����
    bool _isWin;            //�¸��� �������� �Ǵ����ִ� bool ����
private:
    stack<string> _playerAct;   //�÷��̾��� �ൿ�� ������ Stack
    vector<TEXT> _antiEnemy;  

    RECT _enemyArea;    //���� �����ϴ� ����
    RECT _playerArea;   //�÷��̾ ����, ����, ȸ�Ǹ� �ϴ� ����
    RECT _actButton[4]; //4���� �ൿ ��ư. 0��: ����, 1��: �ൿ, 2��: ������, 3��: �ں�
private:
    enemyManager* _enemyManager;    //���ʹ��� �����Ͱ� ����ִ� Ŭ���� ��ü
    player* _player;                //�÷��̾� �����Ͱ� ����ִ� Ŭ���� ��ü
    HFONT _bigFont, _oBigFont;      //ū �۾��� �׸� �� ����� HFONT
    HFONT _nFont, _oNFont;          //�߰� ũ���� �۾��� �׸� �� ����� HFONT
    HFONT _sFont, _oSFont;          //���� �۾��� �׸� �� ����� HFONT
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

  