#include "GameScene2.h"
#include "SearchCamera.h"
#include "Player.h"
#include "Mummy.h"
#include "Cat.h"
#include "Bug.h"
#include "Snake.h"
#include "Sphinx.h"
#include "Pharaoh.h"

#include "PathFind.h"
#include "GameScene.h"
#include "MainMenu.h"

bool GameScene2::init()
{
	if (!LayerColor::initWithColor(Color4B::BLACK))
		return false;

	ObserberTarget = nullptr;

	this->setTouchEnabled(true);
	this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);

	srand((unsigned)time(NULL));

	// ------------------------------------------------------------[ Layer Initialize ]
	floorLayer = Layer::create();
	this->addChild(floorLayer, 1, "FLOOR");
	itemLayer = Layer::create();
	this->addChild(itemLayer, 2, "ITEM");
	unitLayer = Layer::create();
	this->addChild(unitLayer, 3, "UNIT");
	interfaceLayer = Layer::create();
	this->addChild(interfaceLayer, 4, "INTERFACE");
	enemyLayer = Layer::create();
	this->addChild(enemyLayer, 5, "ENEMY");

	// ------------------------------------------------------------[ Tiled Map Setting ]
	tiled_map = TMXTiledMap::create("Floor/GameStage/GameStage2.tmx");
	tiled_map->setAnchorPoint(Vec2(0.0f, 1.0f));
	tiled_map->setPosition(Vec2(0.0f, HEIGHT * 4));
	tiled_map->setTag(4);
	floorLayer->addChild(tiled_map);

	auto tiled_layer_background = tiled_map->getLayer("Background");
	tiled_layer_background->setTag(4);

	auto tiled_layer_up = tiled_map->getLayer("down");
	tiled_layer_up->setZOrder(10);
	tiled_layer_up->setTag(4);

	auto tiled_layer_down = tiled_map->getLayer("up");
	tiled_layer_down->setZOrder(-10);
	tiled_layer_down->setTag(4);

	tiled_layer_zorder = tiled_map->getLayer("ZORDER");
	tiled_layer_zorder->setVisible(false);
	tiled_layer_zorder->setTag(4);

	tiled_layer_metainfo = tiled_map->getLayer("MetaInfo");
	tiled_layer_metainfo->setVisible(false);
	tiled_layer_metainfo->setTag(4);

	// ------------------------------------------------------------[ Player Initialize ]

	player = Player::createPlayer("Player/player.plist", Vec2(2570, 233), 250.0f, this);
	tiled_map->addChild(player, 1);

	// ------------------------------------------------------------ [ 미니맵의 플레이어 ]
	Sprite* player_tile = Sprite::create("Minimap/1.png");
	player_tile->setColor(Color3B::GREEN);
	player_tile->setName("PLAYER");
	interfaceLayer->addChild(player_tile, 2);

	interfaceLayer->setPosition(player->getPosition()); // --- [ 인터페이스에 부착 ]
	enemyLayer->setPosition(player->getPosition());

	// ------------------------------------------------------- [ 인터페이스의 시작 ]
	// 인터페이스 아이콘
	auto interface_health_icon = Sprite::create("Interface_health.png");
	interface_health_icon->setPosition(Vec2(-66, 121));
	interface_health_icon->setScale(0.4f);
	interfaceLayer->addChild(interface_health_icon);

	// 인터페이스 프로그래스 바
	auto interface_health_bar = ProgressTimer::create(Sprite::create("Health_bar.png"));
	interface_health_bar->setType(ProgressTimerType::BAR);
	interface_health_bar->setMidpoint(Vec2(0.0f, 0.5f));
	interface_health_bar->setPercentage(100.0f);
	interface_health_bar->setBarChangeRate(Vec2(1, 0));
	interface_health_bar->setName("HEALTH");
	interface_health_bar->setPosition(Vec2(9, 121));
	interface_health_bar->setScale(0.3f);
	interfaceLayer->addChild(interface_health_bar);

	auto first = Item::TRAP(Vec2(-66, 162));
	first->setName("TRAP");
	first->setScale(0.55f);
	interfaceLayer->addChild(first);

	auto first_label = Label::create("0", "Arial", 25);
	first_label->setPosition(Vec2(-33, 162));
	first_label->setColor(Color3B::WHITE);
	first_label->setName("FirstLabel");
	interfaceLayer->addChild(first_label);

	auto second = Item::WIRE(Vec2(0, 162));
	second->setName("WIRE");
	second->setScale(0.55f);
	interfaceLayer->addChild(second);

	auto second_label = Label::create("0", "Arial", 25);
	second_label->setPosition(Vec2(33, 162));
	second_label->setColor(Color3B::WHITE);
	second_label->setName("SecondLabel");
	interfaceLayer->addChild(second_label);

	auto arrow_label = Label::create("0", "Arial", 27);
	arrow_label->setPosition(second_label->getPosition() + Vec2(25, 0));
	arrow_label->setColor(Color3B::GREEN);
	arrow_label->setName("ArrowLabel");
	interfaceLayer->addChild(arrow_label);

	auto score_label = Label::create("Score : 0", "Arial", 35);
	score_label->setPosition(Vec2(-533, 280));
	score_label->setName("SCORE");
	interfaceLayer->addChild(score_label);

	gameover_spr = Sprite::create("game_over_background.png");
	gameover_spr->setPosition(Vec2(0, 0));
	gameover_spr->setVisible(false);
	interfaceLayer->addChild(gameover_spr);

	restart = Sprite::create("restart.png");
	restart->setPosition(Vec2(-136, -44));
	restart->setVisible(false);
	interfaceLayer->addChild(restart);

	exit = Sprite::create("exit.png");
	exit->setPosition(Vec2(136, -44));
	exit->setVisible(false);
	interfaceLayer->addChild(exit);

	//---------------------------------------------------- [ 인터페이스 끝 ]
	// 미니맵을 만든다.
	ArraySaveTileMap();

	pathFind = new PathFinding();
	pathFind->loadMap(this->TileMapArray);

	// ------------------------------------------------------- [ 적 셋팅 ]

	auto pharaoh = Pharaoh::createEnemy("Enemy/Pharaoh/pharaoh.plist", player, interfaceLayer);
	pharaoh->setPosition(Vec2(2564.16895, 2322.66211));
	pharaoh->setZOrder(2);
	pharaoh->setName("Sphinx");
	pharaoh->setTag(91);
	pharaoh->setTMX(this->tiled_map, this->tiled_layer_metainfo);
	tiled_map->addChild(pharaoh);

	auto trapItem2 = Item::TRAP(Vec2(3332, 1633));
	tiled_map->addChild(trapItem2);
	item_vector.push_back(trapItem2);


	auto trapItem3 = Item::TRAP(Vec2(4791, 1300));
	tiled_map->addChild(trapItem3);
	item_vector.push_back(trapItem3);


	auto trapItem4 = Item::TRAP(Vec2(965, 1532));
	tiled_map->addChild(trapItem4);
	item_vector.push_back(trapItem4);

	for (int i = 0; i < 5; i++)
	{
		auto mummy = Mummy::createEnemy("Enemy/Mummy/mummy.plist", player, unitLayer);
		mummy->setPosition(Vec2(1700 + random(1, 400), 1100 + random(1, 250)));
		mummy->setTMX(tiled_map, tiled_layer_metainfo);
		tiled_map->addChild(mummy,3);
	}

	for (int i = 0; i < 5; i++)
	{
		auto mummy = Mummy::createEnemy("Enemy/Mummy/mummy.plist", player, unitLayer);
		mummy->setPosition(Vec2(4700 + random(1, 200), 1260 + random(1, 200)));
		mummy->setTMX(tiled_map, tiled_layer_metainfo);
		tiled_map->addChild(mummy);
	}

	for (int i = 0; i < 5; i++){
		auto bugTest = Bug::createEnemy("Enemy/Bug/bug.plist", player, unitLayer);
		bugTest->setPosition(Vec2(772 + cocos2d::random(1, 20), 1183 + cocos2d::random(1, 20)));
		bugTest->setTMX(tiled_map, tiled_layer_metainfo);
		bugTest->back_pos = bugTest->getPosition();
		tiled_map->addChild(bugTest, 0);
	}

	for (int i = 0; i < 4; i++){
		auto bugTest = Bug::createEnemy("Enemy/Bug/bug.plist", player, unitLayer);
		bugTest->setPosition(Vec2(831 + cocos2d::random(1, 20), 1454 + cocos2d::random(1, 20)));
		bugTest->setTMX(tiled_map, tiled_layer_metainfo);
		bugTest->back_pos = bugTest->getPosition();
		tiled_map->addChild(bugTest, 0);
	}

	for (int i = 0; i < 4; i++){
		auto bugTest = Bug::createEnemy("Enemy/Bug/bug.plist", player, unitLayer);
		bugTest->setPosition(Vec2(4132 + cocos2d::random(1, 20), 1299 + cocos2d::random(1, 20)));
		bugTest->setTMX(tiled_map, tiled_layer_metainfo);
		bugTest->back_pos = bugTest->getPosition();
		tiled_map->addChild(bugTest, 1);
	}

	for (int i = 0; i < 4; i++){
		auto catTest = Cat::createEnemy("Enemy/Cat/cat.plist", player, unitLayer);
		catTest->setPosition(Vec2(1786 + cocos2d::random(1, 35), 603 + cocos2d::random(1, 35)));
		catTest->setMap(TileMapArray);
		tiled_map->addChild(catTest, 1);
	}


	for (int i = 0; i < 5; i++)
	{
		auto snake = Snake::createEnemy("Enemy/Snake/snake.plist", player, unitLayer);
		snake->setPosition(Vec2(4132 + random(1, 100), 1299 + random(1, 50)));
		snake->setTMX(tiled_map, tiled_layer_metainfo);
		tiled_map->addChild(snake);
	}
	log("Enemy Size : %d", enemy_vector.size());

	this->schedule(schedule_selector(GameScene2::Update));
	return true;
}

