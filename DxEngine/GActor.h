#pragma once
#include <list>
#include <set>

#include <..\psklib\BaseName.h>
#include <..\psklib\SmartPtr.h>
#include <..\psklib\MyMath.h>

#include "BaseUpdater.h"

class GScene;
class BaseGComponent;
class ComTransform;

class GActor final
	: public BaseName, public ControlBlock, public BaseUpdater
{
	friend class GScene;
public:
	void UpdatePreprocess( ) override;
	void Update( ) override;
	void UpdatePostprocess( ) override;

	void RenderPreprocess( ) override;
	void RenderPostprocess( ) override;

	void CollidePreprocess( ) override;
	void CollidePostprocess( ) override;

	void Release( ) override;

	void StartSceneChange( );

#pragma region Get, Set
	ComTransform& GetTransform( ) const
	{
		return *mcTransform;
	}

	void SetTransform(ComTransform* _trans)
	{
		mcTransform = _trans;
	}

	void SetParentActor(SmartPtr<GActor> _parentActor);
	GActor& GetParentActor( )
	{
		return *mParentActor;
	}

	GVector4D GetPos( ) const;
	void SetPos(const GVector4D& _pos);
	void SetLocalScale(const GVector4D& _scale);
	GVector4D GetLocalScale( ) const;
	void SetLocalScaleByRatio(const float _scale);
	GVector4D GetRotation( );
	void SetRotation(const GVector4D& _rotation);

	GScene& GetScene( ) const
	{
		return *mScene;
	}

	void SetScene(GScene* _scene)
	{
		mScene = _scene;
	}

	bool IsReady( )
	{
		return mbReadyFlag;
	}
#pragma endregion

	template<typename COM, typename... TYPES>
	SmartPtr<COM> AddComponent(TYPES&&... _args)
	{
		COM* newCom = new COM( );
		AddComponent(*newCom);
		newCom->CreateData(_args...);
		return newCom;
	}

	template<typename COM>
	SmartPtr<COM> GetComponentOrNull( )
	{
		for ( auto& iter : mListOfComponent )
		{
			BaseGComponent* p = iter;

			// note 컴파일 타임에 템플릿에서 함수를 알지 못하는 경우
			// 해당 부분만 따로 구현된 함수를 추가하여준다.
			// true == iter->IsSameType<COM>( )
			if ( true == CheckType(p, &typeid(COM)) )
			{
				return (COM*)p;
			}
		}

		return nullptr;
	}

	bool CheckType(BaseGComponent* _com, const type_info* _ti);

	bool IsAvailableInNextScene(const std::wstring& _nextScene)
	{
		return mSetOfAvailableScene.end( ) != mSetOfAvailableScene.find(_nextScene);
	}

	template<typename ...Rest>
	void SetSceneAvailable(Rest ..._args)
	{
		PushToAvailableSceneSet(_args...);
	}

	bool IsGlobalActor( )
	{
		return 0 != mSetOfAvailableScene.size( );
	}


private:
	GActor( )
		: mListOfComponent(std::list<SmartPtr<BaseGComponent>>( ))
		, mListOfChildActor(std::list<SmartPtr<GActor>>( ))
		, mSetOfAvailableScene(std::set<std::wstring>( ))
		, mbReadyFlag(false), mbChildReadyFlag(false)
		, mScene(nullptr), mcTransform(nullptr), mParentActor(nullptr)
	{
		EMPTY_STATEMENT;
	}

	~GActor( )
	{
		EMPTY_STATEMENT;
	}

	void AddComponent(BaseGComponent& _com);

	void OnReadyFlag( )
	{
		mbReadyFlag = true;
	}

	void OffReadyFlag( )
	{
		mbReadyFlag = false;
	}

	template<typename ...Rest>
	void PushToAvailableSceneSet(const std::wstring& _sceneName, Rest ..._args)
	{
		mSetOfAvailableScene.insert(_sceneName);
		PushToAvailableSceneSet(_args...);
	}

	void PushToAvailableSceneSet( )
	{
		OnGlobalFlag( );
	}

	bool IsReadyChild( )
	{
		return mbChildReadyFlag;
	}

	void UpdateReadyChild( );

private:
	std::list<SmartPtr<BaseGComponent>> mListOfComponent;
	std::list<SmartPtr<GActor>> mListOfChildActor;
	std::set<std::wstring> mSetOfAvailableScene;

	bool mbReadyFlag;
	bool mbChildReadyFlag;

	GActor* mParentActor;
	GScene* mScene;
	ComTransform* mcTransform;
};
