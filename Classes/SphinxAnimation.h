#pragma once
#include "AnimationInterface.h"

class SphinxAnimation : public AnimationInterface
{
private:
public:
	SphinxAnimation(Node* target, SpriteFrameCache* cache) :
		AnimationInterface(target, cache){}

	void Init()
	{
		AnimationInterface::init();

		Animation* fire = Animation::create();
		fire->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("sphinx_1.png")));
		fire->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("sphinx_2.png")));
		fire->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("sphinx_3.png")));
		fire->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("sphinx_2.png")));
		fire->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("sphinx_3.png")));
		fire->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("sphinx_2.png")));
		fire->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("sphinx_1.png")));

		fire->setDelayPerUnit(0.14f);
		this->_animation_container.insert("FIRE", fire);
	}
};