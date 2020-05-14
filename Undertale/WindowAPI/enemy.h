#pragma once
#include "gameNode.h"
#include "bullet.h"

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
//�θ�Ŭ���� => �̳��� ��ӹ޾Ƽ� ����, �Ϲݸ��͸� �����
class enemy : public gameNode
{
public:
	string _string[3];
	vector<string> _vString;
	virtual string getName() = 0;
	virtual vector<vector<string>> getExplain() = 0;
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
	virtual void animation() = 0;	//������Ʈ

	virtual void act(int i) = 0;
	virtual void marcy(int i) = 0;
	//�浹�� ��Ʈ ��������
	enemy() {}
	virtual ~enemy() {}
};

class Dummy : public enemy
{
private:
	image* _image;		//���ʹ� �̹���
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

	vector<string>_act;
	vector<string>_marcy;
private:
	int _phase;
	//������ �̹����� ������ ���� ����
	int _turn;
	int _count;
	int _frame;
	int _currentFrameX;
	int _currentFrameY;
private:
	tear* _tear;
public:
	string getName() { return _name; }
	vector<vector<string>> getExplain() 
	{
		return _explanation;
	}
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
	image* _image;		//���ʹ� �̹���
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

	vector<string>_act;
	vector<string>_marcy;
private:
	int _phase;
	//������ �̹����� ������ ���� ����
	int _turn;
	int _count;
	int _frame;
	int _currentFrameX;
	int _currentFrameY;
private:
	tear* _tear;
public:
	string getName() { return _name; }
	vector<vector<string>> getExplain()
	{
		return _explanation;
	}
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
	image* _image;		//���ʹ� �̹���
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

	vector<string>_act;
	vector<string>_marcy;
private:
	int _phase;
	int _direct;
	//������ �̹����� ������ ���� ����
	int _turn;
	int _count;
	int _frame;
	int _currentFrameX;
	int _currentFrameY;
private:
	tear* _tear;
private:
	RECT _head;
	RECT _body;
public:
	string getName() { return _name; }
	vector<vector<string>> getExplain()
	{
		return _explanation;
	}
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
	void setSansFace(int i);
	void pattern1();
};