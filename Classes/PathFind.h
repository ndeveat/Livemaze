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
		// 부모 노드
		NodeInfo* pParent;
		// 자식 노드
		NodeInfo* pChild;

		// y,x,다음y,다음x
		int row, col, nextRow, nextCol;

		// 전부 0으로 초기화
		// 0 = 비어있음
		// 1 = 들어있음
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

		// 부모노드를 생성해서
		pParent = new NodeInfo(start, 0, 0);
		pParent->calculateCostF(end);
		// 우선순위 큐에 넣음
		q[qIndex].push(*pParent);

		while (q[qIndex].empty() == false)
		{
			pParent = new NodeInfo(
				q[qIndex].top().getPosition(),
				q[qIndex].top().getCostG(),
				q[qIndex].top().getCostF());

			row = pParent->getPosition().x;
			col = pParent->getPosition().y;

			// 가장 우선순위에 있는놈을 삭제
			q[qIndex].pop();
			// 오픈리스트 0으로 
			openList[row][col] = 0;

			// 다시 검사할 필요없는 좌표를 1로
			closeList[row][col] = 1;

			// 길을 찾으면!
			if (row == end.x && col == end.y)
			{
				string path = "";
				while (row != start.x || col != start.y)
				{
					// char[]으로 만듦
					// 출력 결과 : 21465355411111
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
				// string path를 리턴
				return path;
			}

			// 현재 시점에서 모든 방향 검색
			for (int i = 0; i < DIRECTION; i++)
			{
			nextNodeSearch:

				// 다음으로 갈 방향을 결정 (x+1 y-1 등)
				nextRow = row + RowDirection[i];
				nextCol = col + ColDirection[i];

				int direction = (i + DIRECTION / 2) % DIRECTION;

				// 다음 노드가 1,3,5,7이면
				// 다음 노드를 중심으로 한번더 주변을 검사함
				// 한개라도 블럭이 있다면
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
				// 예외처리
				if ((nextRow < 0 ||
					nextCol < 0 ||
					nextRow > ROW_SIZE - 1 ||
					nextCol > COL_SIZE - 1 ||
					blockMap[nextRow][nextCol] == 5 ||
					closeList[nextRow][nextCol] == 1) == false)
				{
					// 다음 자식노드를 생산
					pChild = new NodeInfo(Vec2(nextRow, nextCol), pParent->getCostG(), pParent->getCostF());
					pChild->updateCostG(i);
					pChild->calculateCostF(end);

					// 오픈리스트에 없으면, 추가
					if (openList[nextRow][nextCol] == 0)
					{
						pChild->setInsertIndex(insertCount++);
						openList[nextRow][nextCol] = pChild->getCostF();
						directionMap[nextRow][nextCol] = direction;
						q[qIndex].push(*pChild);
					}
					// 아니면, 현재 리스트를 자식 노드의 F값이랑 비교, 크다면?
					else if (openList[nextRow][nextCol] > pChild->getCostF())
					{
						// 작은 값으로 바꿈
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