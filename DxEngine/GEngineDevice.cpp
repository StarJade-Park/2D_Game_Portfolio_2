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
	// 디버그시 플래그 설정
	iFlag = D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG;
#endif // DEBUG

	mTargetWindow = _targetWindow;

	// 바탕색 설정(파랑)
	mBackColor = GVector4D::Blue;

	HRESULT createResult = D3D11CreateDevice(
		nullptr,  // Device(ID3D11Device) 만들 때 넣을 부가 정보
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE, // 기반 그래픽 디바이스
		NULL, // ???
		iFlag, // device 생성시 사용할 비트 플래그
		NULL,  // pFeatureLevels is set to NULL, this function uses the following array of feature levels
		0, // The number of elements in pFeatureLevels 
		D3D11_SDK_VERSION, // DX SDK 버전
		&mIDevice, // 생성된 device(ID3D11Device) 반환 받을 포인터
		&meFeatureLV, // 성공했을 경우, 성공한 pFeatureLevels 배열로부터 최초의 D3D_FEATURE_LEVEL를 돌려줍니다.
		&mIContext // 생성된 context(ID3D11DeviceContext) 반환 받을 포인터
	);

	if ( S_OK != createResult )
	{
		CRASH_PROG;
		return;
	}

	// 기준이 되는 색상 format을 DXGI_FORMAT_R8G8B8A8_UNORM으로 설정
	// 매개변수 순서는 textrue format, multisampling의 sample 수, adapter가 지원하는 품질 level
	// 하나의 텍스쳐 형식, 표본 개수 조합에 대한 유요한 품질 수준의 범위는 '0 ~ pNumQualityLevels - 1'
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
		// back buffer를 background color로 clear
		mIContext->ClearRenderTargetView(mIBackBufferView, mBackColor.Arr);
		// depth buffer를 1.0f로 stencil buffer를 0으로 clear
		mIContext->ClearDepthStencilView(mIDepthStencilView, D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH | D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}

void GEngineDevice::Present( )
{
	if ( nullptr != mISwapChain )
	{
		// 1. 수직 프레임과 프레임의 표시를 동기화하는 방법을 지정
		// 2. 스왑 체인 표시 옵션(출력에 프레임을 표시하기 위한 옵션)
		mISwapChain->Present(0, 0);
	}
}

void GEngineDevice::SetDefaultRenderTarget( )
{
	if ( nullptr != mIContext && nullptr != mIDepthStencilView )
	{
		// 그려야 할 object 색, 정보를 지정
		mIContext->OMSetRenderTargets(1, &mIBackBufferView, mIDepthStencilView);
	}
}

void GEngineDevice::SetDefaultViewport( )
{
	if ( nullptr != mIContext )
	{
		// 바인딩 할 뷰포트의 수와 viewport struct를 넣어준다.
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

// - mISwapChain을 설정하는 함수
bool GEngineDevice::CreateSwapChain( )
{
	DXGI_SWAP_CHAIN_DESC swapChainInfo = DXGI_SWAP_CHAIN_DESC( ); // DESC = description, 정보 struct

	// buffer desc size
	swapChainInfo.BufferDesc.Width = (UINT)mTargetWindow->GetWndSize( ).m.x;
	swapChainInfo.BufferDesc.Height = (UINT)mTargetWindow->GetWndSize( ).m.y;

	// mointer frame rate(Hz)
	swapChainInfo.BufferDesc.RefreshRate.Numerator = __tag::HZ_Numerator;
	swapChainInfo.BufferDesc.RefreshRate.Denominator = __tag::HZ_Denominator;

	// 색상 format을 DXGI_FORMAT_R8G8B8A8_UNORM
	// 스캔 라인 설정
	// 지정된 모니터의 해상도에 맞추어 이미지가 늘어나는 방법을 지정
	swapChainInfo.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainInfo.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainInfo.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;

	// buffer 용도 -> for render(target output)
	swapChainInfo.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // dxgi.h define

	// - if MSAA on
	//swapChainInfo.SampleDesc.Count = 4; // The number of multisamples per pixel.
	//swapChainInfo.SampleDesc.Quality = mMultiSamplingLV - 1;
	// - if MSAA off
	swapChainInfo.SampleDesc.Count = 1; // The number of multisamples per pixel.
	swapChainInfo.SampleDesc.Quality = 0;
	// set output window
	swapChainInfo.OutputWindow = mTargetWindow->GetHWnd( );
	swapChainInfo.BufferCount = 2; // 사용할 버퍼 수

	// swap 방식 선택
	// flip은 UWP 이식시 GPU 성능 보장을 위함
	// DXGI가 백 버퍼의 내용을 버리도록 지정
	// This flag cannot be used with multisampling and partial presentation.
	swapChainInfo.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainInfo.Windowed = !mTargetWindow->IsFullScr( );
	// 참조: enum DXGI_SWAP_CHAIN_FLAG
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
	texDesc.ArraySize = 1; // 텍스처 배열의 텍스처 수
	texDesc.Width = (UINT)mTargetWindow->GetWndSize( ).m.x;
	texDesc.Height = (UINT)mTargetWindow->GetWndSize( ).m.y;
	// 3byte(24bit)는 depth 값으로 사용하고 뒤에 1byte(8bit)는 0~255 사이의 stencil 값으로 쓴다
	texDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
	// - if MSAA on
	//texDesc.SampleDesc.Count = 4; // The number of multisamples per pixel.
	//texDesc.SampleDesc.Quality = mMultiSamplingLV - 1;
	// - if MSAA off
	texDesc.SampleDesc.Count = 1; // The number of multisamples per pixel.
	texDesc.SampleDesc.Quality = 0; // multisampling quality level
	texDesc.MipLevels = 1; // MipMap
	texDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT; // 기본 설정 -> 그래픽 카드에서 관리
	// Bind a texture as a depth-stencil target for the output-merger stage.
	texDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;

	// 리소스가 멀티 샘플링 된 경우 멀티 샘플링 된 리소스를 만들 때
	// 데이터로 초기화 할 수 없으므로 pInitialData 는 NULL 이어야합니다 .
	mIDevice->CreateTexture2D(&texDesc, NULL, &mIDepthStencilTex);
	if ( nullptr == mIDepthStencilTex )
	{
		CRASH_PROG;
		return false;
	}

	// NULL -> 전체 리소스의 mipmap 0 level에 액세스하는 view를 생성
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
