#pragma once

#include <iostream>

namespace
{
	typedef struct
	{
		uint8_t ID;
		uint8_t Originator[2];
		uint8_t Destination[2];

	} TPDU;
}