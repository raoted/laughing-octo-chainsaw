#include "stdafx.h"
#include "bullet.h"

//=============================================================
//	## bullet ## (공용총알 - 너희들이 만들어야 함)
//=============================================================
HRESULT bullet::init(const char* imageName)
{
	//총알 이미지 초기화
	_imageName = imageName;

	return S_OK;
}

void bullet::release()
{
}

void bullet::update()
{
	this->move();
}

void bullet::render()
{

}

void bullet::fire(float x, float y, float angle, float speed)
{
	//총알 벡터에 담는것을 제한한다
	if (_bulletMax < _vBullet.size() + 1) return;

	//총알 구조체 선언
	tagBullet bullet;
	//총알구조체 초기화
	//제로메모리, 멤셋
	//구조체의 변수들의 값을 한번에 0으로 초기화 시켜준다
	ZeroMemory(&bullet, sizeof(tagBullet));
	bullet.bulletImage = new image;
	bullet.bulletImage = IMAGEMANAGER->findImage(_imageName);
	bullet.speed = 5.0f;
	bullet.angle = angle;
	bullet.x = bullet.fireX = x;
	bullet.y = bullet.fireY = y;
	if (_isFrameImg)
	{
		bullet.rc = RectMakeCenter(bullet.x, bullet.y,
			bullet.bulletImage->getFrameWidth(),
			bullet.bulletImage->getFrameHeight());
	}
	else
	{
		bullet.rc = RectMakeCenter(bullet.x, bullet.y,
			bullet.bulletImage->getWidth(),
			bullet.bulletImage->getHeight());
	}

	//벡터에 담기
	_vBullet.push_back(bullet);
}

void bullet::move()
{
	for (int i = 0; i < _vBullet.size(); i++)
	{
		_vBullet[i].x += cosf(_vBullet[i].angle) * _vBullet[i].speed;
		_vBullet[i].y += -sinf(_vBullet[i].angle) * _vBullet[i].speed;

		if (_isFrameImg)
		{
			_vBullet[i].rc = RectMakeCenter(_vBullet[i].x, _vBullet[i].y,
				_vBullet[i].bulletImage->getFrameWidth(),
				_vBullet[i].bulletImage->getFrameHeight());
		}
		else
		{
			_vBullet[i].rc = RectMakeCenter(_vBullet[i].x, _vBullet[i].y,
				_vBullet[i].bulletImage->getWidth(),
				_vBullet[i].bulletImage->getHeight());
		}

		//총알이 사거리 보다 커졌을때
		float distance = getDistance(_vBullet[i].fireX, _vBullet[i].fireY,
			_vBullet[i].x, _vBullet[i].y);
		if (_range < distance)
		{
			_vBullet.erase(_vBullet.begin() + i);
		}

	}
}
//폭탄 삭제
void bullet::removeBullet(int index)
{
	_vBullet.erase(_vBullet.begin() + index);
}

//===================================================================
//		##  눈물  ##
//===================================================================
HRESULT tear::init(RECT rc)
{
	_rc = rc;
	_count = 0;

	_time = TIMEMANAGER->getWorldTime();
	return S_OK;
}

void tear::release()
{

}

void tear::update(float x, float y)
{
	_x = x;
	_y = y;
	if (TIMEMANAGER->getWorldTime() - _time >= 0.01f)
	{
		if (_count % 15 == 0)
		{
			float angle = getAngle(WINSIZEX / 2, 150, _x, _y);
			this->fire(WINSIZEX / 2, 150, angle);
			angle = getAngle(WINSIZEX / 2 + 41, 145, _x, _y);
			this->fire(WINSIZEX / 2 + 41, 145, angle);
			if (_vTear.size() > 0)
			{
				this->move();
			}
		}
		_count++;
		_time = TIMEMANAGER->getWorldTime();
	}
	this->move();
}

void tear::render()
{
	for (int i = 0; i < _vTear.size(); i++)
	{
		
		IMAGEMANAGER->rotateRender("Tear", getMemDC(), _vTear[i].x, _vTear[i].y, _vTear[i].angle);
	}

}

void tear::fire(float x, float y, float angle)
{
	_tear.fireX = x - 6;
	_tear.x = _tear.fireX;
	_tear.fireY = y;
	_tear.y = _tear.fireY;
	_tear.angle = angle;
	_tear.radius = angle;
	_tear.rc = RectMakeCenter(_tear.x, _tear.y, 10, 10);
	//벡터에 담기
	_vTear.push_back(_tear);
}

void tear::move()
{
	for (int i = 0; i < _vTear.size(); i++)
	{
		_vTear[i].x = cosf(_vTear[i].angle) * _vTear[i].speed + _vTear[i].x;
		_vTear[i].y = -sinf(_vTear[i].angle) * _vTear[i].speed + _vTear[i].y;
		_vTear[i].rc = RectMakeCenter(_vTear[i].x, _vTear[i].y, 10, 10);

		if (_vTear[i].x <= _rc.left || _vTear[i].x + 12 >= _rc.right)
		{
			this->erase(i);
			i--;
			continue;
		}
		if (_vTear[i].y + 13 >= _rc.bottom)
		{
			this->erase(i);
			i--;
			continue;
		}
	}
}

void tear::clear()
{
	_vTear.clear();
}

HRESULT bone::init(RECT rc)
{
	_rc = rc;
	_count = 0;

	_time = TIMEMANAGER->getWorldTime();
	return S_OK;
}

void bone::release()
{
}

void bone::update()
{
	if (TIMEMANAGER->getWorldTime() - _time >= 0.01f)
	{
		if (_count % 60 == 0)
		{
			this->fire(TRUE, 1);
			this->fire(FALSE, 1);
		}
		else if (_count % 30 == 0)
		{
			this->fire(TRUE, 0);
			this->fire(FALSE, 0);
		}
		_count++;
		_time = TIMEMANAGER->getWorldTime();
	}

	for (int i = 0; i < _vBone.size(); i++)
	{
		if (_vBone[i].isleft)
		{
			_vBone[i].rc.left += _vBone[i].speed;
			_vBone[i].rc.right += _vBone[i].speed;
			if (_vBone[i].rc.left > _rc.right)
			{
				erase(i);
				i--;
			}
		}
		else
		{
			_vBone[i].rc.left -= _vBone[i].speed;
			_vBone[i].rc.right -= _vBone[i].speed;
			if (_vBone[i].rc.right < _rc.left)
			{
				erase(i);
				i--;
			}
		}
	}
}

void bone::render()
{
	for (int i = 0; i < _vBone.size(); i++)
	{
		IMAGEMANAGER->scaleRender("Bone1", getMemDC(), _vBone[i].rc.left, _vBone[i].rc.top, 2.0f);
	}
}

void bone::fire(bool isLeft, int pattern)
{
	_bone.isleft = isLeft;
	if (_bone.isleft) { _bone.x = _rc.left - _width; }
	else { _bone.x = _rc.right; }

	if (pattern == 0)
	{
		_bone.y = _rc.bottom - 88;
		_bone.rc = RectMake(_bone.x, _bone.y, 28, 88);
		_vBone.push_back(_bone);

		_bone.y = _rc.top;
		_bone.rc = RectMake(_bone.x, _bone.y, 28, 88);
		_vBone.push_back(_bone);
	}
	else
	{
		_bone.y = _rc.top + 106;
		_bone.rc = RectMake(_bone.x, _bone.y, 28, 88);
		_vBone.push_back(_bone);
	}
}
