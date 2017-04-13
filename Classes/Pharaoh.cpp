#include "Pharaoh.h"
#include "Snake.h"
#include "MainMenu.h"
#include "End.h"

Pharaoh::~Pharaoh()
{

}

void Pharaoh::InitWithEnemy()
{
	execute = false;
	chest = nullptr;
	auto health_bar = Sprite::create("boss2_health.png");
	health_bar->setPosition(Vec2(-11, -214));
	health_bar->setName("BOSS2_HEALTH");
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

	this->schedule(schedule_selector(Pharaoh::Update));
}

void Pharaoh::Update(float delta)
{
	if (chest)
	{
		if (chest->getBoundingBox().intersectsRect(_target->getBoundingBox()))
		{
			arrow_vector.clear();
			attack_item_vector.clear();
			enemy_vector.clear();
			item_vector.clear();
			ObserberTarget = nullptr;
			Director::getInstance()->replaceScene(TransitionFade::create(1.5f, End::createScene()));
		}
	}

	if (!execute)
	{
		if (this->getBoundingBox().intersectsRect(_target->getBoundingBox()))
		{
			if (enemy_vector.size() <= 1){
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
				auto tempString = Label::create("��� ����\n�����϶�", "Arial", 50);
				tempString->setPosition(this->getPosition() + Vec2(0, -200));
				this->getParent()->addChild(tempString, 10);

				tempString->runAction(Sequence::create(FadeOut::create(1.0f), RemoveSelf::create(), nullptr));
			}
		}
	}

	if (execute)
	{
		if (!stiff){
			// �÷��̾�� ��
			FindTargetUpdate(delta);

			// ���� ������Ʈ
			AttackUpdate(delta);
		}
	}
}

