#include <..\CommonLibrary\GameOption.h>

#include <..\DxEngine\ComRender2DSprite.h>
#include <..\DxEngine\DebugRenderer.h>
#include <..\DxEngine\ComCollider.h>
#include <..\DxEngine\GSound.h>

#include <..\psklib\GameKeyInput.h>
#include <..\psklib\GameTime.h>
#include "RbrbMap04.h"
#include "CameraMoveAble.h"

#include "Erina.h"
#include "ErinaFSM.h"
#include "Background.h"
#include "SandBagFSM.h"
#include "OldBoss3FSM.h"
#include "Boss3N.h"
#include "Boss3N_FSM.h"

#include <..\CommonLibrary\RbrbRoom.h>

// note z 축 그리는 것이 scale에 영향을 받을 수 있음

RbrbMap04::RbrbMap04( )
	: mRbrbMap( ), mTestMap( ), comCam( ), mActors( ), mainCamera( )
	, sandBag(nullptr)
{
	RbrbRoom::SRoom.SRoomInit( );
}

RbrbMap04::~RbrbMap04( )
{
	if ( nullptr != player )
	{
		delete player;
		player = nullptr;
	}

	if ( nullptr != sandBag )
	{
		delete sandBag;
		sandBag = nullptr;
	}

	for ( auto& oldBoss : vecOldBoss3 )
	{
		delete oldBoss;
		oldBoss = nullptr;
	}
	vecOldBoss3.clear( );

	for ( auto& boss3n : vecBoss3n )
	{
		delete boss3n;
		boss3n = nullptr;
	}
	vecBoss3n.clear( );
}

