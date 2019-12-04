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
		// ���� ������ ��ġ ������ �̵�
		file.seekg(0, std::ios::end);

		size_t size = file.tellg( );
		json.resize(size);

		// ���� ó������ ��ġ ������ �̵�
		file.seekg(0, std::ios::beg);

		// ���ڿ� ���� ������ �ּ�, ������ ���ڿ� ũ�� �����ϰ� �б�
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