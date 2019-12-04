#include "Monster.h"
#include "MonsterFSM.h"

Monster::Monster(GScene& _scene, const std::wstring& _imgName, const float imgCutSize)
	: mActor(nullptr), mSprite(nullptr), mAnimation(nullptr)
	, mDefaultAniSpeed(0.15f)
	, mDefaultJumpPower(1000.0f), mDefaultGravityAcceleration(1000.0f)
	, mGravityAcceleration(0.0f), mJumpPower(0.0f)
	, mImgCutSize(imgCutSize)
{
	mActor = _scene.CreateActor(_imgName);
	mImgName = _imgName; 

	Init( );
}

Monster::~Monster( )
{
	EMPTY_STATEMENT;
}

void Monster::Init( )
{
	//GResource::Load<GTexture>(GFileSystem::Join(GFileSystem::FindPathOrNull(L"ENEMY").GetAbsPath( ), mImgName));
	if ( nullptr == GResource::Find<GSprite>(mImgName) )
	{
		GResource::Create<GSprite>(mImgName, mImgName, mImgCutSize, mImgCutSize);
	}

	mActor->SetPos(0.0f);
	mActor->SetLocalScale({ 100.0f, 100.0f, 1.0f });
	//mActor->SetSceneAvailable(L"Town");

	mSprite = mActor->AddComponent<ComRender2DSprite>(RENDER_GROUP::GROUP_MONSTER);
	mSprite->SetSprite(mImgName);
}

void Monster::SetPos(const GVector4D& _pos)
{
	mActor->SetPos(_pos);
}

GVector4D Monster::GetPos( ) const
{
	return mActor->GetPos( );
}