#pragma once
#include <vector>

#include <..\psklib\Debug.h>
#include <..\psklib\MyMath.h>

class RbrbRoom
{
public:
	static RbrbRoom SRoom;
	static void SRoomInit( )
	{
		SRoom.MakeRooms( );
	}

public:
	struct RoomData
	{
		UINT indexX;
		UINT indexY;
		GVector4D centerPos;
	};

public:
	RbrbRoom( )
		: mVecOfRooms( ), mCurRoom( )
	{
		EMPTY_STATEMENT;
	}

	~RbrbRoom( )
	{
		EMPTY_STATEMENT;
	}

	void MakeRooms( );

	void SetCurRoom(const GVector4D& _pos)
	{
		int x = std::lroundf((_pos.m.x - (RbrbRoom::mRoomSizeX / 2 * 40)) / RbrbRoom::mRoomSizeX / 40);
		int y = std::lroundf((_pos.m.y - (RbrbRoom::mRoomSizeY / 2 * 40)) / RbrbRoom::mRoomSizeY / 40);

		if ( x < 0 || x > mVecOfRooms.size( ) )
		{
			return;
		}

		if ( y < 0 || y > mVecOfRooms[0].size( ) )
		{
			return;
		}

		mCurRoom = mVecOfRooms[x][y];
	}

	void SetCurRoom(const UINT _x, const UINT _y)
	{
		mCurRoom = mVecOfRooms[_x][_y];
	}

	bool IsCurRoomByPos(const GVector4D& _pos)
	{
		RoomData posRoom = GetRoomData(_pos);
		
		if ( mCurRoom.indexX != posRoom.indexX )
		{
			return false;
		}

		if ( mCurRoom.indexY != posRoom.indexY )
		{
			return false;
		}

		return true;
	}

	GVector4D GetCurRoomCenterPos( ) const
	{
		return mCurRoom.centerPos;
	}

	RoomData GetCurRoomData( ) const
	{
		return mCurRoom;
	}

	RoomData GetRoomData(const GVector4D& _pos) const
	{
		int x = std::lroundf((_pos.m.x - (RbrbRoom::mRoomSizeX / 2 * 40)) / RbrbRoom::mRoomSizeX / 40);
		int y = std::lroundf((_pos.m.y - (RbrbRoom::mRoomSizeY / 2 * 40)) / RbrbRoom::mRoomSizeY / 40);

		if ( x < 0 || x > mVecOfRooms.size( ) )
		{
			return RoomData( );
		}

		if ( y < 0 || y > mVecOfRooms[0].size( ) )
		{
			return RoomData( );
		}

		return mVecOfRooms[x][y];
	}

public:
	static const UINT mRoomSizeX;
	static const UINT mRoomSizeY;

private:
	RoomData mCurRoom;
	std::vector<std::vector<RoomData>> mVecOfRooms;
};

