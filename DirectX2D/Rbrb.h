#pragma once
#include <..\psklib\Debug.h>
#include <string>
#include <vector>

constexpr size_t rbrb_data_max_x_size = 500;
constexpr size_t rbrb_data_max_y_size = 200;

struct Rbrb
{
	bool mbVisible = false;
	float mAlphaValue = 0.0f;
	float mWidth = 0.0f;
	float mHeight = 0.0f;
	float mStartPosX = 0.0f;
	float mStartPosY = 0.0f;
	std::string mTileLayerName;
	std::string mTileLayerType;
	std::vector<UINT> mData;
};

