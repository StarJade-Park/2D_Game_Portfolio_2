#pragma once
#include <vector>
#include <unordered_map>

#include <..\psklib\SmartPtr.h>
#include <..\psklib\BaseName.h>
#include <..\psklib\GameFileSystem.h>

#include "GEngineDevice.h"

class GResource
	: public ControlBlock, public BaseName, public GFileSystem
{
private:
#pragma region Resource Manager
	template<typename RESTYPE>
	class GResourceManager
	{
	public:
		static SmartPtr<RESTYPE> Find(const std::wstring& _key)
		{
			const auto& findIter = mMapOfResource.find(_key);

			if ( mMapOfResource.end( ) == findIter )
			{
				return nullptr;
			}

			return findIter->second;
		}

		template<typename ...TYPES>
		static SmartPtr<RESTYPE> Load(const std::wstring& _path, TYPES&& ..._args)
		{
			GFileSystem path = GFileSystem(_path);
			if ( nullptr != Find(path.GetDirName( )) )
			{
				CRASH_PROG;
				return nullptr;
			}

			RESTYPE* newRes = new RESTYPE( );
			newRes->SetName(path.GetDirName( ));
			newRes->SetPath(_path);

			if ( false == newRes->Load(_args...) )
			{
				CRASH_PROG;
				return nullptr;
			}
			mMapOfResource.insert(
				std::unordered_map<std::wstring, SmartPtr<RESTYPE>>::value_type(newRes->GetName( ), newRes)
			);
			
			return newRes;
		}

		template<typename ...TYPES>
		static SmartPtr<RESTYPE> LoadToKey(const std::wstring& _key, const std::wstring& _path, TYPES&&... _args)
		{
			if ( nullptr != Find(_key) )
			{
				CRASH_PROG;
				return nullptr;
			}

			RESTYPE* newRes = new RESTYPE( );
			newRes->SetName(_key);
			newRes->SetPath(_path);
			if ( false == newRes->Load(_args...) )
			{
				CRASH_PROG;
				return nullptr;
			}

			mMapOfResource.insert(
				std::unordered_map<std::wstring, SmartPtr<RESTYPE>>::value_type(_key, newRes)
			);

			return newRes;
		}

		template<class ...TYPES>
		static SmartPtr<RESTYPE> Create(const std::wstring& _key, TYPES&&... _args)
		{
			if ( nullptr != Find(_key) )
			{
				CRASH_PROG;
				return nullptr;
			}

			RESTYPE* newRes = new RESTYPE( );
			newRes->SetName(_key);

			if ( false == newRes->Create(_args...) )
			{
				CRASH_PROG;
				return nullptr;
			}

			mMapOfResource.insert(
				std::unordered_map<std::wstring, SmartPtr<RESTYPE>>::value_type(_key, newRes)
			);

			return newRes;
		}

	public:
		static std::unordered_map<std::wstring, SmartPtr<RESTYPE>> mMapOfResource;
	};
#pragma endregion

public:
	GResource( ) = default;
	virtual ~GResource( ) = default;

	template<typename RESTYPE, typename ...TYPES>
	static SmartPtr<RESTYPE> Find(const std::wstring& _key, TYPES&&... _args)
	{
		return GResourceManager<RESTYPE>::Find(_key, _args...);
	}

	template<typename RESTYPE, typename ...TYPES>
	static SmartPtr<RESTYPE> Create(const std::wstring& _key, TYPES&&... _args)
	{
		return GResourceManager<RESTYPE>::Create(_key, _args...);
	}

	template<typename RESTYPE, typename ...TYPES>
	static SmartPtr<RESTYPE> Load(const std::wstring& _key, TYPES&&... _args)
	{
		return GResourceManager<RESTYPE>::Load(_key, _args...);
	}
};

template<typename RESTYPE>
typename std::unordered_map<std::wstring, SmartPtr<RESTYPE>> GResource::GResourceManager<RESTYPE>::mMapOfResource = std::unordered_map<std::wstring, SmartPtr<RESTYPE>>( );