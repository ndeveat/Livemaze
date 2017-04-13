#pragma once
#include "pch.h"
#include "EnemyInterface.h"
#include "CatAnimation.h"

class PathFinding;
class Cat : public EnemyInterface, CatAnimation
{
private:
	bool target_move;
	bool target_move_search;
	float target_move_time;
	float attack_timer = 0.0f;
	float set_attack_timer = 0.0f;
	float stiff_time;
	PathFinding* pathFind;
	ActionInterval* moveAction;
public:
	static Cat* createEnemy(const string& fileName, Player* nTarget, Layer* lTarget)
	{
		SpriteFrameCache* cache = SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile(fileName);

		Cat* cat = new Cat(nTarget, lTarget, cache);
		if (cat&&cat->initWithFile("Enemy/Cat/cat_1.png"))
		{
			cat->Init();
			cat->InitWithEnemy();

			auto physicsBody = PhysicsBody::createBox(cat->getContentSize() *0.85f, PhysicsMaterial(1.0f, 1.0f, 0.5f));
			physicsBody->setDynamic(true);
			physicsBody->setGravityEnable(false);
			physicsBody->setRotationEnable(false);

			cat->setPhysicsBody(physicsBody);

			enemy_vector.push_back(cat);
		}

		return cat;
	}

	Cat(Player* nTarget, Layer* lTarget, SpriteFrameCache* cache)
		: EnemyInterface(nTarget, lTarget,EnemyID::CAT)
		, CatAnimation(this, cache)
	{
		target_move = true;
		target_move_search = false;
		target_move_time = 0.0f;
		stiff_time = 0.0f;
		moveAction = nullptr;
	}
	~Cat();
	void InitWithEnemy()	 override; 
	void Update(float delta) override;
	void setMap(int _map[][40]);
private:
	void AttackUpdate(float delta);
	void ArrowUpdate();


	Vec2 GetTileCoordinateWithNodeSpacePosition(Vec2 position)
	{
		Size ms = Size(40, 40);
		Size ts = Size(128,64) / CC_CONTENT_SCALE_FACTOR();

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