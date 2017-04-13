#pragma once

#include "AnimationInterface.h"

class PlayerAnimation : public AnimationInterface
{
private:
public:
	PlayerAnimation(Node* target, SpriteFrameCache* cache) : 
		AnimationInterface(target, cache){}

	~PlayerAnimation()
	{

	};

public:
	void Init() override
	{
		AnimationInterface::init();

		// UP
		Animation* player_up = Animation::create();
		player_up->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_back_%d.png", 1)));
		player_up->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_back_%d.png", 2)));
		player_up->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_back_%d.png", 3)));
		player_up->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_back_%d.png", 2)));
		player_up->setDelayPerUnit(0.08f);
		this->_animation_container.insert("UP", player_up);


		// DOWN
		Animation* player_down = Animation::create();
		player_down->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_front_%d.png", 1)));
		player_down->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_front_%d.png", 2)));
		player_down->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_front_%d.png", 3)));
		player_down->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_front_%d.png", 2)));
		player_down->setDelayPerUnit(0.08f);
		this->_animation_container.insert("DOWN", player_down);


		// LEFT_RIGHT
		Animation* player_left = Animation::create();
		player_left->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_left_%d.png", 1)));
		player_left->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_left_%d.png", 2)));
		player_left->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_left_%d.png", 3)));
		player_left->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_left_%d.png", 2)));
		player_left->setDelayPerUnit(0.08f);
		this->_animation_container.insert("LEFT", player_left);

		// ---------------------------------------------------- [ PLAYER CHOP ]

		// CHOP_FRONT
		Animation* player_attack_front = Animation::create();
		player_attack_front->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_attack_front_%d.png", 1)));
		player_attack_front->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_attack_front_%d.png", 2)));
		player_attack_front->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_front_%d.png", 2)));
		player_attack_front->setDelayPerUnit(0.12f);
		this->_animation_container.insert("DOWN_ATTACK", player_attack_front);

		// CHOP_BACK
		Animation* player_attack_back = Animation::create();
		player_attack_back->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_attack_back_%d.png", 1)));
		player_attack_back->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_attack_back_%d.png", 2)));
		player_attack_back->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_back_%d.png", 2)));
		player_attack_back->setDelayPerUnit(0.12f);
		this->_animation_container.insert("UP_ATTACK", player_attack_back);

		// CHOP_LEFT
		Animation* player_attack_right = Animation::create();
		player_attack_right->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_attack_right_%d.png", 1)));
		player_attack_right->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_attack_right_%d.png", 2)));
		player_attack_right->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_left_%d.png", 2)));
		player_attack_right->setDelayPerUnit(0.12f);
		this->_animation_container.insert("RIGHT_ATTACK", player_attack_right);

		// ---------------------------------------------------- [ ITEM USE ]

		// Use Item
		Animation* player_use_item = Animation::create();
		player_use_item->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_use_%d.png", 1)));
		player_use_item->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_use_%d.png", 2)));
		player_use_item->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_use_%d.png", 3)));
		player_use_item->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_use_%d.png", 2)));
		player_use_item->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_use_%d.png", 1)));
		player_use_item->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_front_%d.png", 2)));
		player_use_item->setDelayPerUnit(0.12f);
		this->_animation_container.insert("USE_ITEM", player_use_item);

		// ---------------------------------------------------- [ ARROW ]

		// ARROW UP
		Animation* player_arrow_up = Animation::create();
		player_arrow_up->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_arrow_back_%d.png", 1)));
		player_arrow_up->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_arrow_back_%d.png", 2)));
		player_arrow_up->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_back_%d.png", 2)));
		player_arrow_up->setDelayPerUnit(0.12f);
		this->_animation_container.insert("UP_ARROW", player_arrow_up);

		// ARROW DOWN
		Animation* player_arrow_down = Animation::create();
		player_arrow_down->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_arrow_front_%d.png", 1)));
		player_arrow_down->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_arrow_front_%d.png", 2)));
		player_arrow_down->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_front_%d.png", 2)));
		player_arrow_down->setDelayPerUnit(0.12f);
		this->_animation_container.insert("DOWN_ARROW", player_arrow_down);
		
		// CHOP_LEFT
		Animation* player_arrow_left = Animation::create();
		player_arrow_left->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_arrow_left_%d.png", 1)));
		player_arrow_left->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_arrow_left_%d.png", 2)));
		player_arrow_left->addSpriteFrame(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_left_%d.png", 2)));
		player_arrow_left->setDelayPerUnit(0.12f);
		this->_animation_container.insert("LEFT_ARROW", player_arrow_left);
	}
};