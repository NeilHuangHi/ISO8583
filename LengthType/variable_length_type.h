#pragma once
#include "i_length_type.h"
#include "../UTIL/bcd.h"
#include <memory>
#include <math.h>
#include <string>

namespace 
{
	class VariableLengthType : public ILengthType
	{
	public:

		VariableLengthType(int len_indicator, string format) : len_indicator_(len_indicator), format_(format)
		{
			;
		};

		int LengthOfLengthIndicator() override
		{
			return len_indicator_;
		};

		string Description() override
		{
			return format_;
		};


		int GetLengthOfField(vector<uint8_t> msg, int offset) override
		{
			int len = 0;
			if (Description() == "LLVAR")
				len = BCD().Bcd2Int({ msg.begin() + offset ,  msg.begin() + offset + 1 });
			else if(Description() == "LLLVAR")
				len = BCD().Bcd2Int({ msg.begin() + offset ,  msg.begin() + offset + 2 });
			
			
			return len > LengthOfLengthIndicator() ? -1 : len;
		};


	private:
		int len_indicator_ = 0;
		string format_;
	};

}