void GameScene2::Update(float delta)
{
	CheateKeyUpdate();			//------- [치트키를 업데이트 합니다 ]
	InterfaceUpdate();			//------- [인터페이스 & 플레이어 & 카메라 ]
	Player_Item_Update();		//------- [플레이어와 아이템 ]
	ArrowUpdate();				//------- [화살이 부딪치는지 확인한다 ]
	EnemyUpdate(delta);
	Enemy_AttackItem_Update();  //------- [Enemy와 공격형 아이템 ]
	SortUpdate();				//------- [Y값으로 그리기 순서를 정한다 ]

	//log("Enemy Size : %d", enemy_vector.size());
}

bool GameScene2::NextPosition(Node* target)
{
	Vec2 pos = GetTileCoordinateWithNodeSpacePosition(target->getPosition());
	if (pos == Vec2(-1, -1))
		return false;

	int tileGid = this->tiled_layer_metainfo->getTileGIDAt(pos);
	int zOrder = this->tiled_layer_zorder->getTileGIDAt(pos);

	if (zOrder)
	{
		Value& properties = tiled_map->getPropertiesForGID(zOrder);
		if (!properties.isNull())
		{
			std::string Zbuf = properties.asValueMap()["Zorder"].asString();

			if (Zbuf == "1")
			{
				target->setZOrder(0);
			}
			else if (Zbuf == "2")
			{
				target->setZOrder(2);
			}
		}
	}

	if (tileGid)
	{
		// 타일맵에서 GID에 해당하는 부분의 속성 읽어 오기
		Value& properties = tiled_map->getPropertiesForGID(tileGid);

		if (!properties.isNull())
		{
			std::string wall = properties.asValueMap()["Wall"].asString();

			if (wall == "Yes")
			{
				// 벽과의 충돌
				// log("Wall!!!!!");
				return false;
			}
		}
	}

	return true;
}

