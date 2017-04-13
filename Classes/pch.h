#pragma once

#include "cocos2d.h"
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <time.h>
#include <random>
#include <map>
#include <chrono>

using namespace cocos2d;
using namespace std;

#define WIDTH 1280
#define HEIGHT 640
#define WINDOW_RATIO 1.0

extern Node* ObserberTarget;	// 플레이어

class Item;
class EnemyInterface;
extern std::vector<Item*>		item_vector;
extern std::vector<Sprite*>		arrow_vector;
extern std::vector<EnemyInterface*> enemy_vector;
extern std::vector<Sprite*>		attack_item_vector;

enum class Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	NORMAL,
	
	ATTACK,
};

enum class ItemID
{
	ARROW=1,
	WIRE,
	TRAP,
	HEALTH,
	SPEED,
	GUARD,
	NORMAL,
};

enum class EnemyID
{
	MUMMY,
	CAT,
	PHARAOH,
	SPHINX,
	SNAKE,
	BUG
};

#define ROW_SIZE 40
#define COL_SIZE 40
#define	DIRECTION 8
