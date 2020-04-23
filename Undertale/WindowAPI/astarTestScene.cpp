#include "stdafx.h"
#include "astarTestScene.h"

HRESULT astarTestScene::init()
{
	IMAGEMANAGER->addImage("bg", "Images/background.bmp", 864, 624);
	IMAGEMANAGER->addImage("player", "Images/player.bmp", 48, 96);

	//��ü��� �ʱ�ȭ
	for (int y = 0; y < MAX_Y; y++)
	{
		for (int x = 0; x < MAX_X; x++)
		{
			//���ο� ���� ��Ʈ��ġ ����
			_totalNode[x][y] = new node(x, y);
			_totalNode[x][y]->rc = RectMake(x * 48, y * 48, 48, 48);
		}
	}
	_block = -1;

	//ùŬ���� ¦���� �ǰ� �ϱ� ���ؼ� -1�� �ʱ�ȭ
	_count = -1;
	//�� ã�ҳ�?
	_isFind = false;
	setWellNodeH();
	setWellNodeV();
	return S_OK;
}

void astarTestScene::release()
{
}

void astarTestScene::update()
{
	//����, ������ �����ϱ�
	if (_isFind)
	{
		if (INPUT->GetKeyDown(VK_SPACE)) 
		{
   			if (_block == _finalList.size()) 
			{
				_block = -1;
				_startNode->nodeState = NODE_EMPTY;
				_startNode = _endNode;
				_startNode->nodeState = NODE_START;
				_endNode = NULL;
				
				_finalList.clear();

				_isFind = FALSE; 
			}
			else { _block++; }
		}
	}
	else
	{
		if (INPUT->GetKeyDown(VK_LBUTTON))
		{
			//����, �����尡 �̹� ���õǾ� �ִٸ� �׳� ������
			if (_startNode && _endNode) { return; }
			else if (_startNode && !_endNode)
			{
				for (int y = 0; y < MAX_Y; y++)
				{
					for (int x = 0; x < MAX_X; x++)
					{
						if (PtInRect(&_totalNode[x][y]->rc, _ptMouse))
						{

							_totalNode[x][y]->nodeState = NODE_END;
							_endNode = _totalNode[x][y];

						}
					}
				}
			}
			else
			{
				for (int y = 0; y < MAX_Y; y++)
				{
					for (int x = 0; x < MAX_X; x++)
					{
						if (PtInRect(&_totalNode[x][y]->rc, _ptMouse))
						{
							_count++;
							if (_count % 2 == 0)
							{
								_totalNode[x][y]->nodeState = NODE_START;
								_startNode = _totalNode[x][y];
							}
							else
							{
								_totalNode[x][y]->nodeState = NODE_END;
								_endNode = _totalNode[x][y];
							}
						}
					}
				}
			}
		}

		//��(��ֹ�) ��� �����ϱ� (����, ������ �������� �������� ���ϰ� ����)
		if (INPUT->GetKeyDown(VK_RBUTTON) && _startNode && _endNode)
		{
			for (int y = 0; y <= MAX_Y; y++)
			{
				for (int x = 0; x <= MAX_X; x++)
				{
					if (PtInRect(&_totalNode[x][y]->rc, _ptMouse))
					{
						//���۳��, ������� �������� ���ϰ� ����
						if (_totalNode[x][y]->nodeState == NODE_START) continue;
						if (_totalNode[x][y]->nodeState == NODE_END) continue;
						_totalNode[x][y]->nodeState = NODE_WALL;
					}
				}
			}
		}

		//�����̽� ������ ��ã�ƶ�
		if (INPUT->GetKeyDown(VK_SPACE))
		{
			_openList.clear();
			_closeList.clear();
			setWellNodeH();
			setWellNodeV();
			this->pathFinding();
		}
	}
}