bool GameScene2::EnemyNextPosition(Vec2 position)
{
	Vec2 pos = GetTileCoordinateWithNodeSpacePosition(position);
	if (pos == Vec2(-1, -1))
		return false;

	int tileGid = this->tiled_layer_metainfo->getTileGIDAt(pos);

	if (tileGid)
	{
		// 타일맵에서 GID에 해당하는 부분의 속성 읽어 오기
		Value& properties = tiled_map->getPropertiesForGID(tileGid);

		if (!properties.isNull())
		{
			std::string wall = properties.asValueMap()["Wall"].asString();

			if (wall == "Yes")
			{
				// 벽과의 충돌
				// log("Wall!!!!!");
				return false;
			}
		}
	}

	return true;
}

bool GameScene2::NextPosition(Vec2 position)
{
	Vec2 pos = GetTileCoordinateWithNodeSpacePosition(position);
	if (pos == Vec2(-1, -1))
		return false;

	int tileGid = this->tiled_layer_metainfo->getTileGIDAt(pos);
	int zOrder = this->tiled_layer_zorder->getTileGIDAt(pos);

	if (zOrder)
	{
		Value& properties = tiled_map->getPropertiesForGID(zOrder);
		if (!properties.isNull())
		{
			std::string Zbuf = properties.asValueMap()["Zorder"].asString();

			if (Zbuf == "1")
			{
				player->setZOrder(0);
			}
			else if (Zbuf == "2")
			{
				player->setZOrder(2);
			}
		}
	}

	if (tileGid)
	{
		// 타일맵에서 GID에 해당하는 부분의 속성 읽어 오기
		Value& properties = tiled_map->getPropertiesForGID(tileGid);

		if (!properties.isNull())
		{
			std::string wall = properties.asValueMap()["Wall"].asString();

			if (wall == "Yes")
			{
				// 벽과의 충돌
				// log("Wall!!!!!");
				return false;
			}
		}
	}

	return true;
}

