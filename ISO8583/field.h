#pragma once
#include "i_field.h"
#include "../FieldDescriptor/i_field_descriptor.h"
#include <cstdio>
#include <memory>


namespace
{
	class Field : public IField
	{
    public:
        Field(shared_ptr<IFieldDescriptor> descriptor) : field_descriptor_(descriptor)
        {
            // Bitmap-style fields start zero-filled; everything else starts empty.
            value_ = field_descriptor_->GetLenType()->InitialValue();
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
            if (!field_descriptor_->GetFieldValidator()->IsValid(value))
                return false;

            if (!SetValue(field_descriptor_->GetDataType()->GetBytes(value)))
                return false;

            // The caller's own digit count is exact, unlike the one derived from
            // the packed bytes, so an odd number of BCD digits survives ToMsg().
            length_indicator_ = (int)value.size();
            return true;
        };

        bool SetValue(vector<uint8_t> value) override
        {
            auto len_type = field_descriptor_->GetLenType();
            auto data_type = field_descriptor_->GetDataType();
            int len = data_type->BytesForUnits(len_type->MaxLength());

            // Fixed-length fields pad out to their full width; variable ones don't.
            len_type->JustifyValue(value, len, *field_descriptor_->GetJustified());

            // Compare in size_t so a value larger than INT_MAX can't slip past.
            if (value.size() > (size_t)len)
            {
                cout << "setting data too long " << endl;
                return false;
            }
            value_ = value;
            length_indicator_ = data_type->UnitsForBytes((int)value_.size());

            // Log through cout (not printf) so callers that redirect the
            // stream, like the tests, actually silence this dump.
            char hex_byte[4];
            for (auto i : value_)
            {
                snprintf(hex_byte, sizeof(hex_byte), "%02x ", i);
                cout << hex_byte;
            }
            cout << endl;
            return true;
        }

        vector<uint8_t> ToMsg() override
        {
            // The prefix counts digits, not bytes, so it comes from
            // length_indicator_ rather than from value_.size().
            // Build it onto a copy: value_ must survive ToMsg() unchanged
            // so that packing twice yields the same message.
            vector<uint8_t> msg = field_descriptor_->GetLenType()->EncodeLengthPrefix(length_indicator_);
            msg.insert(msg.end(), value_.begin(), value_.end());
            return msg;
        };

        int Unpack(vector<uint8_t> msg, int offset) override
        {
            value_.clear();
            if (offset < 0) return -1;

            auto len_type = field_descriptor_->GetLenType();
            auto data_type = field_descriptor_->GetDataType();

            int data_len = len_type->GetLengthOfField(msg, offset);
            cout << "data_len" <<  data_len << endl;
            if (data_len < 0) return -1;

            // Keep the indicator exactly as declared on the wire: converting to
            // bytes and back loses whether an odd digit count was 2n-1 or 2n.
            length_indicator_ = data_len;
            packed_lenght_ = data_type->BytesForUnits(data_len);

            int prefix_len = len_type->LengthOfLengthPrefix();

            cout << "msg.size[" << msg.size() << "]" << endl;;
            cout << "offset size [" << (offset + prefix_len + packed_lenght_) << "]" << endl;;

            // size_t arithmetic: immune to int overflow and to messages > INT_MAX.
            if ((size_t)offset + prefix_len + packed_lenght_ > msg.size())
            {
                cout << "parse data length error " << endl;
                return -1;
            }
            value_.insert(value_.begin(), msg.begin() + offset + prefix_len, msg.begin() + offset + prefix_len + packed_lenght_);
            return (offset + prefix_len + packed_lenght_);
        };

    protected:
        shared_ptr<IFieldDescriptor> field_descriptor_;
        //int field_num_ = 0;
        int packed_lenght_ = 0;
        int length_indicator_ = 0;
        vector<uint8_t> value_;
	};

}
