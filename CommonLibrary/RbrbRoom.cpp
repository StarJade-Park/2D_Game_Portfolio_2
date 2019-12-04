#include <..\DirectX2D\Rbrb.h>

#include "RbrbRoom.h"

const UINT RbrbRoom::mRoomSizeX = 20;
const UINT RbrbRoom::mRoomSizeY = 11;
RbrbRoom RbrbRoom::SRoom;

void RbrbRoom::MakeRooms( )
{
	mVecOfRooms.resize(rbrb_data_max_x_size);
	for ( auto& room : mVecOfRooms )
	{
		room.resize(rbrb_data_max_y_size);
	}

	for ( UINT x = 0; x < mVecOfRooms.size( ); ++x )
	{
		for ( UINT y = 0; y < mVecOfRooms[x].size( ); ++y )
		{
			RoomData room = RoomData( );
			room.indexX = x;
			room.indexY = y;

			if ( 1 == y )
			{
				room.centerPos = GVector4D(
					float(room.indexX * RbrbRoom::mRoomSizeX * 40) + (RbrbRoom::mRoomSizeX / 2 * 40),
					float(room.indexY * RbrbRoom::mRoomSizeY * 40) + 40.0f + (RbrbRoom::mRoomSizeY / 2 * 40),
					-50.f, 0.f
				);
			}
			else
			{
				room.centerPos = GVector4D(
					float(room.indexX * RbrbRoom::mRoomSizeX * 40) + (RbrbRoom::mRoomSizeX / 2 * 40),
					float(room.indexY * RbrbRoom::mRoomSizeY * 40) + (RbrbRoom::mRoomSizeY / 2 * 40),
					-50.f, 0.f
				);
			}

			mVecOfRooms[x][y] = room;
		}
	}

	mCurRoom = mVecOfRooms[0][0];
}
