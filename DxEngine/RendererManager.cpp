#include "RendererManager.h"
#include "GEngineDevice.h"
#include "ComCamera.h"
#include "GActor.h"
#include "ComTransform.h"
#include "DebugRenderer.h"

bool(*RendererManager::SortFunc[RenderSort::SORT_MAX])(const SmartPtr<ComRender>&, const SmartPtr<ComRender>&) = { nullptr };

std::map<std::wstring, SmartPtr<InstanceData>> RendererManager::mMapOfInstanceData = std::map<std::wstring, SmartPtr<InstanceData>>( );

bool RendererManager::ZSort(const SmartPtr<ComRender>& _left, const SmartPtr<ComRender>& _right)
{
	return _left->GetActor( ).GetTransform( ).GetWorldPos( ).m.z > _right->GetActor( ).GetTransform( ).GetWorldPos( ).m.z;
}

bool RendererManager::YSort(const SmartPtr<ComRender>& _left, const SmartPtr<ComRender>& _right)
{
	return _left->GetActor( ).GetTransform( ).GetWorldPos( ).m.y > _right->GetActor( ).GetTransform( ).GetWorldPos( ).m.y;
}

bool RendererManager::XSort(const SmartPtr<ComRender>& _left, const SmartPtr<ComRender>& _right)
{
	return _left->GetActor( ).GetTransform( ).GetWorldPos( ).m.x > _right->GetActor( ).GetTransform( ).GetWorldPos( ).m.x;
}

RendererManager::RendererManager(GScene* _scene)
	: mScene(_scene)
	, mMapOfCameraList(std::map<int, std::list<SmartPtr<ComCamera>>>( ))
	, mListOfCamera(std::list<SmartPtr<ComCamera>>( ))
	, mListOfRender(std::list<SmartPtr<ComRender>>( ))
{
	RendererManager::SortFunc[RenderSort::SORT_X] = ZSort;
	RendererManager::SortFunc[RenderSort::SORT_Y] = ZSort; // ??
	RendererManager::SortFunc[RenderSort::SORT_Z] = ZSort;
	RendererManager::SortFunc[RenderSort::SORT_CAM_X] = nullptr;
	RendererManager::SortFunc[RenderSort::SORT_CAM_Y] = nullptr;
	RendererManager::SortFunc[RenderSort::SORT_CAM_Z] = nullptr;
}

RendererManager::~RendererManager( )
{
	EMPTY_STATEMENT;
}

void RendererManager::Render( )
{
	GEngineDevice::MainDevice( ).ClearTargetWindow( );
	GEngineDevice::MainDevice( ).SetDefaultRenderTarget( );
	GEngineDevice::MainDevice( ).SetDefaultViewport( );

	for ( auto& iterList : mMapOfRendererList )
	{
		// 랜더링할 카메라를 준비
		SetRenderList(iterList.first);

		// 카메라들이 랜더 그룹에 속한 랜더러들을 랜더링
		for ( auto& iterCam : mListOfCamera )
		{
			const auto& iterSort = mMapOfRendererSort.find(iterList.first);
			if ( RenderSort::SORT_NONE != iterSort->second )
			{
				// 아무것도 안 넣어준 상태면 아마 값 비교로 정렬.
				iterList.second.sort(SortFunc[iterSort->second]);
			}

			if ( false == iterCam->IsCanUpdate( ) )
			{
				continue;
			}

			for ( auto& iterRender : iterList.second )
			{
				iterRender->Render(iterCam);
			} // iterList.second(std::list<SmartPtr<ComRender>>)
		} // mListOfCamera
	} // mMapOfRendererList

	GEngineDevice::ClearSlotInMainDevice( );

	// note instace data를 기반으로 rendering
	for ( auto& instData : mMapOfInstanceData )
	{
		instData.second->UpdateInstanceData( );
		instData.second->Render( );
	}

	// note 디버그 정보 랜더링
	DebugRenderer::RenderDebugInfo( );
	GEngineDevice::ClearSlotInMainDevice( );

	GEngineDevice::MainDevice( ).Present( );
}

void RendererManager::Release( )
{
	for ( auto& nextList : mMapOfRendererList )
	{
		auto next = nextList.second.begin( );
		const auto& end = nextList.second.end( );

		for ( ; next != end; )
		{
			if ( true == (*next)->IsDied( ) )
			{
				next = nextList.second.erase(next);
			}
			else
			{
				++next;
			}
		}
	}
}

void RendererManager::SetGlobalRenderer( )
{
	for ( auto& nextList : mMapOfRendererList )
	{
		auto next = nextList.second.begin( );
		const auto& end = nextList.second.end( );

		for ( ; next != end; )
		{
			if ( true == (*next)->GetActor( ).IsReady( ) )
			{
				next = nextList.second.erase(next);
			}
			else
			{
				++next;
			}
		}
	}
}

void RendererManager::PushCamToMap(SmartPtr<ComCamera> _cam)
{
	auto findIter = mMapOfCameraList.find(_cam->GetOrder( ));

	if ( findIter == mMapOfCameraList.end( ) )
	{
		mMapOfCameraList.insert(
			std::map<int, std::list<SmartPtr<ComCamera>>>::value_type(
			_cam->GetOrder( ), std::list<SmartPtr<ComCamera>>( )
		)
		);
		findIter = mMapOfCameraList.find(_cam->GetOrder( ));
	}

	findIter->second.push_back(_cam);
}

void RendererManager::PushRenderToMap(SmartPtr<ComRender> _render)
{
	auto findIter = mMapOfRendererList.find(_render->GetOrder( ));

	if ( findIter == mMapOfRendererList.end( ) )
	{
		mMapOfRendererList.insert(
			std::map<int, std::list<SmartPtr<ComRender>>>::value_type(
			_render->GetOrder( ), std::list<SmartPtr<ComRender>>( )
		)
		);

		mMapOfRendererSort.insert(
			std::map<int, RenderSort>::value_type(_render->GetOrder( ), RenderSort::SORT_Z)
		);

		findIter = mMapOfRendererList.find(_render->GetOrder( ));
	}

	findIter->second.push_back(_render);
}

void RendererManager::SetRenderList(const int _order)
{
	mListOfCamera.clear( );

	for ( auto& iterMap : mMapOfCameraList )
	{
		for ( auto& iterList : iterMap.second )
		{
			if ( true == iterList->IsAllRender( ) || true == iterList->IsCanRender(_order) )
			{
				mListOfCamera.push_back(iterList);
			}
		}
	}
}

void RendererManager::CheckOrderOfCamera( )
{
	for ( auto& iterList : mMapOfCameraList )
	{
		for ( auto iterCamBegin = iterList.second.begin( ), iterCamEnd = iterList.second.end( );
			iterCamBegin != iterCamEnd;
			++iterCamBegin
			)
		{
			if ( iterList.first != (*iterCamBegin)->GetOrder( ) )
			{
				mListOfCamera.push_back((*iterCamBegin));
				iterCamBegin = iterList.second.erase(iterCamBegin);
			}

			if ( iterCamBegin == iterCamEnd )
			{
				break;
			}
		}
	}

	for ( auto& iter : mListOfCamera )
	{
		PushCamToMap(iter);
	}

	mListOfCamera.clear( );
}
