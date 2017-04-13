#pragma once
#include "AnimationInterface.h"

class CatAnimation : public AnimationInterface
{
public:
	CatAnimation(Node* target, SpriteFrameCache* cache) :
		AnimationInterface(target, cache){}
	void Init()
	{
		AnimationInterface::init();

		Animation* left = Animation::create();
		left->setDelayPerUnit(0.13f);
		left->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("cat_1.png")));
		left->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("cat_2.png")));
		left->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("cat_1.png")));

		this->_animation_container.insert("LEFT", left);
	}
};