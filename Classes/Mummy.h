#pragma once
#include "pch.h"
#include "EnemyInterface.h"
#include "MummyAnimation.h"

class Mummy : public EnemyInterface, MummyAnimation
{
private:
	float _time ;
	float attack_time ;
	float attack_getTime;
	float mmvelocity;

	bool noTMX = false;

	TMXTiledMap* ownerTileMap;
	TMXLayer*    ownerTileLayer;
public:
	static Mummy* createEnemy(const string& fileName, Player* nTarget, Layer* lTarget)
	{
		SpriteFrameCache* cache = SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile(fileName);

		Mummy* mummy = new Mummy(nTarget,lTarget,cache);
		if (mummy&&mummy->initWithFile("Enemy/Mummy/mummy_down_2.png"))
		{
			mummy->Init();
			mummy->InitWithEnemy();

			auto physicsBody = PhysicsBody::createBox(mummy->getContentSize()/2, PhysicsMaterial(1.0f, 1.0f, 0.5f));
			physicsBody->setDynamic(true);
			physicsBody->setGravityEnable(false);
			physicsBody->setRotationEnable(false);

			mummy->setPhysicsBody(physicsBody);
			enemy_vector.push_back(mummy);
		}

		return mummy;
	}

	Mummy(Player* nTarget, Layer* lTarget, SpriteFrameCache* cache)
		: EnemyInterface(nTarget,lTarget,EnemyID::MUMMY)
		, MummyAnimation(this, cache)
	{
		mmvelocity = 10.0f;
		_time = 0.0f;
		attack_time = 0.0f;
		attack_getTime = 0.0f;
	}
	~Mummy();
	
	void Update(float delta) override;

	void FindTargetUpdate(float delta);
	void SearchingTargetUpdate(float delta);
	void AttackUpdate(float delta);

	void setTMX(TMXTiledMap* tile, TMXLayer* layer)
	{
		ownerTileLayer = layer;
		ownerTileMap = tile;
	}

	void FIND()
	{
		mmvelocity = 15.0f;
		find_target = true;
		noTMX = true;
		//this->_isPlay = false;
		//this->culAnimationStop();
	}
private:
	void InitWithEnemy()	 override;
	void dgree_to_animation(float dgree);
	void rand_to_animation(int);

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