#pragma once
#include "AnimationInterface.h"

class SnakeAnimation : public AnimationInterface
{
private:
public:
	SnakeAnimation(Node* target, SpriteFrameCache* cache) :
		AnimationInterface(target, cache){}

	void Init()
	{
		AnimationInterface::init();

		Animation* left = Animation::create();
		left->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName("snake1.png"));
		left->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName("snake2.png"));
		left->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName("snake1.png"));
		left->setDelayPerUnit(0.13f);

		this->_animation_container.insert("LEFT", left);
	}
};