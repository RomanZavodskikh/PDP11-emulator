#pragma once
#include "vcpu.h"
#include <iostream>

enum INSTR_TYPE
{
    SINGLE_OPERAND,
    CONDITIONAL,
    DOUBLE_OPERAND_REG,
    DOUBLE_OPERAND,
    UNDEFINED
};

struct exec_status_t
{
    bool status;
};

struct args_prototype_t
{
    INSTR_TYPE instrType = UNDEFINED;

    uint16_t mode1 = 0;
    uint16_t arg1 = 0;
    uint16_t mode2 = 0;
    uint16_t arg2 = 0;
};

struct args_t
{
    INSTR_TYPE instrType = UNDEFINED;

    uint16_t* arg1 = 0;
    uint16_t* arg2 = 0;

    bool isOneReg = false;
};

struct opcode_t
{
    uint16_t value;
};

struct decode_op_t
{
    uint16_t mask;
    uint16_t value;
    bool(*execute) (Vcpu *vcpu, opcode_t opcode, args_t args);
    std::string op_name;
};

class Decoder
{
    public:
        Decoder();
        ~Decoder();

        void defineArguments(args_prototype_t* args_prot, uint16_t instr);
        bool decodeAndExecute(Vcpu* vcpu, opcode_t opcode, args_t args);
        bool decodeAndDisasm(Vcpu* vcpu, opcode_t opcode, args_prototype_t args, QString& ret_str);
    private:
        bool appendOperand(QString &str, args_prototype_t &args_prototype, uint16_t num);
};

//double-operand instruction test
bool mov(Vcpu *vcpu, opcode_t opcode, args_t args);
bool movb(Vcpu *vcpu, opcode_t opcode, args_t args);
bool cmp(Vcpu *vcpu, opcode_t opcode, args_t args);
bool cmpb(Vcpu *vcpu, opcode_t opcode, args_t args);
bool bit(Vcpu *vcpu, opcode_t opcode, args_t args);
bool bitb(Vcpu *vcpu, opcode_t opcode, args_t args);
bool bic(Vcpu *vcpu, opcode_t opcode, args_t args);
bool bicb(Vcpu *vcpu, opcode_t opcode, args_t args);
bool bis(Vcpu *vcpu, opcode_t opcode, args_t args);
bool bisb(Vcpu *vcpu, opcode_t opcode, args_t args);
bool add(Vcpu *vcpu, opcode_t opcode, args_t args);
bool sub(Vcpu *vcpu, opcode_t opcode, args_t args);

bool mul(Vcpu *vcpu, opcode_t opcode, args_t args);
bool div(Vcpu *vcpu, opcode_t opcode, args_t args);
bool ash(Vcpu *vcpu, opcode_t opcode, args_t args);
bool ashc(Vcpu *vcpu, opcode_t opcode, args_t args);
bool xor_instr(Vcpu *vcpu, opcode_t opcode, args_t args);
bool sob(Vcpu *vcpu, opcode_t opcode, args_t args);

//single-operand instruction test
bool swab(Vcpu *vcpu, opcode_t opcode, args_t args);
bool jsr(Vcpu *vcpu, opcode_t opcode, args_t args);
bool emt(Vcpu *vcpu, opcode_t opcode, args_t args);
bool clr(Vcpu *vcpu, opcode_t opcode, args_t args);
bool clrb(Vcpu *vcpu, opcode_t opcode, args_t args);
bool com(Vcpu *vcpu, opcode_t opcode, args_t args);
bool comb(Vcpu *vcpu, opcode_t opcode, args_t args);
bool inc(Vcpu *vcpu, opcode_t opcode, args_t args);
bool incb(Vcpu *vcpu, opcode_t opcode, args_t args);
bool dec(Vcpu *vcpu, opcode_t opcode, args_t args);
bool decb(Vcpu *vcpu, opcode_t opcode, args_t args);
bool neg(Vcpu *vcpu, opcode_t opcode, args_t args);
bool negb(Vcpu *vcpu, opcode_t opcode, args_t args);
bool adc(Vcpu *vcpu, opcode_t opcode, args_t args);
bool adcb(Vcpu *vcpu, opcode_t opcode, args_t args);
bool sbc(Vcpu *vcpu, opcode_t opcode, args_t args);
bool sbcb(Vcpu *vcpu, opcode_t opcode, args_t args);
bool tst(Vcpu *vcpu, opcode_t opcode, args_t args);
bool tstb(Vcpu *vcpu, opcode_t opcode, args_t args);
bool ror(Vcpu *vcpu, opcode_t opcode, args_t args);
bool rorb(Vcpu *vcpu, opcode_t opcode, args_t args);
bool rol(Vcpu *vcpu, opcode_t opcode, args_t args);
bool rolb(Vcpu *vcpu, opcode_t opcode, args_t args);
bool asr(Vcpu *vcpu, opcode_t opcode, args_t args);
bool asrb(Vcpu *vcpu, opcode_t opcode, args_t args);
bool asl(Vcpu *vcpu, opcode_t opcode, args_t args);
bool aslb(Vcpu *vcpu, opcode_t opcode, args_t args);
bool mark(Vcpu *vcpu, opcode_t opcode, args_t args);
bool mtps(Vcpu *vcpu, opcode_t opcode, args_t args);
bool mfpi(Vcpu *vcpu, opcode_t opcode, args_t args);
bool mfpd(Vcpu *vcpu, opcode_t opcode, args_t args);
bool mtpi(Vcpu *vcpu, opcode_t opcode, args_t args);
bool mfpd(Vcpu *vcpu, opcode_t opcode, args_t args);
bool mtpi(Vcpu *vcpu, opcode_t opcode, args_t args);
bool mtpd(Vcpu *vcpu, opcode_t opcode, args_t args);
bool sxt(Vcpu *vcpu, opcode_t opcode, args_t args);
bool mfps(Vcpu *vcpu, opcode_t opcode, args_t args);