void RbrbMap04::LoadScene( )
{
	SaveJsonMapToRbrb( );
	LoadJsonMap( );

	//GetScene( ).mCollisionMGR.LinkGroup(COLLIDER_GROUP::COL_PLAYER, COLLIDER_GROUP::COL_MONSTER);
	GetScene( ).mCollisionMGR.LinkGroup(COLLIDER_GROUP::COL_MONSTER, COLLIDER_GROUP::COL_PLAYER_ATTCK);

	//GetScene( ).mCollisionMGR.LinkGroup(COLLIDER_GROUP::COL_PLAYER_ATTCK, COLLIDER_GROUP::COL_MONSTER);

	// texture load
	//GResource::Load<GTexture>(GFileSystem::Join(GFileSystem::FindPathOrNull(L"TILE").GetAbsPath( ), tileImgName));
	//GResource::Load<GTexture>(GFileSystem::Join(GFileSystem::FindPathOrNull(L"TILE").GetAbsPath( ), colImgName));
	//GResource::Load<GTexture>(GFileSystem::Join(GFileSystem::FindPathOrNull(L"BG").GetAbsPath( ), L"BONUS_A.PNG"));
	//GResource::Load<GTexture>(GFileSystem::Join(GFileSystem::FindPathOrNull(L"FONT").GetAbsPath( ), L"DAMAGE2_A.PNG"));

	//GResource::Load<GTexture>(GFileSystem::Join(GFileSystem::FindPathOrNull(L"MAIN").GetAbsPath( ), playerImgName));
	//GResource::Load<GTexture>(GFileSystem::Join(GFileSystem::FindPathOrNull(L"MAIN").GetAbsPath( ), L"HAMMER_A.PNG"));

	// create sprite
	SmartPtr<GSprite> tileSprite = GResource::Create<GSprite>(tileImgName, tileImgName, 32.0f, 32.0f);
	GResource::Create<GSprite>(L"DAMAGE2_A.PNG", L"DAMAGE2_A.PNG", 5.0f, 8.0f);

	/*SmartPtr<GSprite> tileSprite = */GResource::Create<GSprite>(colImgName, colImgName, 32.0f, 32.0f);

	/*********** camera ***********/
	{
		mainCamera = GetScene( ).CreateActor(L"mainCamera");
		mainCamera->GetTransform( ).TranslateLocal(GVector4D(40.0f * (240.0f - 1.0f), 40.0f * (200.f - 95.f), -100.0f));
		//mainCamera->AddComponent<CameraMoveAble>( );
		comCam = mainCamera->AddComponent<ComCamera>(
			RENDER_GROUP::GROUP_ETC_OBJECT, RENDER_GROUP::GROUP_PLAYER,
			RENDER_GROUP::GROUP_PLAYER_FX, RENDER_GROUP::GROUP_MONSTER
			, RENDER_GROUP::GROUP_BULLET
			);
		comCam->SetProjMode(PROJECTION_MODE::ORTHOGONAL);
		//comCam->SetWidthAndHeight(GameOption::instance.GetWndWidth( ), GameOption::instance.GetWndHeight( ));
		comCam->SetWidthAndHeight(40.0f * 20.0f, 40.0f * 11.0f);
	}

	{
		backGroundCam = GetScene( ).CreateActor(L"backGroundCam");
		backGroundCam->GetTransform( ).SetWorldPos(GVector4D(0.f, 0.f, -5.f));
		comCam = backGroundCam->AddComponent<ComCamera>(RENDER_GROUP::GROUP_BACKGROUND/*, RENDER_GROUP::GROUP_BULLET*/);
		comCam->SetProjMode(PROJECTION_MODE::PERSPECTIVE);
		comCam->SetWidthAndHeight(GameOption::instance.GetWidthAspectRatio( ), GameOption::instance.GetHeightAspectRatio( ));
	}

	/************** back ground **************/
	Background bg = Background(&GetScene( ), L"TITLE_A.PNG");

	//comCam->SetWidthAndHeight(32.0f * 500.0f * 2.0f, 32.0f * 200.0f* 2.0f);

	/************** game objects **************/
	// todo 필요한 만큼 배치하기
	player = new Erina(GetScene( ));
	//player->SetPos(GVector4D(40.0f * (430.f - 1.0f), 40.0f * (200.f - 37.f), -100.0f));
	player->SetPos(GVector4D(40.0f * (84.f - 1.0f), 40.0f * (200.f - 97.f), -100.0f));

	sandBag = new SandBag(GetScene( ), L"SANDBAG_A.PNG");
	sandBag->SetPos(GVector4D(40.0f * (90.f - 1.0f), 40.0f * (200.f - 97.f), -100.0f));
	
	{
		auto oldBoss3 = new OldBoss3(GetScene( ), L"OLDBOSS3_A.PNG");
		oldBoss3->SetPos(GVector4D(40.0f * (112.f - 1.0f), 40.0f * (200.f - 96.f), -100.0f));
		vecOldBoss3.push_back(oldBoss3);
	}

	{
		auto oldBoss3 = new OldBoss3(GetScene( ), L"OLDBOSS3_A.PNG");
		oldBoss3->SetPos(GVector4D(40.0f * (112.f - 1.0f), 40.0f * (200.f - 96.f), -100.0f));
		vecOldBoss3.push_back(oldBoss3);
	}

	{
		auto oldBoss3 = new OldBoss3(GetScene( ), L"OLDBOSS3_A.PNG");
		oldBoss3->SetPos(GVector4D(40.0f * (112.f - 1.0f), 40.0f * (200.f - 96.f), -100.0f));
		vecOldBoss3.push_back(oldBoss3);
	}

	auto boss3n = new Boss3N(GetScene( ), L"BOSS3N_A.PNG");
	boss3n->SetPos(GVector4D(40.0f * (112.f - 1.0f), 40.0f * (200.f - 96.f), -100.0f));
	vecBoss3n.push_back(boss3n);

	auto boss3n2 = new Boss3N(GetScene( ), L"BOSS3N_A.PNG");
	boss3n2->SetPos(GVector4D(40.0f * (112.f - 1.0f), 40.0f * (200.f - 96.f), -100.0f));
	vecBoss3n.push_back(boss3n2);

	/************** tile map load & set up **************/
	LoadTestMap( );

	for ( auto& map : mTestMap )
	{
		map->SetCenterPos({ mainCamera->GetTransform( ).GetLocalPos( ).m.x / 40.0f, mainCamera->GetTransform( ).GetLocalPos( ).m.y / 40.0f });
	}

	//titleBG = GetScene( ).CreateActor(L"Main Menu BG");
	//titleBG->GetTransform( ).TranslateLocal(GVector4D(40.0f * 229.0f, 40.0f * 90.0f));
	//titleBG->GetTransform( ).SetLocalScale({ 100.0f, 100.0f, 1.0f });
	//SmartPtr<ComRender2DSprite> testRender = titleBG->AddComponent<ComRender2DSprite>(RENDER_GROUP::GROUP_ETC_OBJECT);
	//testRender->SetSprite(tileImgName, 558);

	//test.SetCurRoom(0, 0);
	//mainCamera->GetTransform( ).SetLocalPos(test.GetCurRoomCenterPos( ));
	GameTime::Reset( );
	GSound::Play(L"BGM",L"BGM11.OGG", 128);
}

