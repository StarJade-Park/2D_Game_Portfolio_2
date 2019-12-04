#include "CollisionManager.h"
#include "ComCollider.h"
#include "GActor.h"

CollisionManager::CollisionManager( )
	: mMapOfColGroup(std::map<int, SmartPtr<ColliderGroup>>( ))
{
	EMPTY_STATEMENT;
}

CollisionManager::~CollisionManager( )
{
	EMPTY_STATEMENT;
}

void CollisionManager::PushColliderToMap(const SmartPtr<ComCollider> _collider)
{
	auto iterFind = mMapOfColGroup.find(_collider->GetOrder( ));
	if ( mMapOfColGroup.end( ) == iterFind )
	{
		mMapOfColGroup.insert(
			std::map<int, SmartPtr<ColliderGroup>>::value_type(_collider->GetOrder( ), new ColliderGroup( ))
		);
		iterFind = mMapOfColGroup.find(_collider->GetOrder( ));
	}

	iterFind->second->PushToColliderList(_collider);
}

void CollisionManager::Collide( )
{
	for ( auto& iter : mMapOfColGroup )
	{
		iter.second->Collide( );
	}
}

SmartPtr<CollisionManager::ColliderGroup> CollisionManager::FindGroupOrNull(const int _key)
{
	const auto& iterFind = mMapOfColGroup.find(_key);

	if ( mMapOfColGroup.end( ) == iterFind )
	{
		return nullptr;
	}

	return iterFind->second;
}

void CollisionManager::LinkGroup(const int _src, const int _dest)
{
	ColliderGroup* _srcGroup = FindGroupOrNull(_src);
	ColliderGroup* _destGroup = FindGroupOrNull(_dest);

	if ( nullptr == _srcGroup )
	{
		_srcGroup = new ColliderGroup( );
		mMapOfColGroup.insert(std::map<int, SmartPtr<ColliderGroup>>::value_type(_src, _srcGroup));
	}

	if ( nullptr == _destGroup )
	{
		_destGroup = new ColliderGroup( );
		mMapOfColGroup.insert(std::map<int, SmartPtr<ColliderGroup>>::value_type(_dest, _destGroup));
	}

	_srcGroup->PushToOtherGroupList(_destGroup);
}

void CollisionManager::Release( )
{
	for ( auto& nextGroup : mMapOfColGroup )
	{
		nextGroup.second->Release( );
	}
}

void CollisionManager::SetGlobalCollider( )
{
	for ( auto& next : mMapOfColGroup )
	{
		next.second->SetGlobalCollider( );
	}
}

CollisionManager::ColliderGroup::ColliderGroup( )
	: mListOfOtherGroup(std::list<ColliderGroup*>( ))
	, mListOfCollider(std::list<SmartPtr<ComCollider>>( ))
{
	EMPTY_STATEMENT;
}

CollisionManager::ColliderGroup::~ColliderGroup( )
{
	EMPTY_STATEMENT;
}

void CollisionManager::ColliderGroup::Collide( )
{
	for ( auto& otherCollider : mListOfOtherGroup )
	{
		if ( this != otherCollider )
		{
			CollideOtherGroup(otherCollider);
		}
		else
		{
			CollideThisGroup( );
		}
	}
}

void CollisionManager::ColliderGroup::CollideOtherGroup(ColliderGroup* _otherGroup)
{
	for ( auto& curCollider : mListOfCollider )
	{
		if ( false == curCollider->IsCanUpdate( ) )
		{
			continue;
		}

		for ( auto& otherCollider : _otherGroup->mListOfCollider )
		{
			if ( false == otherCollider->IsCanUpdate( ) )
			{
				curCollider->ExitWhenUpdateOff(otherCollider);
				continue;
			}

			curCollider->Collide(otherCollider);
		}
	}
}

void CollisionManager::ColliderGroup::CollideThisGroup( )
{
	// note 아래 코드 동작 가능
	//auto next = std::find(mListOfCollider.begin( ), mListOfCollider.end( ), nullptr);
	const auto& end = mListOfCollider.end( );

	for ( auto& curCollider : mListOfCollider )
	{
		if ( false == curCollider->IsCanUpdate( ) )
		{
			continue;
		}

		// note 느리거나 문제가 발생할 가능성이 있음
		auto next = std::find(mListOfCollider.begin( ), mListOfCollider.end( ), curCollider);
		++next;

		for ( ; next != end; ++next )
		{
			if ( false == (*next)->IsCanUpdate( ) )
			{
				curCollider->ExitWhenUpdateOff((*next));
				continue;
			}

			curCollider->Collide((*next));
		}
	}
}

void CollisionManager::ColliderGroup::PushToColliderList(const SmartPtr<ComCollider> _collider)
{
	if ( nullptr == _collider )
	{
		CRASH_PROG;
		return;
	}

	mListOfCollider.push_back(_collider);
}

void CollisionManager::ColliderGroup::PushToOtherGroupList(ColliderGroup* _group)
{
	if ( nullptr == _group )
	{
		CRASH_PROG;
		return;
	}

	mListOfOtherGroup.push_back(_group);
}

void CollisionManager::ColliderGroup::Release( )
{
	auto next = mListOfCollider.begin( );
	const auto& end = mListOfCollider.end( );
	for ( ; next != end;)
	{
		if ( true == (*next)->IsDied( ) )
		{
			next = mListOfCollider.erase(next);
		}
		else
		{
			++next;
		}
	}
}

void CollisionManager::ColliderGroup::SetGlobalCollider( )
{
	auto next = mListOfCollider.begin( );
	const auto& end = mListOfCollider.end( );
	for ( ; next != end; )
	{
		if ( true == (*next)->GetActor( ).IsReady( ) )
		{
			next = mListOfCollider.erase(next);
		}
		else
		{
			++next;
		}
	}
}
