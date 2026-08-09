// Round-trip regression tests: Unpack followed by ToMsg must reproduce the
// original wire bytes, and ToMsg must be repeatable.
#include "test_message.h"
#include "test_util.h"
#include "../ISO8583/iso8583.h"
#include "../ISO8583/iso8583_defs_1987.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace {

	string ToHex(const vector<uint8_t>& data)
	{
		ostringstream out;
		out << hex << uppercase << setfill('0');
		for (uint8_t b : data)
			out << setw(2) << (int)b << ' ';
		return out.str();
	}

	void CheckBytesEqual(const vector<uint8_t>& actual, const vector<uint8_t>& expected, const string& what)
	{
		if (actual == expected)
		{
			Check(true, what);
			return;
		}

		Check(false, what);
		if (actual.size() != expected.size())
			cout << "         size: expected " << expected.size() << ", got " << actual.size() << endl;

		size_t n = min(actual.size(), expected.size());
		int reported = 0;
		for (size_t i = 0; i < n && reported < 8; i++)
		{
			if (actual[i] != expected[i])
			{
				cout << "         offset " << i << ": expected "
					<< ToHex({ expected[i] }) << "got " << ToHex({ actual[i] }) << endl;
				reported++;
			}
		}
	}

} // namespace

int main()
{
	CoutSilencer silencer;

	const vector<uint8_t>& message = TestMessage();
	const vector<uint8_t> expected_body(message.begin(), message.end() - 1);

	shared_ptr<IISO8583Defs> defs = make_shared<ISO8583Defs1987>();
	ISO8583 iso8583(defs);

	bool unpacked = iso8583.Unpack(message, 0);
	vector<uint8_t> first = iso8583.ToMsg();
	vector<uint8_t> second = iso8583.ToMsg();

	string bitmap;
	iso8583.GetDataElement(0, bitmap);

	silencer.Restore();

	cout << "--- ISO8583 round-trip ---" << endl;

	Check(unpacked, "Unpack accepts the captured message");

	Check(bitmap == "0011000000100000000001011000000000100000110000010000001000100100",
		"bitmap matches the one encoded in the message");

	// Regression for the pack/unpack asymmetry on BCD variable-length fields:
	// Unpack read field 35's LLVAR indicator as 37 digits and stored 19 bytes,
	// but ToMsg used to re-emit the byte count (0x19) instead of the digit count.
	Check(first.size() > 34 && first[34] == 0x37,
		"field 35 LLVAR prefix is re-emitted as a digit count (0x37)");

	CheckBytesEqual(BodyOf(first), expected_body,
		"ToMsg reproduces the original wire bytes");

	Check(first.size() >= 2 && (first[0] | (first[1] << 8)) == (int)expected_body.size(),
		"length prefix is the body length, little-endian");

	// Regression for ToMsg mutating value_: the LL/LLL prefix used to be inserted
	// into the stored field value, so a second pack prefixed it twice.
	CheckBytesEqual(second, first, "ToMsg is idempotent across repeated calls");

	return Summary();
}
