#pragma once
#include "fixed_length_type.h"

namespace
{
	using namespace std;

	// Fixed-width field that starts zero-filled, so bitmap bits can be read
	// and flipped before any value has been packed into the field.
	class BitmapLengthType : public FixedLengthType
	{
	public:
		BitmapLengthType(int max_length) : FixedLengthType(max_length)
		{
			;
		}

		vector<uint8_t> InitialValue() override
		{
			return vector<uint8_t>(MaxLength(), 0x00);
		};
	};

}
