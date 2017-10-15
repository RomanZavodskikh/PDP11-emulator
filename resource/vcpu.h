#pragma once

class Vcpu
{
	public:
		Vcpu();
		~Vcpu();

		uint16_t	getRegValue(uint16_t numReg);
		void		setRegValue(uint16_t numReg, uint16_t value);
		uint16_t Vcpu::getMemValue(uint16_t address);
		void Vcpu::setMemValue(uint16_t address, uint16_t value);

	private:
		uint16_t reg0_;
		uint16_t reg1_;
		uint16_t reg2_;
		uint16_t reg3_;
		uint16_t reg4_;
		uint16_t reg5_;
		uint16_t reg6_;
		uint16_t reg7_;

		uint16_t memory_[32 * 1024];
};