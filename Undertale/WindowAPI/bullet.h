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
	//총알 구조체를 담을 벡터, 반복자
	vector<tagBullet> _vBullet;
	vector<tagBullet>::iterator _viBullet;

private:
	const char* _imageName;			//총알 이미지 이름
	float _range;					//총알 사거리
	int _bulletMax;					//총알 최대갯수
	bool _isFrameImg;				//프레임 이미지냐?

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
	void removeBomb();

	tear() {}
	~tear() {}
};