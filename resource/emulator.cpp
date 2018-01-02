#include <cstdint>
#include <cstring>
#include "emulator.h"
#include <fstream>
#include <iterator>
#include <qstring.h>
#include <qthread.h>
#include "ui_mainwindow.h"

Emulator ::Emulator()
{
    vcpu_ = Vcpu();
    decoder_ = Decoder();
}

Emulator::~Emulator()
{
}

bool Emulator::loadBin(std::string pathToBin)
{
    std::ifstream fd;
    fd.open(pathToBin, std::ios::binary | std::ios::ate );

    if (!fd.is_open())
        return false;

    size_t fileSize = fd.tellg();
    fd.seekg(0, std::ios::beg);

    fd.read((char*)getVcpu()->getMemAddr(0), fileSize);

    fd.close();
    return true;
}

bool Emulator::tryToEmulate()
{
    int currentPointer = 0;
    this->getVcpu()->setRegValue(7, 0);
    this->dumpState();

    while (true)
    {
        args_prototype_t args_prototype;
        args_t args;
        uint16_t instr = getVcpu()->getMemValue(getVcpu()->getRegValue(7) / 2);

        decoder_.defineArguments(&args_prototype, instr);
        fillArguments(&args, &args_prototype, &currentPointer);

        decoder_.decodeAndExecute(&vcpu_, opcode_t{ instr }, args);
        this->dumpState();
        this->showState();
        QThread::sleep(1);
        *getVcpu()->getRegAddr(7) += 2;
    }

    return true;
}

args_t Emulator::fillArguments(args_t* args, args_prototype_t* args_prototype, int* currentPointer)
{
    switch (args_prototype->instrType)
    {
        case SINGLE_OPERAND:
        {
            args->arg1 = getArgViaMode(args_prototype->arg1, args_prototype->mode1, currentPointer);
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
            args->arg1 = getArgViaMode(args_prototype->arg1, args_prototype->mode1, currentPointer);
            args->arg2 = getArgViaMode(args_prototype->arg2, args_prototype->mode2, currentPointer);
            break;
        }
    }
}

uint16_t* Emulator::getArgViaMode(uint16_t arg, uint16_t mode, int* currentPointer)
{
    switch (mode)
    {
        case 0:
        {
            uint16_t* value = vcpu_.getRegAddr(arg);
            return value;
        }
        case 1:
        {
            uint16_t value = vcpu_.getRegValue(arg);
            return vcpu_.getMemAddr(value);
        }
        case 2:
        {
            uint16_t value = vcpu_.getRegValue(arg);
            vcpu_.setRegValue(arg, value + 1);
            return vcpu_.getMemAddr(value);
        }
        case 3:
        {
            uint16_t value = vcpu_.getRegValue(arg);
            uint16_t address = vcpu_.getMemValue(value);
            vcpu_.setRegValue(arg, value + 2);
            return vcpu_.getMemAddr(address);
        }
        case 4:
        {
            uint16_t value = vcpu_.getRegValue(arg) - 1;
            vcpu_.setRegValue(arg, value);
            return vcpu_.getMemAddr(value);
        }
        case 5:
        {
            uint16_t value = vcpu_.getRegValue(arg) - 2;
            vcpu_.setRegValue(arg, value);
            uint16_t address = vcpu_.getMemValue(value);
            return vcpu_.getMemAddr(address);
        }
        case 6:
        {
            uint16_t value = vcpu_.getRegValue(arg) + vcpu_.getMemValue(vcpu_.getRegValue(7));
            uint16_t* pc_addr = vcpu_.getMemAddr(vcpu_.getRegValue(7));
            *pc_addr += 2;
            return vcpu_.getMemAddr(value);
        }
        case 7:
        {
            uint16_t value = vcpu_.getRegValue(arg) + vcpu_.getMemValue(vcpu_.getRegValue(7));
            uint16_t* pc_addr = vcpu_.getMemAddr(vcpu_.getRegValue(7));
            *pc_addr += 2;
            uint16_t address = vcpu_.getMemValue(value);
            return vcpu_.getMemAddr(address);
        }
    };
}

bool Emulator::showState()
{
    ui->lineEdit_2->setText(QString::number(vcpu_.getRegValue(0)));
    ui->lineEdit_3->setText(QString::number(vcpu_.getRegValue(1)));
    ui->lineEdit_4->setText(QString::number(vcpu_.getRegValue(2)));
    ui->lineEdit_5->setText(QString::number(vcpu_.getRegValue(3)));
    ui->lineEdit_6->setText(QString::number(vcpu_.getRegValue(4)));
    ui->lineEdit_7->setText(QString::number(vcpu_.getRegValue(5)));
    ui->lineEdit_8->setText(QString::number(vcpu_.getRegValue(6)));
    ui->lineEdit_9->setText(QString::number(vcpu_.getRegValue(7)));

    QString memDump("");
    vcpu_.getMemString(memDump);
    ui->textBrowser->setText(memDump);
    qApp->processEvents();
    return true;
}

bool Emulator::dumpState()
{
    std::cout << "============STATE DUMP============\n";
    std::cout << "  R0 = " << vcpu_.getRegValue(0) << std::endl;
    std::cout << "  R1 = " << vcpu_.getRegValue(1) << std::endl;
    std::cout << "  R2 = " << vcpu_.getRegValue(2) << std::endl;
    std::cout << "  R3 = " << vcpu_.getRegValue(3) << std::endl;
    std::cout << "  R4 = " << vcpu_.getRegValue(4) << std::endl;
    std::cout << "  R5 = " << vcpu_.getRegValue(5) << std::endl;
    std::cout << "  R6 = " << vcpu_.getRegValue(6) << std::endl;
    std::cout << "  R7 = " << vcpu_.getRegValue(7) << std::endl;
    std::cout << "============   END    ============\n";
}

bool Emulator::setUi(Ui::MainWindow* ui) {
    this->ui = ui;
    return true;
}

Vcpu* Emulator::getVcpu() {
    return &(this->vcpu_);
}
