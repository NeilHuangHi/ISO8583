#pragma once

#include"../DataType/i_data_type.h"
#include "../LengthType/i_length_type.h"
#include "../FieldValidator/i_field_validator.h"
#include "../JustifiedType/i_ justified_type.h"
#include <memory>

namespace
{
	using namespace std;
	class IFieldDescriptor
	{
	public :
		virtual shared_ptr<IDataType> GetDataType() = 0;
		virtual shared_ptr<ILengthType> GetLenType() = 0;
		virtual shared_ptr<IFieldValidator> GetFieldValidator() = 0;
		virtual shared_ptr<IJustifiedType> GetJustified() = 0;
	};
}