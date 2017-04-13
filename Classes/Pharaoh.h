#pragma once
#include "pch.h"
#include "EnemyInterface.h"
#include "PharaohAnimation.h"

class Snake;
class MainMenu;
class Pharaoh : public EnemyInterface,PharaohAnimation
{
private:
	Sprite* chest;
	TMXTiledMap* ownerTileMap;
	TMXLayer*    ownerTileLayer;

	bool  first_attack = false;
	float bug_fix =0.0f;
	float target_getAttack_time = 0.0f;

	float pharaoh_arrow_time = 0.0f;
	float pharaoh_snake_time = 0.0f;
	std::vector<Sprite*> pharaoh_vector;
public:
	static Pharaoh* createEnemy(const string& fileName, Player* nTarget, Layer* lTarget)
	{
		SpriteFrameCache* cache = SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile(fileName);

		Pharaoh* pharaoh = new Pharaoh(nTarget, lTarget, cache);
		if (pharaoh&&pharaoh->initWithFile("Enemy/Pharaoh/pharaoh_front_2.png"))
		{
			pharaoh->Init();
			pharaoh->InitWithEnemy();

			auto physicsBody = PhysicsBody::createBox(pharaoh->getContentSize() / 1.5, PhysicsMaterial(10, 10, 3));
			physicsBody->setDynamic(true);
			physicsBody->setGravityEnable(false);
			physicsBody->setRotationEnable(false);

			pharaoh->setPhysicsBody(physicsBody);
			enemy_vector.push_back(pharaoh);
		}

		return pharaoh;
	}

	Pharaoh(Player* nTarget, Layer* lTarget, SpriteFrameCache* cache)
		: EnemyInterface(nTarget, lTarget,EnemyID::PHARAOH)
		, PharaohAnimation(this, cache)
	{
		execute = true;
		bug_fix = 0.0f;
	}
	~Pharaoh();

	void InitWithEnemy()	 override;
	void Update(float delta) override;

	void setTMX(TMXTiledMap* tile, TMXLayer* layer)
	{
		ownerTileMap = tile;
		ownerTileLayer = layer;
	}

private:
	void AttackUpdate(float delta);
	void dgree_to_animation(float dgree);
	void FindTargetUpdate(float delta);

	Vec2 GetTileCoordinateWithNodeSpacePosition(Vec2 position)
	{
		Size ms(40,40);
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