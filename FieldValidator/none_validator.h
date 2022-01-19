#pragma once
#include "i_field_validator.h"

namespace
{
	class NoneValidator : public IFieldValidator
	{
	public :
		bool IsValid(string value) override
		{
			return true;
		};
	};
}
