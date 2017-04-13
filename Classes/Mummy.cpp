#include "Mummy.h"

Mummy::~Mummy()
{

}

void Mummy::InitWithEnemy()
{
	rand_to_animation(3);

	this->schedule(schedule_selector(Mummy::Update));
}

void Mummy::Update(float delta)
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
					// 벡터에서 지운다.
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

						//-----------------------------------------------
						Health = 0;
						int random_num = cocos2d::random(1, 5);

						if (random_num == 3)
						{

							this->stopAllActions();
							
							Sprite* effect = Sprite::create("hit_1.png");
							effect->setPosition(this->getPosition());
							this->_ownerLayer->getParent()->getChildByName("UNIT")->addChild(effect);

							Animation* hit_animation = Animation::create();
							hit_animation->setDelayPerUnit(0.09f);
							hit_animation->addSpriteFrameWithFileName("hit_1.png");
							hit_animation->addSpriteFrameWithFileName("hit_2.png");
							hit_animation->addSpriteFrameWithFileName("hit_3.png");
							hit_animation->addSpriteFrameWithFileName("hit_2.png");

							effect->runAction(Sequence::create(Animate::create(hit_animation), RemoveSelf::create(), nullptr));

							// 아이템을 생성한다 -- [ HEALTH ]
							Item* healthItem = Item::HEALTH(this->getPosition());
							_ownerLayer->getParent()->getChildByName("ITEM")->addChild(healthItem);
							item_vector.push_back(healthItem);
						}

						this->runAction(Sequence::create(FadeOut::create(0.5f), RemoveSelf::create(), nullptr));
						execute = false;
						return;
					}
					// 공격을 취소하고 -> AttackDirection으로 바꿈
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

		// 타겟을 찾지 못하면 플레이어가 주변에 올때까지 기다린다.
		if (!find_target)			
			SearchingTargetUpdate(delta);

		// 찾은후 this가 Target에 가도록 설정한다.
		else
		{
			// 타겟을 찾아서 이동한다.
			FindTargetUpdate(delta);

			// 공격의 유무
			AttackUpdate(delta);
		}
	}
}

void Mummy::AttackUpdate(float delta)
{
	if (target_setAttack)
	{
		attack_time += delta;
		if (attack_time >= 2.5f)
		{
			attack_time -= attack_time;
			target_setAttack = false;
		}
	}
	else
	{
		attack_time -= attack_time;
	}

	if (target_getAttack)
	{
		attack_getTime += delta;
		if (attack_getTime >= 2.0f)
		{
			target_getAttack = false;
			attack_getTime -= attack_getTime;
		}
	}
	else
	{
		attack_getTime -= attack_getTime;
	}

	// ------------------------------------ [ 공격 당함]

	if (!target_getAttack) {
		// 타겟이 공격일때
		if (_target->getDirection() == Direction::ATTACK) {
			if (!_target->_arrow){
				// 범위안에 들어갔다면
				if (this->getBoundingBox().intersectsRect(_target->getBoundingBox()))
				{
					target_getAttack = true;
					this->Health -= 15;
					// ----------------------------------------------------------------- [ 죽음 ]
					if (Health <= 0)
					{
						_target->_score += 40;
						// --------------------------------------------
						auto iter = std::find(enemy_vector.begin(), enemy_vector.end(), this);
						if (iter != enemy_vector.end())
						{
							enemy_vector.erase(iter);
						}

						//-----------------------------------------------

						int random_num = cocos2d::random(1, 2);

						if (random_num == 2) {
							// 아이템을 생성한다 -- [ HEALTH ]
							Item* healthItem = Item::HEALTH(this->getPosition());
							_ownerLayer->getParent()->getChildByName("ITEM")->addChild(healthItem);
							item_vector.push_back(healthItem);
						}

						this->runAction(Sequence::create(FadeOut::create(0.5f), RemoveSelf::create(), nullptr));
						execute = false;
						return;
					}
					// 공격을 취소하고 -> AttackDirection으로 바꿈
					_target->setDirection();

					this->runAction(Sequence::create(
						Repeat::create(
						Sequence::create(TintTo::create(0.05, Color3B::RED), TintTo::create(0.05f, Color3B::WHITE),
						nullptr), 2), nullptr));
				}
				// 0.3초후에 다시 공격을 받을수 있다.
				this->runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([&](){target_getAttack = false; }), nullptr));
			}
		}

		// ------------------------------------ [ 공격 함 ]

		if (!target_setAttack)
		{
			target_setAttack = true;
			_curDirection = Direction::ATTACK;
			// 애니메이션 실행후 -> 공격
			// 애니메이션 실행

			this->runAction(Sequence::create(DelayTime::create(2.0f),
			CallFunc::create([&]()
			{
				if (this->getBoundingBox().intersectsRect(_target->getBoundingBox()))
				{
					_target->Health -= 10;
					_target->runAction(Sequence::create(
											Repeat::create(
												Sequence::create(TintTo::create(0.05, Color3B::RED), TintTo::create(0.05f, Color3B::WHITE),
													nullptr),2), nullptr));
				}
			}), DelayTime::create(0.5f),CallFunc::create([&](){target_setAttack = false; }), nullptr));
		}
	}
}

