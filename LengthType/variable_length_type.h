#pragma once
#include "i_length_type.h"
#include "../UTIL/bcd.h"
#include <memory>

namespace
{
	class VariableLengthType : public ILengthType
	{
	public:

		VariableLengthType(int max_length, int prefix_bytes)
			: max_length_(max_length), prefix_bytes_(prefix_bytes)
		{
			;
		};

		int MaxLength() override
		{
			return max_length_;
		};

		int LengthOfLengthPrefix() override
		{
			return prefix_bytes_;
		};

		// The prefix is the length in BCD digits; a short encoding is
		// zero-filled on the left up to the prefix width (e.g. LLLVAR 08 -> 00 08).
		vector<uint8_t> EncodeLengthPrefix(int length_indicator) override
		{
			vector<uint8_t> prefix = BCD().Int2Bcd(length_indicator);
			if ((int)prefix.size() < prefix_bytes_)
				prefix.insert(prefix.begin(), prefix_bytes_ - prefix.size(), 0x00);
			return prefix;
		};

		int GetLengthOfField(vector<uint8_t> msg, int offset) override
		{
			// size_t arithmetic: immune to int overflow and to messages > INT_MAX.
			if (offset < 0 || (size_t)offset + prefix_bytes_ > msg.size())
				return -1;

			int len = BCD().Bcd2Int({ msg.begin() + offset, msg.begin() + offset + prefix_bytes_ });
			return len > MaxLength() ? -1 : len;
		};

	private:
		int max_length_ = 0;
		int prefix_bytes_ = 0;
	};

	class LLVarLengthType : public VariableLengthType
	{
	public:
		LLVarLengthType(int max_length) : VariableLengthType(max_length, 1)
		{
			;
		};
	};

	class LLLVarLengthType : public VariableLengthType
	{
	public:
		LLLVarLengthType(int max_length) : VariableLengthType(max_length, 2)
		{
			;
		};
	};

}
