#pragma once

//����ȭ�� Ÿ��
#define TILESIZE 40		//Ÿ�� �ȼ�
#define TILEX 20		//�� Ÿ���� ���ΰ� �� ĭ����
#define TILEY 15		//�� Ÿ���� ���ΰ� �� ĭ����
#define TILESIZEX (TILEX * TILESIZE)		//�� Ÿ���� ������ �ȼ� ��
#define TILESIZEY (TILEY * TILESIZE)		//�� Ÿ���� ������ �ȼ� ��

//�̹��� Ÿ��(ȭ�� ������ܿ� �ٿ����� ����Ÿ��)
#define SAMPLETILEX 12
#define SAMPLETILEY 6

//����Ÿ�� ���� ����
//���� Ÿ���� �� 69������ �ֽ��ϴ�


//���̾� ���� ����
#define LAYERCOUNT 3
//����
struct TERRAIN
{
	int x = 0;
	int y = 0;		//Ÿ��(x, y)���� ����.
};

//Ÿ�ϱ���ü
//�� Ÿ�ϴ� ���̾� ���� 3������
struct TILE
{
	TERRAIN terrain;
	RECT rc;
	bool canMove[LAYERCOUNT] = { false, };			//�� Ÿ�� ���� �̵��� �� �ִ��� Ȯ�����ִ� bool ����
													//�⺻�� false
													//�ϳ��� false�� �� Ÿ�� ���δ� �̵��� �� ����
	bool direct[4] = { false, };					//�̵��� �����ϴٸ� � �������� �̵��� �� �ִ��� ����
													//true�� ��쿡�� �̵� ����.
													//�⺻�� == false
													//0: ��, 1: �Ʒ�, 2: ��, 3: ��
	int imagePage[LAYERCOUNT] = { -1, -1, -1};		//� ���ø��� �׷ȴ��� �����ϴ� ����
	int tileFrameX[LAYERCOUNT];						//imagePage�� ���η� �� ��° Ÿ������ �����ϴ� ����
	int tileFrameY[LAYERCOUNT];						//imagePage�� ���η� �� ��° Ÿ������ �����ϴ� ����
};
struct tagTile
{
	vector<TILE> tile;
	const short xSizeMax = 300;
	const short ySizeMax = 300;

	unsigned int xSize = 20;
	unsigned int ySize = 20;
};

//�̹��� Ÿ�� ����ü
struct tagSampleTile
{
	RECT rc;

	int imagePage;							//����Ÿ�� ��ȣ
	int tileFrameX = 0;							//���� �� ��°�� �׷��������� ������ ����
	int tileFrameY = 0;							//���� �� ��°�� �׷��������� ������ ����
	
	bool canMove = false;					//�� Ÿ�Ϸ� �̵��� �����Ѱ�?
											//�⺻�� : �̵��Ұ�
	bool direct[4] = { false, };			//�� Ÿ�Ͽ��� � �������� �̵��� �����Ѱ�?
											//�⺻�� : �̵��Ұ�
											//0:��  1:�Ʒ�  2:��  3:��
};

//����Ÿ�� ����ü
struct tagCurrentTile
{
	int pageNumber;			//����Ÿ�� ��ȣ
	int x, y;				//Ÿ�� ��ǥ�� ����� (x, y�迭)
};


