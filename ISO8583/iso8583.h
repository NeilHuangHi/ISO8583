#pragma once
#include "i_iso8583_defs.h"
#include "tpdu.h"
#include "msg_type.h"
#include "bitmap.h"
#include "i_field.h"
#include "field.h"
#include "../FieldDescriptor/i_field_descriptor.h"
#include <arpa/inet.h>
#include <iostream>
#include <map>
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

		void GetDataElement(int field_no, vector<uint8_t>& data)
		{
			if (field_no == kBITMAP || GetBitmapPos(field_no) == true)
				data = fields_[field_no]->GetBytes();
		};

		void GetDataElement(int field_no, string& data) 
		{
			if (field_no == kBITMAP || GetBitmapPos(field_no) == true)
				data = fields_[field_no]->GetString();
		};

		bool SetDataElement(int field_no, string data) 
		{
			if (fields_[field_no]->SetValue(data))
			{
				cout << "SetDataElement[" << field_no << "]" << endl;
				SetBitmapPos(field_no, true);
				return true;
			}
			else
				SetBitmapPos(field_no, false);
			return false; 
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
			data_len = htole16(data_len);
			data_.insert(begin(data_), (uint8_t*)&data_len, (uint8_t*)&data_len + 2);
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