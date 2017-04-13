#include "Sphinx.h"
#include "Mummy.h"
#include "GameScene2.h"

Sphinx::~Sphinx()
{

}

void Sphinx::InitWithEnemy()
{
	execute = false;

	key = nullptr;

	auto health_bar = Sprite::create("boss1_health.png");
	health_bar->setPosition(Vec2(-11, -214));
	health_bar->setName("BOSS1_HEALTH");
	health_bar->setVisible(false);
	health_bar->setTag(90);
	_ownerLayer->addChild(health_bar);

	auto interface_health_bar = ProgressTimer::create(Sprite::create("boss1_health_bar.png"));
	interface_health_bar->setType(ProgressTimerType::BAR);
	interface_health_bar->setMidpoint(Vec2(0.0f, 0.5f));
	interface_health_bar->setPercentage(100.0f);
	interface_health_bar->setBarChangeRate(Vec2(1, 0));
	interface_health_bar->setName("BOSS1_HEALTH_BAR");
	interface_health_bar->setTag(91);
	interface_health_bar->setPosition(Vec2(health_bar->getContentSize().width / 2, health_bar->getContentSize().height / 2));
	interface_health_bar->setVisible(false);
	health_bar->addChild(interface_health_bar);

	this->schedule(schedule_selector(Sphinx::Update));
}

