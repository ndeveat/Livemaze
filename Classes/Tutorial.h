#pragma once

#include "pch.h"

class Tutorial : public Layer
{
private:
public:
	static Scene* createScene()
	{
		auto scene = Scene::create();
		auto layer = Tutorial::create();
		scene->addChild(layer);

		return scene;
	}


	bool init() override
	{
		if (!Layer::init())
			return false;
		auto tutorial_label = Label::create("조작 키\n\nX : 공격\nCtrl : 아이템 1\nShift : 아이템2\n\n뱀 : 한번 들키면 모든 뱀들이 당신에게 달려들 것입니다. - 스피드\n고양이 : 당신이 어디에 있든지 계속해서 쫓아올 것입니다.-활\n미라 : 속도는 느리지만 많이 공격해야 할 것입니다.-체력\n곤충 : 단단하고 많은 양의 피해를 줄 것입니다.-밧줄\n스핑크스 : 거대한 불을 당신에게 뿜을 것이고 미라를 소환합니다.\n파라오 : 강력한 힘과 체력으로 당신을 곤경에 빠뜨리게 할 것입니다.", "Arial", 40);
		tutorial_label->setPosition(Vec2(WIDTH / 2, HEIGHT / 2));
		tutorial_label->setColor(Color3B::WHITE);
		this->addChild(tutorial_label);

		this->schedule(schedule_selector(Tutorial::Update));
		return true;
	}

	void Update(float delta)
	{
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			Director::getInstance()->popScene();
			this->unschedule(schedule_selector(Tutorial::Update));
		}
	}
	CREATE_FUNC(Tutorial);
};