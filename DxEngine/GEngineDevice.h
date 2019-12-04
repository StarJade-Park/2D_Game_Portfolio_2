#pragma once

#include <d3d11_4.h>
#include <d3dcompiler.h> // shader compiler

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler") // shader compiler
#pragma comment(lib, "dxguid")

#include <..\psklib\MyMath.h>
#include <..\psklib\SmartPtr.h>

class WindowController;

// - 그래픽 디바이스 사용을 위한 클래스
// - d3dx의 기능을 모아놓음
class GEngineDevice
	: public ControlBlock
{
public:
	static GEngineDevice& MainDevice( )
	{
		return *mMainGDevice;
	}

	static void InitPath( );

	static void ClearSlotInMainDevice( );

	GEngineDevice( );
	~GEngineDevice( );

	void Init(WindowController* _targetWindow);
	void ClearTargetWindow( );
	void Present( );

	void SetDefaultRenderTarget( );
	void SetDefaultViewport( );

	ID3D11Device& GetIDevice( ) const
	{
		return *mIDevice;
	}

	ID3D11DeviceContext& GetIContext( ) const
	{
		return *mIContext;
	}

private:
	bool CreateSwapChain( );
	bool CreateDepthStencilView( );
	bool CreateViewPort( );

	void InitDefaultResource( );

private:
	enum __tag
	{
		HZ_Numerator = 60,
		HZ_Denominator = 1,
	};

	static GEngineDevice* mMainGDevice; // main target grapic device
	bool mbDefualtInitFlag;

	WindowController* mTargetWindow; // rendering target window
	UINT mMultiSamplingLV; // 사용 가능한 multi-sampling level
	UINT mMultiSamplingCount;

	// DX에서 grapic device에 대한 모든 동작은 아래의 두 가지 포인터를 사용하여 이루어진다.
	ID3D11Device* mIDevice; // 그래픽카드의 모든 자원에 대한 권한을 담당. (o)
	ID3D11DeviceContext* mIContext; // 그래픽카드의 모든 연산에 대한 권한을 담당한다.(랜더) (o)

	ID3D11RenderTargetView* mIBackBufferView; // 깊이버퍼 랜더링파이프라인의 단계에 관여한다.
	IDXGISwapChain* mISwapChain; // 깊이 버퍼 랜더링 파이프라인의 단계에 관여한다.
	ID3D11DepthStencilView* mIDepthStencilView; // 깊이버퍼 랜더링파이프라인의 단계에 관여한다.
	ID3D11Texture2D* mIDepthStencilTex; // 깊이버퍼 랜더링파이프라인의 단계에 관여한다.

	GVector4D mBackColor; // Background color
	D3D11_VIEWPORT mViewPortInfo; // 뷰포트 행렬 정보.

	D3D_FEATURE_LEVEL meFeatureLV; // 사용 가능한 feature level(쓸 일 없음)
};

