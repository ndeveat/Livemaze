#include "Cat.h"
#include "PathFind.h"

Cat::~Cat()
{

}

void Cat::InitWithEnemy()
{
	pathFind = new PathFinding();

	this->schedule(schedule_selector(Cat::Update));
}

void Cat::Update(float delta)
{
	if (execute){

		if (!stiff){
			target_move_time += delta;
			if (target_move_time >= 1.0f)
			{
				target_move_time -= target_move_time;

				this->setTargetLoad(pathFind->vec2_pathFinding(
					this->GetTileCoordinateWithNodeSpacePosition(this->getPosition()),
					this->GetTileCoordinateWithNodeSpacePosition(_target->getPosition())));

				if (this->getTargetLoad().size() <= 4)
					target_move_search = true;
				else
					target_move_search = false;

				target_move = false;
			}
		}

		if (stiff)
		{
			stiff_time += delta;
			if (stiff_time >= 2.0f)
			{
				stiff_time -= stiff_time;
				this->_isPlay = false;
				this->culAnimationStop();
				this->offStiff();
			}
		}
		else
		{
			stiff_time -= stiff_time;
		}

		if (!stiff)
		{
			// 고양이는 바로 플레이어에게 달려든다.
			Vec2 interval = (_target->getPosition() + Vec2(0, 30)) - this->getPosition();

			float angle = atan2f(interval.y, interval.x);
			angle = CC_RADIANS_TO_DEGREES(angle);
			if (angle >= -90 && angle <= 90) {
				this->play("LEFT");
				this->setFlipX(true);
			}
			else
			{
				this->play("LEFT");
				this->setFlipX(false);
			}


			if (!target_move_search)
			{
				if (!target_move)
				{
					this->stopAction(moveAction);

					Vector<FiniteTimeAction*> master_move;
					std::vector<Vec2> move_vector;
					target_move = true;
					for (auto it : this->target_load)
					{
						float xpos = (it.x - it.y) * 64 + (WIDTH * 2);
						float ypos = (HEIGHT * 4) - (it.x + it.y) * 32 - 32;

						move_vector.push_back(Vec2(xpos, ypos));
					}

					for (auto it : move_vector)
					{
						master_move.pushBack(MoveTo::create(0.7f, Vec2(it)));
					}

					moveAction = Sequence::create(master_move);
					this->runAction(moveAction);
				}
			}
			else
			{
				//this->stopAction(moveAction);
				// 각도를 계산한다.

				this->setPosition(this->getPosition() + (interval.getNormalized() * 60 * delta));
			}
			// --------------------[ 공격 업데이트 ]
			ArrowUpdate();
			AttackUpdate(delta);
		}
		else
			this->stopAllActions();
	}
}

void Cat::ArrowUpdate()
{
	// ------------------------------------ [ ARROW ]
	if (arrow_vector.size())
	{
		for (auto it : arrow_vector)
		{
			if (it->getBoundingBox().intersectsRect(this->getBoundingBox()))
			{
				// 벡터에서 지운다.
				it->runAction(Sequence::create(FadeOut::create(0.5f), RemoveSelf::create(), nullptr));
				arrow_vector.erase(find(arrow_vector.begin(), arrow_vector.end(), it));

				target_getAttack = true;
				this->Health -= 30;
				if (Health <= 0)
				{
					_target->_score += 10;
					// --------------------------------------------
					auto iter = std::find(enemy_vector.begin(), enemy_vector.end(), this);
					if (iter != enemy_vector.end())
					{
						enemy_vector.erase(iter);
					}

					//-----------------------------------------------

					Sprite* effect = Sprite::create("hit_1.png");
					effect->setPosition(this->getPosition());
					this->getParent()->getParent()->getParent()->getChildByName("UNIT")->addChild(effect);

					Animation* hit_animation = Animation::create();
					hit_animation->setDelayPerUnit(0.2f);
					hit_animation->addSpriteFrameWithFileName("hit_1.png");
					hit_animation->addSpriteFrameWithFileName("hit_2.png");
					hit_animation->addSpriteFrameWithFileName("hit_3.png");
					hit_animation->addSpriteFrameWithFileName("hit_2.png");

					effect->runAction(Sequence::create(Animate::create(hit_animation), RemoveSelf::create(), nullptr));

					this->runAction(Sequence::create(FadeOut::create(1.3f), RemoveSelf::create(), nullptr));
					execute = false;
					// return;
				}
				// 공격을 취소하고 -> AttackDirection으로 바꿈
				_target->setDirection();

				this->runAction(Sequence::create(
					Repeat::create(
					Sequence::create(TintTo::create(0.05, Color3B::RED), TintTo::create(0.05f, Color3B::WHITE),
					nullptr), 2), nullptr));

				return;
			}
		}
	}
}

void Cat::AttackUpdate(float delta)
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
		target_getAttack = false;
		set_attack_timer -= set_attack_timer;
	}

	// ------------------------------------ [ 공격 당함]

	if (!target_getAttack) {
		// 타겟이 공격일때
		if (_target->getDirection() == Direction::ATTACK) {
			// 범위안에 들어갔다면
			if (!_target->_arrow){
				if (this->getBoundingBox().intersectsRect(_target->getBoundingBox()))
				{
					target_getAttack = true;
					this->Health -= 60;
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
						Health = 0;
						int radom_num = cocos2d::random(1, 2);
						if (radom_num == 2)
						{
							Item* arrow = Item::ARROW(this->getPosition());
							_ownerLayer->getParent()->getChildByName("ITEM")->addChild(arrow);
							item_vector.push_back(arrow);
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
						// return;
					}
					// 공격을 취소하고 -> AttackDirection으로 바꿈
					_target->setDirection();

					this->runAction(Sequence::create(
						Repeat::create(
						Sequence::create(TintTo::create(0.05, Color3B::RED), TintTo::create(0.05f, Color3B::WHITE),
						nullptr), 2), nullptr));
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
				_target->Health -= 3;
				_target->runAction(Sequence::create(
					Repeat::create(
					Sequence::create(TintTo::create(0.05, Color3B::RED), TintTo::create(0.05f, Color3B::WHITE),
					nullptr), 2), nullptr));
			}

			//this->runAction(Sequence::create(DelayTime::create(0.8f),
			//	CallFunc::create([&]()
			//{
			//	if (this->getBoundingBox().intersectsRect(_target->getBoundingBox()))
			//	{
			//		_target->Health -= 3;
			//		_target->runAction(Sequence::create(
			//			Repeat::create(
			//			Sequence::create(TintTo::create(0.05, Color3B::RED), TintTo::create(0.05f, Color3B::WHITE),
			//			nullptr), 2), nullptr));
			//	}
			//}), DelayTime::create(0.38f), nullptr));
		}
	}
}

void Cat::setMap(int _map[][40])
{
	int temp[40][40];
	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 40; j++)
		{
			temp[i][j] = _map[i][j];
		}
	}
	pathFind->loadMap(temp);
}