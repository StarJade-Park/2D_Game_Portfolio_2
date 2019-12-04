#include <..\psklib\GameTime.h>
#include <..\psklib\Well512Random.h>

#include <..\DxEngine\GScene.h>
#include <..\DxEngine\ComRender2DSprite.h>

#include "UIDamage.h"

UIDamage::UIDamage( )
	: mNormalSizeTime(1.0f), mbR2G(false), mbG2B(false), mbB2R(false)
	, mOffsetX(0), mOffsetY(0), mColor(0.0f), mVecOfDamage( ), mDamageSizeRatio(0.4f)
{
	EMPTY_STATEMENT;
}

UIDamage::~UIDamage( )
{
	EMPTY_STATEMENT;
}

void UIDamage::AddDamage(const UINT _damage)
{
	FindingDigits(_damage);
	float a = Well512Random::Instance( ).GetFloatValue(0.20f, 0.40f);

	for ( size_t idx = 0; idx < mVecOfDamage.size( ); ++idx )
	{
		auto actor = GetScene( ).CreateActor(L"DIGIT");
		actor->SetParentActor(&GetActor( ));
		actor->SetLocalScaleByRatio(mDamageSizeRatio);
		actor->SetPos((GVector4D::vRight * a) + (GVector4D::vUp * 0.5f));
		mVecOfDamageActors.push_back(actor);

		auto sprite = actor->AddComponent<ComRender2DSprite>(RENDER_GROUP::GROUP_PLAYER_HAMMER);
		sprite->SetSprite(L"DAMAGE2_A.PNG");
		sprite->SetSpriteIdx(mVecOfDamage[idx]);
		sprite->SetLocalPos(GVector4D::vLeft * (float)idx);
	}
}

void UIDamage::Init( )
{
	int rgbStart = Well512Random::Instance( ).GetValue(0, 2 + 1);

	mColor.c.a = 1.0f;

	switch ( rgbStart )
	{
	case 0:
		mColor.c.r = color_max;
		mColor.c.g = color_min;
		mColor.c.b = color_min;
		break;
	case 1:
		mColor.c.r = color_min;
		mColor.c.g = color_max;
		mColor.c.b = color_min;
		break;
	case 2:
		mColor.c.r = color_min;
		mColor.c.g = color_min;
		mColor.c.b = color_max;
		break;
	default:
		CRASH_PROG;
		break;
	}
}

void UIDamage::Update( )
{
	if ( mColor.c.r >= color_max && mColor.c.g <= color_min && mColor.c.b <= color_min )
	{
		mbR2G = true;
		mbG2B = false;
		mbB2R = false;
	}
	else if ( mColor.c.r <= color_min && mColor.c.g >= color_max && mColor.c.b <= color_min )
	{
		mbR2G = false;
		mbG2B = true;
		mbB2R = false;
	}
	else if ( mColor.c.r <= color_min && mColor.c.g <= color_min && mColor.c.b >= color_max )
	{
		mbR2G = false;
		mbG2B = false;
		mbB2R = true;
	}

	if ( true == mbR2G )
	{
		mColor.c.r -= GameTime::DeltaTime( );
		mColor.c.g += GameTime::DeltaTime( );

		if ( color_min >= mColor.c.r )
		{
			mColor.c.r = color_min;
		}

		if ( color_max <= mColor.c.g )
		{
			mColor.c.g = color_max;
		}
	}

	if ( true == mbG2B )
	{
		mColor.c.g -= GameTime::DeltaTime( );
		mColor.c.b += GameTime::DeltaTime( );

		if ( color_min >= mColor.c.g )
		{
			mColor.c.g = color_min;
		}

		if ( color_max <= mColor.c.b )
		{
			mColor.c.b = color_max;
		}
	}

	if ( true == mbB2R )
	{
		mColor.c.b -= GameTime::DeltaTime( );
		mColor.c.r += GameTime::DeltaTime( );

		if ( color_min >= mColor.c.b )
		{
			mColor.c.b = color_min;
		}

		if ( color_max <= mColor.c.r )
		{
			mColor.c.r = color_max;
		}
	}

	mColor += 0.01f;

	mDamageSizeRatio -= GameTime::DeltaTime( );

	for ( auto& actor : mVecOfDamageActors )
	{
		if ( 0.2f <= mDamageSizeRatio )
		{
			actor->SetLocalScale(mDamageSizeRatio);
		}
		else
		{
			mbScaleStart = true;
		}

		auto sprite = actor->GetComponentOrNull<ComRender2DSprite>( );

		sprite->SetColor(mColor);
	}

	if ( true == mbScaleStart )
	{
		mNormalSizeTime -= GameTime::DeltaTime( );

		for ( auto& actor : mVecOfDamageActors )
		{
			actor->SetPos(actor->GetPos( ) + (GVector4D::vUp * GameTime::DeltaTime( ) * 2.0f));
		}
	}

	if ( 0.0f >= mNormalSizeTime )
	{
		mDamageSizeRatio -= GameTime::DeltaTime( );

		for ( auto& actor : mVecOfDamageActors )
		{
			actor->SetLocalScale(mDamageSizeRatio);
		}
	}

	if ( 0.0f >= mDamageSizeRatio )
	{
		for ( auto& actor : mVecOfDamageActors )
		{
			actor->Death( );
		}

		mVecOfDamageActors.clear( );
	}
}

void UIDamage::FindingDigits(const UINT _damage)
{
	UINT temp = _damage;
	mDamage = _damage;

	if ( 0 == temp )
	{
		mVecOfDamage.push_back(0);
		return;
	}

	for ( UINT idx = 0; 0 != temp; ++idx )
	{
		int num = temp % 10;
		mVecOfDamage.push_back(num);
		temp /= 10;
	}
}
