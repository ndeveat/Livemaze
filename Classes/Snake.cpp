#include "Snake.h"

Snake::~Snake()
{

}

void Snake::InitWithEnemy() {
	this->schedule(schedule_selector(Snake::Update));
}

void Snake::Update(float delta)
{
	if (execute){
		// ------------------------------------ [ ARROW ]
		if (arrow_vector.size())
		{
			for (auto it : arrow_vector)
			{
				if (it->getBoundingBox().intersectsRect(this->getBoundingBox()))
				{
					ObserberTarget = _target;

					// ���Ϳ��� �����.
					it->runAction(Sequence::create(FadeOut::create(0.5f), RemoveSelf::create(), nullptr));
					arrow_vector.erase(find(arrow_vector.begin(), arrow_vector.end(), it));

					target_getAttack = true;
					this->Health -= 20;
					if (Health <= 0)
					{
						_target->_score += 20;
						// --------------------------------------------
						auto iter = std::find(enemy_vector.begin(), enemy_vector.end(), this);
						if (iter != enemy_vector.end())
						{
							enemy_vector.erase(iter);
						}
						this->runAction(Sequence::create(FadeOut::create(1.5f), RemoveSelf::create(), nullptr));
						execute = false;
						// return;
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

		// �����ڸ� ����մϴ�.
		// ObserberTarget�� �����մϴ�. �����Ҷ��� nullptr�� �ʱ�ȭ �ٶ�
		if (!ObserberTarget) {
			// �÷��̾ �߰��ϸ�..
			if (this->getBoundingBox().intersectsRect(_target->getBoundingBox())) {
				ObserberTarget = _target;
				return;
			}
		}
		else
		{
			if (!stiff){
				//log("SnakeTag : %d", this->getTag());
				Vec2 interval = _target->getPosition() - this->getPosition();

				float angle = atan2f(interval.y, interval.x);

				angle = CC_RADIANS_TO_DEGREES(angle);

				if (angle >= -90 && angle <= 90) {
					this->play("LEFT");
					this->setFlipX(true);
				}
				else {
					this->play("LEFT");
					this->setFlipX(false);
				}

				// ------------------------------------------ [ Next Position ]
				Vec2 position = this->getPosition() + (interval.getNormalized() * snake_velocity * delta);
				//Vec2 pos = GetTileCoordinateWithNodeSpacePosition(position);
				//if (pos == Vec2(-1, -1))
				//	return;

				//int tileGid = this->ownerLayer->getTileGIDAt(pos);

				//if (tileGid)
				//{
				//	// Ÿ�ϸʿ��� GID�� �ش��ϴ� �κ��� �Ӽ� �о� ����
				//	Value& properties = ownerTile->getPropertiesForGID(tileGid);

				//	if (!properties.isNull())
				//	{
				//		std::string wall = properties.asValueMap()["Wall"].asString();

				//		if (wall == "Yes")
				//		{
				//			// ������ �浹
				//			// log("Wall!!!!!");
				//			return;
				//		}
				//	}
				//}
				//// ���͸� �븻������� ���Ⱚ�� �����.
				//// ���� �����ǿ��� ���� * �ʴ� �ȼ� * ��Ÿ���� ���Ѵ�.
				this->setPosition(position);


				AttackUpdate(delta);
			}
		}
	}
}

void Snake::AttackUpdate(float delta)
{
	if (target_getAttack)
	{
		target_getAttack_time += delta;
		if (target_getAttack_time >= 0.38f)
		{
			target_getAttack = false;
		}
	}
	else
	{
		target_getAttack_time -= target_getAttack_time;
	}

	if (target_setAttack)
	{
		target_setAttack_time += delta;
		if (target_setAttack_time >= 2.0f)
		{
			target_setAttack_time -= target_setAttack_time;
			target_setAttack = false;
		}
	}
	else
	{
		target_setAttack_time -= target_setAttack_time;
	}

	// ------------------------------------[���� ����]

	if (!target_getAttack) {
		// Ÿ���� �����϶�
		if (_target->getDirection() == Direction::ATTACK) {
			if (!_target->_arrow){
				// �����ȿ� ���ٸ�
				if (this->getBoundingBox().intersectsRect(_target->getBoundingBox()))
				{
					target_getAttack = true;
					this->Health -= 35;
					if (Health <= 0)
					{
						// --------------------------------------------
						auto iter = std::find(enemy_vector.begin(), enemy_vector.end(), this);
						if (iter != enemy_vector.end())
						{
							enemy_vector.erase(iter);
						}

						//-----------------------------------------------
						Health = 0;
						int random_num = cocos2d::random(1, 4);

						if (random_num == 3)
						{
							_target->_score += 20;
							// --------------------------------------------
							auto iter = std::find(enemy_vector.begin(), enemy_vector.end(), this);
							if (iter != enemy_vector.end())
							{
								enemy_vector.erase(iter);
							}

							//-----------------------------------------------

							// �������� �����Ѵ� -- [ HEALTH ]
							Item* speedItem = Item::SPEED(this->getPosition());
							_ownerLayer->getParent()->getChildByName("ITEM")->addChild(speedItem);
							item_vector.push_back(speedItem);
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

						this->runAction(Sequence::create(FadeOut::create(0.3f), RemoveSelf::create(), nullptr));
						execute = false;
						// return;
					}
					// ������ ����ϰ� -> AttackDirection���� �ٲ�
					_target->setDirection();

					this->runAction(Repeat::create(
						Sequence::create(TintTo::create(0.05, Color3B::RED), TintTo::create(0.05f, Color3B::WHITE),
						nullptr), 2));
				}
			}
		}

	}
	// ------------------------------------ [ ���� �� ]

	if (!target_setAttack)
	{
		target_setAttack = true;
		_curDirection = Direction::ATTACK;
		// �ִϸ��̼� ������ -> ����
		// �ִϸ��̼� ����
		if (this->getBoundingBox().intersectsRect(_target->getBoundingBox()))
		{
			_target->Health -= 2;
			_target->runAction(Sequence::create(
				Repeat::create(
				Sequence::create(TintTo::create(0.05, Color3B::RED), TintTo::create(0.05f, Color3B::WHITE),
				nullptr), 2), nullptr));
		}

		//this->runAction(Sequence::create(DelayTime::create(2.8f),
		//	CallFunc::create([&]()
		//{
		//	if (this->getBoundingBox().intersectsRect(_target->getBoundingBox()))
		//	{
		//		_target->Health -= 5;
		//		_target->runAction(Sequence::create(
		//			Repeat::create(
		//			Sequence::create(TintTo::create(0.05, Color3B::RED), TintTo::create(0.05f, Color3B::WHITE),
		//			nullptr), 2), nullptr));
		//	}
		//}), DelayTime::create(0.5f), CallFunc::create([&](){target_setAttack = false; }), nullptr));
	}
}