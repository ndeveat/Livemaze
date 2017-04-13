#pragma once

#include "pch.h"
#include "NodeInfo.h"
#include "Utils.h"

class PathFinding
{
private:
	PathFinding(const PathFinding&);
	PathFinding& operator= (const PathFinding&);
protected:
public:
	priority_queue<NodeInfo> q[2];
	int qIndex;
	int insertCount;
	string sPath;

	int blockMap[ROW_SIZE][COL_SIZE];
	int openList[ROW_SIZE][COL_SIZE];
	int closeList[ROW_SIZE][COL_SIZE];
	int search[ROW_SIZE][COL_SIZE];
	int directionMap[ROW_SIZE][COL_SIZE];

public:
	PathFinding() : qIndex(0), insertCount(0)
	{

	};
	~PathFinding(){}

	//---------------------------------------

	void loadMap(int map[][COL_SIZE])
	{
		for (int row = 0; row <= ROW_SIZE; row++)
		{
			for (int col = 0; col <= COL_SIZE; col++)
			{
				blockMap[row][col] = map[row][col];
			}
		}
	}

	//--------------------------------------

	string pathFinding(const Vec2& start, const Vec2& end)
	{
		// �θ� ���
		NodeInfo* pParent;
		// �ڽ� ���
		NodeInfo* pChild;

		// y,x,����y,����x
		int row, col, nextRow, nextCol;

		// ���� 0���� �ʱ�ȭ
		// 0 = �������
		// 1 = �������
		for (row = 0; row < ROW_SIZE; row++)
		{
			for (col = 0; col < COL_SIZE; col++)
			{
				closeList[row][col] = 0;
				openList[row][col] = 0;
				directionMap[row][col] = N;
				search[row][col] = 0;
			}
		}

		// �θ��带 �����ؼ�
		pParent = new NodeInfo(start, 0, 0);
		pParent->calculateCostF(end);
		// �켱���� ť�� ����
		q[qIndex].push(*pParent);

		while (q[qIndex].empty() == false)
		{
			pParent = new NodeInfo(
				q[qIndex].top().getPosition(),
				q[qIndex].top().getCostG(),
				q[qIndex].top().getCostF());

			row = pParent->getPosition().x;
			col = pParent->getPosition().y;

			// ���� �켱������ �ִ³��� ����
			q[qIndex].pop();
			// ���¸���Ʈ 0���� 
			openList[row][col] = 0;

			// �ٽ� �˻��� �ʿ���� ��ǥ�� 1��
			closeList[row][col] = 1;

			// ���� ã����!
			if (row == end.x && col == end.y)
			{
				string path = "";
				while (row != start.x || col != start.y)
				{
					// char[]���� ����
					// ��� ��� : 21465355411111
					int direction = directionMap[row][col];
					char c = '0' + (direction + DIRECTION / 2) % DIRECTION;
					path = c + path;

					row += RowDirection[direction];
					col += ColDirection[direction];
				}

				//---------------------------------------------------------

				while (q[qIndex].empty() == false)
				{
					q[qIndex].pop();
				}

				CC_SAFE_DELETE(pParent);
				// log("%s", path.c_str());
				// string path�� ����
				return path;
			}

			// ���� �������� ��� ���� �˻�
			for (int i = 0; i < DIRECTION; i++)
			{
			nextNodeSearch:

				// �������� �� ������ ���� (x+1 y-1 ��)
				nextRow = row + RowDirection[i];
				nextCol = col + ColDirection[i];

				int direction = (i + DIRECTION / 2) % DIRECTION;

				// ���� ��尡 1,3,5,7�̸�
				// ���� ��带 �߽����� �ѹ��� �ֺ��� �˻���
				// �Ѱ��� ���� �ִٸ�
				if (i % 2 != 0)
				{
					for (int j = 0; j < DIRECTION; j++)
					{

						int exeptRow = nextRow + RowDirection[j];
						int exeptCol = nextCol + ColDirection[j];

						if (j % 2 == 0)
						{
							if (blockMap[exeptRow][exeptCol] == 2)
							{
								i++;
								goto nextNodeSearch;
								break;

							}
						}
					}
				}
				// ����ó��
				if ((nextRow < 0 ||
					nextCol < 0 ||
					nextRow > ROW_SIZE - 1 ||
					nextCol > COL_SIZE - 1 ||
					blockMap[nextRow][nextCol] == 5 ||
					closeList[nextRow][nextCol] == 1) == false)
				{
					// ���� �ڽĳ�带 ����
					pChild = new NodeInfo(Vec2(nextRow, nextCol), pParent->getCostG(), pParent->getCostF());
					pChild->updateCostG(i);
					pChild->calculateCostF(end);

					// ���¸���Ʈ�� ������, �߰�
					if (openList[nextRow][nextCol] == 0)
					{
						pChild->setInsertIndex(insertCount++);
						openList[nextRow][nextCol] = pChild->getCostF();
						directionMap[nextRow][nextCol] = direction;
						q[qIndex].push(*pChild);
					}
					// �ƴϸ�, ���� ����Ʈ�� �ڽ� ����� F���̶� ��, ũ�ٸ�?
					else if (openList[nextRow][nextCol] > pChild->getCostF())
					{
						// ���� ������ �ٲ�
						pChild->setInsertIndex(insertCount);
						openList[nextRow][nextCol] = pChild->getCostF();
						directionMap[nextRow][nextCol] = direction;

						while ((q[qIndex].top().getPosition().x == nextRow &&
							q[qIndex].top().getPosition().y == nextCol) == false)
						{
							q[1 - qIndex].push(q[qIndex].top());
							q[qIndex].pop();
						}

						q[qIndex].pop();

						if (q[qIndex].size() > q[1 - qIndex].size())
						{
							qIndex = 1 - qIndex;
						}

						while (q[qIndex].empty() == false)
						{
							q[1 - qIndex].push(q[qIndex].top());
							q[qIndex].pop();
						}

						qIndex = 1 - qIndex;

						q[qIndex].push(*pChild);

					}
				}
				else
				{
					//CC_SAFE_DELETE(pChild);
				}
			}

			//CC_SAFE_DELETE(pParent);
		}
		return "";
	}
	std::vector<Vec2> vec2_pathFinding(const Vec2&start, const Vec2& end)
	{
		string path = this->pathFinding(start, end);
		std::vector<Vec2> pathList = pathListPaser(path, start);

		return pathList;
	}
};