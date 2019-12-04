#pragma once
#include <experimental/filesystem>
#include <map>

#include "Debug.h"

namespace stdfs = std::experimental::filesystem;

class GFileSystem
{
public:
	static void Initialize( );
	static bool InsertPathMap(const std::wstring& _path);
	static bool InsertPathMap(const std::wstring& _name, const std::wstring& _path);

	static stdfs::path& GetRootPath( );
	static bool MoveRootPathTo(const std::wstring& _name);
	static GFileSystem FindPathOrNull(const std::wstring& _name);

	static stdfs::path Join(stdfs::path _path, std::wstring _name);
	static bool IsExsit(const stdfs::path& _path);

	static std::vector<stdfs::path> GetAllDirectories( );
	static std::vector<stdfs::path> GetAllResourceFiles( );
	static std::vector<stdfs::path> GetAllImgFiles( );
	static std::vector<stdfs::path> GetAllImgFiles(const stdfs::path& _path);
	static std::vector<stdfs::path> GetAllSoundFiles( );

	GFileSystem( );
	GFileSystem(const GFileSystem& _other) = default;
	GFileSystem(GFileSystem&& _filesystem) = default;
	GFileSystem(const stdfs::path& _path);
	~GFileSystem( );

	void SetPath(const std::wstring& _path);
	void SetPath(const stdfs::path& _path);

	stdfs::path GetAbsPath( ) const;
	stdfs::path GetExtension( );
	stdfs::path GetDirName( );
	bool MoveToParent( );

	GFileSystem& operator/=(const std::wstring& _name)
	{
		mPath /= _name;
		return *this;
	}

	GFileSystem& operator=(const stdfs::path& _path)
	{
		std::wcout << L"Set new path... ";
		if ( true == IsExsit(mPath) )
		{
			mPath = stdfs::path(_path);
			std::wcout << mPath << std::endl;
		}
		else
		{
			std::wcout << L"fail!! (" << mPath << L")" << std::endl;
		}

		return *this;
	}
	GFileSystem& operator=(const GFileSystem& _other) = default;
	GFileSystem& operator=(GFileSystem&& _filesystem) = default;

protected:
	static std::map<std::wstring, GFileSystem> mMapOfPath;
	static stdfs::path mRootPath;

	stdfs::path mPath;
};