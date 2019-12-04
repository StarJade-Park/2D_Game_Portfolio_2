#pragma once
#include <..\DxEngine\GScene.h>
#include <..\DxEngine\ComCamera.h>

#include <..\psklib\GameFileSystem.h>
#include <..\psklib\JsonPaser.h>
#include <..\psklib\GameFileIO.h>

#include <..\CommonLibrary\GTileRenderer.h>

#include "Rbrb.h"
#include "SandBag.h"
#include "OldBoss3.h"

class Erina;
class Boss3N;

class RbrbMap04
	: public GScene::GSceneUpdater
{
	enum __tag
	{
		COL = 7,
	};

public:
	RbrbMap04( );
	~RbrbMap04( );

	void LoadScene( ) override;
	void UpdateScene( ) override;
	void StartSceneChange( ) override;
	void EndSceneChange( ) override;

	void LoadJsonMap( );
	void SaveJsonMapToRbrb( );
	Rbrb ReadMapFile(const std::string& _path);

	void LoadTestMap( );

private:
	const std::wstring& tileImgName = L"TILE1_A.PNG";
	const std::wstring& colImgName = L"COLLISION_TILES.PNG";
	const std::wstring& playerImgName = L"PLAYER_A.PNG";

	SmartPtr<GActor> mActors[8];
	SmartPtr<GTileRenderer> mTestMap[8];
	SmartPtr<ComCamera> comCam;
	SmartPtr<GActor> mainCamera;
	SmartPtr<GActor> backGroundCam;
	Rbrb mRbrbMap[8];

	SmartPtr<GActor> titleBG;
	Erina* player;
	SandBag* sandBag;
	std::vector<OldBoss3*> vecOldBoss3;

	std::vector<Boss3N*> vecBoss3n;
};

