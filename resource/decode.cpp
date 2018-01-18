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
    { 0xffc0,  0x00c0, swab,            "swab"},
    { 0xffc0,  0x0a00, clr,             "clr"},
    { 0xffc0,  0x8a00, clrb,            "clrb"},
    { 0xffc0,  0x0a40, com,             "com"},
    { 0xffc0,  0x8a40, comb,            "comb"},
    { 0xffc0,  0x0a80, inc,             "inc"},
    { 0xffc0,  0x8a80, incb,            "incb"},
    { 0xffc0,  0x0ac0, dec,             "dec"},
    { 0xffc0,  0x8ac0, decb,            "decb"},
    { 0xffc0,  0x0b00, neg,             "neg"},
    { 0xffc0,  0x8b00, negb,            "negb"},
    { 0xffc0,  0x0b40, adc,             "adc"},
    { 0xffc0,  0x8b40, adcb,            "adcb"},
    { 0xffc0,  0x0b80, sbc,             "sbc"},
    { 0xffc0,  0x8b80, sbcb,            "sbcb"},
    { 0xffc0,  0x0bc0, tst,             "tst"},
    { 0xffc0,  0x8bc0, tstb,            "tstb"},
    { 0xffc0,  0x0c00, ror,             "ror"},
    { 0xffc0,  0x8c00, rorb,            "rorb"},
    { 0xffc0,  0x0c40, rol,             "rol"},
    { 0xffc0,  0x8c40, rolb,            "rolb"},
    { 0xffc0,  0x0c80, asr,             "asr"},
    { 0xffc0,  0x8c80, asrb,            "asrb"},
    { 0xffc0,  0x0cc0, asl,             "asl"},
    { 0xffc0,  0x8cc0, aslb,            "aslb"},
    { 0xffc0,  0x0d00, mark,            "mark"},
    { 0xffc0,  0x8d00, mtps,            "mtps"},
    { 0xffc0,  0x0d40, mfpi,            "mfpi"},
    { 0xffc0,  0x8d40, mfpd,            "mfpd"},
    { 0xffc0,  0x0d80, mtpi,            "mtpi"},
    { 0xffc0,  0x8d80, mtpd,            "mtpd"},
    { 0xffc0,  0x0dc0, sxt,             "sxt"},
    { 0xffc0,  0x8dc0, mfps,            "mfps"},
    { 0xff00,  0x0100, br,              "br"},
    { 0xff00,  0x0200, bne,             "bne"},
    { 0xff00,  0x0300, beq,             "beq"},
    { 0xff00,  0x0400, bge,             "bge"},
    { 0xff00,  0x0500, blt,             "blt"},
    { 0xff00,  0x0600, bgt,             "bgt"},
    { 0xff00,  0x0700, ble,             "ble"},
    { 0xff00,  0x8000, bpl,             "bpl"},
    { 0xff00,  0x8100, bmi,             "bmi"},
    { 0xff00,  0x8200, bhi,             "bhi"},
    { 0xff00,  0x8300, blos,            "blos"},
    { 0xff00,  0x8400, bvc,             "bvc"},
    { 0xff00,  0x8500, bvs,             "bvs"},
    { 0xff00,  0x8600, bcc,             "bcc"},
    { 0xff00,  0x8700, bcs,             "bcs"},
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
    if ((instr & 0x7800) == 0x0800 || (instr & 0xffc0) == 0x00c0 /*swab*/)
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
    else if ((instr & 0xF000) == 0x7000)
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
            if (args_prototype.instrType == CONDITIONAL)
            {
                ret_str = QString(decode_table[i].op_name.c_str());
                ret_str += " " + QString::number((int8_t)args_prototype.arg1);
                break;
            }
            else if (args_prototype.instrType == DOUBLE_OPERAND ||
                     args_prototype.instrType == DOUBLE_OPERAND_REG)
            {
                ret_str = QString(decode_table[i].op_name.c_str());
                appendOperand(ret_str, args_prototype, 1);
                ret_str += ",";
                appendOperand(ret_str, args_prototype, 2);
                break;
            }
            else
            {
                ret_str = QString(decode_table[i].op_name.c_str());
                break;
            }
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
    vcpu -> c = (((res & 0xFFFF0000) !=          0) && ((res & (1U << 31)) ==         0 )) ||
                (((res & 0xFFFF0000) != 0xFFFF0000) && ((res & (1U << 31)) == (1U << 31)));

    return true;
}

