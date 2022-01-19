#pragma once

#include<iostream>
#include<vector>
#include<string>
namespace {
	using namespace std;
	class IJustifiedType 
	{
	public:
		virtual void Format(vector<uint8_t>& value, int width) = 0;
	};
}