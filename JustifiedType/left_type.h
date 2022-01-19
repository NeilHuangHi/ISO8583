#pragma once
#include "i_ justified_type.h"

namespace {
	using namespace std;

	class LeftType : public IJustifiedType
	{
	public:
		void Format(vector<uint8_t>& value, int width)
		{
			if (value.size() < width)
			{
				value.insert(value.end(), width - value.size(), 0x00);
			}
		}
	};
}