#pragma once
#include "pch.h"
#include "EnemyInterface.h"
#include "SphinxAnimation.h"

class Mummy;
class GameScene2;
class Sphinx : public EnemyInterface, SphinxAnimation
{
private:
	float snake_time;
	float create_time ;
	float get_attack_time;
	Rect fire_boundingBox;
	bool first_attack = false;

	Sprite* key;
public:
	static Sphinx* createEnemy(const string& fileName, Player* nTarget, Layer* lTarget)
	{
		SpriteFrameCache* cache = SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile(fileName);

		Sphinx* sphinx = new Sphinx(nTarget, lTarget, cache); 
		if (sphinx&&sphinx->initWithFile("Enemy/Sphinx/sphinx_1.png"))
		{
			sphinx->Init();
			sphinx->InitWithEnemy();
			sphinx->setAnchorPoint(Vec2(0.5f, 0.0f));

			auto physicsBody = PhysicsBody::createBox(sphinx->getContentSize() *0.95f, PhysicsMaterial(250.1f, 250.0f, 250.0f));
			physicsBody->setDynamic(true);
			physicsBody->setGravityEnable(false);
			physicsBody->setRotationEnable(false);

			sphinx->setPhysicsBody(physicsBody);
			enemy_vector.push_back(sphinx);
		}

		return sphinx;
	}

	Sphinx(Player* nTarget, Layer* lTarget, SpriteFrameCache* cache)
		: EnemyInterface(nTarget, lTarget,EnemyID::SPHINX)
		, SphinxAnimation(this, cache)
	{
		snake_time = 0.0f;
		create_time = 0.0f;
		get_attack_time = 0.0f;
	}
	~Sphinx();

	void InitWithEnemy()	 override;
	void Update(float delta) override;

private:
	void AttackUpdate(float delta);
};