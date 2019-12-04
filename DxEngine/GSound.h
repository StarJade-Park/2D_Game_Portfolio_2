#pragma once
#include "GResource.h"
#include <..\psklib\SoundDef.h>
#include <..\ThirdParty\FMOD\inc\fmod.hpp>

class GSoundPlayer
	: public ControlBlock
{
	friend class GSound;
public:
	GSoundPlayer( );
	~GSoundPlayer( );

	void Stop( );

public:
	std::wstring mName;
	GSound* mSoundClip;
	FMOD::Channel* mChannel;
};

class GSound
	: public GResource
{
public:
	GSound( );
	~GSound( );

	static FMOD_RESULT __stdcall SoundPlayerCallBack(
		FMOD_CHANNELCONTROL *channelcontrol,
		FMOD_CHANNELCONTROL_TYPE controltype,
		FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype,
		void *commanddata1,
		void *commanddata2);

	static bool Init( );
	static void Update( );

	static void Play(const wchar_t* _Name);
	static void Play(const wchar_t* _PlayerName, const wchar_t* _Name, int _LoopCount = 1);
	static void Stop(const wchar_t* _PlayerName);

	static SmartPtr<GSoundPlayer> FindPlayer(const wchar_t* _PlayerName);
	static void ErasePlayer(const wchar_t* _PlayerName);
	static std::map<std::wstring, SmartPtr<GSoundPlayer>> m_PlayList;

	UINT SoundLen( )
	{
		UINT Value = gconst::inf<UINT>;
		m_pSound->getLength(&Value, FMOD_TIMEUNIT_MS);
		return Value;
	}

	bool Load( );
public:
	static FMOD::System* mMainSoundSystem;
	FMOD::Sound* m_pSound;
};

