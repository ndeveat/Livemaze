#include "Player.h"

Player::~Player()
{
	// if (animationManager)
	//	 delete animationManager;


}

bool Player::InitWithPlayer()
{
	this->setAnchorPoint(Vec2(0.5f, 0.0f));

	this->schedule(schedule_selector(Player::PlayerUpdate));
	return true;
}

void Player::PlayerUpdate(float delta)
{
	deltaTime = delta;

	ArrowUpdate();

	if (execute)
	{
		if (_stiff)
		{
			_stiff_tiem += delta;
			if (_stiff_tiem >= 3.0f)
			{
				_stiff_tiem -= _stiff_tiem;
				_stiff = false;
			}
		}

		// GUARD 아이템을 먹었을때
		if (isGod)
		{
			if (backup_Health_god != Health)
			{
				log("Health : %d", Health);
				isGod = false;
				Health = backup_Health_god;
				log("Health : %d", Health);

				this->setColor(Color3B::WHITE);
			}
		}

		if (_arrow)
		{
			if (backup_health_arrow != Health)
			{
				_arrow = false;
			}
		}
		else
			_arrow_score = 0;

		// 체력이 0 이하이면 죽음
		if (Health <= 0)
		{
			_up = false;
			_down = false;
			_left = false;
			_right = false;
			_curDirection = Direction::NORMAL;
			_attack_dir = Direction::NORMAL;
			this->runAction(Sequence::create(FadeOut::create(1.0f), nullptr));
			execute = false;
		}

		if (execute){
			KeyUpdate();
			DirectionUpdate();
			// PositionUpdate();

			// ----------------[ 아이템의 상태를 업그레이드 한다 ]
			ItemUpdate();
		}
	}
	else
	{
		this->culAnimationStop();
	}
}


