#include <..\DxEngine\GResource.h>
#include <..\DxEngine\GActor.h>
#include <..\DxEngine\GScene.h>
#include <..\DxEngine\ComRender2DSprite.h>
#include <..\DxEngine\GTexture.h>
#include <..\DxEngine\GSprite.h>

#include "Background.h"
#include <..\CommonLibrary\GameOption.h>

Background::Background(SmartPtr<GScene> _curScene, const std::wstring& _imgName)
	: mImgName(_imgName)
	, mActor(nullptr), mCurScene(_curScene)
	, mTexture(nullptr), mSprite(nullptr), mImgRender(nullptr)
{
	mActor = mCurScene->CreateActor(mImgName);

	Initailize( );
}

Background::~Background( )
{
	EMPTY_STATEMENT;
}

void Background::Initailize( )
{
	InitResource( );
	InitComopnent( );

	mActor->SetPos({ 0.0f, 0.0f, 75.0f });
	mActor->SetLocalScale({ GameOption::instance.GetWndWidth( ), GameOption::instance.GetWndHeight( ) });
}

GVector4D Background::GetLocalScale( ) const
{
	return mActor->GetLocalScale( );
}

void Background::SetLocalScale(const GVector4D& _scale)
{
	mActor->SetLocalScale(_scale);
}

GVector4D Background::GetPos( ) const
{
	return mActor->GetPos( );
}

void Background::SetLocalPos(const GVector4D& _pos)
{
	mActor->SetPos(_pos);
}

void Background::InitResource( )
{
	// create sprite
	mSprite = GResource::Create<GSprite>(mImgName, mImgName, 2560.f, 720.f);
	mTexture = &mSprite->GetTexture( );
}

void Background::InitComopnent( )
{
	mImgRender = mActor->AddComponent<ComRender2DSprite>(RENDER_GROUP::GROUP_BACKGROUND);
	mImgRender->SetSprite(mImgName, 0);
	mImgRender->SetLocalPos(GVector4D::vBack * 0.f);

	mImgRender = mActor->AddComponent<ComRender2DSprite>(RENDER_GROUP::GROUP_BACKGROUND);
	mImgRender->SetSprite(mImgName, 1);
	mImgRender->SetLocalPos(GVector4D::vBack * 1.f);

	mImgRender = mActor->AddComponent<ComRender2DSprite>(RENDER_GROUP::GROUP_BACKGROUND);
	mImgRender->SetSprite(mImgName, 2);
	mImgRender->SetLocalPos(GVector4D::vBack * 2.f);
}

void Background::ChangeImage(const std::wstring& _imgName)
{
	mImgRender->SetSprite(_imgName);
}
