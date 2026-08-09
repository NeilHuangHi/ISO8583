#pragma once
#include <cstdint>
#include <vector>

namespace {
	using namespace std;

	// Emits a 16-bit value least-significant byte first regardless of host byte
	// order. Replaces htole16, which is glibc-only and unavailable on MSVC.
	inline vector<uint8_t> ToLittleEndian16(uint16_t value)
	{
		return { (uint8_t)(value & 0xFF), (uint8_t)((value >> 8) & 0xFF) };
	}
}
