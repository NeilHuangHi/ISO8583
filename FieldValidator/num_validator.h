#pragma once
#include "i_field_validator.h"
#include <iostream>
#include <string>
#include <ctype.h>


namespace
{
	using namespace std;
	class NumValidator : public IFieldValidator
	{
	public:

		bool IsValid(string value) override
		{

			for (char a : value)
			{
				if (isdigit(a) == 0) return false;
			}
			return true;
		};
	};
}