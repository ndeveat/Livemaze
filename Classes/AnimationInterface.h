#pragma once
#include "pch.h"

// �ִϸ��̼� �Ŵ���
class AnimationInterface
{
private:

	Node*						  _targetNode;				// ����� �� Target
	Animate*					  _cur_Animation;			// ���� �ִϸ��̼�
protected:
	bool						  _isPlay;
	SpriteFrameCache*			  _player_animation_cache;	// �ִϸ��̼��� ����ִ� plist
	Map<string, Animation*>		  _animation_container;		// �ִϸ��̼� �����̳�

	AnimationInterface(Node* target, SpriteFrameCache* cache)
		: _targetNode(target)
		, _player_animation_cache(cache){}
	~AnimationInterface(){}
	
public:
public:
	bool init()
	{
		_isPlay = false;
		_cur_Animation = nullptr;
		_animation_container.clear();
		return true;
	}

	virtual void Init() = 0;

	bool play(string type,int repeat_num=1)
	{
		if (!_isPlay) {
			_isPlay = true;
			// ���� Target�� ���ٸ� ���
			if (!_targetNode)
				return false;

			// ���� �÷������� �ִϸ��̼��� ����
			if (_cur_Animation)
				_targetNode->stopAction(_cur_Animation);

			// �ִϸ��̼��� ������
			_cur_Animation = Animate::create(_animation_container.at(type));

			// �ִϸ��̼� �÷��� ( �ִϸ��̼� -> �ʱ�ȭ)
			_targetNode->runAction(Sequence::create(Repeat::create(_cur_Animation, repeat_num), CallFunc::create([&](){_isPlay = false; }), nullptr));
		}
	}

	void culAnimationStop()
	{
		//_isPlay = false;
		if (_cur_Animation){
			_targetNode->stopAction(_cur_Animation);
			_cur_Animation = nullptr;
		}
	}
};