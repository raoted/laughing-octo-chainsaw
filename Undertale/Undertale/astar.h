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

//��� Ŭ����
class node
{
	//���̽�Ÿ A* (��ã�� �˰���)
	//1. ��� (F = G + H)
	//2. �θ��� ������ (���� ��ġ���� ���� ��ġ�� ��� ������)
public:
	RECT rc;				//�簢��
	int idx, idy;			//�ε���
	int F, G, H;			//F = G + H, G = ���� to ����, H = ���� to ����
	node* parentNode;		//�θ� ����ų ���(�������)
	NODESTATE nodeState;

	//��� ������
	node() {}
	//��� ������ (������ �����ε�)
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
	int MAX_X, MAX_Y;				//x��� y���� �ִ밪
	vector<node*> _totalNode;
	node* _startNode;				//���۳��
	node* _endNode;					//������
	node* _curNode;					//������

	vector<node*> _openList;		//���¸���Ʈ(Ž���� ������ ��Ƶ� ����)
	vector<node*> _closeList;		//Ŭ�����Ʈ
	vector<node*> _finalList;		//���̳θ���Ʈ (��ã�� ���� Ŭ�����Ʈ�� ����ִ� ������ ��������ų�뵵)

	int _count;						//���۳��, ������ �ѹ����� �����ϱ� ���� ����
	bool _isFind;					//�� ã�ҳ�?

public:
	HRESULT init();
	void release();
	void update();
	void render();

	//���Ǹ� ���� �Լ�
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

	/*�߿��Լ�*/
	//��ã�� �Լ�
	void pathFinding();
	//���¸���Ʈ �߰�
	void addOpenList(int idx, int idy);
	//���¸���Ʈ ����
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

