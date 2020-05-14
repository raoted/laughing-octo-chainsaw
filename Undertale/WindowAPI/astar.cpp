#include "stdafx.h"
#include "astar.h"

HRESULT astar::init()
{
	//전체노드 초기화
	
	//첫클릭이 짝수가 되게 하기 위해서 -1로 초기화
	//길 찾았냐?
	_isFind = false;

	return S_OK;
}

void astar::release()
{
}

void astar::update()
{
	//벽(장애물) 노드 세팅하기 (시작, 종료노드 설정전에 벽세우지 못하게 막기)
	if (INPUT->GetKeyDown(VK_RBUTTON) && _startNode && _endNode)
	{
		for (int y = 0; y < MAX_Y; y++)
		{
			for (int x = 0; x < MAX_X; x++)
			{
				if (PtInRect(&_totalNode[y * MAX_X + x]->rc, _ptMouse))
				{
					//시작노드, 종료노드는 선택하지 못하게 막기
					if (_totalNode[y* MAX_X + x]->nodeState == NODE_START) continue;
					if (_totalNode[y* MAX_X + x]->nodeState == NODE_END) continue;
					_totalNode[y* MAX_X + x]->nodeState = NODE_WALL;
				}
			}
		}
	}

	//스페이스 누르면 길찾아라
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
	//길찾았을때 보여주기
	if (_isFind)
	{
		for (int i = 0; i < _finalList.size(); i++)
		{
			setNodeColor(_finalList[i], RGB(255, 255, 0));
		}
	}

}

//편의를 위한 함수
void astar::setNodeColor(node* node, COLORREF color)
{
	HBRUSH brush = CreateSolidBrush(color);
	FillRect(getMemDC(), &node->rc, brush);
	DeleteObject(brush);
	FrameRect(getMemDC(), node->rc, RGB(0, 0, 0));
}

void astar::pathFinding()
{
	_openList.clear();		//길 찾기 전에 _openList 초기화
	_closeList.clear();		//길 찾기 전에 _closeList 초기화
	_finalList.clear();		//길 찾기 전에 _finalList 초기화
	//종료노드가 없는 경우 길찾기 못함
	if (!_endNode) return;

	//길찾기를 해보자
	//검색을 하려면 무조건 오픈리스트에 담는다
	//F와 H값 가장 작은 놈을 찾아서 그놈을 현재노드로 변경한다
	//오픈리스트에서 현재노드는 삭제하고
	//현재노드는 클로즈리스트에 담아둔다
	//길을 다 찾았다면 클로즈리스트 리버스값을 파이널 리스트로 옮긴다

	//1. 시작노드가 있어야 출발이 가능하니
	//시작노드를 오픈리스트에 추가를 해줘야 한다
	_openList.push_back(_startNode);

	//2. 오픈리스트안에 담겨 있는 벡터를 검사해서
	//종료노드에 도착할때까지 무한 루프
	while (_openList.size() > 0)
	{
		_curNode = _openList[0];

		setNodeColor(_curNode, RGB(0, 255, 0));
		//오픈리스트중 F가 가장 작거나 F가 같다면
		//H가 작은 걸 현재노드로 하고
		//현재노드를 오픈리스트에서 클로즈 리스트로 옮기기
		//비교를 하려고 하면 최소 시작노드에서 주변을 탐색한 이후
		//길찾기가 시작된다

		for (int i = 1; i < _openList.size(); i++)
		{
			if (_openList[i]->F <= _curNode->F && _openList[i]->H < _curNode->H)
			{
				_curNode = _openList[i];
			}
		}

		//클로즈 리스트에 넣어둔다
		for (int i = 0; i < _openList.size(); i++)
		{
			if (_curNode == _openList[i])
			{
				this->delOpenList(i);
				_closeList.push_back(_curNode);
			}
		}

		//현재노드가 마지막 노드와 같냐? (길찾았다)
		if (_curNode == _endNode)
		{
			node* endNode = _endNode;
			vector<node*> tempNode;
			//마지막 노드로부터 시작노드까지 부모노드를 벡터에 담는다
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
			//종료하고 빠져 나온다
			return;
		}

		//상하좌우 (순서는 상관없음 - 어짜피 주변 4개의 노드를 모두 오픈리스트에 담아서 검사할 예정임)
		addOpenList(_curNode->idx, _curNode->idy - 1);	//상
		addOpenList(_curNode->idx, _curNode->idy + 1);	//하
		addOpenList(_curNode->idx - 1, _curNode->idy);	//좌
		addOpenList(_curNode->idx + 1, _curNode->idy);	//우
		addOpenList(_curNode->idx - 1, _curNode->idy - 1);	//상
		addOpenList(_curNode->idx - 1, _curNode->idy + 1);	//하
		addOpenList(_curNode->idx + 1, _curNode->idy - 1);	//좌
		addOpenList(_curNode->idx + 1, _curNode->idy + 1);	//우

		//추후에 대각 4방향도 추가하면 대각선 이동 처리도 가능함
		//우상, 좌상, 우하, 좌하
		//예외처리만 잘해주면 된다
		//벽사이로 막가 안되도록 처리한다
	}

}

void astar::addOpenList(int idx, int idy)
{
	//예외처리 인덱스 범위안에서 추가할 수 있어야 한다
	if (idx < 0 || idx >= MAX_X || idy < 0 || idy >= MAX_Y) { return; }

	//벽은 오픈리스트에 담을 수 없다
	if (_totalNode[idy * MAX_X + idx]->nodeState == NODE_WALL) { return; }

	//클로즈리스트(지나온길)에 있다면 오픈리스트에 담으면 안된다
	for (int i = 0; i < _closeList.size(); i++)
	{
		if (_totalNode[idy * MAX_X + idx] == _closeList[i]) { return; }
	}

	//여기까지 왔으면 문제가 없으니 이제 오픈리스트에 추가를 하자
	//현재노드의 4방향 노드를 이웃노드라고 하고 직선10, 대각은 14비용을 추가한다
	node* neighborNode = _totalNode[idy* MAX_X + idx];
	int moveCost = _curNode->G + ((_curNode->idx - idx == 0 || _curNode->idy - idy == 0) ? 10 : 14);

	//오픈리스트안에 이웃노드가 있으면 안된다
	for (int i = 0; i < _openList.size(); i++)
	{
		if (_openList[i] == neighborNode) return;
	}

	//마지막으로 오픈리스트에도 없는경우
	//G, H, ParentNode 설정후 오픈리스트에 추가
	//F = G + H
	//G = 시작에서 현재
	//H = 현재에서 종료
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