void Pharaoh::AttackUpdate(float delta)
{
	// ------------------------------------ [ ARROW ]
	if (arrow_vector.size())
	{
		for (auto it : arrow_vector)
		{
			if (it->getBoundingBox().intersectsRect(this->getBoundingBox()))
			{
				// ---------------- [ ȭ���� �¾Ҵٸ� ] 

				// ���Ϳ��� �����.
				it->runAction(Sequence::create(FadeOut::create(0.5f), RemoveSelf::create(), nullptr));
				arrow_vector.erase(find(arrow_vector.begin(), arrow_vector.end(), it));

				target_getAttack = true;
				this->Health -= 5;
				if (!first_attack){
					_target->_score += 70;
					// --------------------------------------------
					auto iter = std::find(enemy_vector.begin(), enemy_vector.end(), this);
					if (iter != enemy_vector.end())
					{
						enemy_vector.erase(iter);
					}

					//-----------------------------------------------

					first_attack = true;
					_ownerLayer->getChildByTag(90)->setVisible(true);
					_ownerLayer->getChildByTag(90)->getChildByTag(91)->setVisible(true);
				}
				if (Health <= 20)
				{

					this->getPhysicsBody()->removeFromWorld();
					Health = 0;
					// ---------------------------------------------------------- [ ������ ���� ]
					chest = Sprite::create("Chest.png");
					chest->setPosition(this->getPosition() + Vec2(0,100));
					this->getParent()->addChild(chest, 1);

					// ---------------------------------------------------------- [ ���� ]
					this->runAction(Sequence::create(FadeOut::create(1.5f), nullptr));
					execute = false;
					// return;
				}
				// ������ ����ϰ� -> AttackDirection���� �ٲ�
				_target->setDirection();

				// ------------------------------------------------ [ �´� ��� ]
				this->runAction(Sequence::create(
					Repeat::create(
					Sequence::create(TintTo::create(0.05, Color3B::BLUE), TintTo::create(0.05f, Color3B::WHITE),
					nullptr), 2), nullptr));

				goto ARROW_EXIT;
			}
		}
	}

ARROW_EXIT:

	if (target_getAttack)
	{
		target_getAttack_time += delta;
		if (target_getAttack_time >= 0.37f)
		{
			target_getAttack_time -= target_getAttack_time;
			target_getAttack = false;
		}
	}
	else
	{
		target_getAttack_time -= target_getAttack_time;
	}

	// ------------------------------------ [ ���� ����]

	if (!target_getAttack) {
		// Ÿ���� �����϶�
		if (_target->getDirection() == Direction::ATTACK)
		{
			if (!_target->_arrow)
			{
				// �����ȿ� ���ٸ�
				if (this->getBoundingBox().intersectsRect(_target->getBoundingBox()))
				{
					target_getAttack = true;
					this->Health -= -10;
					if (!first_attack)
					{
						first_attack = true;
						_ownerLayer->getChildByTag(90)->setVisible(true);
						_ownerLayer->getChildByTag(90)->getChildByTag(91)->setVisible(true);
					}
					// ----------------------------------------------------------------- [ ���� ]
					if (Health <= 20)
					{
						this->getPhysicsBody()->removeFromWorld();

						_target->_score += 90;
						Health = 0;
						this->stopAllActions();
						// ------------------------------------------------------------- [ ������ ���� ]

						chest = Sprite::create("Chest.png");
						chest->setPosition(this->getPosition() + Vec2(0, 100));
						this->getParent()->addChild(chest, 1);
						// --------------------------------------------
						auto iter = std::find(enemy_vector.begin(), enemy_vector.end(), this);
						if (iter != enemy_vector.end())
						{
							enemy_vector.erase(iter);
						}

						//-----------------------------------------------
						// ------------------------------------------------------------- [ �״� ��� ]
						this->runAction(Sequence::create(FadeOut::create(1.5f), nullptr));
						execute = false;
						// return;
					}
					// ������ ����ϰ� -> AttackDirection���� �ٲ�
					_target->setDirection();

					this->runAction(Sequence::create(
						Repeat::create(
						Sequence::create(TintTo::create(0.05, Color3B::RED), TintTo::create(0.05f, Color3B::WHITE),
						nullptr), 2), nullptr));
				}
			}
		}
	}


	if (target_setAttack)
	{
		bug_fix += delta;
		if (bug_fix >= 3.0f)
		{
			bug_fix -= bug_fix;
			target_setAttack = false;
		}
	}
	else
	{
		bug_fix -= bug_fix;
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


			_target->Health -= 5;
			_target->runAction(Sequence::create(
				Repeat::create(
				Sequence::create(TintTo::create(0.05, Color3B::RED), TintTo::create(0.05f, Color3B::WHITE),
				nullptr), 2), nullptr));


			log("ATTACK");
		}
	}

	pharaoh_arrow_time += delta;
	if (pharaoh_arrow_time >= 5.0f)
	{
		if (pharaoh_vector.size())
		{
			for (auto it : pharaoh_vector)
			{
				this->getParent()->removeChild(it);
				pharaoh_vector.erase(find(pharaoh_vector.begin(), pharaoh_vector.end(), it));
				return;
			}
		}

		pharaoh_arrow_time -= pharaoh_arrow_time;
		float degree = 0.0f;
		for (int i = 0; i < 8; i++)
		{
			Sprite* arrow = Sprite::create("arrow.png");
			arrow->setRotation(degree);
			arrow->setPosition(this->getPosition());
			degree += 45;

			pharaoh_vector.push_back(arrow);
			this->getParent()->addChild(arrow);
		}
	}

	pharaoh_snake_time += delta;
	if (pharaoh_snake_time >= 6.0f)
	{
		pharaoh_snake_time -= pharaoh_snake_time;

		for (int i = 0; i < 3; i++)
		{
			auto snake = Snake::createEnemy("Enemy/Snake/snake.plist", _target, _ownerLayer);
			snake->setPosition(this->getPosition() + Vec2(-250, 0.0f));
			snake->setTMX(this->ownerTileMap, this->ownerTileLayer);
			this->getParent()->addChild(snake);
		}

		for (int i = 0; i < 3; i++)
		{
			auto snake = Snake::createEnemy("Enemy/Snake/snake.plist", _target, _ownerLayer);
			snake->setPosition(this->getPosition() + Vec2(250, 0.0f));
			snake->setTMX(this->ownerTileMap, this->ownerTileLayer);
			this->getParent()->addChild(snake);
		}
	}



	if (pharaoh_vector.size())
	{
		for (auto it : pharaoh_vector)
		{
			float dirX = sinf(CC_DEGREES_TO_RADIANS(it->getRotation()));
			float dirY = cosf(CC_DEGREES_TO_RADIANS(it->getRotation()));

			it->setPosition(it->getPosition() + (Vec2(dirX, dirY) * 350 * delta));
		}
	}

	if (pharaoh_vector.size())
	{
		for (auto it : pharaoh_vector)
		{
			if (_target->getBoundingBox().containsPoint(it->getPosition()))
			{
				it->runAction(Sequence::create(FadeOut::create(0.3f), nullptr));
				pharaoh_vector.erase(find(pharaoh_vector.begin(), pharaoh_vector.end(), it));

				_target->Health -= 5;
				_target->runAction(Sequence::create(
					Repeat::create(
					Sequence::create(TintTo::create(0.05, Color3B::RED), TintTo::create(0.05f, Color3B::WHITE),
					nullptr), 2), nullptr));

				//log("ATTACK");
				return;
			}
		}
	}

}



void Pharaoh::dgree_to_animation(float dgree)
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

void Pharaoh::FindTargetUpdate(float delta)
{
	// �ڽſ��� Ÿ�ٱ����� ���͸� ����
	Vec2 interval = (_target->getPosition() + Vec2(0, _target->getContentSize().height / 2)) - this->getPosition();

	//------------------------- [ ����, �ִϸ��̼� ]
	float angle = atan2f(interval.y, interval.x);
	// log("Angle : %.f", CC_RADIANS_TO_DEGREES(angle));
	angle = CC_RADIANS_TO_DEGREES(angle);

	// ���� -> �ִϸ��̼�
	dgree_to_animation(angle);

	if (interval.x > -5.f && interval.x < 5.f &&
		interval.y > -5.f && interval.y < 5.f)
		return;

	// ------------------------------------------ [ Next Position ]
	Vec2 position = this->getPosition() + (interval.getNormalized() * 100.0f * delta);
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
				return;
			}
		}
	}
	// ���͸� �븻������� ���Ⱚ�� �����.
	// ���� �����ǿ��� ���� * �ʴ� �ȼ� * ��Ÿ���� ���Ѵ�.
	this->setPosition(position);
}