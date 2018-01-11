#include <stdint.h>
#include <stdio.h>
#include <string>
#include "decode.h"

decode_op_t decode_table[] =
{
    { 0xf000,  0x1000, mov,             "mov"},
    { 0xf000,  0x9000, movb,            "movb"},
    { 0xf000,  0x2000, cmp,             "cmp"},
    { 0xf000,  0xa000, cmpb,            "cmpb"},
    { 0xf000,  0x3000, bit,             "bit"},
    { 0xf000,  0xb000, bitb,            "bitb"},
    { 0xf000,  0x4000, bic,             "bic"},
    { 0xf000,  0xc000, bicb,            "bicb"},
    { 0xf000,  0x5000, bis,             "bis"},
    { 0xf000,  0xd000, bisb,            "bisb"},
    { 0xf000,  0x6000, add,             "add"},
    { 0xf000,  0xe000, sub,             "sub"},
    { 0xfe00,  0x7000, mul,             "mul"},
    { 0xfe00,  0x7200, div,             "div"},
    { 0xfe00,  0x7400, ash,             "ash"},
    { 0xfe00,  0x7600, ashc,            "ashc"},
    { 0xfe00,  0x7800, xor_instr,       "xor"},
    { 0xfe00,  0x7e00, sob,             "sob"},
    { NULL,    NULL,   nullptr,         "none"}
};

Decoder::Decoder()
{

}

Decoder::~Decoder()
{

}

void Decoder::defineArguments(args_prototype_t* args, uint16_t instr)
{
    if ((instr & 0x7800) == 0x0800)
    {
        args->instrType = SINGLE_OPERAND;
        args->mode1 = (instr & 0x0038) >> 3;
        args->arg1  = (instr & 0x0007);
    }
    else if ((instr & 0x7800) == 0x0000)
    {
        args->instrType = CONDITIONAL;
        args->arg1 = (instr & 0x00FF);
    }
    else if ((instr & 0xF000) == 7000)
    {
        args->instrType = DOUBLE_OPERAND_REG;
        args->arg1 = (instr & 0x00E0) >> 6;
        args->mode2 = (instr & 0x0038) >> 3;
        args->arg2 = (instr & 0x0007);
    }
    else
    {
        args->instrType = DOUBLE_OPERAND;
        args->mode1 = (instr & 0x0700) >> 9;
        args->arg1 = (instr & 0x01C0) >> 6;
        args->mode2 = (instr & 0x0038) >> 3;
        args->arg2 = (instr & 0x0007);
    }
}

bool Decoder::decodeAndExecute(Vcpu* vcpu, opcode_t opcode, args_t args)
{
    int i = 0;

    while (decode_table[i].execute != nullptr)
    {
        if ((decode_table[i].mask & opcode.value) == decode_table[i].value)
        {
            std::cout << decode_table[i].op_name << std::endl;
            return decode_table[i].execute(vcpu, opcode, args);
        }

        i++;
    }

    return false;
}

bool Decoder::appendOperand(QString &str, args_prototype_t &args_prototype, uint16_t num)
{
    uint16_t mode = (num == 1) ? args_prototype.mode1 : args_prototype.mode2;
    uint16_t arg = (num == 1) ? args_prototype.arg1 : args_prototype.arg2;
    switch (mode) {
        case 0:
        {
            str += " R" + QString::number(arg);
            break;
        }
        case 1:
        {
            str += " (R" + QString::number(arg) + ")";
            break;
        }
        case 2:
        {
            str += " (R" + QString::number(arg) + ")+";
            break;
        }
        case 3:
        {
            str += " @(R" + QString::number(arg) + ")+";
            break;
        }
        case 4:
        {
            str += " -(R" + QString::number(arg) + ")";
            break;
        }
        case 5:
        {
            str += " @-(R" + QString::number(arg) + ")";
            break;
        }
        case 6:
        {
            str += " X(R" + QString::number(arg) + ")";
            break;
        }
        case 7:
        {
            str += " @X(R" + QString::number(arg) + ")";
            break;
        }
        default: return false;
    }

    return true;
}

bool Decoder::decodeAndDisasm(Vcpu* vcpu, opcode_t opcode, args_prototype_t args_prototype, QString& ret_str)
{
    int i = 0;

    while (decode_table[i].execute != nullptr)
    {
        if ((decode_table[i].mask & opcode.value) == decode_table[i].value)
        {
            ret_str = QString(decode_table[i].op_name.c_str());
            appendOperand(ret_str, args_prototype, 1);
            ret_str += ",";
            appendOperand(ret_str, args_prototype, 2);
            break;
        }

        i++;
    }

    return false;
}

bool mov(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    *args.arg2 = *args.arg1;

    vcpu -> n = (*args.arg2) & (1 << 15);
    vcpu -> z = (*args.arg2 == 0);
    vcpu -> v = false;
    
    return true;
}

