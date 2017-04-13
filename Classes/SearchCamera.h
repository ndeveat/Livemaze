#pragma once
#include "pch.h"

class SearchCamera : public Node
{
private:
	Layer  *m_pTargetLayer;
	Size	m_mapSize;
	Size	m_screenSize;
	Node	*m_target;
public:
	void init(Layer *,Node*, Size, Size);
	void update(float);
};