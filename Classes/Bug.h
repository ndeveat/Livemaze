#pragma once
#include "pch.h"
#include "EnemyInterface.h"
#include "BugAnimation.h"

class Bug : public EnemyInterface, BugAnimation
{
private:
	float attack_timer;
	float set_attack_timer;

	float time = 0.0f;
	float attack_time = 0.0f;
	float _time ;


	TMXTiledMap* ownerTileMap;
	TMXLayer*    ownerTileLayer;
public:
	Vec2  back_pos;
	float back_time = 0.0f;
	static Bug* createEnemy(const string& fileName, Player* nTarget, Layer* lTarget)
	{
		SpriteFrameCache* cache = SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile(fileName);

		Bug* bug = new Bug(nTarget, lTarget, cache);
		if (bug&&bug->initWithFile("Enemy/Bug/left_2.png"))
		{
			bug->Init();
			bug->InitWithEnemy();

			enemy_vector.push_back(bug);
		}

		return bug;
	}

	Bug(Player* nTarget, Layer* lTarget, SpriteFrameCache* cache)
		: EnemyInterface(nTarget, lTarget, EnemyID::BUG)
		, BugAnimation(this, cache)
	{
		attack_timer = 0.0f;
		set_attack_timer = 0.0f;
		_time = 0.0f;
	}
	~Bug();
	void InitWithEnemy()	 override;
	void Update(float delta) override;
	void rand_to_animation(int num);
	void SearchingTargetUpdate(float delta);
	void AttackUpdate(float delta);

	void MovingUpdate(float delta);

	void setTMX(TMXTiledMap* tile, TMXLayer* layer)
	{
		ownerTileLayer = layer;
		ownerTileMap = tile;
	}

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