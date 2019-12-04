#pragma once
#include "VertexBuffer.h"

class InstanceBuffer
	: public VertexBuffer
{
public:
	InstanceBuffer( )
		: mDataType(nullptr), mDataCount(0), mData(std::vector<char>( ))
	{
		EMPTY_STATEMENT;
	}

	~InstanceBuffer( )
	{
		EMPTY_STATEMENT;
	}

	template<typename T>
	void PushData(const T& _data)
	{
		if ( mDataType != &typeid(T) )
		{
			CRASH_PROG;
			return;
		}

		if ( ((mDataCount + 1) * mVertexSize) > mData.size( ) )
		{
			DebugConsole::PrintOutputWnd(std::wstring(L"data resize before : ").append(std::to_wstring(mData.size( ))));
			mData.resize(mDataCount * 2 * mVertexSize);
			DebugConsole::PrintOutputWnd(std::wstring(L"data resize after : ").append(std::to_wstring(mData.size( ))));
		}

		char* p = &mData[0];
		T* DataPointer = (T*)p;
		DataPointer[mDataCount] = _data;

		++mDataCount;
	}

	bool Create(UINT _maxCount, const type_info* _typeInfo, size_t _typeSize);
	void UpdateInstanceData( );

	void ResetDataCounter( )
	{
		mDataCount = 0;
	}

	UINT GetDataCurrentCount( )
	{
		return mDataCount;
	}

private:
	const type_info* mDataType;
	UINT mDataCount; // data 갯수
	std::vector<char> mData; // instancing 용 데이터
};