Vec2 GameScene2::GetTileCoordinateWithNodeSpacePosition(Vec2 position)
{
	Size ms = tiled_map->getMapSize();
	Size ts = tiled_map->getTileSize() / CC_CONTENT_SCALE_FACTOR();

	float halfMapWidth = ms.width * 0.5f;
	Vec2 tilePosDiv = Vec2(position.x / ts.width, position.y / ts.height);
	float invereseTileY = ms.height - tilePosDiv.y;
	float posX = (int)(invereseTileY + tilePosDiv.x - halfMapWidth);
	float posY = (int)(invereseTileY - tilePosDiv.x + halfMapWidth);

	if (0 <= posX && posX < ms.width && 0 <= posY && posY < ms.height)
		return Vec2(posX, posY);

	return Vec2(-1, -1);
}

bool GameScene2::onTouchBegan(Touch *touch, Event *unused_even)
{
	Vec2 convert = interfaceLayer->convertToNodeSpace(touch->getLocation());
	log("X : %.f  Y : %.f", convert.x, convert.y);

	if (restart->isVisible())
	{
		if (restart->getBoundingBox().containsPoint(interfaceLayer->convertToNodeSpace(touch->getLocation())))
		{
			arrow_vector.clear();
			attack_item_vector.clear();
			enemy_vector.clear();
			item_vector.clear();
			ObserberTarget = nullptr;
			Director::getInstance()->replaceScene(GameScene2::createScene());
			//log("OUT");
		}
	}

	if (exit->isVisible())
	{
		if (exit->getBoundingBox().containsPoint(interfaceLayer->convertToNodeSpace(touch->getLocation())))
		{
			arrow_vector.clear();
			attack_item_vector.clear();
			enemy_vector.clear();
			item_vector.clear();
			ObserberTarget = nullptr;
			Director::getInstance()->replaceScene(MainMenu::createScene());
			//log("OUT");
		}
	}


	return true;
}