bool div(Vcpu *vcpu, opcode_t opcode, args_t args) 
{
    vcpu -> c = (*args.arg2 == 0);
    if (*args.arg2 == 0)
    {
        vcpu -> v = true;
        return false;
    }

    uint32_t dividend = (*args.arg1 << 16) + *(args.arg1 + 1);
    uint16_t quotient = dividend / *args.arg2;
    uint32_t big_quotient = dividend / *args.arg2;
    uint16_t remainder = dividend - quotient * *args.arg2;
    if (big_quotient > (uint32_t)(quotient & 0x7FFF)) // quotient exceed 15 bits
    {
        vcpu -> v = true;
        return false;
    }

    *args.arg1 = quotient;
    *(args.arg1 + 1) = remainder;

    vcpu -> n = ((quotient & (1 << 15)) != 0);
    vcpu -> z = (quotient == 0);
    return true;
}

bool ash(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    int16_t shift = (int16_t)*args.arg2;
    if (shift > 32 || shift < -31)
        return false;

    uint32_t result = (shift > 0) ? *args.arg1 << shift :
                                    (int32_t)*args.arg1 >> -shift;

    vcpu -> n = ((result & (1 << 15)) != 0);
    vcpu -> z = (result == 0);
    vcpu -> v = ((result & (1 << 15)) != (*args.arg1 & (1 << 15)));
    if (shift > 0)
    { //left
        vcpu -> c = (*args.arg1 << (shift - 1)) & (1 << 15);
    }
    else
    { //rigth
        vcpu -> c = ((int32_t)*args.arg1 >> (-shift + 1)) & 0x1;
    }
    return true;
}

bool ashc(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    int16_t shift = (int16_t)*args.arg2;
    uint32_t word = (*args.arg1 << 16) + *(args.arg1 + 1);
    if (shift > 32 || shift < -31)
        return false;

    uint32_t result = (shift > 0) ? word << shift :
                                    (int32_t)word >> -shift;

    vcpu -> n = ((result & (1 << 15)) != 0);
    vcpu -> z = (result == 0);
    vcpu -> v = ((result & (1 << 15)) != (word & (1 << 31)));
    if (shift > 0)
    { //left
        vcpu -> c = (word << (shift - 1)) & (1 << 31);
    }
    else
    { //rigth
        vcpu -> c = ((int32_t)word >> (-shift + 1)) & 0x1;
    }

    *args.arg1       = (uint16_t)result >> 16;
    *(args.arg1 + 1) = (uint16_t)result;
    return true;
}

bool xor_instr(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    *args.arg2 ^= *args.arg1;

    vcpu -> n = ((*args.arg2 & (1 << 15)) != 0);
    vcpu -> z = (*args.arg2 == 0);
    vcpu -> v = false;

    return true;
}

bool sob(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    (*args.arg1)--;
    (*vcpu->getRegAddr(7)) -= 2 * *args.arg2;
    (*vcpu->getRegAddr(7)) -= 2; //to compensate PC += 2 in the end of emulation step

    return true;
}

bool swab(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    uint8_t lo = (*args.arg1 & 0xFF);
    uint8_t hi = (*args.arg1 >> 8);

    *args.arg1 = (lo << 8) + hi;

    vcpu -> n = ((*args.arg1 & (1 << 7)) != 0);
    vcpu -> z = (hi == 0);
    vcpu -> v = false;
    vcpu -> c = false;
    return true;
}

bool clr(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    *args.arg1 = 0;

    vcpu -> n = false;
    vcpu -> z = true;
    vcpu -> v = false;
    vcpu -> c = false;
    return true;
}

bool clrb(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    *args.arg1 &= 0xFF00;

    vcpu -> n = false;
    vcpu -> z = true;
    vcpu -> v = false;
    vcpu -> c = false;
    return true;
}

bool com(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    *args.arg1 = ~(*args.arg1);

    vcpu -> n = ((*args.arg1 & (1 << 15)) != 0);
    vcpu -> z = (*args.arg1 == 0);
    vcpu -> v = false;
    vcpu -> c = true;
    return true;
}

bool comb(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    *((uint8_t*)args.arg1 + 1) = ~(*((uint8_t*)args.arg1 + 1));

    vcpu -> n = ((*args.arg1 & (1 << 15)) != 0);
    vcpu -> z = (*args.arg1 == 0);
    vcpu -> v = false;
    vcpu -> c = true;
    return true;
}

bool inc(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    (*args.arg1)++;

    vcpu -> n = ((*args.arg1 & (1 << 15)) != 0);
    vcpu -> z = (*args.arg1 == 0);
    vcpu -> v = (*args.arg1 - 1 == 0x7fff);
    return true;
}

bool incb(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    (*(uint8_t*)args.arg1)++;

    vcpu -> n = (((*(uint8_t*)args.arg1) & (1 << 7)) != 0);
    vcpu -> z = ((*(uint8_t*)args.arg1) == 0);
    vcpu -> v = ((*(uint8_t*)args.arg1) - 1 == 0x7f);
    return true;
}

