#pragma once

#include <d3d11_4.h>
#include <d3dcompiler.h> // shader compiler

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler") // shader compiler
#pragma comment(lib, "dxguid")

#include <..\psklib\MyMath.h>
#include <..\psklib\SmartPtr.h>

class WindowController;

// - �׷��� ����̽� ����� ���� Ŭ����
// - d3dx�� ����� ��Ƴ���
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
	UINT mMultiSamplingLV; // ��� ������ multi-sampling level
	UINT mMultiSamplingCount;

	// DX���� grapic device�� ���� ��� ������ �Ʒ��� �� ���� �����͸� ����Ͽ� �̷������.
	ID3D11Device* mIDevice; // �׷���ī���� ��� �ڿ��� ���� ������ ���. (o)
	ID3D11DeviceContext* mIContext; // �׷���ī���� ��� ���꿡 ���� ������ ����Ѵ�.(����) (o)

	ID3D11RenderTargetView* mIBackBufferView; // ���̹��� ������������������ �ܰ迡 �����Ѵ�.
	IDXGISwapChain* mISwapChain; // ���� ���� ������ ������������ �ܰ迡 �����Ѵ�.
	ID3D11DepthStencilView* mIDepthStencilView; // ���̹��� ������������������ �ܰ迡 �����Ѵ�.
	ID3D11Texture2D* mIDepthStencilTex; // ���̹��� ������������������ �ܰ迡 �����Ѵ�.

	GVector4D mBackColor; // Background color
	D3D11_VIEWPORT mViewPortInfo; // ����Ʈ ��� ����.

	D3D_FEATURE_LEVEL meFeatureLV; // ��� ������ feature level(�� �� ����)
};

