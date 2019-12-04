#include "ComCollider.h"
#include "GActor.h"
#include "GScene.h"

ComCollider::ComCollider( )
	: mSetOfOtherCollider(std::set<ComCollider*>( )), mEndIterOtherColSet(mSetOfOtherCollider.end( ))
{
	EMPTY_STATEMENT;
}

ComCollider::~ComCollider( )
{
	EMPTY_STATEMENT;
}

void ComCollider::CreateData(int _order)
{
	mOrder = _order;
	GetScene( ).mCollisionMGR.PushColliderToMap(this);
}

void ComCollider::Init( )
{
	if ( nullptr == &GetActor( ).GetTransform( ) )
	{
		CRASH_PROG;
	}

	SetParentTransform(&GetActor( ).GetTransform( ));
}

bool ComCollider::CheckShape(ComCollider* _other)
{
	_other;
	CRASH_PROG;
	return true;
}

void ComCollider::StartSceneChange( )
{
	GetScene( ).mCollisionMGR.PushColliderToMap(this);
}

void ComCollider::Collide(ComCollider* _other)
{
	if ( true == CheckShape(_other) )
	{
		if ( mEndIterOtherColSet == mSetOfOtherCollider.find(_other) )
		{
			mSetOfOtherCollider.insert(std::set<ComCollider*>::value_type(_other));
			Enter(_other);
		}
		else
		{
			Stay(_other);
		}
	}
	else
	{
		if ( mEndIterOtherColSet != mSetOfOtherCollider.find(_other) )
		{
			Exit(_other);
			mSetOfOtherCollider.erase(_other);
		}
	}

}

void ComCollider::Enter(ComCollider* _other)
{
	for ( auto& next : mListOfEnterFunction )
	{
		next(this, _other);
	}
}

void ComCollider::Stay(ComCollider* _other)
{
	for ( auto& next : mListOfStayFunction )
	{
		next(this, _other);
	}
}

void ComCollider::Exit(ComCollider* _other)
{
	for ( auto& next : mListOfExitFunction )
	{
		next(this, _other);
	}
}

void ComCollider::ExitWhenUpdateOff(ComCollider* _other)
{
	if ( mSetOfOtherCollider.end( ) != mSetOfOtherCollider.find(_other) )
	{
		Exit(_other);
		_other->Exit(this);
		mSetOfOtherCollider.erase(_other);
	}
}
