#pragma once
#include <iostream>
#include <fstream>

namespace fio
{
	template<typename T>
	std::ostream& write(std::ostream& stream, const T& value)
	{
		return stream.write(reinterpret_cast<const char*>(&value), sizeof(T));
	}

	template<typename T>
	std::istream& read(std::istream& stream, T& value)
	{
		return stream.read(reinterpret_cast<char*>(&value), sizeof(T));
	}
};

#pragma region base_class
class GameFileStream
{
public:
	GameFileStream(const char* _fileName, const char* _mode);
	GameFileStream(const wchar_t* _fileName, const wchar_t* _mode);
	virtual ~GameFileStream( );

public:
	template<typename Type>
	void operator<<(const Type& _data)
	{
		fwrite(&_data, sizeof(Type), 1, mFile);
	}

	template<typename Type>
	void operator>>(Type& _data)
	{
		fread_s(&_data, sizeof(Type), sizeof(Type), 1, mFile);
	}

	template<typename Type>
	void read(Type& _data, int count)
	{
		fread_s(&_data, sizeof(Type), sizeof(Type), count, mFile);
	}

public:
	bool IsOpen( );

protected:
	FILE* mFile;
};
#pragma endregion

#pragma region normal_class
class GameFileRStream final
	: public GameFileStream
{
public:
	GameFileRStream(const char* _fileName);
	GameFileRStream(const wchar_t* _fileName);
	~GameFileRStream( );
};

class GameFileWStream final
	: public GameFileStream
{
public:
	GameFileWStream(const char* _fileName);
	GameFileWStream(const wchar_t* _fileName);
	~GameFileWStream( );
};
#pragma endregion

#pragma region binary_class
class GameFileRBStream final
	: public GameFileStream
{
public:
	GameFileRBStream(const char* _fileName);
	GameFileRBStream(const wchar_t* _fileName);
	~GameFileRBStream( );
};

class GameFileWBStream final
	: public GameFileStream
{
public:
	GameFileWBStream(const char* _fileName);
	GameFileWBStream(const wchar_t* _fileName);
	~GameFileWBStream( );
};
#pragma endregion
