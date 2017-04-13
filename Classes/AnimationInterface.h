#pragma once
#include "pch.h"

// 애니메이션 매니저
class AnimationInterface
{
private:

	Node*						  _targetNode;				// 대상이 될 Target
	Animate*					  _cur_Animation;			// 현재 애니메이션
protected:
	bool						  _isPlay;
	SpriteFrameCache*			  _player_animation_cache;	// 애니메이션이 들어있는 plist
	Map<string, Animation*>		  _animation_container;		// 애니메이션 컨테이너

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
			// 현재 Target이 없다면 취소
			if (!_targetNode)
				return false;

			// 현재 플레이중인 애니메이션을 멈춤
			if (_cur_Animation)
				_targetNode->stopAction(_cur_Animation);

			// 애니메이션을 저장함
			_cur_Animation = Animate::create(_animation_container.at(type));

			// 애니메이션 플레이 ( 애니메이션 -> 초기화)
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