void Mummy::SearchingTargetUpdate(float delta)
{
	if (_target->getBoundingBox().intersectsRect(this->getBoundingBox()))
	{
		// 모든 애니메이션을 멈춘다.
		this->find_target = true;
		this->_isPlay = false;
		this->stopAllActions();
		FIND();
		return;
	}
	else
	{
		_time += delta;
		if (_time >= 7.0f)
		{
			_time -= _time;
			this->stopAllActions();
			rand_to_animation(cocos2d::random(1, 4));
		}
	}
}

void Mummy::FindTargetUpdate(float delta)
{
	if (!stiff){
		// 자신에서 타겟까지의 벡터를 구함
		Vec2 interval = (_target->getPosition() + Vec2(0, _target->getContentSize().height / 2)) - this->getPosition();

		//------------------------- [ 뱡향, 애니메이션 ]
		float angle = atan2f(interval.y, interval.x);
		// log("Angle : %.f", CC_RADIANS_TO_DEGREES(angle));
		angle = CC_RADIANS_TO_DEGREES(angle);

		// 각도 -> 애니메이션
		dgree_to_animation(angle);

		if (interval.x > -5.f && interval.x < 5.f &&
			interval.y > -5.f && interval.y < 5.f)
			return;
		// 벡터를 노말라이즈로 방향값만 남긴다.
		// 현재 포지션에서 방향 * 초당 픽셀 * 델타값을 더한다.


		Vec2 position = this->getPosition() + (interval.getNormalized() * mmvelocity * delta);

		if (!noTMX){
			Vec2 pos = GetTileCoordinateWithNodeSpacePosition(position);
			if (pos == Vec2(-1, -1))
				return;

			int tileGid = this->ownerTileLayer->getTileGIDAt(pos);

			if (tileGid)
			{
				// 타일맵에서 GID에 해당하는 부분의 속성 읽어 오기
				Value& properties = ownerTileMap->getPropertiesForGID(tileGid);

				if (!properties.isNull())
				{
					std::string wall = properties.asValueMap()["Wall"].asString();

					if (wall == "Yes")
					{
						// 벽과의 충돌
						// log("Wall!!!!!");
						return;
					}
				}
			}
		}
		// 벡터를 노말라이즈로 방향값만 남긴다.
		// 현재 포지션에서 방향 * 초당 픽셀 * 델타값을 더한다.
		this->setPosition(position);

		// return this->getPosition() + (interval.getNormalized() * mmvelocity * delta);
		// this->setPosition(this->getPosition() + (interval.getNormalized() * mmvelocity * delta));
	}
}

void Mummy::dgree_to_animation(float dgree)
{
	if (45 <= dgree && 135 >= dgree)
	{
		this->play("UP");
		this->setFlipX(false);
	}
	else if (135 <= dgree || -135 >= dgree)
	{
		this->play("LEFT");
		this->setFlipX(false);
	}
	else if (-135 <= dgree && -45 >= dgree)
	{
		this->play("DOWN");
		this->setFlipX(false);
	}
	else if (-45 <= dgree && 45 >= dgree)
	{
		this->play("LEFT");
		this->setFlipX(true);
	}
}

void Mummy::rand_to_animation(int num)
{
	static MoveBy* moveAnimation;
	this->_isPlay = false;
	this->culAnimationStop();

	if (moveAnimation)
		this->stopAction(moveAnimation);

	switch (num)
	{
	case 1:
		this->play("UP", 5);
		this->setFlipX(false);
		moveAnimation = MoveBy::create(5.0f, Vec2(0.0f, 30.0f));
		break;
	case 2:
		this->play("LEFT", 5);
		this->setFlipX(true);
		moveAnimation = MoveBy::create(5.0f, Vec2(30.0f, 0.0f));
		break;
	case 3:
		this->play("DOWN", 5);
		this->setFlipX(false);
		moveAnimation = MoveBy::create(5.0f, Vec2(0.0f, -30.0f));
		break;
	case 4:
		this->play("LEFT", 5);
		this->setFlipX(false);
		moveAnimation = MoveBy::create(5.0f, Vec2(-30.0f, 0.0f));
		break;
	}
	this->runAction(moveAnimation);
}