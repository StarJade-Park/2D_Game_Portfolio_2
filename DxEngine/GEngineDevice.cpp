#include <..\psklib\GameFileSystem.h>
#include <..\psklib\WindowController.h>

#include "GEngineDevice.h"

GEngineDevice* GEngineDevice::mMainGDevice = nullptr;

void GEngineDevice::InitPath( )
{
	GFileSystem::Initialize( ); // init root path to BIN directory
}

void GEngineDevice::ClearSlotInMainDevice( )
{
	ID3D11Buffer* nullBuffer[8] = { nullptr, };
	UINT nullBufferSize[8] = { 0, };
	UINT nullBufferOffset[8] = { 0, };
	mMainGDevice->GetIContext( ).IASetVertexBuffers(0, 8, nullBuffer, nullBufferSize, nullBufferOffset);
	
	//mMainGDevice->GetIContext( ).IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	mMainGDevice->GetIContext( ).IASetIndexBuffer(nullptr, DXGI_FORMAT::DXGI_FORMAT_UNKNOWN, 0);
	mMainGDevice->GetIContext( ).VSSetShader(nullptr, nullptr, 0);
	mMainGDevice->GetIContext( ).HSSetShader(nullptr, nullptr, 0);
	mMainGDevice->GetIContext( ).GSSetShader(nullptr, nullptr, 0);
	mMainGDevice->GetIContext( ).CSSetShader(nullptr, nullptr, 0);
	mMainGDevice->GetIContext( ).DSSetShader(nullptr, nullptr, 0);
	mMainGDevice->GetIContext( ).PSSetShader(nullptr, nullptr, 0);
	mMainGDevice->GetIContext( ).OMSetDepthStencilState(nullptr, 0);
}

// - Grapic Device Class Initialize
void GEngineDevice::Init(WindowController* _targetWindow)
{
	int iFlag = NULL;
#ifdef _DEBUG
	// ����׽� �÷��� ����
	iFlag = D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG;
#endif // DEBUG

	mTargetWindow = _targetWindow;

	// ������ ����(�Ķ�)
	mBackColor = GVector4D::Blue;

	HRESULT createResult = D3D11CreateDevice(
		nullptr,  // Device(ID3D11Device) ���� �� ���� �ΰ� ����
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE, // ��� �׷��� ����̽�
		NULL, // ???
		iFlag, // device ������ ����� ��Ʈ �÷���
		NULL,  // pFeatureLevels is set to NULL, this function uses the following array of feature levels
		0, // The number of elements in pFeatureLevels 
		D3D11_SDK_VERSION, // DX SDK ����
		&mIDevice, // ������ device(ID3D11Device) ��ȯ ���� ������
		&meFeatureLV, // �������� ���, ������ pFeatureLevels �迭�κ��� ������ D3D_FEATURE_LEVEL�� �����ݴϴ�.
		&mIContext // ������ context(ID3D11DeviceContext) ��ȯ ���� ������
	);

	if ( S_OK != createResult )
	{
		CRASH_PROG;
		return;
	}

	// ������ �Ǵ� ���� format�� DXGI_FORMAT_R8G8B8A8_UNORM���� ����
	// �Ű����� ������ textrue format, multisampling�� sample ��, adapter�� �����ϴ� ǰ�� level
	// �ϳ��� �ؽ��� ����, ǥ�� ���� ���տ� ���� ������ ǰ�� ������ ������ '0 ~ pNumQualityLevels - 1'
	HRESULT MsLvCheckResult =
		mIDevice->CheckMultisampleQualityLevels(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, mMultiSamplingCount, &mMultiSamplingLV);

	if ( FAILED(MsLvCheckResult) )
	{
		mMultiSamplingCount = 1;
		mMultiSamplingLV = 0;
		MsLvCheckResult =
			mIDevice->CheckMultisampleQualityLevels(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, mMultiSamplingCount, &mMultiSamplingLV);

		return;
	}

	if ( false == CreateSwapChain( ) )
	{
		CRASH_PROG;
		OutputDebugStringW(L"ERR : CreateSwapChain( ) fail");
		return;
	}

	if ( false == CreateDepthStencilView( ) )
	{
		CRASH_PROG;
		OutputDebugStringW(L"ERR : CreateDepthStencilView( ) fail");
		return;
	}

	if ( false == CreateViewPort( ) )
	{
		CRASH_PROG;
		OutputDebugStringW(L"ERR : CreateViewPort( ) fail");
		return;
	}

	if ( nullptr == mMainGDevice )
	{
		mMainGDevice = this;
	}

	InitDefaultResource( );
}

