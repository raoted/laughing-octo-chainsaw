#include "stdafx.h"
#include "astarTestScene.h"

HRESULT astarTestScene::init()
{
	IMAGEMANAGER->addImage("bg", "Images/background.bmp", 864, 624);
	IMAGEMANAGER->addImage("player", "Images/player.bmp", 48, 96);

	//전체노드 초기화
	for (int y = 0; y < MAX_Y; y++)
	{
		for (int x = 0; x < MAX_X; x++)
		{
			//새로운 노드와 렉트위치 설정
			_totalNode[x][y] = new node(x, y);
			_totalNode[x][y]->rc = RectMake(x * 48, y * 48, 48, 48);
		}
	}
	_block = -1;

	//첫클릭이 짝수가 되게 하기 위해서 -1로 초기화
	_count = -1;
	//길 찾았냐?
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
	//시작, 종료노드 세팅하기
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
			//시작, 종료노드가 이미 세팅되어 있다면 그냥 나가자
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

		//벽(장애물) 노드 세팅하기 (시작, 종료노드 설정전에 벽세우지 못하게 막기)
		if (INPUT->GetKeyDown(VK_RBUTTON) && _startNode && _endNode)
		{
			for (int y = 0; y <= MAX_Y; y++)
			{
				for (int x = 0; x <= MAX_X; x++)
				{
					if (PtInRect(&_totalNode[x][y]->rc, _ptMouse))
					{
						//시작노드, 종료노드는 선택하지 못하게 막기
						if (_totalNode[x][y]->nodeState == NODE_START) continue;
						if (_totalNode[x][y]->nodeState == NODE_END) continue;
						_totalNode[x][y]->nodeState = NODE_WALL;
					}
				}
			}
		}

		//스페이스 누르면 길찾아라
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
			//전체노드 렉트 보여주기
			HBRUSH myBrush = (HBRUSH)CreateSolidBrush(NULL_BRUSH);
			HBRUSH oldBrush = (HBRUSH)SelectObject(getMemDC(), GetStockObject(NULL_BRUSH));
			Rectangle(getMemDC(), _totalNode[x][y]->rc);
			SelectObject(getMemDC(), oldBrush);
			DeleteObject(myBrush);

			//시작노드 보여주기
			if (_totalNode[x][y]->nodeState == NODE_START)
			{
				setNodeColor(_startNode, RGB(255, 0, 0));
				TextOut(getMemDC(), _startNode->rc.left, _startNode->rc.top + 10, "[Start]", strlen("[Start]"));
			}
			//종료노드 보여주기
			else if (_totalNode[x][y]->nodeState == NODE_END)
			{
				setNodeColor(_endNode, RGB(0, 0, 255));
				TextOut(getMemDC(), _endNode->rc.left, _endNode->rc.top + 10, "[END]", strlen("[END]"));
			}
			//벽노드 보여주기
			else if (_totalNode[x][y]->nodeState == NODE_WALL)
			{
				TextOut(getMemDC(), _totalNode[x][y]->rc.left, _totalNode[x][y]->rc.top + 10, "[WALL]", strlen("[WALL]"));
			}
			else
			{
				//전체노드의 인덱스 보여주기(맨마지막에 출력)
				sprintf_s(str, "[%d, %d]", _totalNode[x][y]->idx, _totalNode[x][y]->idy);
				TextOut(getMemDC(), _totalNode[x][y]->rc.left, _totalNode[x][y]->rc.top + 10, str, strlen(str));
			}
		}
	}

	//길찾았을때 보여주기
	if (_isFind)
	{
		for (int i = 0; i < _finalList.size(); i++)
		{
			setNodeColor(_finalList[i], RGB(255, 255, 0));
			sprintf_s(str, "[%d, %d]      %d번 노드", _finalList[i]->idx, _finalList[i]->idy, i + 1);
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

//편의를 위한 함수
void astarTestScene::setNodeColor(node * node, COLORREF color)
{
	HBRUSH brush = CreateSolidBrush(color);
	FillRect(getMemDC(), &node->rc, brush);
	DeleteObject(brush);
	FrameRect(getMemDC(), node->rc, RGB(0, 0, 0));
}

void astarTestScene::pathFinding()
{
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

		//오픈리스트중 F가 가장 작거나 F가 같다면
		//H가 작은 걸 현재노드로 하고
		//현재노드를 오픈리스트에서 클로즈 리스트로 옮기기
		//비교를 하려고 하면 최소 시작노드에서 주변을 탐색한 이후
		//길찾기가 시작된다

		for (int i = 1; i < _openList.size()-1; i++)
		{
			//if 조건이 터짐
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
		addOpenList(_curNode->idx, _curNode->idy - 1);		//상
		addOpenList(_curNode->idx, _curNode->idy + 1);		//하
		addOpenList(_curNode->idx - 1, _curNode->idy);		//좌
		addOpenList(_curNode->idx + 1, _curNode->idy);		//우
		//addOpenList(_curNode->idx - 1, _curNode->idy - 1);	//좌상
		//addOpenList(_curNode->idx + 1, _curNode->idy - 1);	//우상
		//addOpenList(_curNode->idx - 1, _curNode->idy + 1);	//좌하
		//addOpenList(_curNode->idx + 1, _curNode->idy + 1);	//우하

		//추후에 대각 4방향도 추가하면 대각선 이동 처리도 가능함
		//우상, 좌상, 우하, 좌하
		//예외처리만 잘해주면 된다
		//벽사이로 막가 안되도록 처리한다
	}

}

void astarTestScene::addOpenList(int idx, int idy)
{
	//예외처리 인덱스 범위안에서 추가할 수 있어야 한다
	if (idx < 0 || idx > MAX_X || idy < 0 || idy > MAX_Y) return;

	//벽은 오픈리스트에 담을 수 없다
	if (_totalNode[idx][idy]->nodeState == NODE_WALL) return;
	/*if (idx > 1 && idx < MAX_X && idy > 1 && idy < MAX_Y)
	{
		if (_totalNode[idx - 1][idy]->nodeState == NODE_WALL && _totalNode[idx][idy - 1]->nodeState == NODE_WALL) return;
		if (_totalNode[idx - 1][idy]->nodeState == NODE_WALL && _totalNode[idx][idy + 1]->nodeState == NODE_WALL) return;
		if (_totalNode[idx + 1][idy]->nodeState == NODE_WALL && _totalNode[idx][idy - 1]->nodeState == NODE_WALL) return;
		if (_totalNode[idx + 1][idy]->nodeState == NODE_WALL && _totalNode[idx][idy + 1]->nodeState == NODE_WALL) return;
	}*/
	//클로즈리스트(지나온길)에 있다면 오픈리스트에 담으면 안된다
	for (int i = 0; i < _closeList.size(); i++)
	{
		if (_totalNode[idx][idy] == _closeList[i]) return;
	}

	//여기까지 왔으면 문제가 없으니 이제 오픈리스트에 추가를 하자
	//현재노드의 4방향 노드를 이웃노드라고 하고 직선10, 대각은 14비용을 추가한다
	node* neighborNode = _totalNode[idx][idy];
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