bool movb(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    uint16_t val = (*args.arg2 & 0xFF00) + (*args.arg1 & 0x00FF);
    *args.arg2 = val;

    vcpu -> n = val & (1 << 7);
    vcpu -> z = ((val & 0x00FF) == 0);
    vcpu -> v = false;
    
    return true;
}

bool cmp(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    uint32_t res = *args.arg1 - *args.arg2;

    vcpu -> n = res & (1 << 16);
    vcpu -> z = (res == 0);
    vcpu -> c = ((res & (1 << 16)) != 0);
    vcpu -> v = ((*args.arg1 & (1 << 15)) != (*args.arg2 & (1 << 15))) &&
                ((*args.arg2 & (1 << 15)) == (       res & (1 << 15)));
    
    return true;
}

bool cmpb(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    uint16_t res = (*args.arg1 & 0x00FF) - (*args.arg2 & 0x00FF);
    
    vcpu -> n = res & (1 << 8);
    vcpu -> z = (res == 0);
    vcpu -> c = ((res & (1 << 8)) != 0);
    vcpu -> v = ((*args.arg1 & (1 << 7)) != (*args.arg2 & (1 << 7))) &&
                ((*args.arg2 & (1 << 7)) == (       res & (1 << 7)));
    
    return true;
}

bool bit(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    uint16_t res = (*args.arg1 & *args.arg2);
    
    vcpu -> n = ((res & (1 << 15)) != 0);
    vcpu -> z = (res == 0);
    vcpu -> v = false;
    
    return true;
}

bool bitb(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    uint16_t res = (*args.arg1 & *args.arg2) & 0x00FF;

    vcpu->n = ((res & (1 << 7)) != 0);
    vcpu->z = (res == 0);
    vcpu->v = false;

    return true;
}

bool bic(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    uint16_t res = ((~*args.arg1) & *args.arg2);
    *args.arg2 = res;

    vcpu -> n = ((res & (1 << 15)) != 0);
    vcpu -> z = (res == 0);
    vcpu -> v = false;
    
    return true;
}

bool bicb(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    uint16_t res = ((~*args.arg1) & *args.arg2) & 0x00FF;
    *args.arg2 = (0xFF00 & *args.arg2) + res;

    vcpu -> n = ((res & (1 << 7)) != 0);
    vcpu -> z = (res == 0);
    vcpu -> v = false;
    
    return true;
}

bool bis(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    uint16_t val = (*args.arg1 | *args.arg2);
    *args.arg2 = val;

    vcpu -> n = ((val & (1 << 15)) != 0);
    vcpu -> z = (val == 0);
    vcpu -> v = false;
    
    return true;
}

bool bisb(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    uint16_t val = (*args.arg1 | *args.arg2) & 0x00FF;
    *args.arg2 = val;

    vcpu-> n = ((val & (1 << 7)) != 0);
    vcpu-> z = (val == 0);
    vcpu-> v = false;
    
    return true;
}

bool add(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    uint32_t res = *args.arg2 + *args.arg1;
    *args.arg2 = res;
    
    vcpu -> c = ((res & (1 << 16)) != 0);
    vcpu -> n = ((res & (1 << 15)) != 0);
    vcpu -> z = (res == 0);
    vcpu -> v = ((*args.arg1 & (1 << 15)) == (*args.arg2 & (1 << 15))) &&
                ((*args.arg1 & (1 << 15)) != (       res & (1 << 15)));

    return true;
}

bool sub(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    uint32_t res = *args.arg2 - *args.arg1;
    *args.arg2 = res;

    vcpu -> c = ((res & (1 << 16)) != 0);
    vcpu -> n = ((res & (1 << 15)) != 0);
    vcpu -> z = (res == 0);
    vcpu -> v = ((*args.arg1 & (1 << 15)) != (*args.arg2 & (1 << 15))) &&
                ((*args.arg1 & (1 << 15)) == (       res & (1 << 15)));

    return true;
}

bool mul(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    uint32_t res = (*args.arg1) * (*args.arg2);
    if (args.isOneReg)
        *args.arg1 = uint16_t(res);
    else
    {
        *args.arg1 = uint16_t(res >> 16);
        *(args.arg1 + 1) = uint16_t(res);
    }

    vcpu -> n = res & (1 << 15);
    vcpu -> z = (res == 0);
    vcpu -> v = false;
    vcpu -> c = ((res & 0xFFFF0000 !=          0) && (res & (1 << 31) ==        0 )) ||
                ((res & 0xFFFF0000 != 0xFFFF0000) && (res & (1 << 31) == (1 << 31)));

    return true;
}

bool div(Vcpu *vcpu, opcode_t opcode, args_t args) 
{
    return true;
}

bool ash(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    return true;
}

bool ashc(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    return true;
}

bool xor_instr(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    *args.arg2 ^= *args.arg1;
    return true;
}

bool sob(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    return true;
}
