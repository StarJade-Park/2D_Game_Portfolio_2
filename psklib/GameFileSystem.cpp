#include "GameFileSystem.h"

std::map<std::wstring, GFileSystem> GFileSystem::mMapOfPath = std::map<std::wstring, GFileSystem>( );
stdfs::path GFileSystem::mRootPath = stdfs::current_path( );

void GFileSystem::Initialize( )
{
	// build output binary path 설정을 BIN으로 하면 됨
	// 디렉토리 명이 겹치는 경우 상위 디렉토리만 남게됨
	// x64만 고려하였음
	// BIN directory 찾기
	mRootPath = mRootPath.parent_path( );
	for ( stdfs::recursive_directory_iterator next(mRootPath), end; next != end; ++next )
	{
		if ( true == next->path( ).has_extension( ) )
		{
			continue;
		}

		if ( L"BIN" == next->path( ).filename( ) )
		{
			mRootPath = next->path( );
			break;
		}
	}

	std::vector<stdfs::path> allFiles = GFileSystem::GetAllDirectories( );

	GFileSystem::InsertPathMap(GFileSystem::GetRootPath( ));

	for ( auto& iter : allFiles )
	{
		GFileSystem::InsertPathMap(iter);
	}
}

bool GFileSystem::InsertPathMap(const std::wstring& _path)
{
	stdfs::path temp = stdfs::path(_path);
	if ( false == IsExsit(temp) )
	{
		CRASH_PROG;
		return false;
	}

	return InsertPathMap(temp.filename( ), temp);
}

bool GFileSystem::InsertPathMap(const std::wstring& _name, const std::wstring& _path)
{
	if ( false == IsExsit(_path) )
	{
		return false;
	}

	mMapOfPath.insert(
		std::map<std::wstring, GFileSystem>::value_type(_name, _path)
	);

	return true;
}

stdfs::path& GFileSystem::GetRootPath( )
{
	return mRootPath;
}

bool GFileSystem::MoveRootPathTo(const std::wstring& _name)
{
	stdfs::path temp = mRootPath;

	if ( false == IsExsit(temp /= _name) )
	{
		return false;
	}
	else
	{
		mRootPath /= _name;

		return true;
	}
}

GFileSystem GFileSystem::FindPathOrNull(const std::wstring& _name)
{
	const auto& iter = mMapOfPath.find(_name);
	if ( mMapOfPath.end( ) == iter )
	{
		CRASH_PROG;
		return GFileSystem( );
	}
	else
	{
		return iter->second;
	}
}

stdfs::path GFileSystem::Join(stdfs::path _path, std::wstring _name)
{
	return _path /= _name;
}

bool GFileSystem::IsExsit(const stdfs::path& _path)
{
	return stdfs::exists(_path);
}

std::vector<stdfs::path> GFileSystem::GetAllDirectories( )
{
	std::vector<stdfs::path> vecOfAllFiles = std::vector<stdfs::path>( );

	// __file_system::recursive_directory_iterator() = end;
	for ( stdfs::recursive_directory_iterator next(mRootPath), end; next != end; ++next )
	{
		if ( false == next->path( ).has_extension( ) )
		{
			vecOfAllFiles.push_back(next->path( ));
		}
	}

	return vecOfAllFiles;
}

std::vector<stdfs::path> GFileSystem::GetAllResourceFiles( )
{
	std::vector<stdfs::path> vecOfAllFiles = std::vector<stdfs::path>( );

	// 확장자의 대소문자 구별함
	std::vector<std::wstring> vecOfExt = { L".png" , L".PNG", L".jpg", L".JPG", L".tga", L".TGA", L".dds", L".DDS", L".ogg", L".OGG", L".wav", L".WAV" };

	vecOfAllFiles.reserve(300);
	// __file_system::recursive_directory_iterator() = end;
	for ( stdfs::recursive_directory_iterator next(mRootPath), end; next != end; ++next )
	{
		if ( false == next->path( ).has_extension( ) )
		{
			continue;
		}

		for ( auto& iter : vecOfExt )
		{
			if ( iter == next->path( ).extension( ) )
			{
				vecOfAllFiles.push_back(next->path( ));
				break;
			}
		}
	}

	return vecOfAllFiles;
}