void Sphinx::Update(float delta)
{
	if (key)
	{
		if (key->getBoundingBox().intersectsRect(_target->getBoundingBox()))
		{
			arrow_vector.clear();
			attack_item_vector.clear();
			enemy_vector.clear();
			item_vector.clear();

			ObserberTarget = nullptr;
			Director::getInstance()->replaceScene(TransitionFade::create(1.0f, GameScene2::createScene()));
			this->unschedule(schedule_selector(Sphinx::Update));
		}
	}

	if (!execute){
		Rect bouding = Rect(this->getPosition().x - 40, this->getPosition().y, 80, this->getContentSize().height / 2);
		if (bouding.intersectsRect(_target->getBoundingBox()))
		{
			if (enemy_vector.size() <= 1)
			{
				execute = true;

				if (!first_attack)
				{
					first_attack = true;
					_ownerLayer->getChildByTag(90)->setVisible(true);
					_ownerLayer->getChildByTag(90)->getChildByTag(91)->setVisible(true);
				}
			}
			else
			{
				auto tempString = Label::create("모든 적을\n섬멸하라", "Arial", 50);
				tempString->setPosition(this->getPosition() + Vec2(0, -200));
				this->getParent()->addChild(tempString,10);

				tempString->runAction(Sequence::create(FadeOut::create(1.0f), RemoveSelf::create(), nullptr));
			}
		}
	}

	if (execute)
	{
		// ------------------------------------ [ ARROW ]
		if (arrow_vector.size())
		{
			for (auto it : arrow_vector)
			{
				Rect bounding = Rect(this->getPosition().x - 70, this->getPosition().y, 140, it->getContentSize().height / 2);
				if (it->getBoundingBox().intersectsRect(bounding))
				{
					// 벡터에서 지운다.
					it->runAction(Sequence::create(FadeOut::create(0.2f), RemoveSelf::create(), nullptr));
					arrow_vector.erase(find(arrow_vector.begin(), arrow_vector.end(), it));

					target_getAttack = true;
						if (!first_attack)
						{
							first_attack = true;
							_ownerLayer->getChildByTag(90)->setVisible(true);
							_ownerLayer->getChildByTag(90)->getChildByTag(91)->setVisible(true);
						}
					this->Health -= 2;
					if (Health <= 20)
					{
						key = Sprite::create("Key.png");
						key->setPosition(this->getPosition() + Vec2(0,100.0f));
						this->getParent()->addChild(key, 1);
						key->setVisible(true);
						_target->_score += 50;
						// --------------------------------------------
						auto iter = std::find(enemy_vector.begin(), enemy_vector.end(), this);
						if (iter != enemy_vector.end())
						{
							enemy_vector.erase(iter);
						}

						//-----------------------------------------------
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
		if (!stiff)
			AttackUpdate(delta);
	}
	else
	{
		this->culAnimationStop();
	}
}

void Sphinx::AttackUpdate(float delta)
{
	// 계속해서 공격이 되지 않을시
	if (target_getAttack)
	{
		get_attack_time += delta;
		if (get_attack_time >= 2.0f)
		{
			get_attack_time -= get_attack_time;
			target_getAttack = false;
		}
	}
	else
	{
		get_attack_time -= get_attack_time;
	}

	snake_time += delta;

	if (snake_time >= 15.0f)
	{
		create_time += delta;
		static int number = 0;
		// 뱀을 생성한다.
		if (create_time >= 1.5f)
		{
			create_time -= create_time;
			if (number >= 4)
			{
				number -= number;
				snake_time -= snake_time;
				return;
			}

			if (_target){
				// 미라를 생성한다.
				Mummy* mummy = Mummy::createEnemy("Enemy/Mummy/mummy.plist", _target, _ownerLayer);
				mummy->setPosition(this->getPosition() + Vec2(random(1,40), random(5,20)));
				mummy->FIND();
				this->getParent()->addChild(mummy, 0);
				number += 1;
			}
		}
	}

	// ------------------------------------ [ 공격 당함 ]
	if (!target_getAttack) {
		// 타겟이 공격일때
		if (_target->getDirection() == Direction::ATTACK) {
			if (!_target->_arrow){
				// 범위안에 들어갔다면

				Rect bouding = Rect(this->getPosition().x - 40, this->getPosition().y, 80, this->getContentSize().height / 2);
				if (bouding.intersectsRect(_target->getBoundingBox()))
				{
					target_getAttack = true;
					this->Health -= 1;
					if (Health <= 20)
					{
						_target->_score += 70;
						// --------------------------------------------
						auto iter = std::find(enemy_vector.begin(), enemy_vector.end(), this);
						if (iter != enemy_vector.end())
						{
							enemy_vector.erase(iter);
						}

						//-----------------------------------------------
						key = Sprite::create("Key.png");
						key->setPosition(this->getPosition() + Vec2(0, 100.0f));
						this->getParent()->addChild(key, 1);
						Health = 0;

						this->stopAllActions();


						this->runAction(Sequence::create(FadeOut::create(0.5f), nullptr));
						this->getPhysicsBody()->removeFromWorld();
						execute = false;
						// return;
					}
					// 공격을 취소하고 -> AttackDirection으로 바꿈
					_target->setDirection();

					if (!first_attack)
					{
						first_attack = true;
						_ownerLayer->getChildByTag(90)->setVisible(true);
						_ownerLayer->getChildByTag(90)->getChildByTag(91)->setVisible(true);
					}

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
			if (this->getBoundingBox().intersectsRect(_target->getBoundingBox()))
			{
				_curDirection = Direction::ATTACK;
				
				// 만약 플레이어가 곁에 있다면 
				// 1.5초 대기 -> 공격 -> 4.5초 대기 -> 공격준비 완료
				this->runAction(Sequence::create(
					DelayTime::create(2.5f),
					CallFunc::create([&](){target_setAttack = true; this->play("FIRE"); }),
					DelayTime::create(0.98f), CallFunc::create([&]()
				{
					target_setAttack = false; 
				}), nullptr));
			}
			else
			{

			}
		}
		// 만약 지금 공격중이라면
		else
		{
			static bool hit_dammage = false;

			if (!hit_dammage)
			{
				fire_boundingBox = Rect(this->getPosition().x - 60, this->getPosition().y-10, 120, this->getContentSize().height/2);
				// 공격중일때 
				if (fire_boundingBox.intersectsRect(_target->getBoundingBox()))
				{
					hit_dammage = true;
					_target->Health -= 45;

					_target->runAction(Sequence::create(
						Repeat::create(
						Sequence::create(TintTo::create(0.05, Color3B::RED), TintTo::create(0.05f, Color3B::WHITE),
						nullptr), 2), nullptr));
				}
				this->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([&](){hit_dammage = false; }), nullptr));
			}
		}
	}
}