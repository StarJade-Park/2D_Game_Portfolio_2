#include <..\psklib\GameFileSystem.h>

#include "GTexture.h"
#include "GEngineDevice.h"
#include "GShader.h"

GTexture::GTexture( )
	: mImage(DirectX::ScratchImage( ))
	, miTexture(nullptr)
	, miRenderTargetView(nullptr), miDepthStencilView(nullptr), miShaderResourceView(nullptr)
{
	EMPTY_STATEMENT;
}

GTexture::~GTexture( )
{
	// note 메모리 해제는 역순으로
	mImage.Release( );

	if ( nullptr != miRenderTargetView )
	{
		miRenderTargetView->Release( );
		miRenderTargetView = nullptr;
	}
	if ( nullptr != miDepthStencilView )
	{
		miDepthStencilView->Release( );
		miDepthStencilView = nullptr;
	}
	if ( nullptr != miShaderResourceView )
	{
		miShaderResourceView->Release( );
		miShaderResourceView = nullptr;
	}
	if ( nullptr != miTexture )
	{
		miTexture->Release( );
		miTexture = nullptr;
	}
}

bool GTexture::Load( )
{
	//if ( false == GameIO::IsExist(mFullPath) ) // img 파일 존재 확인
	//{
	//	CRASH_PROG;
	//	return false;
	//}

	if ( nullptr == &GEngineDevice::MainDevice( ) )
	{
		CRASH_PROG;
		return false;
	}

	stdfs::path ext = GetExtension( );
	if ( L".DDS" == ext || L".dds" == ext ) // .dds
	{
		//
		if ( FAILED(DirectX::LoadFromDDSFile(GetAbsPath( ).c_str( ), DirectX::DDS_FLAGS::DDS_FLAGS_NONE, nullptr, mImage)) )
		{
			CRASH_PROG;
			return false;
		}
	}
	else if ( L".TGA" == ext || L".tga" == ext ) // .tga
	{
		//
		if ( FAILED(DirectX::LoadFromTGAFile(GetAbsPath( ).c_str( ), nullptr, mImage)) )
		{
			CRASH_PROG;
			return false;
		}
	}
	else // png, jpeg, ...
	{
		// Loads a WIC-supported bitmap file.
		if ( FAILED(DirectX::LoadFromWICFile(GetAbsPath( ).c_str( ), DirectX::WIC_FLAGS::WIC_FLAGS_NONE, nullptr, mImage)) )
		{
			CRASH_PROG;
			return false;
		}
	}

	if ( NULL == mImage.GetImageCount( ) )
	{
		CRASH_PROG;
		return false;
	}

	// shader 접근 권한
	if ( FAILED(DirectX::CreateShaderResourceView(&GEngineDevice::MainDevice( ).GetIDevice( ), mImage.GetImages( ), mImage.GetImageCount( ), mImage.GetMetadata( ), &miShaderResourceView)) )
	{
		CRASH_PROG;
		return false;
	}

	return true;
}

void GTexture::Update(shader_type::TYPE _shaderType, UINT _setIndex)
{
	if ( nullptr == miShaderResourceView )
	{
		CRASH_PROG;
		return;
	}

	switch ( _shaderType )
	{
	case shader_type::TYPE::VS:
		GEngineDevice::MainDevice( ).GetIContext( ) \
			.VSSetShaderResources(_setIndex, 1, &miShaderResourceView); // note Get, Set 구분 잘하자!
		break;
	case shader_type::TYPE::HS:
		GEngineDevice::MainDevice( ).GetIContext( ) \
			.HSSetShaderResources(_setIndex, 1, &miShaderResourceView);
		break;
	case shader_type::TYPE::DS:
		GEngineDevice::MainDevice( ).GetIContext( ) \
			.DSSetShaderResources(_setIndex, 1, &miShaderResourceView);
		break;
	case shader_type::TYPE::GS:
		GEngineDevice::MainDevice( ).GetIContext( ) \
			.GSSetShaderResources(_setIndex, 1, &miShaderResourceView);
		break;
	case shader_type::TYPE::PS:
		GEngineDevice::MainDevice( ).GetIContext( ) \
			.PSSetShaderResources(_setIndex, 1, &miShaderResourceView);
		break;
	case shader_type::TYPE::NONE: FALL_THROUGH;
	default:
		CRASH_PROG;
		break;
	}
}
