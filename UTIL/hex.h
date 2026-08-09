#pragma once
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
	using namespace std;

	// Replaces boost::algorithm::hex / unhex so the library carries no external
	// dependency. Same contract: uppercase output, throws on a non-hex digit.
	class Hex
	{
	public:
		// "1A2B" -> { 0x1A, 0x2B }
		static vector<uint8_t> Decode(const string& text)
		{
			if (text.size() % 2 != 0)
				throw invalid_argument("hex input has an odd number of digits");

			vector<uint8_t> ret;
			ret.reserve(text.size() / 2);
			for (size_t i = 0; i < text.size(); i += 2)
				ret.push_back((uint8_t)((DigitOf(text[i]) << 4) | DigitOf(text[i + 1])));

			return ret;
		}

		// { 0x1A, 0x2B } -> "1A2B"
		static string Encode(const vector<uint8_t>& data)
		{
			static const char kDigits[] = "0123456789ABCDEF";

			string ret;
			ret.reserve(data.size() * 2);
			for (uint8_t b : data)
			{
				ret += kDigits[(b >> 4) & 0x0F];
				ret += kDigits[b & 0x0F];
			}

			return ret;
		}

	private:
		static int DigitOf(char c)
		{
			if (c >= '0' && c <= '9') return c - '0';
			if (c >= 'A' && c <= 'F') return c - 'A' + 10;
			if (c >= 'a' && c <= 'f') return c - 'a' + 10;
			throw invalid_argument(string("not a hex digit: ") + c);
		}
	};
}
