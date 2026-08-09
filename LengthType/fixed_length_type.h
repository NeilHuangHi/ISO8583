#pragma once
#include "i_length_type.h"
#include <memory>
namespace
{
	using namespace std;

	class FixedLengthType : public ILengthType
	{
	public:
		FixedLengthType(int max_length) : max_length_(max_length)
		{
			;
		}

		int MaxLength() override
		{
			return max_length_;
		};

		// The wire carries no length for a fixed field.
		int LengthOfLengthPrefix() override
		{
			return 0;
		};

		vector<uint8_t> EncodeLengthPrefix(int) override
		{
			return {};
		};

		int GetLengthOfField(vector<uint8_t> msg, int offset) override
		{
			return max_length_;
		};

		void JustifyValue(vector<uint8_t>& value, int width, IJustifiedType& justifier) override
		{
			justifier.Format(value, width);
		};

	private:
		int max_length_ = 0;
	};

}
