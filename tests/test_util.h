// Shared scaffolding for the test executables (each test is its own TU).
#pragma once
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

inline int g_failures = 0;

inline void Check(bool passed, const std::string& what)
{
	std::cout << (passed ? "[ PASS ] " : "[ FAIL ] ") << what << std::endl;
	if (!passed) g_failures++;
}

// Prints the summary footer and returns the process exit code.
inline int Summary()
{
	std::cout << "--- " << (g_failures == 0 ? "all checks passed"
		: std::to_string(g_failures) + " check(s) failed") << " ---" << std::endl;
	return g_failures == 0 ? 0 : 1;
}

// The library logs to cout on every field operation; construct one of these
// to silence it so the test results stay readable.
class CoutSilencer
{
public:
	CoutSilencer() : saved_(std::cout.rdbuf(nullptr)) {}
	~CoutSilencer() { Restore(); }
	void Restore()
	{
		if (saved_) std::cout.rdbuf(saved_);
		saved_ = nullptr;
	}
private:
	std::streambuf* saved_;
};

// Strips the 2-byte little-endian length prefix that ISO8583::ToMsg prepends.
inline std::vector<uint8_t> BodyOf(const std::vector<uint8_t>& packed)
{
	if (packed.size() < 2) return {};
	return std::vector<uint8_t>(packed.begin() + 2, packed.end());
}