void astarTestScene::render()
{
	IMAGEMANAGER->findImage("bg")->render(getMemDC());
	char str[128];
	for (int y = 0; y < MAX_Y; y++)
	{
		for (int x = 0; x < MAX_X; x++)
		{
			//��ü��� ��Ʈ �����ֱ�
			HBRUSH myBrush = (HBRUSH)CreateSolidBrush(NULL_BRUSH);
			HBRUSH oldBrush = (HBRUSH)SelectObject(getMemDC(), GetStockObject(NULL_BRUSH));
			Rectangle(getMemDC(), _totalNode[x][y]->rc);
			SelectObject(getMemDC(), oldBrush);
			DeleteObject(myBrush);

			//���۳�� �����ֱ�
			if (_totalNode[x][y]->nodeState == NODE_START)
			{
				setNodeColor(_startNode, RGB(255, 0, 0));
				TextOut(getMemDC(), _startNode->rc.left, _startNode->rc.top + 10, "[Start]", strlen("[Start]"));
			}
			//������ �����ֱ�
			else if (_totalNode[x][y]->nodeState == NODE_END)
			{
				setNodeColor(_endNode, RGB(0, 0, 255));
				TextOut(getMemDC(), _endNode->rc.left, _endNode->rc.top + 10, "[END]", strlen("[END]"));
			}
			//����� �����ֱ�
			else if (_totalNode[x][y]->nodeState == NODE_WALL)
			{
				TextOut(getMemDC(), _totalNode[x][y]->rc.left, _totalNode[x][y]->rc.top + 10, "[WALL]", strlen("[WALL]"));
			}
			else
			{
				//��ü����� �ε��� �����ֱ�(�Ǹ������� ���)
				sprintf_s(str, "[%d, %d]", _totalNode[x][y]->idx, _totalNode[x][y]->idy);
				TextOut(getMemDC(), _totalNode[x][y]->rc.left, _totalNode[x][y]->rc.top + 10, str, strlen(str));
			}
		}
	}

	//��ã������ �����ֱ�
	if (_isFind)
	{
		for (int i = 0; i < _finalList.size(); i++)
		{
			setNodeColor(_finalList[i], RGB(255, 255, 0));
			sprintf_s(str, "[%d, %d]      %d�� ���", _finalList[i]->idx, _finalList[i]->idy, i + 1);
			TextOut(getMemDC(), _finalList[i]->rc.left + 10, _finalList[i]->rc.top + 10, str, strlen(str));
			sprintf_s(str, "G: %d, H: %d, F: %d", _finalList[i]->G, _finalList[i]->H, _finalList[i]->F);
			TextOut(getMemDC(), _finalList[i]->rc.left + 10, _finalList[i]->rc.bottom - 30, str, strlen(str));
		}
		if (_block == -1)
		{
			IMAGEMANAGER->findImage("player")->render(getMemDC(),
				_startNode->rc.left, _startNode->rc.top - 48);
		}
		else if (_block == _finalList.size())
		{
			IMAGEMANAGER->findImage("player")->render(getMemDC(),
				_endNode->rc.left, _endNode->rc.top - 48);
		}
		else
		{
			IMAGEMANAGER->findImage("player")->render(getMemDC(),
				_finalList[_block]->rc.left, _finalList[_block]->rc.top - 48);
		}
	}

}

//���Ǹ� ���� �Լ�
void astarTestScene::setNodeColor(node * node, COLORREF color)
{
	HBRUSH brush = CreateSolidBrush(color);
	FillRect(getMemDC(), &node->rc, brush);
	DeleteObject(brush);
	FrameRect(getMemDC(), node->rc, RGB(0, 0, 0));
}

