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
    int _bNumber;       //��ư ��ȣ
    int _enemyNumber;   //�� ��ȣ
    int _x, _y;
    int _imageWidth, _imageHeight;
    int _selectCount;

    int _battleCounter;     //���������� ������ �����Ѵ�.

    float _time;
    int _count;
    int _frame;             //Animation ������
    int _round;             //���������� �� ��
    int _vDialogSize;
    int _dialogSize;        //dialog���� ���庰 �ִ� ���ڼ��� �����Ѵ�.
    int _dialogCount;       //dialog���� ���° ���ڱ��� ����ߴ��� ��Ÿ����.
    int _demage;
    string _sDemage;
private:
    bool _isBattleEnd;      //������ �������� �Ǵ����ִ� bool ����
    bool _isWin;            //�¸��� �������� �Ǵ����ִ� bool ����
    bool _isPAttack;        //�÷��̾ ������ �ߴ��� �Ǵ����ִ� bool ����
    bool _isDialog;         //Dialog�� �����ϴ��� �Ǵ����ִ� bool ����
private:
    ATTACKBAR _attBar;
    stack<string> _playerAct;   //�÷��̾��� �ൿ�� ������ Stack
    vector<TEXT> _antiEnemy;    
    vector<vector<string>> _dialogText;

    RECT _enemyArea;    //���� �����ϴ� ����
    RECT _playerArea;   //�÷��̾ ����, ����, ȸ�Ǹ� �ϴ� ����
    RECT _actButton[4]; //4���� �ൿ ��ư. 0��: ����, 1��: �ൿ, 2��: ������, 3��: �ں�
private:
    enemyManager* _enemyManager;    //���ʹ��� �����Ͱ� ����ִ� Ŭ���� ��ü
    player* _player;                //�÷��̾� �����Ͱ� ����ִ� Ŭ���� ��ü
    HFONT _font, _oldFont;      //ū �۾��� �׸� �� ����� HFONT
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

  