#include <stdint.h>
#include "emulator.h"

Emulator ::Emulator()
{
	vcpu_ = Vcpu();
	decoder_ = Decoder();
}

Emulator::~Emulator()
{
	delete[] binBuffer_;
}

bool Emulator::loadBin()
{
	binBuffer_ = new uint16_t(1024);
	return true;
}

bool Emulator::tryToEmulate()
{
	int currentPointer = 0;

	while (currentPointer < 1024)
	{
		uint16_t instr = binBuffer_[currentPointer++];
		args_t args = decoder_.defineArguments(instr);

		fillArguments(&args, &currentPointer);
		decoder_.decodeAndExecute(&vcpu_, opcode_t{ instr }, args);
	}

	return true;
}

bool Emulator::fillArguments(args_t* args_prototype, int* currentPointer)
{
	switch (args_prototype->instrType)
	{
		case SINGLE_OPERAND:
		{
			args_prototype->arg1 = getArgViaMode(args_prototype->arg1, args_prototype->mode1, currentPointer);
			break;
		}
		case CONDITIONAL:
		{
			break;
		}
		case DOUBLE_OPERAND_REG:
		{
			break;
		}
		case DOUBLE_OPERAND:
		{
			args_prototype->arg1 = getArgViaMode(args_prototype->arg1, args_prototype->mode1, currentPointer);
			args_prototype->arg2 = getArgViaMode(args_prototype->arg2, args_prototype->mode2, currentPointer);
			break;
		}
	}

	return true;
}

uint16_t Emulator::getArgViaMode(uint16_t arg, uint16_t mode, int* currentPointer)
{
	switch (mode)
	{
		case 0:
		{
			uint16_t value = vcpu_.getRegValue(arg);
			return value;
		}
		case 1:
		{
			uint16_t value = vcpu_.getRegValue(arg);
			return vcpu_.getMemValue(value);
		}
		case 2:
		{
			uint16_t value = vcpu_.getRegValue(arg);
			vcpu_.setRegValue(arg, value + 1);
			return vcpu_.getMemValue(value);
		}
		case 3:
		{
			uint16_t value = vcpu_.getRegValue(arg);
			uint16_t address = vcpu_.getMemValue(value);
			vcpu_.setRegValue(arg, value + 2);
			return vcpu_.getMemValue(address);
		}
		case 4:
		{
			uint16_t value = vcpu_.getRegValue(arg) - 1;
			vcpu_.setRegValue(arg, value);
			return vcpu_.getMemValue(value);
		}
		case 5:
		{
			uint16_t value = vcpu_.getRegValue(arg) - 2;
			vcpu_.setRegValue(arg, value);
			uint16_t address = vcpu_.getMemValue(value);
			return vcpu_.getMemValue(address);
		}
		case 6:
		{
			uint16_t value = vcpu_.getRegValue(arg) + binBuffer_[*currentPointer];
			*currentPointer++;
			return vcpu_.getMemValue(value);
		}
		case 7:
		{
			uint16_t value = vcpu_.getRegValue(arg) + binBuffer_[*currentPointer];
			*currentPointer++;
			uint16_t address = vcpu_.getMemValue(value);
			return vcpu_.getMemValue(address);
		}
	};
}