#pragma once
#include "i_field.h"
#include "../FieldDescriptor/i_field_descriptor.h"
#include "../UTIL/bcd.h"
#include "../DataType/bcd_type.h"
#include <memory>


namespace 
{
	class Field : public IField
	{
    public:
        Field(shared_ptr<IFieldDescriptor> descriptor) : field_descriptor_(descriptor)
        {
            auto len_type = field_descriptor_->GetLenType();
            if (len_type->Description() == "Binary")              //Init bitmap value
                value_.insert(value_.begin(), len_type->LengthOfLengthIndicator(), 0x00);
        }

        int GetPackedLength() override
        {
            return packed_lenght_;
        }

        string GetString() override
        {
            return field_descriptor_->GetDataType()->GetString(value_);
        };

        vector<uint8_t> GetBytes() override
        {
            return value_;
        };

        bool SetValue(string value) override
        {
            if (field_descriptor_->GetFieldValidator()->IsValid(value))
                return SetValue(field_descriptor_->GetDataType()->GetBytes(value));
            else
                return false;
        };

        bool SetValue(vector<uint8_t> value) override
        {
            auto len_type = field_descriptor_->GetLenType();
            int len = len_type->LengthOfLengthIndicator();
            if (typeid(*field_descriptor_->GetDataType()) == typeid(BCDType))
                len = ceil(double(len_type->LengthOfLengthIndicator()) / 2);

            if (len_type->Description() == "Fixed")
            {
                cout << "padding len [" << len << "]" << endl;
                field_descriptor_->GetJustified()->Format(value, len);
            }

            if (value.size() > len)
            {
                cout << "setting data too long " << endl;
                return false;
            }
            value_ = value;

            for (auto i : value_)
                printf("%02x ", i);
            cout << endl;
            return true;
        }

        vector<uint8_t> ToMsg() override
        {
            if (field_descriptor_->GetLenType()->Description() == "LLVAR" || field_descriptor_->GetLenType()->Description() == "LLLVAR")
            {
                vector<uint8_t> len = BCD().Int2Bcd(value_.size());
                if (field_descriptor_->GetLenType()->Description() == "LLLVAR" && len.size() == 1)
                    len.insert(len.begin(), 0x00);

                value_.insert(value_.begin(), len.begin(), len.end());
                return value_;
            }
            return value_;
        };

        int Unpack(vector<uint8_t> msg, int offset) override
        {
            value_.clear();
            auto len_type = field_descriptor_->GetLenType();
            int data_len = len_type->GetLengthOfField(msg, offset);
            cout << "data_len" <<  data_len << endl;
            if (data_len < 0) return -1;    
            
            int len = 0;

            if (typeid(*field_descriptor_->GetDataType()) == typeid(BCDType))
                data_len = ceil(double(data_len) / 2);

            packed_lenght_ = data_len;
            len += len_type->Description() == "LLVAR" ? 1 : 0;
            len += len_type->Description() == "LLLVAR" ? 2 : 0;

            cout << "msg.size[" << msg.size() << "]" << endl;;
            cout << "offset size [" << (offset + len + packed_lenght_) << "]" << endl;;

            if ((offset + len + packed_lenght_) > msg.size())
            {
                cout << "parse data length error " << endl;
                return -1;
            }
            value_.insert(value_.begin(),msg.begin() + offset+ len, msg.begin() +offset + len + packed_lenght_);
            return (offset+ len + packed_lenght_);
        };

    protected:
        shared_ptr<IFieldDescriptor> field_descriptor_;
        //int field_num_ = 0;
        int packed_lenght_ = 0;
        vector<uint8_t> value_;
	};

}