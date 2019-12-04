#pragma once
#include <vector>
#include <..\psklib\SmartPtr.h>
#include <..\psklib\MyMath.h>

class GTexture;
class GSprite;
class GScene;
class GActor;

class Background
{
public:
	Background(SmartPtr<GScene> _curScene, const std::wstring& _imgName);
	~Background( );

	GVector4D GetLocalScale( ) const;
	void SetLocalScale(const GVector4D& _scale);
	GVector4D GetPos( ) const;
	void SetLocalPos(const GVector4D& _pos);

private:
	void Initailize( );
	void InitResource( );
	void InitComopnent( );

	void ChangeImage(const std::wstring& _imgName);

public:
	SmartPtr<GActor> mActor;
	SmartPtr<GTexture> mTexture;
	SmartPtr<GSprite> mSprite;
	SmartPtr<GScene> mCurScene;
	ComRender2DSprite* mImgRender;
	std::wstring mImgName;
};