void Player::RudderKeyUpdate()
{
	if (!_attack && !_useItem){
		//----------------------------
		if (GetAsyncKeyState(VK_UP))
		{
			_up = true;
			_curDirection = { Direction::UP };
			_attack_dir = { Direction::UP };

			if (_right || _left){
				_curDirection = { Direction::UP };
				_attack_dir = { Direction::UP };
			}

		}
		else
			_up = false;
		//-----------------------------
		if (GetAsyncKeyState(VK_DOWN))
		{
			_down = true;
			_curDirection = { Direction::DOWN };
			_attack_dir = { Direction::DOWN };
		}
		else
			_down = false;
		//----------------------------
		if (GetAsyncKeyState(VK_RIGHT))
		{
			_right = true;
			_curDirection = { Direction::RIGHT };
			_attack_dir = { Direction::RIGHT };

			if (_up) {
				_curDirection = { Direction::UP };
				_attack_dir = { Direction::UP };
			}
		}
		else
			_right = false;
		//----------------------------
		if (GetAsyncKeyState(VK_LEFT))
		{
			_left = true;
			_curDirection = { Direction::LEFT };
			_attack_dir = { Direction::LEFT };

			if (_up) {
				_curDirection = { Direction::UP };
				_attack_dir = { Direction::UP };
			}
		}
		else
			_left = false;
		//----------------------------

		if (!_left && !_right && !_up && !_down)
		{
			if (!_attack)
			{
				_curDirection = { Direction::NORMAL };
				this->culAnimationStop();
			}
		}

		// log("Direction : %d", _curDirection);
	}
}
void Player::AttackKeyUpdate()
{
	// 공격 버튼을 눌렀을때
	if (GetAsyncKeyState('X') & 0x8000)
	{
		if (!_attack)
		{
			_curDirection = Direction::ATTACK;

			_attack = true; // 활성화를 한다. 바로 실행을 막기위해서
			this->_isPlay = false;
			this->culAnimationStop();
			//this->stopAllActions();
			// 현재 애니메이션을 종료시킨다.

			// log("Action Stop");
			// 공격 방향
			switch (_attack_dir)
			{
			case Direction::UP:
				//log("UP ATTACK");
				if (!_arrow)
					this->play("UP_ATTACK");
				else{
					this->play("UP_ARROW");

					Sprite* arrow = Sprite::create("arrow.png");
					arrow->setPosition(this->getPosition() + Vec2(0, 30));

					this->getParent()->addChild(arrow, 1);
					_arrow_score -= 1;
					arrow->setRotation(0);
					arrow->setTag(1);
					arrow_vector.push_back(arrow);
				}
				break;
			case Direction::DOWN:
				//log("DOWN ATTACK");
				if (!_arrow)
					this->play("DOWN_ATTACK");
				else{
					this->play("DOWN_ARROW");

					Sprite* arrow = Sprite::create("arrow.png");
					arrow->setPosition(this->getPosition() + Vec2(0, 30));
					this->getParent()->addChild(arrow, 1);

					arrow->setRotation(180);
					arrow->setTag(3);
					arrow_vector.push_back(arrow);

					_arrow_score -= 1;
				}
				break;
			case Direction::LEFT:
				//log("LEFT ATTACK");
				if (!_arrow)
					this->play("RIGHT_ATTACK");
				else{
					this->play("LEFT_ARROW");

					Sprite* arrow = Sprite::create("arrow.png");
					arrow->setPosition(this->getPosition() + Vec2(0, 30));
					this->getParent()->addChild(arrow, 1);
					_arrow_score -= 1;
					arrow->setRotation(-90);
					arrow->setTag(4);
					arrow_vector.push_back(arrow);
				}
				this->setFlipX(false);
				break;
			case Direction::RIGHT:
				//log("RIGHT ATTACK");
				if (!_arrow)
					this->play("RIGHT_ATTACK");
				else{
					this->play("LEFT_ARROW");

					Sprite* arrow = Sprite::create("arrow.png");
					arrow->setPosition(this->getPosition() + Vec2(0, 30));
					this->getParent()->addChild(arrow, 1);
					_arrow_score -= 1;
					arrow->setRotation(90);
					arrow->setTag(2);
					arrow_vector.push_back(arrow);
				}
				this->setFlipX(true);
				break;
			}

			if (_arrow_score <= 0)
				_arrow = false;
		}
	}
}
void Player::ItemKeyUpdate()
{
	static float item_stiff = 0.0f;
	if (_item_stiff)
	{
		item_stiff += deltaTime;
		if (item_stiff >= 3.0f)
		{
			item_stiff -= item_stiff;
			_item_stiff = false;
		}
	}
	else
	{
		item_stiff -= item_stiff;
	}


	// 아이템을 사용할때 FIRST ----------------------//
	if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
	{
		// 성공 // 움직일때 사용 가능
		if (!_useItem) {
			if (_trap_score <= 0)
				return;

			_useItem = true;
			this->_isPlay = false;
			this->culAnimationStop();
			this->play("USE_ITEM");
			_trap_score -= 1;


			this->runAction(Sequence::create(DelayTime::create(1.0f),
				CallFunc::create([&]()
			{
				// 트랩을 설치
				Sprite* trap = Sprite::create("Trap.png");
				trap->setPosition(this->getPosition());
				trap->setTag(1);
				trap->setScale(0.7f);
				this->getParent()->addChild(trap);

				_item_stiff = true;
				attack_item_vector.push_back(trap);
				_useItem = false;
			}), nullptr));
		}
	}
	// 아이템을 사용할때 SECOND --------------------//
	if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
	{
		// 성공 // 움직일때 사용 가능
		if (!_useItem) {
			if (_wire_score <= 0)
				return;

			_useItem = true;
			this->_isPlay = false;
			this->culAnimationStop();
			this->play("USE_ITEM");
			_wire_score -= 1;

			this->runAction(Sequence::create(DelayTime::create(1.0f),
				CallFunc::create([&]()
			{
				Sprite* wire = Sprite::create("Wire.png");
				wire->setPosition(this->getPosition() + Vec2(0, -10));
				wire->setTag(2);
				wire->setScale(0.7f);
				this->getParent()->addChild(wire);
				attack_item_vector.push_back(wire);

				_item_stiff = true;
				_useItem = false;
			}), nullptr));
		}
	}
}
void Player::KeyUpdate()
{
	static float attack_time = 0.0f;
	if (_attack)
	{
		attack_time += deltaTime;
		_curDirection = Direction::ATTACK;
		if (attack_time >= 0.37f)
		{
			attack_time -= attack_time;
			_attack = false;
		}
	}

	RudderKeyUpdate();
	AttackKeyUpdate();
	ItemKeyUpdate();
}

