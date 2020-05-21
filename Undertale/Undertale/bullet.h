#pragma once
#include "gameNode.h"

//총알 구조체
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
//	## bullet ## (공용총알 - 너희들이 만들어야 함)
//=============================================================
class bullet : public gameNode
{
private:
	//총알 구조체를 담을 벡터, 반복자
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

private:
	const char* _imageName;	//총알 이미지 이름
	float _range;			//총알 사거리
	int _bulletMax;			//총알 최대갯수
	bool _isFrameImg;		//프레임 이미지냐?

public:
	HRESULT init(const char* imageName);
	void release();
	void update();
	void render();

	//총알발사
	void fire(float x, float y, float angle, float speed);
	//총알무브
	void move();

	//총알벡터 가져오기
	vector<tagBullet> getBullet() { return _vBullet; }
	//폭탄 삭제
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

	//총알발사
	void fire(float x, float y, float angle);
	//총알무브
	void move();

	//총알벡터 가져오기
	vector<tagTear> getTear() { return _vTear; }
	//폭탄 삭제
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