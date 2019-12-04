#include "GSound.h"
#include <list>
#include <atlstr.h>

FMOD::System* GSound::mMainSoundSystem = nullptr;
std::map<std::wstring, SmartPtr<GSoundPlayer>> GSound::m_PlayList;



FMOD_RESULT __stdcall GSound::SoundPlayerCallBack(
	FMOD_CHANNELCONTROL *channelcontrol,
	FMOD_CHANNELCONTROL_TYPE controltype,
	FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype,
	void *commanddata1,
	void *commanddata2)
{
	commanddata1;
	commanddata2;

	switch ( controltype )
	{
	case FMOD_CHANNELCONTROL_CHANNEL:

		switch ( callbacktype )
		{
		case FMOD_CHANNELCONTROL_CALLBACK_END:
			{
				FMOD::Channel* pPointer = ((FMOD::Channel*)channelcontrol);
				void* pSoundPlayer = nullptr;
				pPointer->getUserData(&pSoundPlayer);
				if ( nullptr == pSoundPlayer )
				{
					return FMOD_OK;
				}
				GSoundPlayer* TempPlayer = (GSoundPlayer*)pSoundPlayer;
				ErasePlayer(TempPlayer->mName.c_str( ));
				return FMOD_OK;
			}
		case FMOD_CHANNELCONTROL_CALLBACK_VIRTUALVOICE:
			break;
		case FMOD_CHANNELCONTROL_CALLBACK_SYNCPOINT:
			break;
		case FMOD_CHANNELCONTROL_CALLBACK_OCCLUSION:
			break;
		case FMOD_CHANNELCONTROL_CALLBACK_MAX:
			break;
		case FMOD_CHANNELCONTROL_CALLBACK_FORCEINT:
			break;
		default:
			break;
		}
		break;
	case FMOD_CHANNELCONTROL_CHANNELGROUP:
		break;
	case FMOD_CHANNELCONTROL_MAX:
		break;
	case FMOD_CHANNELCONTROL_FORCEINT:
		break;
	default:
		break;
	}

	return FMOD_OK;
}

GSoundPlayer::GSoundPlayer( )
{
}

GSoundPlayer::~GSoundPlayer( )
{
}

void GSoundPlayer::Stop( )
{
	if ( nullptr == mChannel )
	{
		CRASH_PROG;
		return;
	}

	mChannel->stop( );
	mChannel = nullptr;
}

bool GSound::Init( )
{
	FMOD_RESULT Return = FMOD::System_Create(&mMainSoundSystem);
	if ( nullptr == mMainSoundSystem || FMOD_OK != Return )
	{
		CRASH_PROG;
		return false;
	}

	Return = mMainSoundSystem->init(64, FMOD_DEFAULT, nullptr);
	if ( FMOD_OK != Return )
	{
		CRASH_PROG;
		return false;
	}

	return true;
}

void GSound::Play(const wchar_t* _Name)
{
	SmartPtr<GSound> FindSound = GResource::Find<GSound>(_Name);

	if ( nullptr == FindSound )
	{
		CRASH_PROG;
		return;
	}

	GSound::mMainSoundSystem->playSound(FindSound->m_pSound, nullptr, false, nullptr);
}

SmartPtr<GSoundPlayer> GSound::FindPlayer(const wchar_t* _PlayerName)
{
	std::map<std::wstring, SmartPtr<GSoundPlayer>>::iterator FindPlayerIter = m_PlayList.find(_PlayerName);

	if ( FindPlayerIter == m_PlayList.end( ) )
	{
		return nullptr;
	}

	return FindPlayerIter->second;
}

void GSound::ErasePlayer(const wchar_t* _PlayerName)
{
	std::map<std::wstring, SmartPtr<GSoundPlayer>>::iterator FindPlayerIter = m_PlayList.find(_PlayerName);

	if ( FindPlayerIter == m_PlayList.end( ) )
	{
		return;
	}
	m_PlayList.erase(FindPlayerIter);
	//FindPlayerIter->second;
}

void GSound::Play(const wchar_t* _PlayerName, const wchar_t* _Name, int _LoopCount)
{
	SmartPtr<GSound> FindSound = GResource::Find<GSound>(_Name);

	if ( nullptr == FindSound )
	{
		CRASH_PROG;
		return;
	}

	if ( nullptr != FindPlayer(_PlayerName) )
	{
		return;
	}

	GSoundPlayer* NewPlayer = new GSoundPlayer( );
	NewPlayer->mName = _PlayerName;
	GSound::mMainSoundSystem->playSound(FindSound->m_pSound, nullptr, false, &NewPlayer->mChannel);

	if ( nullptr == NewPlayer->mChannel )
	{
		CRASH_PROG;
	}

	NewPlayer->mChannel->setCallback(SoundPlayerCallBack);
	NewPlayer->mChannel->setUserData(NewPlayer);
	NewPlayer->mChannel->setMode(FMOD_LOOP_NORMAL /*| FMOD_2D*/);
	NewPlayer->mChannel->setLoopCount(_LoopCount);

	m_PlayList.insert(std::map<std::wstring, SmartPtr<GSoundPlayer>>::value_type(_PlayerName, NewPlayer));
}

void GSound::Stop(const wchar_t* _PlayerName)
{
	SmartPtr<GSoundPlayer> FindPlaySound = FindPlayer(_PlayerName);

	if ( nullptr == FindPlaySound )
	{
		CRASH_PROG;
		return;
	}
	ErasePlayer(_PlayerName);
	FindPlaySound->Stop( );
}

void GSound::Update( )
{
	if ( nullptr != mMainSoundSystem )
	{
		mMainSoundSystem->update( );
	}
}

GSound::GSound( )
{
}

GSound::~GSound( )
{
	if ( nullptr != m_pSound )
	{
		m_pSound->release( );
	}
}

bool GSound::Load( )
{
	std::string Str = CW2A(mPath.c_str( ), CP_UTF8).m_psz;

	FMOD_RESULT Return = mMainSoundSystem->createSound(Str.c_str( ), FMOD_DEFAULT, nullptr, &m_pSound);

	if ( FMOD_OK != Return || nullptr == m_pSound )
	{
		CRASH_PROG;
		return false;
	}
	return true;
}
