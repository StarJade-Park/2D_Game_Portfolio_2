#pragma once
#include <list>
#include <map>

#include <..\psklib\SmartPtr.h>

class ComCollider;

class CollisionManager
{
	friend ComCollider;
	friend class GScene;
private:
	class ColliderGroup
		: public ControlBlock
	{
		friend CollisionManager;
	public:
		ColliderGroup( );
		~ColliderGroup( );

		void Collide( );
		void CollideOtherGroup(ColliderGroup* _otherGroup);
		void CollideThisGroup( );

		void PushToColliderList(const SmartPtr<ComCollider> _collider);
		void PushToOtherGroupList(ColliderGroup* _group);

		void Release( );

		void SetGlobalCollider( );

	public:
		// ���� �浹�� �ٸ� �׷���� ������
		// ������ SmartPtr������ ������ ptr�� �����ϸ� leak�� �߻���
		std::list<ColliderGroup*> mListOfOtherGroup;

	private:
		std::list<SmartPtr<ComCollider>> mListOfCollider;
	};

public:
	void Collide( );
	SmartPtr<CollisionManager::ColliderGroup> FindGroupOrNull(const int _key);

	// note �浹 ���ϸ� linking �ʼ�
	void LinkGroup(const int _src, const int _dest);

	void Release( );

	void SetGlobalCollider( );

private:
	CollisionManager( );
	~CollisionManager( );

	void PushColliderToMap(const SmartPtr<ComCollider> _collider);

private:
	std::map<int, SmartPtr<ColliderGroup>> mMapOfColGroup; // todo function : insert col group
};

