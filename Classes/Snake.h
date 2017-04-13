#pragma once
#include "EnemyInterface.h"
#include "SnakeAnimation.h"

class Snake : public EnemyInterface, SnakeAnimation
{
private:
	float snake_velocity;

	bool target_getAttack;
	bool target_setAttack;

	float target_getAttack_time =0;
	float target_setAttack_time =0;

	TMXLayer* ownerLayer;
	TMXTiledMap* ownerTile;
public:
	static Snake* createEnemy(const string& fileName, Player* nTarget, Layer* lTarget)
	{
		SpriteFrameCache* cache = SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile(fileName);

		Snake* snake = new Snake(nTarget,lTarget,cache);
		if (snake&&snake->initWithFile("Enemy/Snake/snake1.png"))
		{
			snake->Init();
			snake->InitWithEnemy();

			auto physicsBody = PhysicsBody::createBox(snake->getContentSize(), PhysicsMaterial(0.1f, 1.0f, 0.0f));
			physicsBody->setDynamic(true);
			physicsBody->setGravityEnable(false);
			physicsBody->setRotationEnable(false);

			snake->setPhysicsBody(physicsBody);
			enemy_vector.push_back(snake);
		}

		return snake;
	}

	Snake(Player* nTarget, Layer* lTarget, SpriteFrameCache* cache)
		: EnemyInterface(nTarget, lTarget,EnemyID::SNAKE)
		, SnakeAnimation(this, cache)
	{
		snake_velocity = cocos2d::random(20, 30);
		target_getAttack = false;
		target_setAttack = false;
	}
	~Snake();

	void InitWithEnemy()     override;
	void Update(float delta) override;

	void setTMX(TMXTiledMap* tile, TMXLayer* layer)
	{
		ownerTile = tile;
		ownerLayer = layer;
	}
private:
	void AttackUpdate(float delta);
	Vec2 GetTileCoordinateWithNodeSpacePosition(Vec2 position)
	{
		Size ms(40, 40);
		Size ts = Size(128, 64) / CC_CONTENT_SCALE_FACTOR();

		float halfMapWidth = ms.width * 0.5f;
		Vec2 tilePosDiv = Vec2(position.x / ts.width, position.y / ts.height);
		float invereseTileY = ms.height - tilePosDiv.y;
		float posX = (int)(invereseTileY + tilePosDiv.x - halfMapWidth);
		float posY = (int)(invereseTileY - tilePosDiv.x + halfMapWidth);

		if (0 <= posX && posX < ms.width && 0 <= posY && posY < ms.height)
			return Vec2(posX, posY);

		return Vec2(-1, -1);
	}
};