void GameScene2::ArraySaveTileMap()
{
	for (int yy = 0; yy < 40; yy++)
	{
		for (int xx = 0; xx < 40; xx++)
		{
			int tempid = tiled_layer_metainfo->getTileGIDAt(Vec2(xx, yy));
			TileMapArray[xx][yy] = tempid;
			log("%d", tempid);
		}
	}

	for (int y = 0; y < 40; y++){
		for (int x = 0; x < 40; x++)
		{
			Sprite* wall = Sprite::create();
			if (TileMapArray[x][y] != 0)
			{
				wall->setTexture("Minimap/2.png");
			}
			else
			{
				wall->setTexture("Minimap/1.png");
			}
			float xpos = (x - y) * 5 + (WIDTH / 3.0);
			float ypos = (HEIGHT / 2 - 15) - (x + y) * 2.5f - 2.5f;
			wall->setPosition(Vec2(xpos, ypos));
			interfaceLayer->addChild(wall);
		}
	}
}

void GameScene2::CheateKeyUpdate()
{
	if (GetAsyncKeyState(VK_F1))
	{
		god = true;
	}

	if (god)
		player->Health = 100;

	// 초기화 버튼
	if (GetAsyncKeyState(VK_F2) & 0x8000)
	{
		shild = true;
	}
	if (shild)
		player->curItem = Item::GUARD(player->getPosition())->itemID;

	if (GetAsyncKeyState(VK_F3) & 0x8000)
	{
	EEE:
		for (auto it : enemy_vector)
		{
			if (it->getID() == EnemyID::PHARAOH || it->getID() == EnemyID::SPHINX)
				log("Pass");
			else
			{
				it->removeFromParentAndCleanup(true);
				enemy_vector.erase(find(enemy_vector.begin(), enemy_vector.end(), it));
				goto EEE;
			}
		}
	}

	if (GetAsyncKeyState(VK_F4) & 0x8000)
	{
		arrow_vector.clear();
		attack_item_vector.clear();
		enemy_vector.clear();
		item_vector.clear();

		ObserberTarget = nullptr;
		Director::getInstance()->replaceScene(GameScene::createScene());
	}

	if (GetAsyncKeyState(VK_F5) & 0x8000)
	{
		arrow_vector.clear();
		attack_item_vector.clear();
		enemy_vector.clear();
		item_vector.clear();

		ObserberTarget = nullptr;
		Director::getInstance()->replaceScene(GameScene2::createScene());
	}

	if (GetAsyncKeyState(VK_F6) & 0x8000)
	{
		arrow_vector.clear();
		attack_item_vector.clear();
		enemy_vector.clear();
		item_vector.clear();
		ObserberTarget = nullptr;
		Director::getInstance()->replaceScene(MainMenu::createScene());
	}
}

void GameScene2::Player_Item_Update()
{
	// ----------------------- [ 아이템 업데이트 ]
	if (item_vector.size())
	{
		for (auto item : item_vector)
		{
			// -------------- [ 플레이어가 아이템을 먹었을때 ]
			Rect player_boudingBox = Rect(player->getPosition().x - player->getContentSize().width / 2, player->getPosition().y, player->getContentSize().width, player->getContentSize().height / 2 - 25);
			if (item->getBoundingBox().intersectsRect(player_boudingBox))
			{
				player->curItem = item->itemID;

				itemLayer->removeChild(item);
				tiled_map->removeChild(item);

				item_vector.erase(find(item_vector.begin(), item_vector.end(), item));
				return;
			}
		}
	}

	// --------------------------- [ 아이템과 플레이어가 초반에 겹치지 않게 한다. ]
	if (attack_item_vector.size()){
		for (auto trap : attack_item_vector)
		{
			Rect player_bounding = Rect(player->getPosition().x - player->getContentSize().width / 2, player->getPosition().y, player->getContentSize().width, player->getContentSize().height / 2 - 20);
			if (trap->getBoundingBox().intersectsRect(player_bounding))
			{
				if (trap->getTag() == 1)
				{
					if (player->_item_stiff)
						return;
					// log("STIFF");

					player->_stiff = true;

					player->runAction(Sequence::create(DelayTime::create(1.5f),
						CallFuncN::create([&](Node* target)
					{
						static_cast<Player*>(target)->_stiff = false;
					}), nullptr));
					return;
				}
			}
		}
	}
}

