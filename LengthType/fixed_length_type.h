#pragma once
#include "i_length_type.h"
#include <math.h>
#include <memory>
namespace 
{
	using namespace std;

	class FixedLengthType : public ILengthType
	{
	public: 
		FixedLengthType(int len_indicator, string format) : len_indicator_(len_indicator), format_(format)
		{
			;
		}
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
			return len_indicator_;
		};

	private:
		int len_indicator_ = 0;
		string format_;
	};

}