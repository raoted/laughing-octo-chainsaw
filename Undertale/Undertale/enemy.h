#pragma once
#include "gameNode.h"

enum ENEMYTYPE
{
	GHOST, 
	DUMMY, 
	SANS
};

enum ENEMYSTATE
{
	_IDLE, 
	_ATTACK, 
	_DEAD
};
//부모클래스 => 이놈을 상속받아서 보스, 일반몬스터를 만든다
class enemy : public gameNode
{
public:
	string _string[3];
	vector<string> _vString;
	virtual string getName() = 0;
	virtual vector<vector<string>> getExplain(int i) = 0;
	virtual vector<vector<string>> getActText(int i) = 0;
	virtual vector<vector<string>> getTalkText(int i) = 0;
	virtual vector<vector<string>> getStart() = 0;
	virtual int getHP() = 0;
	virtual void setHP(int hp) = 0;
	virtual int getAtk() = 0;
	virtual int getDef() = 0;
	virtual int addMoney() = 0;
	virtual int addExp() = 0;
	virtual vector<string> getAct() = 0;
	virtual vector<string> getMarcy() = 0;
	virtual image* getImage() = 0;

public:
	virtual HRESULT init() = 0;
	virtual void release() = 0;
	virtual void update() = 0;
	virtual void render(int x, int y) = 0;

	virtual void setTurn(int i) = 0;
	virtual void attack() = 0;
	virtual void animation() = 0;	//업데이트

	virtual void act(int i) = 0;
	virtual void marcy(int i) = 0;
	//충돌용 렉트 가져오기
	enemy() {}
	virtual ~enemy() {}
};

class Dummy : public enemy
{
private:
	image* _image;		//에너미 이미지
	ENEMYSTATE _state;

	int _HP;
	int _maxHP;
	int _atk;
	int _def;
	int _getMoney;
	int _getExp;
	int _pattern;
private:
	string _name;
	vector<vector<string>> _explanation;
	vector<vector<string>> _talk;
	vector<vector<string>> _start;

	vector<string>_act;
	vector<string>_marcy;
private:
	int _phase;
	//프레임 이미지를 돌리기 위한 변수
	int _turn;
	int _count;
	int _frame;
	int _currentFrameX;
	int _currentFrameY;
public:
	string getName() { return _name; }
	vector<vector<string>> getExplain(int i) 
	{
		return _explanation;
	}
	vector<vector<string>> getActText(int i) { return getExplain(i); }
	vector<vector<string>> getTalkText(int i) { return getExplain(i); }
	vector<vector<string>> getStart() { return _start; }
	int getHP() { return _HP; }
	void setHP(int hp) { _HP = hp; }
	int getAtk() { return _atk; }
	int getDef() { return _def; }
	int addMoney() { return _getMoney; }
	int addExp() { return _getExp; }
	vector<string> getAct() { return _act; }
	vector<string> getMarcy() { return _marcy; }
	image* getImage() { return _image; }
public:
	HRESULT init();
	void release();
	void update();
	void render(int x, int y);

	void setTurn(int i) { _turn = i; }
	void attack();
	void animation();

	void act(int i) {};
	void marcy(int i) {};

	void pattern1();
};

class Napstablook : public enemy
{
private:
	image* _image;		//에너미 이미지
	ENEMYSTATE _state;

	int _HP;
	int _maxHP;
	int _atk;
	int _def;
	int _getMoney;
	int _getExp;
	int _pattern;
	string _name;
	vector<vector<string>> _explanation;
	vector<vector<string>> _talk;
	vector<vector<string>> _start;
	vector<string>_act;
	vector<string>_marcy;
private:
	int _phase;
	//프레임 이미지를 돌리기 위한 변수
	int _turn;
	int _count;
	int _frame;
	int _currentFrameX;
	int _currentFrameY;
public:
	string getName() { return _name; }
	vector<vector<string>> getExplain(int i)
	{
		return _explanation;
	}
	vector<vector<string>> getActText(int i) { return getExplain(i); }
	vector<vector<string>> getTalkText(int i) { return getExplain(i); }
	vector<vector<string>> getStart() { return _start; }
	int getHP() { return _HP; }
	void setHP(int hp) { _HP = hp; }
	int getAtk() { return _atk; }
	int getDef() { return _def; }
	int addMoney() { return _getMoney; }
	int addExp() { return _getExp; }
	vector<string> getAct() { return _act; }
	vector<string> getMarcy() { return _marcy; }
	image* getImage() { return _image; }
public:
	HRESULT init();
	void release();
	void update();
	void render(int x, int y);

	void setTurn(int i) { _turn = i; }
	void attack();
	void animation();

	void act(int i) {};
	void marcy(int i) {};
	void pattern1();
};

class Sans : public enemy
{
private:
	image* _image;		//에너미 이미지
	ENEMYSTATE _state;

	int _HP;
	int _maxHP;
	int _atk;
	int _def;
	int _getMoney;
	int _getExp;
	int _pattern;
	int _round;
	string _name;
	vector<vector<string>> _explanation;
	vector<vector<string>> _talk;
	vector<vector<string>> _start;
	vector<string>_act;
	vector<string>_marcy;
private:
	int _phase;
	int _direct;
	//프레임 이미지를 돌리기 위한 변수
	int _turn;
	int _count;
	int _frame;
	int _currentFrameX;
	int _currentFrameY;
private:
	RECT _head;
	RECT _body;
public:
	string getName() { return _name; }
	vector<vector<string>> getExplain(int i);
	vector<vector<string>> getActText(int i);
	vector<vector<string>> getTalkText(int i);
	vector<vector<string>> getStart();
	int getHP() { return _HP; }
	void setHP(int hp) { _HP = hp; }
	int getAtk() { return _atk; }
	int getDef() { return _def; }
	int addMoney() { return _getMoney; }
	int addExp() { return _getExp; }
	void setRount(int i) { _round = i; }
	vector<string> getAct() { return _act; }
	vector<string> getMarcy() { return _marcy; }
	image* getImage() { return _image; }
public:
	HRESULT init();
	void release();
	void update();
	void render(int x, int y);

	void attack();

	void setTurn(int i) { _turn = i; }
	void animation();

	void act(int i) {};
	void marcy(int i) {};
	void setSansFace(int i);
	void pattern1();
};