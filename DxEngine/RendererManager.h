#pragma once
#include <map>
#include <list>

#include <..\psklib\SmartPtr.h>
#include "ComRender.h"

#include "InstanceData.h"

enum RenderSort
{
	SORT_NONE, SORT_X, SORT_Y, SORT_Z, SORT_CAM_X, SORT_CAM_Y, SORT_CAM_Z, SORT_MAX
};

class GScene;
class ComCamera;

class RendererManager
{
	friend ComRender;
	friend ComCamera;
public:
	static bool ZSort(const SmartPtr<ComRender>& _left, const SmartPtr<ComRender>& _right);
	static bool YSort(const SmartPtr<ComRender>& _left, const SmartPtr<ComRender>& _right);
	static bool XSort(const SmartPtr<ComRender>& _left, const SmartPtr<ComRender>& _right);

	template<typename T>
	static void PushData(const wchar_t* _name, const T& _data)
	{
		// 매번 데이터 넣을때마다 찾아야 하하는게.
		SmartPtr<InstanceData> pInst = FindInstanceDataOrNull(_name);

		if ( nullptr != pInst )
		{
			CRASH_PROG;
		}
		
		pInst->PushData<T>(_data);
	}

	static SmartPtr<InstanceData> FindInstanceDataOrNull(const std::wstring& _name)
	{
		auto foundIter = mMapOfInstanceData.find(_name);

		if ( mMapOfInstanceData.end( ) == foundIter )
		{
			return nullptr;
		}

		return foundIter->second;
	}

	static SmartPtr<InstanceData> CreateInstanceDataOrNull(
		const std::wstring& _instanceDataName, const std::wstring& _meshName
		/*, UINT _instanceCount = 1000*/
	)
	{
		SmartPtr<InstanceData> foundInstanceData = FindInstanceDataOrNull(_instanceDataName);

		if ( nullptr != foundInstanceData )
		{
			return foundInstanceData;
		}

		foundInstanceData = new InstanceData( );
		foundInstanceData->SetMesh(_meshName);

		if ( nullptr == foundInstanceData->mMesh->GetInstanceBuffer( ) )
		{
			CRASH_PROG;
			return nullptr;
		}

		mMapOfInstanceData.insert(
			std::map<std::wstring, SmartPtr<InstanceData>>::value_type(
			_instanceDataName, foundInstanceData)
		);

		return foundInstanceData;
	}

	static SmartPtr<InstanceData> CreateInstanceDataOrNull(
		const std::wstring& _instanceDataName, const std::wstring& _meshName,
		const RenderingPaths& _renderingPaths/*, UINT _instanceCount = 1000*/
	)
	{
		SmartPtr<InstanceData> foundInstanceData = FindInstanceDataOrNull(_instanceDataName);

		if ( nullptr != foundInstanceData )
		{
			return nullptr;
		}

		foundInstanceData = new InstanceData( );
		foundInstanceData->SetMesh(_meshName);
		foundInstanceData->SetRenderPath(_renderingPaths);

		if ( nullptr == foundInstanceData->mMesh->GetInstanceBuffer( ) )
		{
			CRASH_PROG;
			return nullptr;
		}

		mMapOfInstanceData.insert(
			std::map<std::wstring, SmartPtr<InstanceData>>::value_type(
			_instanceDataName, foundInstanceData)
		);

		return foundInstanceData;
	}

	RendererManager(GScene* _scene);
	~RendererManager( );

	void Render( );
	void Release( );
	void SetGlobalRenderer( );

private:
	void PushCamToMap(SmartPtr<ComCamera> _cam);
	void PushRenderToMap(SmartPtr<ComRender> _render);

	void SetRenderList(const int _order);
	void CheckOrderOfCamera( );

public:
	static bool(*SortFunc[RenderSort::SORT_MAX])(const SmartPtr<ComRender>&, const SmartPtr<ComRender>&);

private:
	static std::map<std::wstring, SmartPtr<InstanceData>> mMapOfInstanceData;

	std::map<int, std::list<SmartPtr<ComCamera>>> mMapOfCameraList;
	std::map<int, std::list<SmartPtr<ComRender>>> mMapOfRendererList;
	std::map<int, RenderSort> mMapOfRendererSort;
	std::list<SmartPtr<ComCamera>> mListOfCamera;
	std::list<SmartPtr<ComRender>> mListOfRender;
	GScene* mScene;
};

