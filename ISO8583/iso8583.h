#pragma once
#include "i_iso8583_defs.h"
#include "tpdu.h"
#include "msg_type.h"
#include "i_field.h"
#include "field.h"
#include "../FieldDescriptor/i_field_descriptor.h"
#include "../UTIL/byte_order.h"
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>
#include <string>
namespace {

#define kTPDU	-2
#define kMSG	-1
#define kBITMAP	 0

	using namespace std;
	using std::begin;
	using std::end;
	class ISO8583
	{
	public:

		ISO8583() {};
		ISO8583(shared_ptr<IISO8583Defs> iso8583_defs)
		{
			fields_ = iso8583_defs->GetISO8583FieldTemplate();
		}

		// Header fields (TPDU/MTI/bitmap, field_no <= 0) are always present and
		// have no bitmap bit, so only fields 1..64 go through the bitmap.
		void GetDataElement(int field_no, vector<uint8_t>& data)
		{
			auto& field = FieldAt(field_no);
			if (IsHeaderField(field_no) || GetBitmapPos(field_no) == true)
				data = field->GetBytes();
		};

		void GetDataElement(int field_no, string& data)
		{
			auto& field = FieldAt(field_no);
			if (IsHeaderField(field_no) || GetBitmapPos(field_no) == true)
				data = field->GetString();
		};

		bool SetDataElement(int field_no, string data)
		{
			// The bitmap is bookkeeping owned by SetBitmapPos; a direct write
			// could leave it advertising fields that carry no data.
			if (field_no == kBITMAP)
				throw out_of_range("ISO8583: the bitmap cannot be set directly");

			// A rejected value must not disturb the bitmap: the field keeps
			// whatever value (and presence bit) it had before the call.
			bool ok = FieldAt(field_no)->SetValue(data);
			if (ok)
			{
				cout << "SetDataElement[" << field_no << "]" << endl;
				if (!IsHeaderField(field_no))
					SetBitmapPos(field_no, true);
			}
			return ok;
		};

		TPDU tpdu;
		MSGType msg_type;
		
		vector<uint8_t> ToMsg()
		{
			data_.clear();
			vector<uint8_t> temp;
			for (int i = -2; i <= 0; i++)
			{
				temp = fields_[i]->ToMsg();
				data_.insert(end(data_), begin(temp), end(temp));
			}
			string bitmaps = fields_[kBITMAP]->GetString();
			cout << "bitmaps : " << bitmaps << endl;
			for (int i = 0; i < bitmaps.size(); i++)
			{
				if (bitmaps.at(i) == '1')
				{
					cout << "bit " << (i + 1) << " is enable." << endl;
					temp = fields_[(i + 1)]->ToMsg();
					data_.insert(end(data_), begin(temp), end(temp));
				}
			}

			short data_len = data_.size();
			cout << "data_len [" << data_len << "]" << endl;
			vector<uint8_t> len_prefix = ToLittleEndian16((uint16_t)data_len);
			data_.insert(begin(data_), begin(len_prefix), end(len_prefix));
			return data_;
		}

		bool Unpack(vector<unsigned char> msg, int offset)
		{
			offset = fields_[kTPDU]->Unpack(msg, offset);
			offset = fields_[kMSG]->Unpack(msg, offset);
			offset = fields_[kBITMAP]->Unpack(msg, offset);

			string bitmaps = fields_[kBITMAP]->GetString();
			cout << "bitmaps : " << bitmaps << endl;

			for (int i = 0; i < bitmaps.size(); i++)
			{
				if (bitmaps.at(i) == '1')
				{
					cout << "bit " << (i+1) << " is enable." << endl;
					offset = fields_[(i + 1)]->Unpack(msg, offset);
					if (offset < 0) return false;
				}
			}
			return true;
		}
		
	private:
		vector<uint8_t> data_;
		map<int, shared_ptr<IField>> fields_;

		// Exactly the ids defined by the header: kTPDU, kMSG, kBITMAP.
		bool IsHeaderField(int field_no)
		{
			return field_no >= kTPDU && field_no <= kBITMAP;
		}

		// Every valid id (header or data field) exists in the template, so
		// unknown ids fail here with a catchable out_of_range. Going through
		// fields_[] instead would default-insert a null field and null-deref.
		shared_ptr<IField>& FieldAt(int field_no)
		{
			auto it = fields_.find(field_no);
			if (it == fields_.end() || it->second == nullptr)
				throw out_of_range("ISO8583: unknown field id " + to_string(field_no));
			return it->second;
		}

		bool GetBitmapPos(int position)
		{
			string bitmaps = fields_[kBITMAP]->GetString();
			return bitmaps.at(position - 1) == '1' ? true : false;
		}
		
		void SetBitmapPos(int position, bool is_present)
		{
			string bitmaps = fields_[kBITMAP]->GetString();
			bitmaps.at(position - 1) = is_present ? '1' : '0';
			fields_[kBITMAP]->SetValue(bitmaps);
		}
	};
}