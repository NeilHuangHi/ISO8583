#pragma once

#include<iostream>
#include<string>
#include<vector>
namespace {
	using namespace std;
	class IDataType
	{
	public:
		virtual vector<uint8_t> GetBytes(string value) = 0;
		virtual string GetString(vector<uint8_t> data) = 0;
	};
}