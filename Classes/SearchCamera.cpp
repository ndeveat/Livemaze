#include "SearchCamera.h"

void SearchCamera::init(Layer *pTargetLayer, Node* target,Size mapSize, Size screenSize)
{
	m_pTargetLayer = pTargetLayer;

	m_mapSize = mapSize;

	m_screenSize = screenSize;

	m_target = target;

	this->schedule(schedule_selector(SearchCamera::update));
}

void SearchCamera::update(float dt)
{
	if (!m_target)
		return;

	Vec2 objPos = m_target->getPosition();
	cocos2d::Vec2 mapPos;

	if (objPos.x < m_screenSize.width / 2)
		mapPos.x = 0.f;
	else if (objPos.x > m_mapSize.width - m_screenSize.width / 2)
		mapPos.x = m_mapSize.width - m_screenSize.width;
	else
		mapPos.x = objPos.x - m_screenSize.width / 2;

	if (objPos.y < m_screenSize.height / 2)
		mapPos.y = 0.f;
	else if (objPos.y > m_mapSize.height - m_screenSize.height / 2)
		mapPos.y = m_mapSize.height - m_screenSize.height;
	else
		mapPos.y = objPos.y - m_screenSize.height / 2;


	m_pTargetLayer->setPosition(-mapPos);
}