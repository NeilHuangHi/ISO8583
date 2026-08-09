#pragma once
#include "i_data_type.h"
#include "../UTIL/hex.h"


namespace 
{
	class BCDType : public IDataType
	{
	public:
		vector<uint8_t> GetBytes(string value) override
		{
			if ((value.size()%2) != 0)
			{
				value = '0' + value;
			}
			vector<uint8_t> ret;

			try
			{
				ret = Hex::Decode(value);
			}
			catch (...)
			{
				cout << "BCD GetBytes Error : " << value << endl;
			}

			return ret;
		};

		string GetString(vector<uint8_t> data) override
		{
			string ret;
			try
			{
				ret = Hex::Encode(data);
			}
			catch (...)
			{
				cout << "BCD GetString Error" << endl;
			}
			return ret;
		};

		// Two BCD digits pack into one byte; an odd digit count rounds up.
		int BytesForUnits(int units) override
		{
			return (units + 1) / 2;
		};

		int UnitsForBytes(int bytes) override
		{
			return bytes * 2;
		};
	};
}