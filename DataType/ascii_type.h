#pragma once
#include "i_data_type.h"

namespace {
	class AsciiType : public IDataType
	{
	public:
		vector<uint8_t> GetBytes(string value) override
		{
			return vector<uint8_t>({ value.begin() ,value.end()});
		};

		string GetString(vector<uint8_t> value) override
		{
			return string({value.begin() ,value.end()});
		};
	};
}