void GameScene2::Enemy_AttackItem_Update()
{
	// ----------------------- [ 공격형 아이템을 밟았을때 ]
	if (attack_item_vector.size()){
		for (auto attack_item : attack_item_vector)
		{
			for (auto enemy : enemy_vector)
			{
				Rect item_bounding = Rect(attack_item->getPosition().x - 5, attack_item->getPosition().y - 5, 10, 10);
				//  [ 아이템과 적이 부딪쳤을때 ]
				if (item_bounding.intersectsRect(enemy->getBoundingBox()))
				{
					Sprite* ef = Sprite::create("ef1.png");
					ef->setPosition(attack_item->getPosition());
					ef->runAction(Sequence::create(CallFuncN::create([&](Node* node){static_cast<Sprite*>(node)->setTexture("ef2.png"); }), FadeOut::create(0.3f), RemoveSelf::create(), nullptr));
					attack_item->getParent()->addChild(ef);

					//  -------------------------------------------------- [ WIRE ]
					if (attack_item->getTag() == 2)
					{
						if (enemy->getID() == EnemyID::MUMMY || enemy->getID() == EnemyID::CAT)
						{
							attack_item->setTexture("Wire2.png");
							attack_item->runAction(Sequence::create(
								FadeOut::create(0.5f),
								RemoveSelf::create(), nullptr));

							attack_item_vector.erase(find(attack_item_vector.begin(), attack_item_vector.end(), attack_item));

							// 그 상태로 고정 시킨다.
							enemy->onStiff();
						}
					}
					// --------------------------------------------------- [ TRAP ]
					else if (attack_item->getTag() == 1)
					{
						attack_item->setTexture("Trap2.png");
						attack_item->runAction(Sequence::create(
							FadeOut::create(0.5f),
							RemoveSelf::create(), nullptr));
						attack_item_vector.erase(find(attack_item_vector.begin(), attack_item_vector.end(), attack_item));
						enemy->onStiff();
						enemy->runAction(Sequence::create(
							Repeat::create(
							Sequence::create(TintTo::create(0.05, Color3B::RED), TintTo::create(0.05f, Color3B::WHITE),
							nullptr), 2), DelayTime::create(2.0f), CallFuncN::create([&](Node* __enemy){static_cast<EnemyInterface*>(__enemy)->offStiff(); }), nullptr));

						enemy->setHealth(enemy->getHealth() - 15);
					}
					return;
				}
			}
		}
	}
}

void GameScene2::ArrowUpdate()
{
	// -------------------------------------------- [ 화살 업데이트 ]
	if (arrow_vector.size())
	{
		for (auto it : arrow_vector)
		{
			if (!NextPosition(it))
			{
				tiled_map->removeChild(it);
				arrow_vector.erase(find(arrow_vector.begin(), arrow_vector.end(), it));
				return;
			}
		}
	}
}

