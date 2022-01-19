#pragma once
#include "i_ justified_type.h"


namespace {
	using namespace std;

	class RightType : public IJustifiedType
	{
	public :
		void Format(vector<uint8_t>& value, int width) 
		{
			if (value.size() < width)
			{
				value.insert(value.begin(), width - value.size(), 0x00);
			}
		}

	private :
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