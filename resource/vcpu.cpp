#include <stdint.h>
#include "vcpu.h"

Vcpu::Vcpu() : n(false), z(false), v(false), c(false)
{

}

Vcpu::~Vcpu()
{

}

uint16_t Vcpu::getRegValue(uint16_t numReg)
{
	switch (numReg)
	{
	case 0: { return reg0_; break; }
	case 1: { return reg1_; break; }
	case 2: { return reg2_; break; }
	case 3: { return reg3_; break; }
	case 4: { return reg4_; break; }
	case 5: { return reg5_; break; }
	case 6: { return reg6_; break; }
	case 7: { return reg7_; break; }

	default: return 0;
	}
}

uint16_t* Vcpu::getRegAddr(uint16_t numReg)
{
	switch (numReg)
	{
	case 0: { return &reg0_; break; }
	case 1: { return &reg1_; break; }
	case 2: { return &reg2_; break; }
	case 3: { return &reg3_; break; }
	case 4: { return &reg4_; break; }
	case 5: { return &reg5_; break; }
	case 6: { return &reg6_; break; }
	case 7: { return &reg7_; break; }

	default: return 0;
	}
}

void Vcpu::setRegValue(uint16_t numReg, uint16_t value)
{
	switch (numReg)
	{
	case 0: { reg0_ = value; break; }
	case 1: { reg1_ = value; break; }
	case 2: { reg2_ = value; break; }
	case 3: { reg3_ = value; break; }
	case 4: { reg4_ = value; break; }
	case 5: { reg5_ = value; break; }
	case 6: { reg6_ = value; break; }
	case 7: { reg7_ = value; break; }
	}
}

uint16_t Vcpu::getMemValue(uint16_t address)
{
	return memory_[address];
}

uint16_t* Vcpu::getMemAddr(uint16_t address)
{
	return memory_ + address;
}

void Vcpu::setMemValue(uint16_t address, uint16_t value)
{
	memory_[address] = value;
}
