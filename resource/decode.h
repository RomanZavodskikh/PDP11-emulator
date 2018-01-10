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
