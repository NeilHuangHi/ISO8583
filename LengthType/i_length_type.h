#pragma once

#include "../JustifiedType/i_ justified_type.h"
#include<iostream>
#include<string>
#include<vector>

namespace
{
	using namespace std;

	// A length strategy answers every fixed-vs-variable question itself, so
	// Field never inspects Description() or the concrete type to decide.
	class ILengthType
	{
	public:
		// Declared field capacity in data-type units (BCD digits, chars, bytes).
		virtual int MaxLength() = 0;

		// Bytes the on-wire length prefix occupies (0 for fixed-length fields).
		virtual int LengthOfLengthPrefix() = 0;

		// Encodes length_indicator into the on-wire prefix ({} for fixed).
		virtual vector<uint8_t> EncodeLengthPrefix(int length_indicator) = 0;

		// Reads the field's data length in units from msg at offset; -1 on error.
		virtual int GetLengthOfField(vector<uint8_t> msg, int offset) = 0;

		// Fixed-length fields fill value out to width; variable ones keep it as-is.
		virtual void JustifyValue(vector<uint8_t>&, int, IJustifiedType&)
		{
		}

		// Value a field holds before anything is set (zero-filled for bitmaps).
		virtual vector<uint8_t> InitialValue()
		{
			return {};
		}
	};
}
