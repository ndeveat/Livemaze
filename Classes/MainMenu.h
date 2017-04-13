#pragma once
#include "pch.h"
#include "Tutorial.h"
#include "GameScene.h"

class MainMenu : public Layer
{
private:
	Sprite* langking_spr;
	Sprite* tutorial_spr;
	Sprite* exit_spr;
public:
	static Scene* createScene()
	{
		auto scene = Scene::create();
		auto layer = MainMenu::create();
		scene->addChild(layer);

		return scene;
	}

	bool init() override
	{
		if (!Layer::init())
			return false;

		this->setTouchEnabled(true);
		this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);

		auto background = Sprite::create("MainMenu/background.png");
		background->setPosition(Vec2(WIDTH / 2, HEIGHT / 2));
		this->addChild(background);

		auto openMenu = MenuItemImage::create("MainMenu/open1.png", "MainMenu/open2.png",
			[&](Ref* sender)
		{
			Director::getInstance()->replaceScene(GameScene::createScene());
		});
		openMenu->setPosition(Vec2(WIDTH / 2 + 7, HEIGHT / 2 - 27));
		
		auto menu = Menu::create(openMenu, nullptr);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu);


		langking_spr = Sprite::create("MainMenu/lanking.png");
		langking_spr->setPosition(Vec2(349, 231));
		this->addChild(langking_spr);


		tutorial_spr = Sprite::create("MainMenu/tutorial.png");
		tutorial_spr->setPosition(Vec2(114, 82));
		this->addChild(tutorial_spr);

		exit_spr = Sprite::create("MainMenu/exit.png");
		exit_spr->setPosition(Vec2(1201, 578));
		this->addChild(exit_spr);

		return true;
	}

	bool onTouchBegan(Touch *touch, Event *unused_event) override
	{
		// log("X : %.f  Y : %.f", touch->getLocation().x, touch->getLocation().y);

		if (langking_spr->getBoundingBox().containsPoint(touch->getLocation()))
		{
			
		}
		else if (tutorial_spr->getBoundingBox().containsPoint(touch->getLocation()))
		{
			Director::getInstance()->pushScene(Tutorial::createScene());
		}
		else if (exit_spr->getBoundingBox().containsPoint(touch->getLocation()))
		{
			Director::getInstance()->end();
		}


		return true;
	}
	CREATE_FUNC(MainMenu);
};