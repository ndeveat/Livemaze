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
		auto tutorial_label = Label::create("���� Ű\n\nX : ����\nCtrl : ������ 1\nShift : ������2\n\n�� : �ѹ� ��Ű�� ��� ����� ��ſ��� �޷��� ���Դϴ�. - ���ǵ�\n����� : ����� ��� �ֵ��� ����ؼ� �Ѿƿ� ���Դϴ�.-Ȱ\n�̶� : �ӵ��� �������� ���� �����ؾ� �� ���Դϴ�.-ü��\n���� : �ܴ��ϰ� ���� ���� ���ظ� �� ���Դϴ�.-����\n����ũ�� : �Ŵ��� ���� ��ſ��� ���� ���̰� �̶� ��ȯ�մϴ�.\n�Ķ�� : ������ ���� ü������ ����� ��濡 ���߸��� �� ���Դϴ�.", "Arial", 40);
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