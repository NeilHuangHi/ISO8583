#pragma once
#include "i_field_descriptor.h"
#include <memory>

namespace {
	using namespace std;
	class FieldDescriptor : public IFieldDescriptor
	{
	public:

		FieldDescriptor(shared_ptr<ILengthType> len_type, 
			shared_ptr<IDataType> data_type, 
			shared_ptr<IFieldValidator> validator, 
			shared_ptr<IJustifiedType> justified) : 
			len_type_(len_type), data_type_(data_type), validator_(validator), justified_(justified)
		{

		}

		shared_ptr<IDataType> GetDataType() override
		{
			return data_type_;
		};

		shared_ptr<ILengthType> GetLenType() override
		{
			return len_type_;
		};
		
		shared_ptr<IFieldValidator> GetFieldValidator() override
		{
			return validator_;
		};

		shared_ptr<IJustifiedType> GetJustified() override
		{
			return justified_;
		};

	private:
		shared_ptr<ILengthType> len_type_;
		shared_ptr<IDataType> data_type_;
		shared_ptr<IFieldValidator> validator_;
		shared_ptr<IJustifiedType> justified_;
	};
}