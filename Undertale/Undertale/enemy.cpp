#include "stdafx.h"
#include "enemy.h"

HRESULT Dummy::init()
{
	_image = IMAGEMANAGER->findImage("Dummy");
	_atk = 0;
	_def = 0;
	_HP = _maxHP = 1;
	_getMoney = 100;
	_getExp = 200;
	_name = "더미";

	_vString.clear();
	_explanation.clear();
	_vString.push_back("*(연습용 인형이다.)");
	_explanation.push_back(_vString);

	_act.push_back("살펴보기");
	_act.push_back("대화하기");

	_marcy.push_back("도망가기");
	_marcy.push_back("살려주기");

	_vString.push_back("*(더미는 인형답게 가만히 서있다.)");
	_start.push_back(_vString);
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
	_image->scaleRender(getMemDC(), x, y, 2.0f);
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
	_HP = _maxHP = 8;
	_getMoney = 0;
	_getExp = 0;
	_name = "냅스타블룩";

	_vString.clear();
	_explanation.clear();
	_vString.push_back("*(이 괴물은 슬프게 울고 있다...)");
	_explanation.push_back(_vString);
	_count = 0;
	_frame = 0;

	_act.push_back("살펴보기");
	_act.push_back("대화하기");

	_marcy.push_back("도망가기");
	_marcy.push_back("살려주기");
	_vString.clear();
	_vString.push_back("*(냅스타블룩이 눈물을 흘리며 다가온다.)");
	_start.push_back(_vString);
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

//void Napstablook::pattern1()
//{
//	_tear->fire(25, 25, RANDOM->Range(0.0f, 3.0f), 5.0);
//	_tear->fire(40, 30, RANDOM->Range(0.0f, 3.0f), 5.0);
//}

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
	_name = "샌즈";

	_count = 0;
	_frame = 0;
	_round = 0;

	_act.push_back("살펴보기");
	_act.push_back("대화하기");

	_marcy.push_back("도망가기");
	_marcy.push_back("살려주기");

	_vString.push_back("*(당신은 죄악이 등을 타고 오르는 것을 느꼈다.)");
	_start.push_back(_vString);
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

vector<vector<string>> Sans::getExplain(int i)
{
	switch (i)
	{
	case 0:
		_vString.clear();
		_explanation.clear();
		_vString.push_back("*(가장 쉬운 적.)");
		_vString.push_back("*(1의 피해만 줄 수 있다.)");
		_explanation.push_back(_vString);
		break;
	default:
		_vString.clear();
		_vString.push_back("*(가장 쉬운 적.)");
		_vString.push_back("*(1의 피해만 줄 수 있다.");
		_explanation.push_back(_vString);
		_vString.clear();
		_vString.push_back("*(영원히 피할 수는 없다.)");
		_vString.push_back("*(계속 공격하자.)");
		_explanation.push_back(_vString);
		break;
	}

	return _explanation;
}

vector<vector<string>> Sans::getActText(int i)
{
	switch (i)
	{
	case 0:
		return getExplain(_round);
		break;
	case 1:
		return getTalkText(_round);
		break;
	default:
		return getTalkText(_round);
		break;
	}
}

vector<vector<string>> Sans::getTalkText(int i)
{
	_talk.clear();
	_vString.push_back("*(대화할 생각이 없는 것 같다)");
	_talk.push_back(_vString);
	_vString.clear();

	return _talk;
}

vector<vector<string>> Sans::getStart()
{
	/*switch (i)
	{
	case 0:
	{
		_vString.push_back("...양심도 없냐?");
		_bTalk.push_back(_vString);
		break;
	}
	case 1:
	{
		_vString.push_back("...양심도 없냐?");
		_bTalk.push_back(_vString);
		break;
	}
	case 2:
	{
		_vString.push_back("무조건 여기서 끝내는거야.");
		_bTalk.push_back(_vString);
		break;
	}
	case 3:
	{
		_vString.push_back("그 난리를 치고 다시 돌아온 이유가 뭐야?");
		_bTalk.push_back(_vString);
		break;
	}
	case 4:
	{
		_vString.push_back("아주머니는 뭘 보고 널 선택한거지?");
		_bTalk.push_back(_vString);
		break;
	}
	}*/
	return _start;
}