void RbrbMap04::UpdateScene( )
{
	DebugRenderer::DrawFontUI(L"FPS : " + std::to_wstring(1.0f / GameTime::DeltaTime( )));

	if ( true == GameKeyInput::Down(L"Debug") )
	{
		DebugRenderer::SwitchDebugMode( );
	}

	GVector4D pos = player->GetActor( ).GetPos( );
	RbrbRoom::SRoom.SetCurRoom(pos);
	auto room = RbrbRoom::SRoom.GetCurRoomData( );

	mainCamera->GetTransform( ).SetLocalPos(room.centerPos);

	for ( auto& map : mTestMap )
	{
		map->SetCenterPos({ mainCamera->GetTransform( ).GetLocalPos( ).m.x / 40.0f, mainCamera->GetTransform( ).GetLocalPos( ).m.y / 40.0f });
	}

	if ( true == DebugRenderer::IsOnDebugMode( ) )
	{
		DebugRenderer::DrawFontUI(DebugRenderer::IsOnDebugMode( ) ? L"DEBUG_MODE : true" : L"DEBUG_MODE : false");

		std::wstring str = L"Room Index X : ";
		DebugRenderer::DrawFontUI(str.append(std::to_wstring(room.indexX)).append(L", Y : ").append(std::to_wstring(room.indexY)));
		DebugRenderer::DrawVectorUI(room.centerPos, GVector4D::Magenta);
	}
}

void RbrbMap04::StartSceneChange( )
{
}

void RbrbMap04::EndSceneChange( )
{
}


void RbrbMap04::LoadJsonMap( )
{
	std::wstring binPath = GFileSystem::FindPathOrNull(L"BIN").GetAbsPath( );
	std::string loadPath(binPath.begin( ), binPath.end( ));
	loadPath.append("\\MapData\\");
	std::string loadExt = ".rbrb";

	for ( auto& m : mRbrbMap )
	{
		m.mData.reserve(rbrb_data_max_x_size * rbrb_data_max_y_size);
	}

	for ( size_t idx = 0; idx < 7; ++idx )
	{
		mRbrbMap[idx] = ReadMapFile(loadPath + std::string("tiles") + std::to_string(idx) + loadExt);
		mRbrbMap[idx].mTileLayerName = std::string("tiles") + std::to_string(idx);
	}

	mRbrbMap[__tag::COL] = ReadMapFile(loadPath + std::string("collision") + loadExt);
	mRbrbMap[__tag::COL].mTileLayerName = "collision";
}

void RbrbMap04::SaveJsonMapToRbrb( )
{
	JsonPaser parser;
	parser.OpenJsonFile( );

	// layers
	auto arr = parser.GetArrayInRoot("layers");
	assert(arr[9].IsObject( ));

	std::string tileTypes[] = { "tilelayer", "objectgroup" };
	std::string layerTileTypes[] = { "opacity", "width", "name", "visible", "y", "x", "type", "data", "height" };
	std::string layerObjectTypes[] = { "opacity", "visible", "objects", "locked", "name", "draworder", "color", "x", "type", "x" };

	std::wstring binPath = GFileSystem::FindPathOrNull(L"BIN").GetAbsPath( );
	std::string savePath(binPath.begin( ), binPath.end( ));
	savePath.append("\\MapData\\");
	std::string saveExt = ".rbrb";

	// layers
	for ( auto& obj : arr )
	{
		// save file 준비
		if ( tileTypes->compare(obj["type"].GetString( )) )
		{
			continue;
		}

		std::ofstream tileBin((savePath + std::string(obj["name"].GetString( )) + saveExt).c_str( ), std::ios::binary | std::ios::trunc);
		assert(tileBin.is_open( ));

		// layer arr[i]
		for ( auto& m : obj.GetObject( ) ) // note 정적 분석 도구가 wingdi와 rapidjson을 헷갈려 함
		{
			switch ( m.value.GetType( ) )
			{
			case rapidjson::kFalseType:
				fio::write(tileBin, false);
				break;
			case rapidjson::kTrueType:
				fio::write(tileBin, true);
				break;
			case rapidjson::kArrayType:
				if ( 0 != layerTileTypes[7].compare(m.name.GetString( )) )
				{
					break;
				}

				for ( auto& member : m.value.GetArray( ) )
				{
					fio::write(tileBin, member.GetUint( ));
				}

				break;

			case rapidjson::kNumberType:
				fio::write(tileBin, m.value.GetFloat( ));
				break;
			case rapidjson::kNullType:		FALL_THROUGH;
			case rapidjson::kObjectType:	FALL_THROUGH;
			case rapidjson::kStringType:	FALL_THROUGH;
			default:
				EMPTY_STATEMENT;
				break;
			}
		}
	}
}

Rbrb RbrbMap04::ReadMapFile(const std::string& _path)
{
	Rbrb result = Rbrb( );

	std::ifstream tileData(_path, std::ios::binary);
	assert(tileData.is_open( ));

	result.mTileLayerType = "tilelayer";

	fio::read(tileData, result.mAlphaValue);
	fio::read(tileData, result.mWidth);
	fio::read(tileData, result.mbVisible);
	fio::read(tileData, result.mStartPosX);
	fio::read(tileData, result.mStartPosY);

	// read tile data
	UINT data = 0;
	for ( size_t size = 0; size < rbrb_data_max_x_size * rbrb_data_max_y_size; ++size )
	{
		fio::read(tileData, data);
		result.mData.push_back(data);
	}

	fio::read(tileData, result.mHeight);

	return result;
}

