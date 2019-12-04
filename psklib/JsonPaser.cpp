#include "JsonPaser.h"

const char* JsonPaser::TypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };

JsonPaser::JsonPaser( )
	: mDocument(rjson::Document( ))
{

}

JsonPaser::~JsonPaser( )
{
	mDocument.RemoveAllMembers( );
}

bool JsonPaser::OpenJsonFile( )
{
	std::string json;
	stdfs::path jsonPath = GetJsonFile( );

	std::ifstream file = std::ifstream(jsonPath.c_str( ));
	if ( true == file.is_open( ) )
	{
		// 파일 끝으로 위치 지정자 이동
		file.seekg(0, std::ios::end);

		size_t size = file.tellg( );
		json.resize(size);

		// 파일 처음으로 위치 지정자 이동
		file.seekg(0, std::ios::beg);

		// 문자열 시작 지점의 주소, 저장할 문자열 크기 지정하고 읽기
		file.read(&json[0], size);
	}
	else
	{
		assert(false);
		return false;
	}

	mDocument.Parse(json.c_str( ));

	assert(mDocument.IsObject( ));
	if ( false == mDocument.IsObject( ) )
	{
		return false;
	}

	return true;
}

rjson::GenericObject<false, rjson::Value> JsonPaser::GetObjectInRoot(const std::string& _name)
{
	if ( false == mDocument[_name.c_str( )].IsObject( ) )
	{
		assert(false);
	}

	return mDocument[_name.c_str( )].GetObject( );
}

rjson::GenericArray<false, rjson::Value> JsonPaser::GetArrayInRoot(const std::string& _name)
{
	if ( false == mDocument[_name.c_str( )].IsArray( ) )
	{
		assert(false);
	}

	return mDocument[_name.c_str( )].GetArray( );
}

stdfs::path JsonPaser::GetJsonFile( )
{
	stdfs::path binDir = stdfs::current_path( ).parent_path( ) /= "BIN";

	// __file_system::recursive_directory_iterator() = end;
	for ( stdfs::recursive_directory_iterator next(binDir), end; next != end; ++next )
	{
		if ( ".json" == next->path( ).extension( ) )
		{
			return next->path( );
		}
	}

	return stdfs::path( );
}