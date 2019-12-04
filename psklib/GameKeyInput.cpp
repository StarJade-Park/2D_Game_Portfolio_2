#include <Windows.h>

#include "GameKeyInput.h"

std::map<std::wstring, SmartPtr<GameKeyInput::GameKey>> GameKeyInput::mAllKeyMap;
SmartPtr<GameKeyInput::GameKey> GameKeyInput::newKey = nullptr;

bool GameKeyInput::Down(const wchar_t* _keyName)
{
	SmartPtr<GameKeyInput::GameKey> findKey = FindKeyOrNull(_keyName);

	if ( nullptr == findKey )
	{
		CRASH_PROG;
	}

	return findKey->mbDown;
}

bool GameKeyInput::AFK(const wchar_t* _keyName)
{
	SmartPtr<GameKeyInput::GameKey> findKey = FindKeyOrNull(_keyName);

	if ( nullptr == findKey )
	{
		CRASH_PROG;
	}

	return findKey->mbAFK;
}

bool GameKeyInput::Press(const wchar_t* _keyName)
{
	SmartPtr<GameKeyInput::GameKey> findKey = FindKeyOrNull(_keyName);

	if ( nullptr == findKey )
	{
		CRASH_PROG;
	}

	return findKey->mbPress;
}

bool GameKeyInput::Up(const wchar_t* _keyName)
{
	SmartPtr<GameKeyInput::GameKey> findKey = FindKeyOrNull(_keyName);

	if ( nullptr == findKey )
	{
		CRASH_PROG;
	}

	return findKey->mbUp;
}

void GameKeyInput::Update( )
{
	for ( auto& iter : mAllKeyMap )
	{
		iter.second->Update( );
	}
}

//void GameKeyInput::EndInput ()
//{
//	for (auto& iter : mAllKeyMap)
//	{
//		if (nullptr != iter.second)
//		{
//			delete iter.second;
//			iter.second = nullptr;
//		}
//	}
//}

SmartPtr<GameKeyInput::GameKey> GameKeyInput::FindKeyOrNull(const wchar_t* _name)
{
	const auto& findIter = mAllKeyMap.find(_name);

	if ( mAllKeyMap.end( ) == findIter )
	{
		return nullptr;
	}

	return findIter->second;
}

bool GameKeyInput::GameKey::IsInputKey( )
{
	for ( auto& iter : mKeyArray )
	{
		if ( NULL == GetAsyncKeyState(iter) )
		{
			return false;
		}
	}

	return true;
}

void GameKeyInput::GameKey::Update( )
{
	if ( true == IsInputKey( ) )
	{
		if ( true == mbAFK ) // Ű �Է�? yes
		{
			// ���� �����ӿ��� Ű �Է�? no
			mbUp = false;
			mbAFK = false;
			mbDown = true;
			mbPress = true;
		}
		else
		{
			// ���� �����ӿ��� Ű �Է�? yes
			mbUp = false;
			mbAFK = false;
			mbDown = false;
			mbPress = true;
		}
	}
	else // Ű �Է�? no
	{
		if ( true == mbPress )
		{
			// ���� �����ӿ��� Ű �Է�? yes
			mbUp = true;
			mbAFK = true;
			mbDown = false;
			mbPress = false;
		}
		else
		{
			// ���� �����ӿ��� Ű �Է�? no
			mbUp = false;
			mbAFK = true;
			mbDown = false;
			mbPress = false;
		}
	}
}
