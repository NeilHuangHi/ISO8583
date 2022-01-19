#pragma once

#include <iostream>
#include <vector>
#include <string>

namespace 
{
	using namespace std;
	class IField
	{
    public:
        //virtual int GetFieldNumber() = 0;
        //virtual void SetFieldNumber(int value) = 0;
        virtual int GetPackedLength() = 0 ;

        virtual string GetString() = 0;
        virtual vector<uint8_t> GetBytes() = 0;
        virtual bool SetValue(string value) = 0;
        virtual bool SetValue(vector<uint8_t> value) = 0;

        virtual vector<uint8_t> ToMsg() = 0;
        virtual int Unpack(vector<uint8_t> msg, int offset) = 0;
	};
}