bool dec(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    (*args.arg1)--;

    vcpu -> n = ((*args.arg1 & (1 << 15)) != 0);
    vcpu -> z = (*args.arg1 == 0);
    vcpu -> v = (*args.arg1 + 1 == 0x8000);
    return true;
}

bool decb(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    (*(uint8_t*)args.arg1)--;

    vcpu -> n = (((*(uint8_t*)args.arg1) & (1 << 7)) != 0);
    vcpu -> z = ((*(uint8_t*)args.arg1) == 0);
    vcpu -> v = ((*(uint8_t*)args.arg1) + 1 == 0x80);
    return true;
}

bool neg(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    *args.arg1 = -*args.arg1;

    vcpu -> n = ((*args.arg1 & (1 << 15)) != 0);
    vcpu -> z = (*args.arg1 == 0);
    vcpu -> v = (*args.arg1 == 0x8000);
    vcpu -> c = (*args.arg1 != 0);
    return true;
}

bool negb(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    *(uint8_t*)args.arg1 = -*(uint8_t*)args.arg1;

    vcpu -> n = ((*args.arg1 & (1 << 7)) != 0);
    vcpu -> z = (*args.arg1 == 0);
    vcpu -> v = (*args.arg1 == 0x80);
    vcpu -> c = (*args.arg1 != 0);
    return true;
}

bool adc(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    uint16_t old = *args.arg1;
    *args.arg1 += vcpu -> c;

    vcpu -> n = ((*args.arg1 & (1 << 15)) != 0);
    vcpu -> z = (*args.arg1 == 0);
    vcpu -> v = (old == 0x7fff) && vcpu -> c;
    vcpu -> c = (old == 0xffff) && vcpu -> c;
    return true;
}

bool adcb(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    uint8_t* p = (uint8_t*)args.arg1;
    uint8_t old = *p;
    *p += vcpu -> c;

    vcpu -> n = ((*p & (1 << 7)) != 0);
    vcpu -> z = (*p == 0);
    vcpu -> v = (old == 0x7f) && vcpu -> c;
    vcpu -> c = (old == 0xff) && vcpu -> c;
    return true;
}

bool sbc(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    uint16_t old = *args.arg1;
    *args.arg1 -= vcpu -> c;

    vcpu -> n = ((*args.arg1 & (1 << 15)) != 0);
    vcpu -> z = (*args.arg1 == 0);
    vcpu -> v = (old == 0x8000);
    vcpu -> c = !((old == 0x0000) && vcpu -> c);
    return true;
}

bool sbcb(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    uint8_t* p = (uint8_t*)args.arg1;
    uint8_t old = *p;
    *p -= vcpu -> c;

    vcpu -> n = ((*p & (1 << 7)) != 0);
    vcpu -> z = (*p == 0);
    vcpu -> v = (old == 0x80);
    vcpu -> c = !((old == 0xff) && vcpu -> c);
    return true;
}

bool tst(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    vcpu -> n = ((*args.arg1 & (1 << 15)) != 0);
    vcpu -> z = (*args.arg1 == 0);
    vcpu -> v = false;
    vcpu -> c = false;
    return true;
}

bool tstb(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    uint8_t* p = (uint8_t*)args.arg1;

    vcpu -> n = ((*p & (1 << 7)) != 0);
    vcpu -> z = (*p == 0);
    vcpu -> v = false;
    vcpu -> c = false;
    return true;
}

bool ror(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    vcpu -> c = ((*args.arg1 & 0x1) != 0);
    *args.arg1 >>= 1;
    *args.arg1 |= ((uint16_t)vcpu->c << 15);

    vcpu -> n = ((*args.arg1 & (1 << 15)) != 0);
    vcpu -> z = (*args.arg1 == 0);
    vcpu -> v = vcpu -> n ^ vcpu -> c;
    return true;
}

bool rorb(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    uint8_t* p = (uint8_t*)args.arg1;
    vcpu -> c = ((*p & 0x1) != 0);
    *p >>= 1;
    *p |= ((uint8_t)vcpu->c << 7);

    vcpu -> n = ((*p & (1 << 7)) != 0);
    vcpu -> z = (*p == 0);
    vcpu -> v = vcpu -> n ^ vcpu -> c;
    return true;
}

bool rol(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    vcpu -> c = ((*args.arg1 & (1 << 15)) != 0);
    *args.arg1 <<= 1;
    *args.arg1 |= vcpu -> c;

    vcpu -> n = ((*args.arg1 & (1 << 15)) != 0);
    vcpu -> z = (*args.arg1 == 0);
    vcpu -> v = vcpu -> n ^ vcpu -> c;
    return true;
}

