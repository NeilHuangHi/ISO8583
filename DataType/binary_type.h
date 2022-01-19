#pragma once
#include "i_data_type.h"
#include <bitset>
namespace
{
	using namespace std;
	class BinaryType : public IDataType
	{
	public:
		vector<uint8_t> GetBytes(string value) override // 1101001010100000
		{
			if (value.size() % 8 != 0)
			{
				int pad_len = ((value.size() / 8) + 1) * 8;
				Pad(value, pad_len, '0',1); 
			}

			vector<uint8_t> ret;

			for (int i = 0; i < value.size(); i += 8)
			{
				bitset<8> b(value.substr(i,8));
				unsigned char c = (b.to_ulong() & 0xFF);
				ret.push_back(static_cast<int>(c));
			}
			return ret;
		};

		string GetString(vector<uint8_t> value) override
		{
			string ret;
			for (auto i : value)
			{
				bitset<8> b((uint8_t)i);
				ret += b.to_string();
			}
			return ret;
		};

	private:

		// The generic approach  alignment 0 : right, 1 left
		template<typename T>
		void Pad(basic_string<T>& s,
			typename basic_string<T>::size_type n, T c, int alignment = 0) {
			if (n > s.length())
			{
				if (alignment == 1)
					s.insert(s.begin(), n - s.length(), c);
				else
					s.append(n - s.length(), c);	
			}
		}
	};
}
