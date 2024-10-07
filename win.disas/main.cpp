#include <distorm.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <windows.h>

#define MAX_INSTRUCTIONS (1000)

int main(int argc, char **argv)
{
	// print distorm version
	unsigned int dver = distorm_version();
	printf("diStorm version: %d.%d.%d\n", (dver >> 16), ((dver) >> 8) & 0xff,
		dver & 0xff);
	
	// read input
	std::string libName, procName;
	std::cout << "Enter library name:" << std::endl;
	std::cin >> libName;
	std::cout << "Enter procedure name:" << std::endl;
	std::cin >> procName;
	std::cout << "enter count of bytes to disas" << std::endl;
	int sizeToDisas = 0;
	std::cin >> sizeToDisas;
	
	// check params
	if ( libName.length() == 0 || procName.length() == 0 || sizeToDisas <= 0 )
	{
		std::cout << "incorrect params" << std::endl;
		return 1;
	}
	
	// get procedure address
	HMODULE hLib = LoadLibrary(libName.c_str());
	if ( NULL == hLib )
	{
		std::cout << "cannot load library!" << std::endl;
		return 2;
	}
	PVOID procAddr = GetProcAddress(hLib, procName.c_str());
	if ( NULL == procAddr )
	{
		std::cout << "cannot get proc address!" << std::endl;
		return 3;
	}
	
	// decode instructions
	_DecodedInst decodedInstructions[ MAX_INSTRUCTIONS ];
	unsigned int decodedInstructionsCount = 0;
	_DecodeResult res = distorm_decode(0,
		reinterpret_cast<unsigned char*>(procAddr),
		sizeToDisas,
		Decode32Bits,
		decodedInstructions,
		MAX_INSTRUCTIONS,
		&decodedInstructionsCount);
	for (int i = 0; i < decodedInstructionsCount; i++)
	{
		printf("%08I64x (%02d) %-24s %s%s%s\r\n",
			decodedInstructions[i].offset,
			decodedInstructions[i].size,
			(char*)decodedInstructions[i].instructionHex.p,
			(char*)decodedInstructions[i].mnemonic.p,
			decodedInstructions[i].operands.length != 0 ? " " : "",
			(char*)decodedInstructions[i].operands.p);
	}
	
	return 0;
}