void GEngineDevice::ClearTargetWindow( )
{
	if ( nullptr != mIContext )
	{
		// back buffer�� background color�� clear
		mIContext->ClearRenderTargetView(mIBackBufferView, mBackColor.Arr);
		// depth buffer�� 1.0f�� stencil buffer�� 0���� clear
		mIContext->ClearDepthStencilView(mIDepthStencilView, D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH | D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}

void GEngineDevice::Present( )
{
	if ( nullptr != mISwapChain )
	{
		// 1. ���� �����Ӱ� �������� ǥ�ø� ����ȭ�ϴ� ����� ����
		// 2. ���� ü�� ǥ�� �ɼ�(��¿� �������� ǥ���ϱ� ���� �ɼ�)
		mISwapChain->Present(0, 0);
	}
}

void GEngineDevice::SetDefaultRenderTarget( )
{
	if ( nullptr != mIContext && nullptr != mIDepthStencilView )
	{
		// �׷��� �� object ��, ������ ����
		mIContext->OMSetRenderTargets(1, &mIBackBufferView, mIDepthStencilView);
	}
}

void GEngineDevice::SetDefaultViewport( )
{
	if ( nullptr != mIContext )
	{
		// ���ε� �� ����Ʈ�� ���� viewport struct�� �־��ش�.
		mIContext->RSSetViewports(1, &mViewPortInfo);
	}
}

GEngineDevice::GEngineDevice( )
	: mTargetWindow(nullptr), mMultiSamplingLV(0), mMultiSamplingCount(4) // defualt MS lv, count
	, mIDevice(nullptr), mIContext(nullptr)
	, mISwapChain(nullptr), mIBackBufferView(nullptr), mIDepthStencilView(nullptr), mIDepthStencilTex(nullptr)
	, mBackColor(GVector4D( )), mViewPortInfo(D3D11_VIEWPORT( ))
	, mbDefualtInitFlag(false)
{
	EMPTY_STATEMENT;
}

GEngineDevice::~GEngineDevice( )
{
	mTargetWindow = nullptr;

	if ( nullptr != mIDevice )
	{
		mIDevice->Release( );
		mIDevice = nullptr;
	}

	if ( nullptr != mIContext )
	{
		mIContext->Release( );
		mIContext = nullptr;
	}

	if ( nullptr != mIBackBufferView )
	{
		mIBackBufferView->Release( );
		mIBackBufferView = nullptr;
	}

	if ( nullptr != mISwapChain )
	{
		mISwapChain->Release( );
		mISwapChain = nullptr;
	}

	if ( nullptr != mIDepthStencilView )
	{
		mIDepthStencilView->Release( );
		mIDepthStencilView = nullptr;
	}

	if ( nullptr != mIDepthStencilTex )
	{
		mIDepthStencilTex->Release( );
		mIDepthStencilTex = nullptr;
	}
}

// - mISwapChain�� �����ϴ� �Լ�
bool GEngineDevice::CreateSwapChain( )
{
	DXGI_SWAP_CHAIN_DESC swapChainInfo = DXGI_SWAP_CHAIN_DESC( ); // DESC = description, ���� struct

	// buffer desc size
	swapChainInfo.BufferDesc.Width = (UINT)mTargetWindow->GetWndSize( ).m.x;
	swapChainInfo.BufferDesc.Height = (UINT)mTargetWindow->GetWndSize( ).m.y;

	// mointer frame rate(Hz)
	swapChainInfo.BufferDesc.RefreshRate.Numerator = __tag::HZ_Numerator;
	swapChainInfo.BufferDesc.RefreshRate.Denominator = __tag::HZ_Denominator;

	// ���� format�� DXGI_FORMAT_R8G8B8A8_UNORM
	// ��ĵ ���� ����
	// ������ ������� �ػ󵵿� ���߾� �̹����� �þ�� ����� ����
	swapChainInfo.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainInfo.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainInfo.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;

	// buffer �뵵 -> for render(target output)
	swapChainInfo.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // dxgi.h define

	// - if MSAA on
	//swapChainInfo.SampleDesc.Count = 4; // The number of multisamples per pixel.
	//swapChainInfo.SampleDesc.Quality = mMultiSamplingLV - 1;
	// - if MSAA off
	swapChainInfo.SampleDesc.Count = 1; // The number of multisamples per pixel.
	swapChainInfo.SampleDesc.Quality = 0;
	// set output window
	swapChainInfo.OutputWindow = mTargetWindow->GetHWnd( );
	swapChainInfo.BufferCount = 2; // ����� ���� ��

	// swap ��� ����
	// flip�� UWP �̽Ľ� GPU ���� ������ ����
	// DXGI�� �� ������ ������ �������� ����
	// This flag cannot be used with multisampling and partial presentation.
	swapChainInfo.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainInfo.Windowed = !mTargetWindow->IsFullScr( );
	// ����: enum DXGI_SWAP_CHAIN_FLAG
	swapChainInfo.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// create swap chain
	IDXGIDevice* idxDevice = nullptr;
	IDXGIAdapter* idxAdapter = nullptr;
	IDXGIFactory* idxFactory = nullptr;

	mIDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&idxDevice);
	if ( nullptr == idxDevice )
	{
		CRASH_PROG;
		return false;
	}

	idxDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&idxAdapter);
	if ( nullptr == idxAdapter )
	{
		CRASH_PROG;
		return false;
	}

	idxAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&idxFactory);
	if ( nullptr == idxFactory )
	{
		CRASH_PROG;
		return false;
	}

	if ( S_OK != idxFactory->CreateSwapChain(mIDevice, &swapChainInfo, &mISwapChain) )
	{
		CRASH_PROG;
		return false;
	}

	idxDevice->Release( );
	idxAdapter->Release( );
	idxFactory->Release( );

	return true;
}