void Player::DirectionUpdate()
{
	if (!_attack && !_useItem){
		// 이전 버전방향과
		// 현재 방향이 다르다면 현재 애니메이션을 중지
		//if (_backDirection != _curDirection)
		//	this->stopAllActions();

		// 액션을 수행함
		switch (_curDirection)
		{
		case Direction::UP:
			//if (_right)
			this->setTexture(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_back_%d.png", 2))->getTexture());
			//this->setFlipX(false);
			if (!_attack){
				if (_backDirection == Direction::DOWN)
				{
					this->_isPlay = false;
					this->culAnimationStop();
					this->stopAllActions();
				}
				this->play("UP");
			}
			//_attack_area->setRotation(-180);
			break;
		case Direction::DOWN:
			if (_backDirection == Direction::RIGHT)
				this->setFlipX(false);
			if (!_attack){
				if (_backDirection == Direction::UP)
				{
					this->_isPlay = false;
					this->culAnimationStop();
					this->stopAllActions();
				}
				this->setTexture(this->_player_animation_cache->getSpriteFrameByName(StringUtils::format("player_front_%d.png", 2))->getTexture());
				this->play("DOWN");
			}
			//_attack_area->setRotation(0);
			break;
		case Direction::LEFT:
			this->play("LEFT");
			this->setFlipX(false);

			//_attack_area->setRotation(-270);
			break;
		case Direction::RIGHT:
			this->play("LEFT");
			this->setFlipX(true);

			//_attack_area->setRotation(-90);
			break;
		case Direction::NORMAL:
			if (!_attack)
				this->culAnimationStop();
			break;
		}
	}
	// 이전방향에 현재 방향을 저장한다.
	_backDirection = _curDirection;
}

Direction Player::getDirection()
{
	return _curDirection;
}

void Player::setDirection()
{
	_curDirection = _attack_dir;
}

void Player::ItemUpdate()
{
	if (curItem != ItemID::NORMAL)
	{
		switch (curItem)
		{
			// 상태를 업그레이드하는 아이템
		case ItemID::HEALTH:
			log("HEALTH UPGRADE");
			Health += 30;
			if (Health >= 100)
				Health = 100;
			break;

		case ItemID::GUARD:
			log("GUARD UPGRADE");
			isGod = true;
			backup_Health_god = this->Health;
			this->setColor(Color3B::BLUE);
			break;

		case ItemID::SPEED:
			log("SPEED UPGADE");
			_velocity += 150.0f;
			this->runAction(Sequence::create(DelayTime::create(10.0f), CallFunc::create([&](){this->_velocity -= 150.0f; }), nullptr));
			break;

			// 공격 아이템을 먹었을때
		case ItemID::ARROW:
			backup_health_arrow = this->Health;
			_arrow_score += 10;
			if (_arrow_score >= 20)
				_arrow_score = 20;
			_arrow = true;
			break;

			// ------------------------------- [ Ctrl ]
		case ItemID::TRAP:
			_trap_score += 2;
			if (_trap_score >= 5)
				_trap_score = 5;
			break;

		case ItemID::WIRE:
			_wire_score += 2;
			if (_wire_score >= 5)
				_wire_score = 5;
			break;
		}

		curItem = ItemID::NORMAL;
	}
}

void Player::ArrowUpdate()
{
	if (arrow_vector.size())
	{
		for (auto it : arrow_vector)
		{
			switch (it->getTag())
			{
			case 1:
				it->setPosition(Vec2(it->getPosition().x, it->getPosition().y + 300.0f * deltaTime));
				break;
			case 2:
				it->setPosition(Vec2(it->getPosition().x + 300.0f* deltaTime, it->getPosition().y));
				break;
			case 3:
				it->setPosition(Vec2(it->getPosition().x, it->getPosition().y + -300.0f * deltaTime));
				break;
			case 4:
				it->setPosition(Vec2(it->getPosition().x + -300.0f* deltaTime, it->getPosition().y));
				break;
			}
		}
	}
}

Vec2 Player::PositionUpdate()
{
	if (!_attack && !_useItem)
	{
		if (!_stiff){
			Vec2 _curPosition = this->getPosition();

			if (_up)
				_curPosition.y = (_curPosition.y + _velocity * deltaTime);
			if (_right)
				_curPosition.x = (_curPosition.x + _velocity * deltaTime);
			if (_left)
				_curPosition.x = (_curPosition.x - _velocity * deltaTime);
			if (_down)
				_curPosition.y = (_curPosition.y - _velocity * deltaTime);

			return _curPosition;
		}
	}
}