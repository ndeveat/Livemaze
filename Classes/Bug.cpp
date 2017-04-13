#include "Bug.h"

Bug::~Bug()
{

}

void Bug::InitWithEnemy()
{
	rand_to_animation(cocos2d::random(1,4));

	this->schedule(schedule_selector(Bug::Update));
}

void Bug::Update(float delta)
{
	if (execute){

		// ------------------------------------ [ ARROW ]
		if (arrow_vector.size())
		{
			for (auto it : arrow_vector)
			{
				if (it->getBoundingBox().intersectsRect(this->getBoundingBox()))
				{
					find_target = true;
					this->find_target = true;
					this->_isPlay = false;
					this->stopAllActions();
					// ���Ϳ��� �����.
					it->runAction(Sequence::create(FadeOut::create(0.5f), RemoveSelf::create(), nullptr));
					arrow_vector.erase(find(arrow_vector.begin(), arrow_vector.end(), it));

					target_getAttack = true;
					this->Health -= 10;
					if (Health <= 0)
					{
						_target->_score += 20;

						// --------------------------------------------
						auto iter = std::find(enemy_vector.begin(), enemy_vector.end(), this);
						if (iter != enemy_vector.end())
						{
							enemy_vector.erase(iter);
						}

						//-----------------------------------------------

						Health = 0;
						int random_num = cocos2d::random(1, 5);

						if (random_num == 3)
						{
							// �������� �����Ѵ� -- [ HEALTH ]
							Item* healthItem = Item::HEALTH(this->getPosition());
							_ownerLayer->getParent()->getChildByName("ITEM")->addChild(healthItem);
							item_vector.push_back(healthItem);
						}

						this->stopAllActions();

						Sprite* effect = Sprite::create("hit_1.png");
						effect->setPosition(this->getPosition());
						this->getParent()->getParent()->getParent()->getChildByName("UNIT")->addChild(effect);

						Animation* hit_animation = Animation::create();
						hit_animation->setDelayPerUnit(0.09f);
						hit_animation->addSpriteFrameWithFileName("hit_1.png");
						hit_animation->addSpriteFrameWithFileName("hit_2.png");
						hit_animation->addSpriteFrameWithFileName("hit_3.png");
						hit_animation->addSpriteFrameWithFileName("hit_2.png");

						effect->runAction(Sequence::create(Animate::create(hit_animation), RemoveSelf::create(), nullptr));

						this->runAction(Sequence::create(FadeOut::create(0.5f), RemoveSelf::create(), nullptr));
						execute = false;
						return;
					}
					// ������ ����ϰ� -> AttackDirection���� �ٲ�
					_target->setDirection();

					this->runAction(Sequence::create(
						Repeat::create(
						Sequence::create(TintTo::create(0.05, Color3B::RED), TintTo::create(0.05f, Color3B::WHITE),
						nullptr), 2), nullptr));

					goto ARROW_EXIT;
				}
			}
		}

		ARROW_EXIT:


		SearchingTargetUpdate(delta);
		// ������ ����
		AttackUpdate(delta);
		MovingUpdate(delta);
	}
}

void Bug::MovingUpdate(float delta)
{
	back_time += delta;
	if (back_time >= 0.05f)
	{
		back_time -= back_time;
		Vec2 position = this->getPosition();
		Vec2 pos = GetTileCoordinateWithNodeSpacePosition(position);
		if (pos == Vec2(-1, -1))
			return;

		int tileGid = this->ownerTileLayer->getTileGIDAt(pos);

		if (tileGid)
		{
			// Ÿ�ϸʿ��� GID�� �ش��ϴ� �κ��� �Ӽ� �о� ����
			Value& properties = ownerTileMap->getPropertiesForGID(tileGid);

			if (!properties.isNull())
			{
				std::string wall = properties.asValueMap()["Wall"].asString();

				if (wall == "Yes")
				{
					// ������ �浹
					// log("Wall!!!!!");
					this->setPosition(back_pos);
					return;
				}
			}
		}
		// ���͸� �븻������� ���Ⱚ�� �����.
		// ���� �����ǿ��� ���� * �ʴ� �ȼ� * ��Ÿ���� ���Ѵ�.
		this->setPosition(position);

	}
}

