#pragma once

#include<iostream>
#include<string>
#include<vector>

namespace
{
	using namespace std;
	class ILengthType
	{
	public:
		virtual int LengthOfLengthIndicator() = 0;
		virtual int GetLengthOfField(vector<uint8_t> msg, int offset) = 0;
		virtual string Description() = 0;
	};
}