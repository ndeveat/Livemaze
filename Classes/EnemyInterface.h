#pragma once
#include "pch.h"
#include "Player.h"

class EnemyInterface : public Sprite
{
private:
protected:
	std::vector<Vec2> target_load;

	EnemyID id; // ID

	int Health; // 초기화시 무조건 100

	Layer* _ownerLayer;
	Player* _target;
	Direction _curDirection;
	Direction _attackDirection;
	
	bool execute;
	bool find_target;

	bool target_getAttack;
	bool target_setAttack;

	bool stiff;
public:
	EnemyInterface(Player* target, Layer* ownerLayer,EnemyID _id)
		: execute(true)
		, Health(100)
		, find_target(false)
		, _target(target)
		, id(_id)
		, _ownerLayer(ownerLayer)
		, _curDirection(Direction::NORMAL)
		,_attackDirection(Direction::DOWN)
		, target_getAttack(false)
		, stiff(false)
		, target_setAttack(false){}
	virtual ~EnemyInterface(){}
	virtual void InitWithEnemy() = 0;
	virtual void Update(float delta)=0;

	void onStiff(){ stiff = true; }
	void offStiff(){ stiff = false; }

	void setHealth(int _health){ Health = _health; }
	int  getHealth(){ return Health; }

	EnemyID getID(){ return id; }

	void setTargetLoad(std::vector<Vec2>& _vload){ target_load.clear(); target_load = _vload; }
	std::vector<Vec2> getTargetLoad(){ return target_load; }
};