bool GEngineDevice::CreateDepthStencilView( )
{
	ID3D11Texture2D* backBuffer2DTex = nullptr;
	if ( S_OK != mISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer2DTex) )
	{
		CRASH_PROG;
		return false;
	}

	// NULL to create a view that accesses all of the subresources in mipmap level 0
	if ( S_OK != mIDevice->CreateRenderTargetView(backBuffer2DTex, NULL, &mIBackBufferView) )
	{
		CRASH_PROG;
		return false;
	}

	if ( nullptr != backBuffer2DTex )
	{
		backBuffer2DTex->Release( );
		backBuffer2DTex = nullptr;
	}

	D3D11_TEXTURE2D_DESC texDesc = D3D11_TEXTURE2D_DESC( );
	texDesc.ArraySize = 1; // �ؽ�ó �迭�� �ؽ�ó ��
	texDesc.Width = (UINT)mTargetWindow->GetWndSize( ).m.x;
	texDesc.Height = (UINT)mTargetWindow->GetWndSize( ).m.y;
	// 3byte(24bit)�� depth ������ ����ϰ� �ڿ� 1byte(8bit)�� 0~255 ������ stencil ������ ����
	texDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
	// - if MSAA on
	//texDesc.SampleDesc.Count = 4; // The number of multisamples per pixel.
	//texDesc.SampleDesc.Quality = mMultiSamplingLV - 1;
	// - if MSAA off
	texDesc.SampleDesc.Count = 1; // The number of multisamples per pixel.
	texDesc.SampleDesc.Quality = 0; // multisampling quality level
	texDesc.MipLevels = 1; // MipMap
	texDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT; // �⺻ ���� -> �׷��� ī�忡�� ����
	// Bind a texture as a depth-stencil target for the output-merger stage.
	texDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;

	// ���ҽ��� ��Ƽ ���ø� �� ��� ��Ƽ ���ø� �� ���ҽ��� ���� ��
	// �����ͷ� �ʱ�ȭ �� �� �����Ƿ� pInitialData �� NULL �̾���մϴ� .
	mIDevice->CreateTexture2D(&texDesc, NULL, &mIDepthStencilTex);
	if ( nullptr == mIDepthStencilTex )
	{
		CRASH_PROG;
		return false;
	}

	// NULL -> ��ü ���ҽ��� mipmap 0 level�� �׼����ϴ� view�� ����
	mIDevice->CreateDepthStencilView(mIDepthStencilTex, NULL, &mIDepthStencilView);

	if ( nullptr == mIDepthStencilView )
	{
		CRASH_PROG;
		return false;
	}

	return true;
}

bool GEngineDevice::CreateViewPort( )
{
	mViewPortInfo.TopLeftX = 0;
	mViewPortInfo.TopLeftY = 0;

	mViewPortInfo.Width = mTargetWindow->GetWndSize( ).m.x;
	mViewPortInfo.Height = mTargetWindow->GetWndSize( ).m.y;
	mViewPortInfo.MinDepth = 0.0f;
	mViewPortInfo.MaxDepth = 1.0f;

	return true;
}
