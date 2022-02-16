#ifndef OP32LIB
#define OP32LIB

#include <windows.h>
#include <iostream>
#include <string>
#include <psapi.h>
#include "TlHelp32.h"

using namespace std;

class Operator32Lib
{
public:
	static uint32_t UnsignedShift32(uint32_t base, uint32_t shift)
	{
		string _inBinary = bitset<32>(base).to_string();
		string _outBinary = "00000000000000000000000000000000";

		for (uint32_t i = 0; i < 32 - shift; i++)
			_outBinary[i] = _inBinary[i + shift];

		uint32_t _out = bitset<32>(_outBinary).to_ulong();
		return _out;
	}
};

#endif