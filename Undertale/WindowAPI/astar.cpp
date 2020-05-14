#include "stdafx.h"
#include "astar.h"

HRESULT astar::init()
{
	//��ü��� �ʱ�ȭ
	
	//ùŬ���� ¦���� �ǰ� �ϱ� ���ؼ� -1�� �ʱ�ȭ
	//�� ã�ҳ�?
	_isFind = false;

	return S_OK;
}

void astar::release()
{
}

void astar::update()
{
	//��(��ֹ�) ��� �����ϱ� (����, ������ �������� �������� ���ϰ� ����)
	if (INPUT->GetKeyDown(VK_RBUTTON) && _startNode && _endNode)
	{
		for (int y = 0; y < MAX_Y; y++)
		{
			for (int x = 0; x < MAX_X; x++)
			{
				if (PtInRect(&_totalNode[y * MAX_X + x]->rc, _ptMouse))
				{
					//���۳��, ������� �������� ���ϰ� ����
					if (_totalNode[y* MAX_X + x]->nodeState == NODE_START) continue;
					if (_totalNode[y* MAX_X + x]->nodeState == NODE_END) continue;
					_totalNode[y* MAX_X + x]->nodeState = NODE_WALL;
				}
			}
		}
	}

	//�����̽� ������ ��ã�ƶ�
	if (INPUT->GetKeyDown(VK_SPACE))
	{
		this->pathFinding();
		_count = -1;
	}
}

void astar::render()
{
	char str[128];
	
	if (_startNode != nullptr) { setNodeColor(_startNode, RGB(255, 0, 0)); }
	if (_endNode != nullptr) { setNodeColor(_endNode, RGB(0, 0, 255)); }
	//��ã������ �����ֱ�
	if (_isFind)
	{
		for (int i = 0; i < _finalList.size(); i++)
		{
			setNodeColor(_finalList[i], RGB(255, 255, 0));
		}
	}

}

//���Ǹ� ���� �Լ�
void astar::setNodeColor(node* node, COLORREF color)
{
	HBRUSH brush = CreateSolidBrush(color);
	FillRect(getMemDC(), &node->rc, brush);
	DeleteObject(brush);
	FrameRect(getMemDC(), node->rc, RGB(0, 0, 0));
}

void astar::pathFinding()
{
	_openList.clear();		//�� ã�� ���� _openList �ʱ�ȭ
	_closeList.clear();		//�� ã�� ���� _closeList �ʱ�ȭ
	_finalList.clear();		//�� ã�� ���� _finalList �ʱ�ȭ
	//�����尡 ���� ��� ��ã�� ����
	if (!_endNode) return;

	//��ã�⸦ �غ���
	//�˻��� �Ϸ��� ������ ���¸���Ʈ�� ��´�
	//F�� H�� ���� ���� ���� ã�Ƽ� �׳��� ������� �����Ѵ�
	//���¸���Ʈ���� ������� �����ϰ�
	//������� Ŭ�����Ʈ�� ��Ƶд�
	//���� �� ã�Ҵٸ� Ŭ�����Ʈ ���������� ���̳� ����Ʈ�� �ű��

	//1. ���۳�尡 �־�� ����� �����ϴ�
	//���۳�带 ���¸���Ʈ�� �߰��� ����� �Ѵ�
	_openList.push_back(_startNode);

	//2. ���¸���Ʈ�ȿ� ��� �ִ� ���͸� �˻��ؼ�
	//�����忡 �����Ҷ����� ���� ����
	while (_openList.size() > 0)
	{
		_curNode = _openList[0];

		setNodeColor(_curNode, RGB(0, 255, 0));
		//���¸���Ʈ�� F�� ���� �۰ų� F�� ���ٸ�
		//H�� ���� �� ������� �ϰ�
		//�����带 ���¸���Ʈ���� Ŭ���� ����Ʈ�� �ű��
		//�񱳸� �Ϸ��� �ϸ� �ּ� ���۳�忡�� �ֺ��� Ž���� ����
		//��ã�Ⱑ ���۵ȴ�

		for (int i = 1; i < _openList.size(); i++)
		{
			if (_openList[i]->F <= _curNode->F && _openList[i]->H < _curNode->H)
			{
				_curNode = _openList[i];
			}
		}

		//Ŭ���� ����Ʈ�� �־�д�
		for (int i = 0; i < _openList.size(); i++)
		{
			if (_curNode == _openList[i])
			{
				this->delOpenList(i);
				_closeList.push_back(_curNode);
			}
		}

		//�����尡 ������ ���� ����? (��ã�Ҵ�)
		if (_curNode == _endNode)
		{
			node* endNode = _endNode;
			vector<node*> tempNode;
			//������ ���κ��� ���۳����� �θ��带 ���Ϳ� ��´�
			while (endNode != _startNode)
			{
				tempNode.push_back(endNode);
				endNode = endNode->parentNode;
			}

			for (int i = tempNode.size() - 1; i > 0; i--)
			{
				_finalList.push_back(tempNode[i]);
			}

			_isFind = true;
			//�����ϰ� ���� ���´�
			return;
		}

		//�����¿� (������ ������� - ��¥�� �ֺ� 4���� ��带 ��� ���¸���Ʈ�� ��Ƽ� �˻��� ������)
		addOpenList(_curNode->idx, _curNode->idy - 1);	//��
		addOpenList(_curNode->idx, _curNode->idy + 1);	//��
		addOpenList(_curNode->idx - 1, _curNode->idy);	//��
		addOpenList(_curNode->idx + 1, _curNode->idy);	//��
		addOpenList(_curNode->idx - 1, _curNode->idy - 1);	//��
		addOpenList(_curNode->idx - 1, _curNode->idy + 1);	//��
		addOpenList(_curNode->idx + 1, _curNode->idy - 1);	//��
		addOpenList(_curNode->idx + 1, _curNode->idy + 1);	//��

		//���Ŀ� �밢 4���⵵ �߰��ϸ� �밢�� �̵� ó���� ������
		//���, �»�, ����, ����
		//����ó���� �����ָ� �ȴ�
		//�����̷� ���� �ȵǵ��� ó���Ѵ�
	}

}