void GameScene2::InterfaceUpdate()
{
	// 플레이어 업데이트
	Vec2 player_position = player->PositionUpdate();
	// ------------------------------------------------------------[ player can't deflect on screen]
	if (player_position.x - player->getContentSize().width / 2 <= 0)
		player_position.x = player->getContentSize().width / 2;
	if (player_position.y - player->getContentSize().height / 2 <= 0)
		player_position.y = player->getContentSize().height / 2;
	if (player_position.x + player->getContentSize().width / 2 >= WIDTH * 4)
		player_position.x = WIDTH * 4 - player->getContentSize().width / 2;
	if (player_position.y + player->getContentSize().height / 2 >= HEIGHT * 4)
		player_position.y = HEIGHT * 4 - player->getContentSize().height / 2;

	// ------------------------------------------------------------[ Player Tile Pos ]
	if (NextPosition(player_position))
	{
		// ---- // 미니맵의 플레이어를 업데이트 한다.
		Vec2 pos = this->GetTileCoordinateWithNodeSpacePosition(player_position);

		float xpos = (pos.x - pos.y) * 5 + (WIDTH / 3.0f);
		float ypos = (HEIGHT / 2 - 15) - (pos.x + pos.y) * 2.5f - 2.5f;
		interfaceLayer->getChildByName("PLAYER")->setPosition(Vec2(xpos, ypos));

		player->setPosition(player_position);
	}

	// ---------------------- [ 화면이 플레이어를 가리킨다 ]
	Vec2 mapPos;
	mapPos.x = player->getPosition().x - this->getContentSize().width / 2;
	mapPos.y = player->getPosition().y - this->getContentSize().height / 2;
	this->setPosition(-mapPos);

	// ----------------------------------------------- [ 인터페이스 업데이트 ] ---- [ 플레이어의 정보를 받아온다 ]
	static_cast<Label*>(interfaceLayer->getChildByName("FirstLabel"))->setString(StringUtils::format("%d", player->_trap_score));
	static_cast<Label*>(interfaceLayer->getChildByName("SecondLabel"))->setString(StringUtils::format("%d", player->_wire_score));
	static_cast<Label*>(interfaceLayer->getChildByName("ArrowLabel"))->setString(StringUtils::format("%d", player->_arrow_score));


	static_cast<ProgressTimer*>(interfaceLayer->getChildByName("HEALTH"))->setPercentage(player->Health);
	static_cast<ProgressTimer*>(interfaceLayer->getChildByTag(90)->getChildByTag(91))->setPercentage(static_cast<Sphinx*>(tiled_map->getChildByName("Sphinx"))->getHealth());
	interfaceLayer->setPosition(player->getPosition());

	if (player->Health <= 0)
	{
		gameover_spr->setVisible(true);
		restart->setVisible(true);
		exit->setVisible(true);
	}

	static_cast<Label*>(interfaceLayer->getChildByName("SCORE"))->setString(StringUtils::format("Score : %d", player->_score));
	//------------------------------------------------
}

void GameScene2::SortUpdate()
{
	std::sort(itemLayer->getChildren().begin(), itemLayer->getChildren().end(),
		[](Node *a, Node *b)
	{
		return a->getPositionY() > b->getPositionY();
	});

	std::sort(unitLayer->getChildren().begin(), unitLayer->getChildren().end(),
		[](Node *a, Node *b)
	{
		return a->getPositionY() > b->getPositionY();
	});
}

void GameScene2::EnemyUpdate(float delta)
{
	enemyLayer->removeAllChildren();

	for (auto it : enemy_vector)
	{
		// ---- // 미니맵의 플레이어를 업데이트 한다.
		Vec2 pos = this->GetTileCoordinateWithNodeSpacePosition(it->getPosition());

		auto Wall = Sprite::create("Minimap/1.png");
		if (it->getID() == EnemyID::PHARAOH || it->getID() == EnemyID::SPHINX)
			Wall->setColor(Color3B::MAGENTA);
		else
			Wall->setColor(Color3B::BLUE);
		float xpos = (pos.x - pos.y) * 5 + (WIDTH / 3.0f);
		float ypos = (HEIGHT / 2 - 15) - (pos.x + pos.y) * 2.5f - 2.5f;
		Wall->setPosition(Vec2(xpos, ypos));
		enemyLayer->addChild(Wall);
	}

	enemyLayer->setPosition(player->getPosition());
}