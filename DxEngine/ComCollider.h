#pragma once
#include <set>
#include <list>
#include <functional>

#include <..\psklib\MyMath.h>

#include "ComSubTransform.h"

enum class COLLIDER_TYPE
{
	COLLIDER_AABB, COLLIDER_OBB,
};

enum COLLIDER_GROUP : int
{
	COL_PLAYER, COL_PLAYER_ATTCK, COL_MONSTER, COL_MONSTER_ATTCK, COL_ETC_OBJECT,
};

class ComCollider
	: public ComSubTransform
{
	friend class CollisionManager;
public:
	ComCollider( );
	virtual ~ComCollider( );
	void CreateData(int _order = 0);
	// NOTE : Shape은 정의(모양), Figure는 정의에 따른 수치(도형)
	// ... A cube is a 3-D figure.
	// ... A circle is a planar figure.
	// ... The figure is in the shape of a square.
	virtual bool CheckShape(ComCollider* _other) = 0;

	void StartSceneChange( ) override;

	int GetOrder( ) const
	{
		return mOrder;
	}

	void SetOrder(const int _order)
	{
		mOrder = _order;
	}

	// - 멤버 함수 등록
	template<typename T>
	void RegistEnterFunction(void(T::*func)(ComCollider*, ComCollider*), T* _object)
	{
		std::function<void(ComCollider*, ComCollider*)> function =
			std::bind(func, _object, std::placeholders::_1, std::placeholders::_2);
		if ( nullptr == function )
		{
			CRASH_PROG;
		}

		mListOfEnterFunction.push_back(function);
	}

	// - 전역 함수 등록
	void RegistEnterFunction(void(*func)(ComCollider*, ComCollider*))
	{
		std::function<void(ComCollider*, ComCollider*)> function = func;
		if ( nullptr == function )
		{
			CRASH_PROG;
		}

		mListOfEnterFunction.push_back(function);
	}

	// - 멤버 함수 등록
	template<typename T>
	void RegistStayFunction(void(T::*func)(ComCollider*, ComCollider*), T* _object)
	{
		std::function<void(ComCollider*, ComCollider*)> function =
			std::bind(func, _object, std::placeholders::_1, std::placeholders::_2);
		if ( nullptr == function )
		{
			CRASH_PROG;
		}

		mListOfStayFunction.push_back(function);
	}

	// - 전역 함수 등록
	void RegistStayFunction(void(*func)(ComCollider*, ComCollider*))
	{
		std::function<void(ComCollider*, ComCollider*)> function = func;
		if ( nullptr == function )
		{
			CRASH_PROG;
		}

		mListOfStayFunction.push_back(function);
	}

	// - 멤버 함수 등록
	template<typename T>
	void RegistExitFunction(void(T::*func)(ComCollider*, ComCollider*), T* _object)
	{
		std::function<void(ComCollider*, ComCollider*)> function =
			std::bind(func, _object, std::placeholders::_1, std::placeholders::_2);
		if ( nullptr == function )
		{
			CRASH_PROG;
		}

		mListOfExitFunction.push_back(function);
	}

	// - 전역 함수 등록
	void RegistExitFunction(void(*func)(ComCollider*, ComCollider*))
	{
		std::function<void(ComCollider*, ComCollider*)> function = func;
		if ( nullptr == function )
		{
			CRASH_PROG;
		}

		mListOfExitFunction.push_back(function);
	}

private:
	void Init( ) override;

	void Collide(ComCollider* _other);

	void Enter(ComCollider* _other);
	void Stay(ComCollider* _other);
	void Exit(ComCollider* _other);

	void ExitWhenUpdateOff(ComCollider* _other);

private:
	int mOrder;
	std::set<ComCollider*> mSetOfOtherCollider;
	const std::set<ComCollider*>::iterator mEndIterOtherColSet;

	std::list<std::function<void(ComCollider*, ComCollider*)>> mListOfEnterFunction;
	std::list<std::function<void(ComCollider*, ComCollider*)>> mListOfStayFunction;
	std::list<std::function<void(ComCollider*, ComCollider*)>> mListOfExitFunction;
};

