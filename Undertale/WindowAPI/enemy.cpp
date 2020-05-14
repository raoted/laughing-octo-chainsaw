#include "stdafx.h"
#include "enemy.h"

HRESULT Dummy::init()
{
	_image = IMAGEMANAGER->findImage("Dummy");
	_atk = 0;
	_def = 0;
	_HP = _maxHP = 5;
	_getMoney = 100;
	_getExp = 200;
	_name = "����";

	_vString.clear();
	_explanation.clear();
	_vString.push_back("*(������ �����̴�.)");
	_explanation.push_back(_vString);

	_act.push_back("���캸��");
	_act.push_back("��ȭ�ϱ�");

	_marcy.push_back("��������");
	_marcy.push_back("����ֱ�");

	return S_OK;
}

void Dummy::release()
{
}

void Dummy::update()
{
}

void Dummy::render(int x, int y)
{
}

void Dummy::attack()
{
}

void Dummy::animation()
{
}

void Dummy::pattern1()
{
}


HRESULT Napstablook::init()
{
	_image = IMAGEMANAGER->findImage("Napstablook");
	_image->setFrameX(0);
	_image->setFrameY(0);

	_atk = 5;
	_def = 4;
	_HP = _maxHP = 88;
	_getMoney = 0;
	_getExp = 0;
	_name = "����Ÿ���";

	_vString.clear();
	_explanation.clear();
	_vString.push_back("*(�� ������ ���� ������ ���� ���δ�...)");
	_explanation.push_back(_vString);
	_vString.clear();
	_vString.push_back("*(�� ������ �ڽŰ��� ���� ���δ�...)");
	_explanation.push_back(_vString);
	_count = 0;
	_frame = 0;

	_act.push_back("���캸��");
	_act.push_back("��ȭ�ϱ�");

	_marcy.push_back("��������");
	_marcy.push_back("����ֱ�");

	return S_OK;
}

void Napstablook::release()
{
}

void Napstablook::update()
{
	this->animation();
}

void Napstablook::render(int x, int y)
{
	_image->scaleFrameRender(getMemDC(), x, y, _image->getFrameX(), _image->getFrameY(), 2.0f);
}

void Napstablook::attack()
{

}

void Napstablook::animation()
{
	_count++;
	if (_count == 10)
	{
		_count = 0;
		_frame++;
		if (_frame > 1) { _frame = 0; }
	}
	_image->setFrameX(_frame);
}

void Napstablook::pattern1()
{
	_tear->fire(25, 25, RANDOM->Range(0.0f, 3.0f), 5.0);
	_tear->fire(40, 30, RANDOM->Range(0.0f, 3.0f), 5.0);
}

HRESULT Sans::init()
{
	_image = IMAGEMANAGER->findImage("Napstablook");
	_image->setFrameX(0);
	_image->setFrameY(0);

	_atk = 5;
	_def = 4;
	_HP = _maxHP = 88;
	_getMoney = 0;
	_getExp = 0;
	_name = "����";

	_vString.clear();
	_explanation.clear();
	_vString.push_back("���� ���� ��. 1�� ���ظ� �� �� �ִ�.");
	_explanation.push_back(_vString);
	_vString.clear();
	_vString.push_back("���� ���� ��. 1�� ���ظ� �� �� �ִ�.");
	_vString.push_back("������ ���� ���� ����. ��� ��������.");
	_explanation.push_back(_vString);
	_count = 0;
	_frame = 0;

	_act.push_back("���캸��");
	_act.push_back("��ȭ�ϱ�");

	_marcy.push_back("��������");
	_marcy.push_back("����ֱ�");

	return S_OK;
}

void Sans::release()
{
}

void Sans::update()
{
	this->animation();
}

void Sans::render(int x, int y)
{
	IMAGEMANAGER->scaleRender("SansBody", getMemDC(), x, y, 2.0f);
	IMAGEMANAGER->scaleFrameRender("SansFace", getMemDC(), x + 22, y + _frame*2,
		IMAGEMANAGER->findImage("SansFace")->getFrameX(), 
		IMAGEMANAGER->findImage("SansFace")->getFrameY(), 
		2.0f);
}

void Sans::attack()
{
	switch (_phase)
	{
	case 1:
		break;
	case 2:
		if (_state == _ATTACK)
		{

		}
		break;
	}
}

void Sans::animation()
{
	_count++;
	if (_count == 10)
	{
		_count = 0;
		_frame++;
		if (_frame > 1) { _frame = 0; }
	}
}

void Sans::setSansFace(int i)
{
	IMAGEMANAGER->findImage("SansFace")->setFrameX(i % 5);
	IMAGEMANAGER->findImage("SamsFace")->setFrameY(i / 5);
}

void Sans::pattern1()
{

}

