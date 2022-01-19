#pragma once

#include "i_iso8583_defs.h"
#include "i_field.h"
#include "field.h"
#include "bitmap.h"
#include "../FieldDescriptor/field_descriptor.h"
#include "../LengthType/variable_length_type.h"
#include "../LengthType/fixed_length_type.h"
#include "../DataType/bcd_type.h"
#include "../DataType/binary_type.h"
#include "../DataType/ascii_type.h"
#include "../FieldValidator/num_validator.h"
#include "../FieldValidator/alpha_num_validator.h"
#include "../FieldValidator/none_validator.h"
#include "../JustifiedType/left_type.h"
#include "../JustifiedType/right_type.h"
#include <map>
#include <memory>

#define mp(type, ...) make_shared<type>(__VA_ARGS__)
namespace {
	using namespace std;
	class ISO8583Defs1987 : public IISO8583Defs
	{
	public:
		ISO8583Defs1987() {
			map<int, shared_ptr<IField>> m = {
			{ -2,  mp(Field,mp(FieldDescriptor, mp(FixedLengthType,5,"Fixed"), mp(AsciiType),mp(NoneValidator),mp(LeftType))) },                       // TPDU
			{ -1,  mp(Field,mp(FieldDescriptor, mp(FixedLengthType,4,"Fixed"), mp(BCDType),mp(NoneValidator),mp(LeftType))) },                         // Message Type Indicator
			{ 0,  mp(Field,mp(FieldDescriptor, mp(FixedLengthType,8,"Binary"), mp(BinaryType),mp(NoneValidator),mp(LeftType))) },                     // Bitmap
			{ 1,  mp(Field,mp(FieldDescriptor, mp(FixedLengthType,8,"Binary"), mp(BinaryType),mp(NoneValidator),mp(LeftType))) },					   // Extend Bitmap
			{ 2,  mp(Field,mp(FieldDescriptor, mp(VariableLengthType,19,"LLVAR"), mp(BCDType),mp(NumValidator),mp(LeftType))) }, 					   // Primary Account Number
			{ 3,  mp(Field,mp(FieldDescriptor, mp(FixedLengthType,6,"Fixed"), mp(BCDType),mp(NumValidator),mp(RightType))) },						   // Processing Code
			{ 4,  mp(Field,mp(FieldDescriptor, mp(FixedLengthType,12,"Fixed"), mp(BCDType),mp(NumValidator),mp(RightType))) },						   // Amount, Txn
			{ 5,  mp(Field,mp(FieldDescriptor, mp(FixedLengthType,12,"Fixed"), mp(BCDType),mp(NumValidator),mp(RightType))) },						   // Amount, Settlement
			{ 6,  mp(Field,mp(FieldDescriptor, mp(FixedLengthType,12,"Fixed"), mp(BCDType),mp(NumValidator),mp(RightType))) },						   // Amount, Cardholder Billing
			{ 7,  mp(Field,mp(FieldDescriptor, mp(FixedLengthType,10,"Fixed"), mp(BCDType),mp(NumValidator),mp(RightType))) },						   // Date and Time, Transmission
			{ 8,  mp(Field,mp(FieldDescriptor, mp(FixedLengthType,8,"Fixed"), mp(BCDType),mp(NumValidator),mp(RightType))) },						   // Amount, Cardholder Billing Fee
			{ 9,  mp(Field,mp(FieldDescriptor, mp(FixedLengthType,8,"Fixed"), mp(BCDType),mp(NumValidator),mp(RightType))) },						   // Conversion Rate, Settlement
			{ 10, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,8,"Fixed"), mp(BCDType),mp(NumValidator),mp(RightType))) },						   // Conversion Rate, Cardholder Billing
			{ 11, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,6,"Fixed"), mp(BCDType),mp(NumValidator),mp(RightType))) },						   // Systems Trace Audit Number
			{ 12, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,6,"Fixed"), mp(BCDType),mp(NumValidator),mp(RightType))) },						   // Time, Local Txn
			{ 13, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,4,"Fixed"), mp(BCDType),mp(NumValidator),mp(RightType))) },						   // Date, Local Txn
			{ 14, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,4,"Fixed"), mp(BCDType),mp(NumValidator),mp(RightType))) },						   // Date, Expiration
			{ 15, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,6,"Fixed"), mp(BCDType),mp(NumValidator),mp(RightType))) },						   // Date, Settlement
			{ 16, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,4,"Fixed"), mp(BCDType),mp(NumValidator),mp(RightType))) },						   // Date, Conversion
			{ 17, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,4,"Fixed"), mp(BCDType),mp(NumValidator),mp(RightType))) },						   // Date, Capture
			{ 18, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,4,"Fixed"), mp(BCDType),mp(NumValidator),mp(RightType))) },						   // Merchant Type
			{ 19, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,3,"Fixed"), mp(BCDType),mp(NumValidator),mp(RightType))) },						   // Country Code, Acquiring Inst
			{ 20, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,3,"Fixed"), mp(BCDType),mp(NumValidator),mp(RightType))) },						   // Country Code, Primary Account Number
			{ 21, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,3,"Fixed"), mp(BCDType),mp(NumValidator),mp(RightType))) },						   // Country Code, Forwarding Inst
			{ 22, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,3,"Fixed"), mp(BCDType),mp(NumValidator),mp(RightType))) },						   // Point of Service Entry Mode
			{ 23, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,3,"Fixed"), mp(BCDType),mp(NumValidator),mp(RightType))) },						   // Application PAN number
			{ 24, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,3,"Fixed"), mp(BCDType),mp(NumValidator),mp(RightType))) },						   // Network International Identifier
			{ 25, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,2,"Fixed"), mp(BCDType),mp(NumValidator),mp(RightType))) },						   // Point of Service Condition Code
			{ 26, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,2,"Fixed"), mp(BCDType),mp(NumValidator),mp(RightType))) },						   // Point of Service PIN Capture Code
			{ 27, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,1,"Fixed"), mp(BCDType),mp(NumValidator),mp(RightType))) },						   // Authorization Identification Response Length
			{ 28, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,8,"Fixed"), mp(BCDType),mp(AlphaNumValidator),mp(RightType))) },					   // Amount, Txn Fee
			{ 29, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,8,"Fixed"), mp(BCDType),mp(AlphaNumValidator),mp(RightType))) },					   // Amount, Settlement Fee
			{ 30, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,8,"Fixed"), mp(BCDType),mp(AlphaNumValidator),mp(RightType))) },					   // Amount, Txn Processing Fee
			{ 31, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,8,"Fixed"), mp(BCDType),mp(AlphaNumValidator),mp(RightType))) },					   // Amount, Settlement Processing Fee
			{ 32, mp(Field,mp(FieldDescriptor, mp(VariableLengthType,11,"LLVAR"), mp(BCDType),mp(NumValidator),mp(LeftType))) },					   // Acquirer Inst Id Code
			{ 33, mp(Field,mp(FieldDescriptor, mp(VariableLengthType,11,"LLVAR"), mp(BCDType),mp(NumValidator),mp(LeftType))) },					   // Forwarding Inst Id Code
			{ 34, mp(Field,mp(FieldDescriptor, mp(VariableLengthType,28,"LLVAR"), mp(BCDType),mp(NoneValidator),mp(LeftType))) },					   // Primary Account Number, Extended
			{ 35, mp(Field,mp(FieldDescriptor, mp(VariableLengthType,37,"LLVAR"), mp(BCDType),mp(NoneValidator),mp(LeftType))) },					   // Track 2 Data
			{ 36, mp(Field,mp(FieldDescriptor, mp(VariableLengthType,104,"LLLVAR"), mp(AsciiType),mp(NumValidator),mp(LeftType))) },				    // Track 3 Data
			{ 37, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,12,"Fixed"), mp(AsciiType),mp(AlphaNumValidator),mp(LeftType))) },				   // Retrieval Reference Number
			{ 38, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,6,"Fixed"), mp(AsciiType),mp(AlphaNumValidator),mp(LeftType))) },					   // Approval Code
			{ 39, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,2,"Fixed"), mp(AsciiType),mp(AlphaNumValidator),mp(LeftType))) },					   // Response Code
			{ 40, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,3,"Fixed"), mp(AsciiType),mp(AlphaNumValidator),mp(LeftType))) },					   // Service Restriction Code
			{ 41, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,8,"Fixed"), mp(AsciiType),mp(NoneValidator),mp(LeftType))) },						   // Card Acceptor Terminal Id
			{ 42, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,15,"Fixed"), mp(AsciiType),mp(NoneValidator),mp(LeftType))) },					   // Card Acceptor Id Code
			{ 43, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,40,"Fixed"), mp(AsciiType),mp(NoneValidator),mp(LeftType))) },					   // Card Acceptor Name/Location
			{ 44, mp(Field,mp(FieldDescriptor, mp(VariableLengthType,25,"LLVAR"), mp(AsciiType),mp(AlphaNumValidator),mp(LeftType))) },				   // Additional Response Data
			{ 45, mp(Field,mp(FieldDescriptor, mp(VariableLengthType,76,"LLVAR"), mp(AsciiType),mp(AlphaNumValidator),mp(LeftType))) },				   // Track 1 Data
			{ 46, mp(Field,mp(FieldDescriptor, mp(VariableLengthType,999,"LLLVAR"), mp(AsciiType),mp(AlphaNumValidator),mp(LeftType))) },			   // Additional Data - ISO
			{ 47, mp(Field,mp(FieldDescriptor, mp(VariableLengthType,999,"LLLVAR"), mp(AsciiType),mp(AlphaNumValidator),mp(LeftType))) },			   // Additional Data - National
			{ 48, mp(Field,mp(FieldDescriptor, mp(VariableLengthType,999,"LLLVAR"), mp(AsciiType),mp(AlphaNumValidator),mp(LeftType))) },			   // Additional Data - Private
			{ 49, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,3,"Fixed"), mp(AsciiType),mp(AlphaNumValidator),mp(LeftType))) },					   // Currency Code, Txn
			{ 50, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,3,"Fixed"), mp(AsciiType),mp(AlphaNumValidator),mp(LeftType))) },					   // Currency Code, Settlement
			{ 51, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,3,"Fixed"), mp(AsciiType),mp(AlphaNumValidator),mp(LeftType))) },					   // Currency Code, Cardholder Billing
			{ 52, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,8,"Fixed"), mp(AsciiType),mp(NoneValidator),mp(LeftType))) },						   // Personal Id Number (PIN) Data
			{ 53, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,16,"Fixed"), mp(BCDType),mp(NumValidator),mp(LeftType))) },						   // Security Related Control Information
			{ 54, mp(Field,mp(FieldDescriptor, mp(VariableLengthType,120,"LLLVAR"), mp(AsciiType),mp(AlphaNumValidator),mp(LeftType))) },			   // Amounts, Additional
			{ 55, mp(Field,mp(FieldDescriptor, mp(VariableLengthType,999,"LLLVAR"), mp(AsciiType),mp(NoneValidator),mp(LeftType))) },				   // Reserved for ISO use
			{ 56, mp(Field,mp(FieldDescriptor, mp(VariableLengthType,999,"LLLVAR"), mp(AsciiType),mp(NoneValidator),mp(LeftType))) },				   // Reserved for ISO use
			{ 57, mp(Field,mp(FieldDescriptor, mp(VariableLengthType,999,"LLLVAR"), mp(AsciiType),mp(NoneValidator),mp(LeftType))) },				   // Reserved for National use
			{ 58, mp(Field,mp(FieldDescriptor, mp(VariableLengthType,999,"LLLVAR"), mp(AsciiType),mp(NoneValidator),mp(LeftType))) },				   // Reserved for National use
			{ 59, mp(Field,mp(FieldDescriptor, mp(VariableLengthType,999,"LLLVAR"), mp(AsciiType),mp(NoneValidator),mp(LeftType))) },				   // Reserved for National use
			{ 60, mp(Field,mp(FieldDescriptor, mp(VariableLengthType,999,"LLLVAR"), mp(AsciiType),mp(NoneValidator),mp(LeftType))) },				   // Reserved for Private use
			{ 61, mp(Field,mp(FieldDescriptor, mp(VariableLengthType,999,"LLLVAR"), mp(AsciiType),mp(NoneValidator),mp(LeftType))) },				   // Reserved for Private use
			{ 62, mp(Field,mp(FieldDescriptor, mp(VariableLengthType,999,"LLLVAR"), mp(AsciiType),mp(NoneValidator),mp(LeftType))) },				   // Reserved for Private use
			{ 63, mp(Field,mp(FieldDescriptor, mp(VariableLengthType,999,"LLLVAR"), mp(AsciiType),mp(NoneValidator),mp(LeftType))) },				   // Reserved for Private use
			{ 64, mp(Field,mp(FieldDescriptor, mp(FixedLengthType,8,"Fixed"), mp(AsciiType),mp(NoneValidator),mp(LeftType))) },						   // Message Authentication Code Field
			};

			defs_ = m;
		};

		~ISO8583Defs1987() {};

		map<int, shared_ptr<IField>> GetISO8583FieldTemplate() override
		{
			return defs_;
		}

	private:
		map<int, shared_ptr<IField>> defs_;
	};
};