void astarTestScene::pathFinding()
{
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

		//���¸���Ʈ�� F�� ���� �۰ų� F�� ���ٸ�
		//H�� ���� �� ������� �ϰ�
		//�����带 ���¸���Ʈ���� Ŭ���� ����Ʈ�� �ű��
		//�񱳸� �Ϸ��� �ϸ� �ּ� ���۳�忡�� �ֺ��� Ž���� ����
		//��ã�Ⱑ ���۵ȴ�

		for (int i = 1; i < _openList.size()-1; i++)
		{
			//if ������ ����
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
		addOpenList(_curNode->idx, _curNode->idy - 1);		//��
		addOpenList(_curNode->idx, _curNode->idy + 1);		//��
		addOpenList(_curNode->idx - 1, _curNode->idy);		//��
		addOpenList(_curNode->idx + 1, _curNode->idy);		//��
		//addOpenList(_curNode->idx - 1, _curNode->idy - 1);	//�»�
		//addOpenList(_curNode->idx + 1, _curNode->idy - 1);	//���
		//addOpenList(_curNode->idx - 1, _curNode->idy + 1);	//����
		//addOpenList(_curNode->idx + 1, _curNode->idy + 1);	//����

		//���Ŀ� �밢 4���⵵ �߰��ϸ� �밢�� �̵� ó���� ������
		//���, �»�, ����, ����
		//����ó���� �����ָ� �ȴ�
		//�����̷� ���� �ȵǵ��� ó���Ѵ�
	}

}

