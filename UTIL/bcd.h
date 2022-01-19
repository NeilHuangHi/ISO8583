#pragma once
#include <iostream>
#include <vector>
#include <math.h>
#include <string>

namespace{
	using namespace std;
	class BCD
	{
	public:
		vector<uint8_t> Int2Bcd(int num)
		{
			if (num < 0) return vector<uint8_t>();

			int digits = to_string(num).length();
			digits = digits % 2 == 0 ? digits : digits + 1;

			int n = digits - 1;
			int high = 0;
			int low = 0;
			vector<uint8_t> ret;

			for (int i = 0; i < digits; i += 2)
			{
				high = num / pow(10, n);
				num %= (int)pow(10, n);

				low = num / (int)pow(10, n - 1);
				num %= (int)pow(10, n - 1);

				ret.push_back((high << 4) | low);
				n -= 2;
			}

			return ret;
		}

		int Bcd2Int(vector<uint8_t> b)
		{
			int ret = 0;
			int n = (b.size() * 2) - 1;

			for (int i = 0; i < b.size(); i++)
			{
				ret += ((b[i] >> 4) & 0x0F) * pow(10, n);
				ret += ((b[i]) & 0x0F) * pow(10, n - 1);
				n -= 2;
			}

			return(ret);
		}		
	};

}