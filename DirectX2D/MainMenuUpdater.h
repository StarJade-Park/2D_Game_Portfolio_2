#pragma once
#include <..\DxEngine\GScene.h>

class MainMenuUpdater
	: public GScene::GSceneUpdater
{
public:
	MainMenuUpdater( );
	~MainMenuUpdater( );

private:
	void LoadScene( ) override;
	void UpdateScene( ) override;
	void StartSceneChange( ) override;
	void EndSceneChange( ) override;
};

