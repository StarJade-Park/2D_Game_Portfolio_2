#pragma once
#include <experimental/filesystem>
#include <iostream>
#include <fstream>

#include <..\ThirdParty\rapidjson\document.h>
#include <..\ThirdParty\rapidjson\stringbuffer.h>

namespace stdfs = std::experimental::filesystem;
namespace rjson = rapidjson;

class JsonPaser
{
public:
	JsonPaser( );
	~JsonPaser( );

	bool OpenJsonFile( );
	// "Null", "False", "True", "Object", "Array", "String", "Number"
	rjson::GenericObject<false, rjson::Value> GetObjectInRoot(const std::string& _name);
	rjson::GenericArray<false, rjson::Value> GetArrayInRoot(const std::string& _name);
	
	rjson::Document& GetDocument( )
	{
		return mDocument;
	}
	
private:
	stdfs::path GetJsonFile( );

public:
	static const char* TypeNames[];

private:
	rjson::Document mDocument;
};

