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
						this->runAction(Sequence::create(FadeOut::create(1.5f), RemoveSelf::create(), nullptr));
						execute = false;
						// return;
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

		// 감시자를 사용합니다.
		// ObserberTarget을 공유합니다. 사용안할때는 nullptr로 초기화 바람
		if (!ObserberTarget) {
			// 플레이어를 발견하면..
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
				//	// 타일맵에서 GID에 해당하는 부분의 속성 읽어 오기
				//	Value& properties = ownerTile->getPropertiesForGID(tileGid);

				//	if (!properties.isNull())
				//	{
				//		std::string wall = properties.asValueMap()["Wall"].asString();

				//		if (wall == "Yes")
				//		{
				//			// 벽과의 충돌
				//			// log("Wall!!!!!");
				//			return;
				//		}
				//	}
				//}
				//// 벡터를 노말라이즈로 방향값만 남긴다.
				//// 현재 포지션에서 방향 * 초당 픽셀 * 델타값을 더한다.
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

	// ------------------------------------[공격 당함]

	if (!target_getAttack) {
		// 타겟이 공격일때
		if (_target->getDirection() == Direction::ATTACK) {
			if (!_target->_arrow){
				// 범위안에 들어갔다면
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

							// 아이템을 생성한다 -- [ HEALTH ]
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
					// 공격을 취소하고 -> AttackDirection으로 바꿈
					_target->setDirection();

					this->runAction(Repeat::create(
						Sequence::create(TintTo::create(0.05, Color3B::RED), TintTo::create(0.05f, Color3B::WHITE),
						nullptr), 2));
				}
			}
		}

	}
	// ------------------------------------ [ 공격 함 ]

	if (!target_setAttack)
	{
		target_setAttack = true;
		_curDirection = Direction::ATTACK;
		// 애니메이션 실행후 -> 공격
		// 애니메이션 실행
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