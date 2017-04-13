#pragma once

#include "AnimationInterface.h"

class MummyAnimation : public AnimationInterface
{
private:
public:
	MummyAnimation(Node* target, SpriteFrameCache* cache) :
		AnimationInterface(target, cache){}

	~MummyAnimation()
	{

	};

public:
	void Init() override
	{
		AnimationInterface::init();

		Animation* up = Animation::create();
		up->setDelayPerUnit(0.27f);
		up->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("mummy_up_%d.png", 1)));
		up->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("mummy_up_%d.png", 2)));
		up->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("mummy_up_%d.png", 3)));
		up->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("mummy_up_%d.png", 2)));

		this->_animation_container.insert("UP", up);

		Animation* down = Animation::create();
		down->setDelayPerUnit(0.27f);
		down->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("mummy_down_%d.png", 1)));
		down->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("mummy_down_%d.png", 2)));
		down->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("mummy_down_%d.png", 3)));
		down->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("mummy_down_%d.png", 2)));

		this->_animation_container.insert("DOWN", down);

		Animation* left = Animation::create();
		left->setDelayPerUnit(0.27f);
		left->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("mummy_left_%d.png", 1)));
		left->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("mummy_left_%d.png", 2)));
		left->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("mummy_left_%d.png", 3)));
		left->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("mummy_left_%d.png", 2)));

		this->_animation_container.insert("LEFT", left);
	}
};