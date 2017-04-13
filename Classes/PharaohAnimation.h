#pragma once
#include "AnimationInterface.h"

class PharaohAnimation : public AnimationInterface
{
private:
public:
	PharaohAnimation(Node* target, SpriteFrameCache* cache) :
		AnimationInterface(target, cache){}
	void Init()
	{
		AnimationInterface::init();

		// UP
		Animation* pharaoh_up = Animation::create();
		pharaoh_up->setDelayPerUnit(0.14f);
		pharaoh_up->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("parao_back_%d.png", 1)));
		pharaoh_up->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("parao_back_%d.png", 2)));
		pharaoh_up->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("parao_back_%d.png", 3)));
		pharaoh_up->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("parao_back_%d.png", 2)));

		this->_animation_container.insert("UP", pharaoh_up);

		// DOWN
		Animation* pharaoh_down = Animation::create();
		pharaoh_down->setDelayPerUnit(0.14f);
		pharaoh_down->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("parao_front_%d.png", 1)));
		pharaoh_down->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("parao_front_%d.png", 2)));
		pharaoh_down->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("parao_front_%d.png", 3)));
		pharaoh_down->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("parao_front_%d.png", 2)));

		this->_animation_container.insert("DOWN", pharaoh_down);

		// LEFT
		Animation* pharaoh_left = Animation::create();
		pharaoh_left->setDelayPerUnit(0.14f);
		pharaoh_left->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("parao_left_%d.png", 1)));
		pharaoh_left->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("parao_left_%d.png", 2)));
		pharaoh_left->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("parao_left_%d.png", 3)));
		pharaoh_left->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("parao_left_%d.png", 2)));

		this->_animation_container.insert("LEFT", pharaoh_left);
	}
};