bool rolb(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    uint8_t* p = (uint8_t*)args.arg1;
    vcpu -> c = ((*p & (1 << 7)) != 0);
    *p <<= 1;
    *p |= vcpu -> c;

    vcpu -> n = ((*p & (1 << 15)) != 0);
    vcpu -> z = (*p == 0);
    vcpu -> v = vcpu -> n ^ vcpu -> c;
    return true;
}

bool asr(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    vcpu -> c = *args.arg1 & 0x1;

    *(int16_t*)args.arg1 >>= 1;
    vcpu -> n = ((*args.arg1 & (1 << 15)) != 0);
    vcpu -> z = (*args.arg1 == 0);
    vcpu -> v = vcpu -> n ^ vcpu -> c;
    return true;
}

bool asrb(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    uint8_t* p = (uint8_t*)args.arg1;
    vcpu -> c = *p & 0x1;

    *(int8_t*)p >>= 1;
    vcpu -> n = ((*args.arg1 & (1 << 7)) != 0);
    vcpu -> z = (*args.arg1 == 0);
    vcpu -> v = vcpu -> n ^ vcpu -> c;
    return true;
}

bool asl(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    vcpu -> c = ((*args.arg1 & (1 << 15)) != 0);

    *args.arg1 <<= 1;
    vcpu -> n = ((*args.arg1 & (1 << 15)) != 0);
    vcpu -> z = (*args.arg1 == 0);
    vcpu -> v = vcpu -> n ^ vcpu -> c;
    return true;
}

bool aslb(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    uint8_t* p = (uint8_t*)args.arg1;
    vcpu -> c = ((*p & (1 << 7)) != 0);

    *args.arg1 <<= 1;
    vcpu -> n = ((*p & (1 << 7)) != 0);
    vcpu -> z = (*p == 0);
    vcpu -> v = vcpu -> n ^ vcpu -> c;
    return true;
}

bool mark(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    return true;
}

bool mtps(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    return true;
}

bool mfpi(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    return true;
}

bool mfpd(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    return true;
}

bool mtpi(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    return true;
}

bool mtpd(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    return true;
}

bool sxt(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    *args.arg1 = vcpu -> n ? 0xffff : 0x0;
    if (vcpu->n == false)
        vcpu->z = true;
    return true;
}

bool mfps(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    return true;
}

bool br(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    *(vcpu->getRegAddr(7)) += 2*args.offset;
    return true;
}

bool bne(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    if (vcpu->z == false)
        *(vcpu->getRegAddr(7)) += 2*args.offset;
    return true;
}

bool beq(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    if (vcpu->z == true)
        *(vcpu->getRegAddr(7)) += 2*args.offset;
    return true;
}

bool bge(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    if (vcpu->n ^ vcpu->v == false)
        *(vcpu->getRegAddr(7)) += 2*args.offset;
    return true;
}

bool blt(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    if (vcpu->n ^ vcpu->v == true)
        *(vcpu->getRegAddr(7)) += 2*args.offset;
    return true;
}

bool bgt(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    if ((vcpu->z||(vcpu->n^vcpu->v)) == false)
        *(vcpu->getRegAddr(7)) += 2*args.offset;
    return true;
}

bool ble(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    if ((vcpu->z||(vcpu->n^vcpu->v)) == true)
        *(vcpu->getRegAddr(7)) += 2*args.offset;
    return true;
}

bool bpl(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    if (vcpu->n == false)
        *(vcpu->getRegAddr(7)) += 2*args.offset;
    return true;
}

bool bmi(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    if (vcpu->n == true)
        *(vcpu->getRegAddr(7)) += 2*args.offset;
    return true;
}

bool bhi(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    if ((vcpu->c || vcpu->z) == false)
        *(vcpu->getRegAddr(7)) += 2*args.offset;
    return true;
}

bool blos(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    if ((vcpu->c || vcpu->z) == true)
        *(vcpu->getRegAddr(7)) += 2*args.offset;
    return true;
}

bool bvc(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    if (vcpu->v == false)
        *(vcpu->getRegAddr(7)) += 2*args.offset;
    return true;
}

bool bvs(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    if (vcpu->v == true)
        *(vcpu->getRegAddr(7)) += 2*args.offset;
    return true;
}

bool bcc(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    if (vcpu->c == false)
        *(vcpu->getRegAddr(7)) += 2*args.offset;
    return true;
}

bool bcs(Vcpu *vcpu, opcode_t opcode, args_t args)
{
    if (vcpu->c == true)
        *(vcpu->getRegAddr(7)) += 2*args.offset;
    return true;
}
