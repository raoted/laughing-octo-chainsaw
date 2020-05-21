#pragma once
#include "gameNode.h"

//�Ѿ� ����ü
struct tagBullet
{
	image* bulletImage;
	RECT rc;
	float x, y;
	float fireX, fireY;
	float speed;
	float angle;
	float gravity;
	float radius;
	bool fire;
	int count;
};

struct tagBone
{
	RECT rc;
	int x, y;
	int speed = 5;
	bool isleft;
};

struct tagTear
{
	RECT rc;
	float x, y;
	float fireX, fireY;
	int speed = 3;
	float angle;
	float radius;
};
//=============================================================
//	## bullet ## (�����Ѿ� - ������� ������ ��)
//=============================================================
class bullet : public gameNode
{
private:
	//�Ѿ� ����ü�� ���� ����, �ݺ���
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

private:
	const char* _imageName;	//�Ѿ� �̹��� �̸�
	float _range;			//�Ѿ� ��Ÿ�
	int _bulletMax;			//�Ѿ� �ִ밹��
	bool _isFrameImg;		//������ �̹�����?

public:
	HRESULT init(const char* imageName);
	void release();
	void update();
	void render();

	//�Ѿ˹߻�
	void fire(float x, float y, float angle, float speed);
	//�Ѿ˹���
	void move();

	//�Ѿ˺��� ��������
	vector<tagBullet> getBullet() { return _vBullet; }
	//��ź ����
	void removeBullet(int index);

	bullet() {}
	~bullet() {}
};



class tear : public gameNode
{
private:
	RECT _rc;
	int _width = 20;
	int _height = 12;

	int _count;

	float _time;
	float _x, _y;
	tagTear _tear;
	vector<tagTear>_vTear;
public:
	HRESULT init(RECT rc);
	void release();
	void update(float x, float y);
	void render();

	//�Ѿ˹߻�
	void fire(float x, float y, float angle);
	//�Ѿ˹���
	void move();

	//�Ѿ˺��� ��������
	vector<tagTear> getTear() { return _vTear; }
	//��ź ����
	void erase(int i) { _vTear.erase(_vTear.begin() + i); }
	void clear();

	tear() {}
	~tear() {}
};

class bone : public gameNode
{
private:
	RECT _rc;
	POINT _p;
	int _width = 20;
	int _height = 12;

	int _count;

	float _time;
	tagBone _bone;
	vector<tagBone> _vBone;
public:
	HRESULT init(RECT rc);
	void release();
	void update();
	void render();

	void fire(bool isLeft, int pattern);
	vector<tagBone> getBone() { return _vBone; }
	void erase(int i) { _vBone.erase(_vBone.begin() + i); }
	void clear() { _vBone.clear(); }
};