void RbrbMap04::LoadTestMap( )
{
	for ( size_t index = 0; index < 8; ++index )
	{
		mActors[index] = GetScene( ).CreateActor( );
		mActors[index]->GetTransform( ).SetLocalScaleByRatio(GameOption::instance.GetAspectRatio( ) * 0.5f);

		mTestMap[index] = mActors[index]->AddComponent<GTileRenderer>(RENDER_GROUP::GROUP_ETC_OBJECT);
		mTestMap[index]->SetMaxSizeOfMap(rbrb_data_max_x_size, rbrb_data_max_y_size);
		mTestMap[index]->SetTileSprite(tileImgName);
		mTestMap[index]->SetMoveSize({ 1.0f, 1.0f });
	}

	// tile layer order => 2,6,5,1,4,3,0,col
	mActors[2]->GetTransform( ).SetLocalPos({ 0.0f, 0.0f, -20.0f });
	mActors[6]->GetTransform( ).SetLocalPos({ 0.0f, 0.0f, -10.0f });
	mActors[5]->GetTransform( ).SetLocalPos({ 0.0f, 0.0f, 10.0f });
	mActors[1]->GetTransform( ).SetLocalPos({ 0.0f, 0.0f, 20.0f });
	mActors[4]->GetTransform( ).SetLocalPos({ 0.0f, 0.0f, 30.0f });
	mActors[3]->GetTransform( ).SetLocalPos({ 0.0f, 0.0f, 40.0f });
	mActors[0]->GetTransform( ).SetLocalPos({ 0.0f, 0.0f, 50.0f });
	mActors[__tag::COL]->GetTransform( ).SetLocalPos({ 0.0f, 0.0f, 100.0f });

	//mTestMap[__tag::COL]->SetTileSprite(colImgName);

	//int index = 0;
	for ( int index = 0; index < 7; ++index )
	{
		for ( int x = 0, maxX = (int)mTestMap[index]->GetMaxSizeX( ); x < maxX; ++x )
		{
			for ( int y = 0, maxY = (int)mTestMap[index]->GetMaxSizeY(x); y < maxY; ++y )
			{
				mTestMap[index]->AddTile(
					std::make_pair(x, maxY - (y + 1)),
					mRbrbMap[index].mData[x + (y * 500)],
					mRbrbMap[index].mbVisible,
					mRbrbMap[index].mAlphaValue
				);
			}
		}
	}

	for ( int x = 0, maxX = (int)mTestMap[__tag::COL]->GetMaxSizeX( ); x < maxX; ++x )
	{
		for ( int y = 0, maxY = (int)mTestMap[__tag::COL]->GetMaxSizeY(x); y < maxY; ++y )
		{
			auto test = mRbrbMap[__tag::COL].mData[x + (y * 500)];
			if ( 5000 < test )
			{
				int a = 0;
			}

			mTestMap[__tag::COL]->AddColTile(
				std::make_pair(x, maxY - (y + 1)),
				mRbrbMap[__tag::COL].mData[x + (y * 500)] % 5000,
				false, // note visible flag
				mRbrbMap[__tag::COL].mAlphaValue
			);
		}
	}

	ErinaFSM* fsm = player->GetActor( ).GetComponentOrNull<ErinaFSM>( );
	fsm->SetColTiles(mTestMap[__tag::COL]);

	SandBagFSM* sandBagFSM = sandBag->GetActor( ).GetComponentOrNull<SandBagFSM>( );
	sandBagFSM->SetColTiles(mTestMap[__tag::COL]);
	sandBagFSM->SetRoomIndex(4, 9);

	for ( auto& oldBoss : vecOldBoss3 )
	{
		OldBoss3FSM* oldBoss3FSM = oldBoss->GetActor( ).GetComponentOrNull<OldBoss3FSM>( );
		oldBoss3FSM->SetColTiles(mTestMap[__tag::COL]);
		oldBoss3FSM->SetRoomIndex(5, 9);
	}

	Boss3NFSM* boss3nfsm = vecBoss3n[0]->GetActor( ).GetComponentOrNull<Boss3NFSM>( );
	boss3nfsm->SetColTiles(mTestMap[__tag::COL]);
	boss3nfsm->SetRoomIndex(5, 9);

	Boss3NFSM* boss3nfsm2 = vecBoss3n[1]->GetActor( ).GetComponentOrNull<Boss3NFSM>( );
	boss3nfsm2->SetColTiles(mTestMap[__tag::COL]);
	boss3nfsm2->SetRoomIndex(5, 9);
}
