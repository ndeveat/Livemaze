#pragma once
#include "pch.h"

class SearchCamera;
class Player;
class Mummy;
class Cat;
class Bug;
class Snake;
class Sphinx;
class Pharaoh;
class Item;
class PathFinding;

class MainMenu;
class GaemScene2; 
class GameScene : public LayerColor
{
private:
	bool god = false;;
	bool shild = false;

	PathFinding* pathFind;

	int TileMapArray[40][40];

	// ----------------------- [ Layer ]
	Layer* floorLayer;
	Layer* unitLayer;
	Layer* itemLayer;
	Layer* interfaceLayer;
	Layer* enemyLayer;

	// ----------------------- [ Player ]
	SearchCamera* search_camera;
	Player* player;

	// ----------------------- [ Tiled ]
	TMXTiledMap* tiled_map;
	TMXLayer   * tiled_layer_zorder;
	TMXLayer   * tiled_layer_metainfo;

	Sprite* gameover_spr;
	Sprite* restart;
	Sprite* exit;
public:
	static Scene* createScene()
	{
		auto scene = Scene::createWithPhysics();
		auto layer = GameScene::create();
		scene->addChild(layer);
		return scene;
	}

	bool onTouchBegan(Touch *touch, Event *unused_event) override;

    bool init()   override;
	// ������Ʈ
	void Update(float delta);
	// ���� ������ -> �÷��̾ ���� ���� ���ϰ� ����
	bool NextPosition(Node* target);
	bool NextPosition(Vec2 position);

	bool EnemyNextPosition(Vec2 position);
	// ���� ��ǥ�� Ÿ�ϸ��� ��ǥ�� ��ȯ�Ѵ�.
	Vec2 GetTileCoordinateWithNodeSpacePosition(Vec2 position);
	// �̴ϸ��� �׸�
	void ArraySaveTileMap();
	CREATE_FUNC(GameScene);

private:
	void CheateKeyUpdate();
	void InterfaceUpdate();
	void Player_Item_Update();
	void ArrowUpdate();
	void Enemy_AttackItem_Update();
	void SortUpdate();

	void EnemyUpdate(float delta);
};
