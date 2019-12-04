#pragma once
#include <..\DxEngine\GScene.h>

class GActor;

class TownUpdater
	: public GScene::GSceneUpdater
{
public:
	TownUpdater( );
	~TownUpdater( );

	void LoadScene( ) override;
	void UpdateScene( ) override;
	void StartSceneChange( ) override;
	void EndSceneChange( ) override;

private:
	SmartPtr<GActor> mMainCamera;
};