void astar::addOpenList(int idx, int idy)
{
	//����ó�� �ε��� �����ȿ��� �߰��� �� �־�� �Ѵ�
	if (idx < 0 || idx >= MAX_X || idy < 0 || idy >= MAX_Y) { return; }

	//���� ���¸���Ʈ�� ���� �� ����
	if (_totalNode[idy * MAX_X + idx]->nodeState == NODE_WALL) { return; }

	//Ŭ�����Ʈ(�����±�)�� �ִٸ� ���¸���Ʈ�� ������ �ȵȴ�
	for (int i = 0; i < _closeList.size(); i++)
	{
		if (_totalNode[idy * MAX_X + idx] == _closeList[i]) { return; }
	}

	//������� ������ ������ ������ ���� ���¸���Ʈ�� �߰��� ����
	//�������� 4���� ��带 �̿������ �ϰ� ����10, �밢�� 14����� �߰��Ѵ�
	node* neighborNode = _totalNode[idy* MAX_X + idx];
	int moveCost = _curNode->G + ((_curNode->idx - idx == 0 || _curNode->idy - idy == 0) ? 10 : 14);

	//���¸���Ʈ�ȿ� �̿���尡 ������ �ȵȴ�
	for (int i = 0; i < _openList.size(); i++)
	{
		if (_openList[i] == neighborNode) return;
	}

	//���������� ���¸���Ʈ���� ���°��
	//G, H, ParentNode ������ ���¸���Ʈ�� �߰�
	//F = G + H
	//G = ���ۿ��� ����
	//H = ���翡�� ����
	neighborNode->G = moveCost;
	neighborNode->H = (abs(neighborNode->idx - _endNode->idx) + abs(neighborNode->idy - _endNode->idy)) * 10;
	neighborNode->F = neighborNode->G + neighborNode->H;
	neighborNode->parentNode = _curNode;
	_openList.push_back(neighborNode);
}

void astar::delOpenList(int index)
{
	_openList.erase(_openList.begin() + index);
}

void astar::setStartNode(int x, int y)
{
	if (_startNode)
	{
		_totalNode[_startNode->idy * MAX_X + _startNode->idx]->nodeState = NODE_EMPTY;

	}
	_totalNode[y * MAX_X + x]->nodeState = NODE_START;
	_startNode = _totalNode[y * MAX_X + x];
}

void astar::setEndNode(int index)
{
	if (_endNode)
	{
		_totalNode[_endNode->idy * MAX_X + _endNode->idx]->nodeState = NODE_EMPTY;
	}
	_totalNode[index]->nodeState = NODE_END;
	_endNode = _totalNode[index];
}
