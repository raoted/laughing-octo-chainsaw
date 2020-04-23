#pragma once
#include "gameNode.h"

#define MAX_X 18
#define MAX_Y 13

//������
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
	NODESTATE nodeState;	//������ (����, ����, ��, ����)

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


class astarTestScene : public gameNode
{
private:
	node* _totalNode[MAX_X][MAX_Y];	//��ü��� 25��(������ ����)
	node* _startNode;				//���۳��
	node* _endNode;					//������
	node* _curNode;					//������

	vector<node*> _openList;		//���¸���Ʈ(Ž���� ������ ��Ƶ� ����)
	vector<node*> _closeList;		//Ŭ�����Ʈ
	vector<node*> _finalList;		//���̳θ���Ʈ (��ã�� ���� Ŭ�����Ʈ�� ����ִ� ������ ��������ų�뵵)

	int _block;
	int _count;						//���۳��, ������ �ѹ����� �����ϱ� ���� ����
	bool _isFind;					//�� ã�ҳ�?

public:
	HRESULT init();
	void release();
	void update();
	void render();

	//���Ǹ� ���� �Լ�
	void setNodeColor(node* node, COLORREF color);

	/*�߿��Լ�*/
	//��ã�� �Լ�
	void pathFinding();
	//���¸���Ʈ �߰�
	void addOpenList(int idx, int idy);
	//���¸���Ʈ ����
	void delOpenList(int index);
	void setWellNodeH();
	void setWellNodeV();


	astarTestScene() : _startNode(NULL), _endNode(NULL) {}
	~astarTestScene() {}
};

