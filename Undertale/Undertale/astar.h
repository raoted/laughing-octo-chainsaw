#pragma once
#include "gameNode.h"
#include "tileNode.h"

enum NODESTATE
{
	NODE_EMPTY = 0,
	NODE_START,
	NODE_END,
	NODE_WALL = 3
};

//노드 클래스
class node
{
	//에이스타 A* (길찾기 알고리즘)
	//1. 비용 (F = G + H)
	//2. 부모노드 포인터 (현재 위치에서 이전 위치의 노드 포인터)
public:
	RECT rc;				//사각형
	int idx, idy;			//인덱스
	int F, G, H;			//F = G + H, G = 시작 to 현재, H = 현재 to 종료
	node* parentNode;		//부모를 가리킬 노드(이전노드)
	NODESTATE nodeState;

	//노드 생성자
	node() {}
	//노드 생성자 (생성자 오버로딩)
	node(int _idx, int _idy)
	{
		idx = _idx;
		idy = _idy;
		F = G = H = 0;
		nodeState = NODE_EMPTY;
		parentNode = NULL;
	}
};

class astar : public gameNode
{
private:
	int MAX_X, MAX_Y;				//x축과 y축의 최대값
	vector<node*> _totalNode;
	node* _startNode;				//시작노드
	node* _endNode;					//종료노드
	node* _curNode;					//현재노드

	vector<node*> _openList;		//오픈리스트(탐색할 노드들을 담아둘 벡터)
	vector<node*> _closeList;		//클로즈리스트
	vector<node*> _finalList;		//파이널리스트 (길찾은 이후 클로즈리스트의 담겨있는 노드들을 리버스시킬용도)

	int _count;						//시작노드, 종료노드 한번씩만 선택하기 위한 변수
	bool _isFind;					//길 찾았냐?

public:
	HRESULT init();
	void release();
	void update();
	void render();

	//편의를 위한 함수
	void setNodeColor(node* node, COLORREF color);

	void setMapData(int x, int y) 
	{
		MAX_X = x;
		MAX_Y = y;
		
		_totalNode.resize(MAX_X * MAX_Y);
		for (int y = 0; y < MAX_Y; y++)
		{
			for (int x = 0; x < MAX_X; x++)
			{
				_totalNode[y * MAX_X + x] = new node(x, y);
			}
		}
	}

	/*중요함수*/
	//길찾기 함수
	void pathFinding();
	//오픈리스트 추가
	void addOpenList(int idx, int idy);
	//오픈리스트 삭제
	void delOpenList(int index);

	bool getBool() { return _isFind; }
	bool setBool(bool _b) { _isFind = _b; }

	vector<node*> getTotalNode() { return _totalNode; }
	void setTotalNodeRc(int i, RECT rc) { _totalNode[i]->rc = rc; }
	void setWall(int i) { _totalNode[i]->nodeState = NODE_WALL; }
	void setStartNode(int x, int y);
	node* getStartNode() { return _startNode; }

	vector<node*> getFinalList() { return _finalList; }
	void eraseFinalList() { _finalList.erase(_finalList.begin()); }

	node* getEndNode() { return _endNode; }
	void setEndNode(int index);

	astar() : _startNode(NULL), _endNode(NULL) {}
	~astar() {}
};

