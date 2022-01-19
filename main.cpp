#include "ISO8583/iso8583_defs_1987.h"
#include "ISO8583/iso8583.h"
#include "ISO8583/i_iso8583_defs.h"
#include <string>
#include <iostream>
#include <boost/algorithm/hex.hpp>

using namespace std;
int main() 
{
	vector<uint8_t> test = { 0x60,0x00,0x11,0x00,0x00,0x02,0x00,0x30,0x20,0x05,0x80,0x20,0xC1,0x02,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x56,0x00,0x00,0x01,0x30,0x50,0x52,0x00,0x11,0x99,0x37,0x3C,0xE7,0x35,0x43,0x21,0x09,0x97,0x66,0x91,0x50,0x2B,0x86,0x72,0xD7,0x56,0x2E,0x87,0x65,0x40,0x39,0x39,0x39,0x39,0x32,0x30,0x39,0x33,0x30,0x30,0x30,0x30,0x30,0x31,0x39,0x39,0x30,0x32,0x33,0x30,0x33,0x33,0x38,0x00,0x18,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x01,0x59,0x57,0x13,0x3C,0xE7,0x35,0x43,0x21,0x09,0x97,0x66,0x91,0x50,0x2B,0x86,0x72,0xD7,0x56,0x2E,0x87,0x65,0x4C,0x5A,0x08,0x35,0x60,0x50,0x00,0x00,0x00,0x10,0x03,0x5F,0x24,0x03,0x27,0x12,0x31,0x5F,0x2A,0x02,0x09,0x01,0x5F,0x34,0x01,0x00,0x82,0x02,0x78,0x00,0x84,0x07,0xA0,0x00,0x00,0x00,0x65,0x10,0x10,0x8A,0x02,0x5A,0x31,0x95,0x05,0x08,0x80,0x00,0x80,0x00,0x9A,0x03,0x21,0x05,0x28,0x9B,0x02,0xE8,0x00,0x9C,0x01,0x00,0x9F,0x02,0x06,0x00,0x00,0x00,0x00,0x56,0x00,0x9F,0x03,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x9F,0x10,0x08,0x07,0x01,0x01,0x04,0xA0,0x88,0x00,0x22,0x9F,0x1A,0x02,0x01,0x58,0x9F,0x26,0x08,0x21,0x7A,0x52,0xDB,0x8C,0x1A,0x68,0x57,0x9F,0x27,0x01,0x80,0x9F,0x33,0x03,0xE0,0x28,0xC8,0x9F,0x34,0x03,0x1E,0x03,0x00,0x9F,0x35,0x01,0x22,0x9F,0x36,0x02,0x00,0x34,0x9F,0x37,0x04,0x63,0xF8,0xC7,0xF8,0x00,0x35,0x45,0x31,0x06,0x20,0x20,0x20,0x20,0x20,0x20,0x54,0x32,0x10,0x65,0x98,0x71,0x66,0x5D,0x6A,0x6D,0x62,0x39,0x38,0x37,0x36,0x35,0x34,0x33,0x32,0x54,0x33,0x04,0x20,0x20,0x20,0x20,0x00,0x06,0x30,0x30,0x30,0x30,0x30,0x31,0x00
	};

	shared_ptr<IISO8583Defs> iso8583_def = make_shared<ISO8583Defs1987>();
	//ISO8583Defs1987 defs;
	ISO8583 I8583(iso8583_def);

	string bitmap;
	I8583.GetDataElement(0, bitmap);
	cout << "Show empty bitmap :" << bitmap << endl;;
	
	cout << "Setting something field " << endl;
	I8583.SetDataElement(3, "123456");
	I8583.SetDataElement(7, "5566");
	I8583.SetDataElement(55, { 0x01,0x02,0x03,0x04,0x31,0x32,0x33,0x05 });
	
	I8583.GetDataElement(0, bitmap);
	cout << "Setting after bitmap :" << bitmap << endl;;

	cout << "Unpack test message [" << I8583.Unpack(test, 0) << "]" << endl;;
	I8583.GetDataElement(0, bitmap);
	cout << "Unpack after bitmap " << bitmap << endl;;

	cout << "Get Field 55" << endl;
	vector<uint8_t> aa; 
	I8583.GetDataElement(55, aa);
	for (auto i : aa)
		printf("%02x ", i);
	cout << endl;
	
	cout << "Pack message " << endl;
	vector<uint8_t> temp = I8583.ToMsg();
	for (auto i : temp)
		printf("%02x ", i);
	cout << endl;


	
	return 0;
}