void astarTestScene::addOpenList(int idx, int idy)
{
	//����ó�� �ε��� �����ȿ��� �߰��� �� �־�� �Ѵ�
	if (idx < 0 || idx > MAX_X || idy < 0 || idy > MAX_Y) return;

	//���� ���¸���Ʈ�� ���� �� ����
	if (_totalNode[idx][idy]->nodeState == NODE_WALL) return;
	/*if (idx > 1 && idx < MAX_X && idy > 1 && idy < MAX_Y)
	{
		if (_totalNode[idx - 1][idy]->nodeState == NODE_WALL && _totalNode[idx][idy - 1]->nodeState == NODE_WALL) return;
		if (_totalNode[idx - 1][idy]->nodeState == NODE_WALL && _totalNode[idx][idy + 1]->nodeState == NODE_WALL) return;
		if (_totalNode[idx + 1][idy]->nodeState == NODE_WALL && _totalNode[idx][idy - 1]->nodeState == NODE_WALL) return;
		if (_totalNode[idx + 1][idy]->nodeState == NODE_WALL && _totalNode[idx][idy + 1]->nodeState == NODE_WALL) return;
	}*/
	//Ŭ�����Ʈ(�����±�)�� �ִٸ� ���¸���Ʈ�� ������ �ȵȴ�
	for (int i = 0; i < _closeList.size(); i++)
	{
		if (_totalNode[idx][idy] == _closeList[i]) return;
	}

	//������� ������ ������ ������ ���� ���¸���Ʈ�� �߰��� ����
	//�������� 4���� ��带 �̿������ �ϰ� ����10, �밢�� 14����� �߰��Ѵ�
	node* neighborNode = _totalNode[idx][idy];
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

void astarTestScene::delOpenList(int index)
{
	_openList.erase(_openList.begin() + index);
}

void astarTestScene::setWellNodeH()
{
	for (int i = 5; i < 9; i++) { _totalNode[0][i]->nodeState = NODE_WALL; }
	
	for (int i = 0; i < 4; i++) { _totalNode[1][i]->nodeState = NODE_WALL; }
	for (int i = 10; i < 12; i++) { _totalNode[1][i]->nodeState = NODE_WALL; }
	
	for (int i = 3; i < 6; i++) { _totalNode[2][i]->nodeState = NODE_WALL; }
	for (int i = 9; i < 12; i++) { _totalNode[2][i]->nodeState = NODE_WALL; }
	
	for (int i = 3; i < 7; i++) { _totalNode[5][i]->nodeState = NODE_WALL; }
	
	for (int i = 3; i < 8; i++) { _totalNode[6][i]->nodeState = NODE_WALL; }
	
	for (int i = 0; i < 2; i++) { _totalNode[7][i]->nodeState = NODE_WALL; }
	for (int i = 6; i < 8; i++) { _totalNode[7][i]->nodeState = NODE_WALL; }
	for (int i = 9; i < 13; i++) { _totalNode[7][i]->nodeState = NODE_WALL; }
	
	for (int i = 0; i < 5; i++) { _totalNode[8][i]->nodeState = NODE_WALL; }
	
	for (int i = 6; i < 8; i++) { _totalNode[9][i]->nodeState = NODE_WALL; }

	for (int i = 4; i < 9; i++) { _totalNode[10][i]->nodeState = NODE_WALL; }
	for (int i = 10; i < 12; i++) { _totalNode[10][i]->nodeState = NODE_WALL; }

	for (int i = 0; i < 5; i++) { _totalNode[13][i]->nodeState = NODE_WALL; }
	for (int i = 8; i < 12; i++) { _totalNode[13][i]->nodeState = NODE_WALL; }

	for (int i = 4; i < 7; i++) { _totalNode[15][i]->nodeState = NODE_WALL; }

	for (int i = 8; i < 13; i++) { _totalNode[16][i]->nodeState = NODE_WALL; }

	for (int i = 0; i < 3; i++) { _totalNode[17][i]->nodeState = NODE_WALL; }
}

void astarTestScene::setWellNodeV()
{
	for (int i = 7; i < 9; i++) { _totalNode[i][0]->nodeState = NODE_WALL; }
	for (int i = 10; i < 14; i++) { _totalNode[i][0]->nodeState = NODE_WALL; }
	for (int i = 15; i < 18; i++) { _totalNode[i][0]->nodeState = NODE_WALL; }
	
	for (int i = 3; i < 6; i++) { _totalNode[i][1]->nodeState = NODE_WALL; }
	for (int i = 7; i < 9; i++) { _totalNode[i][1]->nodeState = NODE_WALL; }

	for (int i = 8; i < 12; i++) { _totalNode[i][2]->nodeState = NODE_WALL; }
	for (int i = 13; i < 16; i++) { _totalNode[i][2]->nodeState = NODE_WALL; }

	for (int i = 1; i < 3; i++) { _totalNode[i][3]->nodeState = NODE_WALL; }
	for (int i = 4; i < 7; i++) { _totalNode[i][3]->nodeState = NODE_WALL; }

	for (int i = 5; i < 7; i++) { _totalNode[i][4]->nodeState = NODE_WALL; }
	for (int i = 15; i < 17; i++) { _totalNode[i][4]->nodeState = NODE_WALL; }

	for (int i = 5; i < 7; i++) { _totalNode[i][5]->nodeState = NODE_WALL; }

	for (int i = 5; i < 8; i++) { _totalNode[i][6]->nodeState = NODE_WALL; }
	for (int i = 9; i < 11; i++) { _totalNode[i][6]->nodeState = NODE_WALL; }
	for (int i = 13; i < 18; i++) { _totalNode[i][6]->nodeState = NODE_WALL; }

	for (int i = 0; i < 4; i++) { _totalNode[i][7]->nodeState = NODE_WALL; }
	for (int i = 6; i < 12; i++) { _totalNode[i][7]->nodeState = NODE_WALL; }

	for (int i = 4; i < 9; i++) { _totalNode[i][9]->nodeState = NODE_WALL; }
	for (int i = 12; i < 15; i++) { _totalNode[i][9]->nodeState = NODE_WALL; }

	for (int i = 1; i < 3; i++) { _totalNode[i][10]->nodeState = NODE_WALL; }

	for (int i = 0; i < 6; i++) { _totalNode[i][11]->nodeState = NODE_WALL; }
	for (int i = 9; i < 12; i++) { _totalNode[i][11]->nodeState = NODE_WALL; }
	for (int i = 13; i < 17; i++) { _totalNode[i][11]->nodeState = NODE_WALL; }

}