std::vector<stdfs::path> GFileSystem::GetAllImgFiles( )
{
	std::vector<stdfs::path> vecOfAllFiles = std::vector<stdfs::path>( );

	// 확장자의 대소문자 구별함
	std::vector<std::wstring> vecOfExt = { L".png" , L".PNG", L".jpg", L".JPG", L".tga", L".TGA" };

	vecOfAllFiles.reserve(300);
	// __file_system::recursive_directory_iterator() = end;
	for ( stdfs::recursive_directory_iterator next(mRootPath / L"rabi-ribi"), end; next != end; ++next )
	{
		if ( false == next->path( ).has_extension( ) )
		{
			continue;
		}

		for ( auto& iter : vecOfExt )
		{
			if ( iter == next->path( ).extension( ) )
			{
				vecOfAllFiles.push_back(next->path( ));
				break;
			}
		}
	}

	return vecOfAllFiles;
}

std::vector<stdfs::path> GFileSystem::GetAllImgFiles(const stdfs::path& _path)
{
	std::vector<stdfs::path> vecOfAllFiles = std::vector<stdfs::path>( );

	// 확장자의 대소문자 구별함
	std::vector<std::wstring> vecOfExt = { L".png" , L".PNG", L".jpg", L".JPG", L".tga", L".TGA" };

	vecOfAllFiles.reserve(100);
	// __file_system::recursive_directory_iterator() = end;
	for ( stdfs::recursive_directory_iterator next(_path), end; next != end; ++next )
	{
		if ( false == next->path( ).has_extension( ) )
		{
			continue;
		}

		for ( auto& iter : vecOfExt )
		{
			if ( iter == next->path( ).extension( ) )
			{
				vecOfAllFiles.push_back(next->path( ));
				break;
			}
		}
	}

	return vecOfAllFiles;
}

std::vector<stdfs::path> GFileSystem::GetAllSoundFiles( )
{
	std::vector<stdfs::path> vecOfAllFiles = std::vector<stdfs::path>( );

	// 확장자의 대소문자 구별함
	std::vector<std::wstring> vecOfExt = { L".ogg", L".OGG", L".wav", L".WAV", L".mp3", L".MP3" };

	vecOfAllFiles.reserve(300);
	// __file_system::recursive_directory_iterator() = end;
	for ( stdfs::recursive_directory_iterator next(mRootPath), end; next != end; ++next )
	{
		if ( false == next->path( ).has_extension( ) )
		{
			continue;
		}

		for ( auto& iter : vecOfExt )
		{
			if ( iter == next->path( ).extension( ) )
			{
				vecOfAllFiles.push_back(next->path( ));
				break;
			}
		}
	}

	return vecOfAllFiles;
}

/**********************************************************************************/

GFileSystem::GFileSystem( )
	: mPath(stdfs::path( ))
{
	EMPTY_STATEMENT;
}

GFileSystem::GFileSystem(const stdfs::path& _path)
	: mPath(_path)
{
	EMPTY_STATEMENT;
}

GFileSystem::~GFileSystem( )
{
	EMPTY_STATEMENT;
}

void GFileSystem::SetPath(const std::wstring& _path)
{
	if ( false == IsExsit(_path) )
	{
		CRASH_PROG;
		return;
	}

	mPath = stdfs::path(_path);
}

void GFileSystem::SetPath(const stdfs::path& _path)
{
	if ( false == IsExsit(_path) )
	{
		CRASH_PROG;
		return;
	}

	mPath = _path;
}

stdfs::path GFileSystem::GetAbsPath( ) const
{
	if ( true == mPath.is_absolute( ) )
	{
		return mPath;
	}
	else
	{
		return mPath.parent_path( ) /= mPath;
	}
}

stdfs::path GFileSystem::GetExtension( )
{
	if ( true == mPath.has_extension( ) )
	{
		return mPath.extension( );
	}
	else
	{
		return stdfs::path( );
	}
}

stdfs::path GFileSystem::GetDirName( )
{
	if ( true == mPath.has_filename( ) )
	{
		return mPath.filename( );
	}
	else
	{
		return stdfs::path( );
	}
}

bool GFileSystem::MoveToParent( )
{
	if ( true == mPath.has_parent_path( ) )
	{
		mPath = mPath.parent_path( );
		return true;
	}
	else
	{
		return false;
	}
}
