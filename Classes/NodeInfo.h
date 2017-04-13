#pragma once

#include "pch.h"
#include "Matrix.h"

class NodeInfo
{
private:
	int _costF;		// ?????
	int _costG;		//???????????? ????
	int _costH;		// ???????????? ????
	Vec2 _rect;	// ???


public:
	int _insertIndex;	// ???????


protected:
public:
	NodeInfo() : _costF(0), _costG(0), _rect(Vec2(0, 0)), _insertIndex(0)
	{

	}

	NodeInfo(const Vec2& pos, int g, int f) :_costF(f), _costG(g)
	{
		_rect.x = pos.x;
		_rect.y = pos.y;
		_insertIndex = 0;
	}
	int getCostG() const
	{
		return _costG;
	}
	int getCostF() const
	{
		return _costF;
	}

	void calculateCostF(const Vec2& pos)
	{
		_costH = getCostH(pos);
		_costF = _costG + _costH;
	}

	void updateCostG(const int& i)
	{
		_costG += (DIRECTION == 8 ? (i % 2 == 0 ? 10 : 14) : 10);
	}

	const int& getCostH(const Vec2& pos) const
	{
		int row = pos.x - _rect.x;
		int col = pos.y - _rect.y;
		return 10 * (abs(row) + abs(col));
	}

	void setInsertIndex(int index)
	{
		_insertIndex = index;
	}

	int getInsertIndex() const
	{
		return _insertIndex;
	}

	Vec2 getPosition() const
	{
		return _rect;
	}

	// priority_queue ???? ?дь???? ???? ??????
	// ??????? ???? ???? ?дь?????? ?????(?????? ?????)
	friend bool operator<(const NodeInfo& a, const NodeInfo& b)
	{
		if (a.getCostF() == b.getCostF())
		{
			return a.getInsertIndex() < b.getInsertIndex();
		}
		else
		{
			return a.getCostF() > b.getCostF();
		}
	}
};