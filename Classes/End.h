#pragma once
#include "pch.h"
#include "MainMenu.h"

class End : public Layer
{
private:
public:
	static Scene* createScene()
	{
		auto scene = Scene::create();
		auto layer = End::create();
		scene->addChild(layer);

		return scene;
	}

	bool init() override
	{
		if (!Layer::init())
			return false;

		auto label = Label::create("THE END", "Arial", 70);
		label->setPosition(Vec2(WIDTH / 2, HEIGHT / 2));
		this->addChild(label);

		label->runAction(Sequence::create(DelayTime::create(5.0f), CallFunc::create([&](){Director::getInstance()->replaceScene(MainMenu::createScene()); }), nullptr));
		return true;
	}

	CREATE_FUNC(End);
};