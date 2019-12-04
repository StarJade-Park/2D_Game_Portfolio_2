#pragma once
#include <..\psklib\MyMath.h>

#include "BaseGComponent.h"

class ComTransform
	: public BaseGComponent
{
	friend class GActor;
public:
	ComTransform( )
		: mvLocalScale(1.0f), mvLocalRotation(GVector4D( )), mvLocalPosition(GVector4D( ))
		, mMatLocalScale(GMatrix( ))
		, mMatLocalRotation(GMatrix( ))
		, mMatLocalPosition(GMatrix( )), mMatParentWorld(GMatrix( ))
		, mParentTransform(nullptr)
		, mbRotationFlag(false), mbScaleFlag(false), mbPositionFlag(false)
		, mvWorldScale(GVector4D( ))
		, mvWorldWRotation(GVector4D( ))
		, mvWorldPosition(GVector4D( ))
	{
		EMPTY_STATEMENT;
	}

	~ComTransform( ) = default;

	void Init( ) override;
	void RenderPreprocess( ) override;
	void RenderPostprocess( ) override;

#pragma region GetSet
	// - 위치 이동
	void SetLocalPos(const GVector4D& _pos)
	{
		TranslateLocal(_pos);
	}

	GVector4D GetLocalPos( ) const
	{
		return mvLocalPosition;
	}

	// - 위치 이동
	void TranslateLocal(const GVector4D& _pos)
	{
		mvLocalPosition = _pos;
		mbPositionFlag = true;
	}

	GVector4D GetLocalScale( ) const
	{
		return mvLocalScale;
	}

	void SetLocalScale(const GVector4D& _scale)
	{
		mvLocalScale = _scale;
		mbScaleFlag = true;
	}

	void SetLocalScaleAddScala(const GVector4D& _scale)
	{
		mvLocalScale.m.x =_scale.m.x;
		mvLocalScale.m.y =_scale.m.y;
		mbScaleFlag = true;
	}

	void SetLocalScaleByRatio(const float _ratio)
	{
		mvLocalScale *= _ratio;
		mbScaleFlag = true;
	}

	GVector4D GetLocalRotation( ) const
	{
		return mvLocalRotation;
	}

	void SetLocalRotation(const GVector4D& _rotation)
	{
		mvLocalRotation = _rotation;
		mbRotationFlag = true;
	}

	GVector4D GetForwardVectorLH( )
	{
		return mMatLocalRotation.FowordVector( );
	}

	GVector4D GetUpVectorLH( )
	{
		return mMatLocalRotation.UpVector( );
	}

	GVector4D GetRightVectorLH( )
	{
		return mMatLocalRotation.RightVector( );
	}

	/***********************************************************************/
	GMatrix GetWorldMatrix( ) const
	{
		return mMatWorld;
	}

	void SetWorldPos(const GVector4D& _pos)
	{
		TranslateWorld(_pos);
	}

	GVector4D GetWorldPos( ) const
	{
		return mvWorldPosition;
	}

	void TranslateWorld(const GVector4D& _pos)
	{
		if ( nullptr == mParentTransform )
		{
			mvLocalPosition = _pos;
			mbPositionFlag = true;
			return;
		}

		GVector4D PScale = mParentTransform->GetLocalScale( );
		GVector4D PivotPos = _pos - mParentTransform->GetLocalPos( );

		mvWorldPosition = _pos;
		mvLocalPosition = GVector4D(
			PScale.m.x != 0.0f ? (PivotPos.m.x / PScale.m.x) : PivotPos.m.x, 
			PScale.m.y != 0.0f ? (PivotPos.m.y / PScale.m.y) : PivotPos.m.y, 
			PScale.m.z != 0.0f ? (PivotPos.m.z / PScale.m.z) : PivotPos.m.z
		);

		mbPositionFlag = true;
	}

	GVector4D GetWorldScale( ) const
	{
		return mvWorldScale;
	}

	void SetWorldScale(const GVector4D& _scale)
	{
		if ( nullptr == mParentTransform )
		{
			mvLocalScale = _scale;
			mbScaleFlag = true;
			return;
		}

		GVector4D ParentScale = mParentTransform->GetLocalScale( );
		mvWorldScale = _scale;
		mvLocalScale = GVector4D(
			_scale.m.x != 0.0f ? (ParentScale.m.x / _scale.m.x) : _scale.m.x,
			_scale.m.y != 0.0f ? (ParentScale.m.y / _scale.m.y) : _scale.m.y,
			_scale.m.z != 0.0f ? (ParentScale.m.z / _scale.m.z) : _scale.m.z
		);

		mbScaleFlag = true;
	}

	void SetWorldScaleByRatio(const float _ratio)
	{
		if ( nullptr == mParentTransform )
		{
			SetLocalScaleByRatio(_ratio);
		}
		else
		{
			SetWorldScale(GetWorldScale( ) *= _ratio);
		}
	}

	GVector4D GetWorldRotation( )
	{
		return mvWorldWRotation;
	}

	void SetWorldRotation(GVector4D _rotation)
	{
		if ( nullptr == mParentTransform )
		{
			mvLocalRotation = _rotation;
			mbRotationFlag = true;
			return;
		}

		GVector4D ParentRot = mParentTransform->GetLocalRotation( );
		mvWorldWRotation = _rotation;
		mvLocalRotation = GVector4D(_rotation.m.x - ParentRot.m.x, _rotation.m.y - ParentRot.m.y, _rotation.m.z - ParentRot.m.z);
		mbRotationFlag = true;
	}

#pragma endregion

protected:
	void SetParentTransform(ComTransform* _parentTransform)
	{
		mParentTransform = _parentTransform;
	}

	void UpdateTransform( );

public:
	bool mbRotationFlag;
	bool mbPositionFlag;
	bool mbScaleFlag;

protected:
	ComTransform* mParentTransform;

private:
	GVector4D mvLocalScale;
	GVector4D mvLocalRotation;
	GVector4D mvLocalPosition;

	GMatrix mMatLocalScale;

	GMatrix mMatLocalRotation;

	GMatrix mMatLocalPosition;

	GVector4D mvWorldScale;
	GVector4D mvWorldWRotation;
	GVector4D mvWorldPosition;

	GMatrix mMatWorld;
	GMatrix mMatParentWorld;

	bool mbUpdateFlag;
};

