#pragma once
#include "AnimationInterface.h"

class BugAnimation : public AnimationInterface
{
public:
	BugAnimation(Node* target, SpriteFrameCache* cache) :
		AnimationInterface(target, cache){}
	void Init()
	{
		AnimationInterface::init();

		Animation* up = Animation::create();
		up->setDelayPerUnit(0.14f);
		up->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("up_%d.png", 1)));
		up->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("up_%d.png", 2)));
		up->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("up_%d.png", 2)));
		up->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("up_%d.png", 2)));

		this->_animation_container.insert("UP", up);

		Animation* down = Animation::create();
		down->setDelayPerUnit(0.14f);
		down->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("down_%d.png", 1)));
		down->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("down_%d.png", 2)));
		down->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("down_%d.png", 2)));
		down->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("down_%d.png", 2)));

		this->_animation_container.insert("DOWN", down);

		Animation* left = Animation::create();
		left->setDelayPerUnit(0.14f);
		left->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("left_%d.png", 1)));
		left->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("left_%d.png", 2)));
		left->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("left_%d.png", 2)));
		left->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("left_%d.png", 2)));

		this->_animation_container.insert("LEFT", left);
	}
};