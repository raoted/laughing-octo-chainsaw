#pragma once
#include "gameNode.h"

#define MAX_X 18
#define MAX_Y 13

//노드상태
enum NODESTATE
{
	NODE_START,
	NODE_END,
	NODE_WALL,
	NODE_EMPTY
};

//[0,0][1,0][2,0][3,0][4,0]
//[0,1][1,1][2,1][3,1][4,1]
//[0,2][1,2][2,2][3,2][4,2]
//[0,3][1,3][2,3][3,3][4,3]
//[0,4][1,4][2,4][3,4][4,4]

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
	NODESTATE nodeState;	//노드상태 (시작, 종료, 벽, 빈노드)

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


class astarTestScene : public gameNode
{
private:
	node* _totalNode[MAX_X][MAX_Y];	//전체노드 25개(보드판 역할)
	node* _startNode;				//시작노드
	node* _endNode;					//종료노드
	node* _curNode;					//현재노드

	vector<node*> _openList;		//오픈리스트(탐색할 노드들을 담아둘 벡터)
	vector<node*> _closeList;		//클로즈리스트
	vector<node*> _finalList;		//파이널리스트 (길찾은 이후 클로즈리스트의 담겨있는 노드들을 리버스시킬용도)

	int _block;
	int _count;						//시작노드, 종료노드 한번씩만 선택하기 위한 변수
	bool _isFind;					//길 찾았냐?

public:
	HRESULT init();
	void release();
	void update();
	void render();

	//편의를 위한 함수
	void setNodeColor(node* node, COLORREF color);

	/*중요함수*/
	//길찾기 함수
	void pathFinding();
	//오픈리스트 추가
	void addOpenList(int idx, int idy);
	//오픈리스트 삭제
	void delOpenList(int index);
	void setWellNodeH();
	void setWellNodeV();


	astarTestScene() : _startNode(NULL), _endNode(NULL) {}
	~astarTestScene() {}
};

