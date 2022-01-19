#pragma once
#include "i_data_type.h"
#include <boost/algorithm/hex.hpp>


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
				boost::algorithm::unhex(value, std::back_inserter(ret));
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
				boost::algorithm::hex(data, back_inserter(ret));
			}
			catch (...)
			{
				cout << "BCD GetString Error" << endl;
			}
			return ret;
		};
	};
}