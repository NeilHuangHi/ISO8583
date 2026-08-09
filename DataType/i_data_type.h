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

		// Wire density: how many bytes carry a value of `units` digits/chars,
		// and back. Defaults model one unit per byte; packed encodings override.
		virtual int BytesForUnits(int units)
		{
			return units;
		}

		virtual int UnitsForBytes(int bytes)
		{
			return bytes;
		}
	};
}