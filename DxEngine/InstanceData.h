#pragma once
#include <..\psklib\SmartPtr.h>

#include "RenderingPaths.h"
#include "GMesh.h"

class GMesh;

class InstanceData
	: public ControlBlock
{
	friend class RendererManager;
public:
	InstanceData( )
		: mRenderingPaths(RenderingPaths( ))
		, mMesh(nullptr)
	{
		EMPTY_STATEMENT;
	}

	~InstanceData( )
	{
		EMPTY_STATEMENT;
	}

	void SetMesh(const std::wstring& _meshName)
	{
		mMesh = GResource::Find<GMesh>(_meshName);

		if ( nullptr == mMesh )
		{
			CRASH_PROG;
			return;
		}

		if ( nullptr == mMesh->GetInstanceBuffer( ) )
		{
			CRASH_PROG;
			return;
		}
	}

	template<typename T>
	void PushData(const T& _data)
	{
		mMesh->GetInstanceBuffer( )->PushData(_data);
	}

	void SetRenderPath(const RenderingPaths& _renderingPath)
	{
		mRenderingPaths = _renderingPath;
	}

	RenderingPaths& GetRenderPath( )
	{
		return mRenderingPaths;
	}

	void UpdateInstanceData( )
	{
		mMesh->GetInstanceBuffer( )->UpdateInstanceData( );
	}

	void Render( )
	{
		mRenderingPaths.Update( );
		mMesh->UpdateAllVertexBuffer( );
		mMesh->UpdateIndexBuffer(0);
		mMesh->RenderInstanced(0);
		mMesh->GetInstanceBuffer( )->ResetDataCounter( );
	}

private:
	RenderingPaths mRenderingPaths;
	SmartPtr<GMesh> mMesh;
};