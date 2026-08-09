// Regression tests for issue #1: SetDataElement/GetDataElement must accept the
// header field ids the class itself defines (kTPDU = -2, kMSG = -1) instead of
// throwing std::out_of_range from the bitmap bookkeeping. Builds an outbound
// message from scratch (the path Unpack-based tests never exercise) and then
// round-trips it through Unpack.
#include "test_util.h"
#include "../ISO8583/iso8583.h"
#include "../ISO8583/iso8583_defs_1987.h"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

int main()
{
	CoutSilencer silencer;

	shared_ptr<IISO8583Defs> defs = make_shared<ISO8583Defs1987>();
	ISO8583 iso8583(defs);

	bool set_threw = false;
	bool tpdu_set = false, mti_set = false, f3_set = false, f11_set = false, f41_set = false;
	try
	{
		tpdu_set = iso8583.SetDataElement(kTPDU, "60001");
		mti_set = iso8583.SetDataElement(kMSG, "0200");
		f3_set = iso8583.SetDataElement(3, "000000");
		f11_set = iso8583.SetDataElement(11, "000001");
		f41_set = iso8583.SetDataElement(41, "TERM0001");
	}
	catch (const std::out_of_range&)
	{
		set_threw = true;
	}

	bool get_threw = false;
	string tpdu, mti, bitmap, unset_field;
	vector<uint8_t> tpdu_bytes;
	try
	{
		iso8583.GetDataElement(kTPDU, tpdu);
		iso8583.GetDataElement(kMSG, mti);
		iso8583.GetDataElement(kTPDU, tpdu_bytes);
		iso8583.GetDataElement(kBITMAP, bitmap);
		iso8583.GetDataElement(2, unset_field);
	}
	catch (const std::out_of_range&)
	{
		get_threw = true;
	}

	// A rejected value on a header field must fail cleanly, not throw.
	bool reject_threw = false;
	bool reject_result = true;
	try
	{
		reject_result = iso8583.SetDataElement(kTPDU, "WAY TOO LONG FOR TPDU");
	}
	catch (const std::out_of_range&)
	{
		reject_threw = true;
	}

	// A rejected update must not disturb the previously stored value or its
	// presence bit (it used to clear the bitmap bit and drop the field).
	bool rejected_update = iso8583.SetDataElement(3, "12345X");
	string f3_after_reject;
	iso8583.GetDataElement(3, f3_after_reject);

	// Unknown ids on either path must fail with a catchable exception, not
	// default-insert a null field and dereference it.
	bool get_invalid_id_threw = false;
	try
	{
		string ignored;
		iso8583.GetDataElement(-3, ignored);
	}
	catch (const std::out_of_range&)
	{
		get_invalid_id_threw = true;
	}

	bool set_negative_id_threw = false;
	try
	{
		iso8583.SetDataElement(-3, "x");
	}
	catch (const std::out_of_range&)
	{
		set_negative_id_threw = true;
	}

	bool set_large_id_threw = false;
	try
	{
		iso8583.SetDataElement(65, "x");
	}
	catch (const std::out_of_range&)
	{
		set_large_id_threw = true;
	}

	// Writing the bitmap directly could make it advertise fields that carry
	// no data; it must throw and leave the bitmap untouched.
	bool set_bitmap_threw = false;
	try
	{
		iso8583.SetDataElement(kBITMAP,
			"0100000000000000000000000000000000000000000000000000000000000000");
	}
	catch (const std::out_of_range&)
	{
		set_bitmap_threw = true;
	}
	string bitmap_after_set;
	iso8583.GetDataElement(kBITMAP, bitmap_after_set);

	vector<uint8_t> packed = iso8583.ToMsg();

	// Round-trip: a fresh instance must parse the message we just built.
	ISO8583 parsed(make_shared<ISO8583Defs1987>());
	vector<uint8_t> body = BodyOf(packed);
	bool unpacked = !body.empty() && parsed.Unpack(body, 0);

	string r_tpdu, r_mti, r_f3, r_f11, r_f41;
	parsed.GetDataElement(kTPDU, r_tpdu);
	parsed.GetDataElement(kMSG, r_mti);
	parsed.GetDataElement(3, r_f3);
	parsed.GetDataElement(11, r_f11);
	parsed.GetDataElement(41, r_f41);

	silencer.Restore();

	cout << "--- ISO8583 build-from-scratch ---" << endl;

	Check(!set_threw, "SetDataElement accepts header field ids without throwing");
	Check(tpdu_set && mti_set, "SetDataElement(kTPDU)/(kMSG) report success");
	Check(f3_set && f11_set && f41_set, "SetDataElement stores data fields 3/11/41");

	Check(!get_threw, "GetDataElement accepts header field ids without throwing");
	Check(tpdu == "60001", "GetDataElement(kTPDU) returns the stored TPDU");
	Check(mti == "0200", "GetDataElement(kMSG) returns the stored MTI");
	Check(tpdu_bytes == vector<uint8_t>({ '6', '0', '0', '0', '1' }),
		"byte overload of GetDataElement works for header fields");
	Check(unset_field.empty(), "GetDataElement leaves an absent field untouched");

	Check(bitmap == "0010000000100000000000000000000000000000100000000000000000000000",
		"bitmap has exactly bits 3, 11 and 41 set");

	Check(!reject_threw && !reject_result,
		"rejected header value fails cleanly instead of throwing");

	Check(!rejected_update && f3_after_reject == "000000",
		"rejected update keeps the field's previous value and presence bit");

	Check(get_invalid_id_threw,
		"GetDataElement on an unknown id fails with out_of_range, not a crash");
	Check(set_negative_id_threw,
		"SetDataElement on an unknown negative id fails with out_of_range, not a crash");
	Check(set_large_id_threw,
		"SetDataElement on an id above 64 fails with out_of_range, not a crash");

	Check(set_bitmap_threw && bitmap_after_set == bitmap,
		"direct bitmap writes throw and leave the bitmap unchanged");

	Check(unpacked, "Unpack accepts the message built from scratch");
	Check(r_tpdu == "60001" && r_mti == "0200",
		"TPDU and MTI survive the round trip");
	Check(r_f3 == "000000" && r_f11 == "000001" && r_f41 == "TERM0001",
		"data fields 3/11/41 survive the round trip");

	return Summary();
}