void Bug::AttackUpdate(float delta)
{
	if (target_setAttack)
	{
		attack_timer += delta;
		if (attack_timer >= 1.0f)
		{
			attack_timer -= attack_timer;
			target_setAttack = false;
		}
	}
	else
	{
		attack_timer -= attack_timer;
	}
	if (target_getAttack)
	{
		set_attack_timer += delta;
		if (set_attack_timer >= 0.38f)
		{
			set_attack_timer -= set_attack_timer;
			target_getAttack = false;
		}
	}
	else
	{
		set_attack_timer -= set_attack_timer;
	}

	// ------------------------------------ [ ���� �� ]
	if (this->getBoundingBox().intersectsRect(_target->getBoundingBox()))
	{
		if (!target_setAttack)
		{
			target_setAttack = true;
			_curDirection = Direction::ATTACK;
			// �ִϸ��̼� ������ -> ����
			// �ִϸ��̼� ����

			Vec2 interval = _target->getPosition() - this->getPosition();

			//------------------------- [ ����, �ִϸ��̼� ]
			float angle = atan2f(interval.y, interval.x);
			// log("Angle : %.f", CC_RADIANS_TO_DEGREES(angle));
			angle = CC_RADIANS_TO_DEGREES(angle);

			this->culAnimationStop();
			this->stopAllActions();
			this->_isPlay = false;

			if (45 <= angle && 135 >= angle)
			{
				this->play("UP");
				this->setFlipX(false);
			}
			else if (135 <= angle || -135 >= angle)
			{
				this->play("LEFT");
				this->setFlipX(false);
			}
			else if (-135 <= angle && -45 >= angle)
			{
				this->play("DOWN");
				this->setFlipX(false);
			}
			else if (-45 <= angle && 45 >= angle)
			{
				this->play("LEFT");
				this->setFlipX(true);
			}


			_target->Health -= 3;
			_target->runAction(
				Sequence::create(
				Repeat::create(Sequence::create(TintTo::create(0.05, Color3B::RED), TintTo::create(0.05f, Color3B::WHITE),
				nullptr), 2), nullptr));

			//this->runAction(Sequence::create(DelayTime::create(1.75f),
			//	CallFunc::create([&]()
			//{
			//	if (this->getBoundingBox().intersectsRect(_target->getBoundingBox()))
			//	{
			//		_target->Health -= 5;
			//		_target->runAction(
			//			Sequence::create(
			//			Repeat::create(Sequence::create(TintTo::create(0.05, Color3B::RED), TintTo::create(0.05f, Color3B::WHITE),
			//			nullptr), 2), nullptr));
			//	}
			//}),nullptr));
		}
	}

	// ------------------------------------ [ ���� ����]


	if (!target_getAttack) {
		// Ÿ���� �����϶�
		if (_target->getDirection() == Direction::ATTACK) {
			if (!_target->_arrow){
				// �����ȿ� ���ٸ�
				if (this->getBoundingBox().intersectsRect(_target->getBoundingBox()))
				{
					target_getAttack = true;
					this->Health -= 30;
					// ----------------------------------------------------------------- [ ���� ]
					if (Health <= 0)
					{
						_target->_score += 30;
						// --------------------------------------------
						auto iter = std::find(enemy_vector.begin(), enemy_vector.end(), this);
						if (iter != enemy_vector.end())
						{
							enemy_vector.erase(iter);
						}

						//-----------------------------------------------

						log("Bug to Die");
						int random_num = cocos2d::random(1, 2);

						if (random_num == 2)
						{
							Item* wire = Item::WIRE(this->getPosition());
							this->getParent()->getParent()->getParent()->getChildByName("ITEM")->addChild(wire);
							item_vector.push_back(wire);
						}

						Sprite* effect = Sprite::create("hit_1.png");
						effect->setPosition(this->getPosition());
						this->getParent()->getParent()->getParent()->getChildByName("UNIT")->addChild(effect);

						Animation* hit_animation = Animation::create();
						hit_animation->setDelayPerUnit(0.09f);
						hit_animation->addSpriteFrameWithFileName("hit_1.png");
						hit_animation->addSpriteFrameWithFileName("hit_2.png");
						hit_animation->addSpriteFrameWithFileName("hit_3.png");
						hit_animation->addSpriteFrameWithFileName("hit_2.png");

						effect->runAction(Sequence::create(Animate::create(hit_animation), RemoveSelf::create(),nullptr));

						this->runAction(Sequence::create(FadeOut::create(0.5f), RemoveSelf::create(), nullptr));
						execute = false;
						return;
					}
					// ������ ����ϰ� -> AttackDirection���� �ٲ�
					_target->setDirection();

					this->runAction(Sequence::create(
						Repeat::create(
						Sequence::create(TintTo::create(0.05, Color3B::RED), TintTo::create(0.05f, Color3B::WHITE),
						nullptr), 2), nullptr));
				}
				// 0.3���Ŀ� �ٽ� ������ ������ �ִ�.
				this->runAction(Sequence::create(DelayTime::create(0.2f), nullptr));
			}
		}
	}
}

void Bug::SearchingTargetUpdate(float delta)
{
	_time += delta;
	if (_time >= 6.5f)
	{
		_time -= _time;
		this->stopAllActions();
		rand_to_animation(cocos2d::random(1, 4));
	}

}

void Bug::rand_to_animation(int num)
{
	this->_isPlay = false;
	this->culAnimationStop();
	MoveBy* moveAnimation;
	switch (num)
	{
	case 1:
		this->play("UP", 7);
		this->setFlipX(false);
		moveAnimation = MoveBy::create(5.0f, Vec2(0.0f, 45.0f));
		break;
	case 2:
		this->play("LEFT", 7);
		this->setFlipX(true);
		moveAnimation = MoveBy::create(5.0f, Vec2(45.0f, 0.0f));
		break;
	case 3:
		this->play("DOWN", 7);
		this->setFlipX(false);
		moveAnimation = MoveBy::create(5.0f, Vec2(0.0f, -45.0f));
		break;
	case 4:
		this->play("LEFT", 7);
		this->setFlipX(false);
		moveAnimation = MoveBy::create(5.0f, Vec2(-45.0f, 0.0f));
		break;
	}
	this->runAction(moveAnimation);
}