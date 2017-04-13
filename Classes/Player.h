#pragma once
#include "pch.h"
#include "Item.h"
#include "PlayerAnimation.h"

class Player : public Sprite , PlayerAnimation
{
private:
	// �������� ����
	bool isGod;
	int  backup_Health_god;

	// Ȱ��ȭ
	bool execute;

	Layer*		_ownerNode;		// ĳ������ ���� = ���̾�
	float		_velocity;		// �ӷ�
	Direction	_curDirection;	// ���� -> �⺻������ ������ DOWN�̴�.
	Direction	_backDirection; // ������ ������ ��Ÿ����.
	Direction   _attack_dir;	// ������ ���� ����, Normal�� ���Ե��� �ʴ´�.

	// ����Ű
	bool _up;
	bool _down;
	bool _right;
	bool _left;


	// ����
	bool _attack;
	int	 _arrow_number_of_use;
	// ������ ���
	bool _useItem;
	// �÷��̾� ��ü�� dt�ð�
	float deltaTime;
public:
	int _score = 0;

	bool _item_stiff; // ������ ��ġ�� �����Ŀ� Ȱ��ȭ
	bool _stiff; // ����
	float _stiff_tiem = 0.0f;

	int _trap_score;
	int _wire_score;
	int _arrow_score;
	// ȭ���� ����Ҷ�
	bool _arrow;
	int  backup_health_arrow;
	// ��� ���� ������
	ItemID curItem;
	int	Health;	// ü�� -> 100
	// ������ ( ��������Ʈ ��Ʈ ���, ������, �ӵ�)
	static Player* createPlayer(const string& cache_filePath,Vec2 playerPosition,float Velocity,Layer* ownerLayer)
	{
		SpriteFrameCache* cache_file = SpriteFrameCache::getInstance();
		cache_file->addSpriteFramesWithFile(cache_filePath);

		Player* player = new Player(Velocity, cache_file, ownerLayer);
		if (player&&player->initWithFile("Player/player_front_2.png"))
		{
			// �ִϸ��̼� Init
			player->Init();
			
			// �÷��̾� 
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
		// �÷��̾� �ִϸ��̼ǿ��� �ڽŰ�, ��������Ʈ ��Ʈ�� �� �ִϸ��̼� ����Ʈ�� �ش�.
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