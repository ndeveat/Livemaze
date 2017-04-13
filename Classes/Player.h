#pragma once
#include "pch.h"
#include "Item.h"
#include "PlayerAnimation.h"

class Player : public Sprite , PlayerAnimation
{
private:
	// 아이템의 상태
	bool isGod;
	int  backup_Health_god;

	// 활성화
	bool execute;

	Layer*		_ownerNode;		// 캐릭터의 주인 = 레이어
	float		_velocity;		// 속력
	Direction	_curDirection;	// 방향 -> 기본적으로 방향은 DOWN이다.
	Direction	_backDirection; // 이전의 방향을 나타낸다.
	Direction   _attack_dir;	// 공격을 위한 방향, Normal은 포함되지 않는다.

	// 방향키
	bool _up;
	bool _down;
	bool _right;
	bool _left;


	// 공격
	bool _attack;
	int	 _arrow_number_of_use;
	// 아이템 사용
	bool _useItem;
	// 플레이어 전체의 dt시간
	float deltaTime;
public:
	int _score = 0;

	bool _item_stiff; // 아이템 설치후 몇초후에 활성화
	bool _stiff; // 경직
	float _stiff_tiem = 0.0f;

	int _trap_score;
	int _wire_score;
	int _arrow_score;
	// 화살을 사용할때
	bool _arrow;
	int  backup_health_arrow;
	// 방금 먹은 아이템
	ItemID curItem;
	int	Health;	// 체력 -> 100
	// 생성시 ( 스프라이트 시트 경로, 포지션, 속도)
	static Player* createPlayer(const string& cache_filePath,Vec2 playerPosition,float Velocity,Layer* ownerLayer)
	{
		SpriteFrameCache* cache_file = SpriteFrameCache::getInstance();
		cache_file->addSpriteFramesWithFile(cache_filePath);

		Player* player = new Player(Velocity, cache_file, ownerLayer);
		if (player&&player->initWithFile("Player/player_front_2.png"))
		{
			// 애니메이션 Init
			player->Init();
			
			// 플레이어 
			player->setPosition(playerPosition);
			player->InitWithPlayer();
			player->autorelease();

			auto physicsBody = PhysicsBody::createBox(player->getContentSize() * 0.7f, PhysicsMaterial(20, 20, 10));
			physicsBody->setDynamic(true);
			physicsBody->setGravityEnable(false);
			physicsBody->setRotationEnable(false);

			player->setPhysicsBody(physicsBody);
		}
		return player;
	}
	Player(float speed, SpriteFrameCache* cache, Layer* ownerLayer)
		: _velocity(speed)
		, execute(true)
		// 플레이어 애니메이션에게 자신과, 스프라이트 시트가 든 애니메이션 리스트를 준다.
		, PlayerAnimation(this, cache)
		, _attack(false)
		, _useItem(false)
		, _ownerNode(ownerLayer)
		, isGod(false)
		, Health(100)
		, _arrow(false)
		, _trap_score(0)
		, _wire_score(0)
		, _arrow_score(0)
		, _stiff(false)
		, _item_stiff(false)
		, _arrow_number_of_use(0)
		, curItem(ItemID::NORMAL)
	{
		_curDirection = { Direction::NORMAL };
		_backDirection = { Direction::NORMAL };
	};
	~Player();

public:
	bool InitWithPlayer();
	void PlayerUpdate(float);

	Direction getDirection();
	void setDirection();
private:
	void KeyUpdate();
	void DirectionUpdate();
	
	void ItemUpdate();
	void ArrowUpdate();

	void RudderKeyUpdate();
	void AttackKeyUpdate();
	void ItemKeyUpdate();
public:
	Vec2 PositionUpdate();
};