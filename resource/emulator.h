#pragma once
#include "vcpu.h"
#include "decode.h"

class Emulator
{
	public:
		Emulator();
		~Emulator ();

        bool loadBin(std::string pathToBin);
		bool tryToEmulate();

	private:
		Vcpu vcpu_;
		Decoder decoder_;

		uint16_t* binBuffer_;

		args_t fillArguments(args_prototype_t* args_prototype, int* currentPointer);
		uint16_t* getArgViaMode(uint16_t arg, uint16_t mode, int* currentPointer);
};
