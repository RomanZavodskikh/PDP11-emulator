#include <cstdint>
#include <cstring>
#include "emulator.h"
#include <fstream>
#include <iterator>
#include <qstring.h>
#include <qthread.h>
#include "ui_mainwindow.h"
#include "screen.h"

const int WINDOW_WIDTH = 531;
const int WINDOW_HEIGHT = 321;

Emulator ::Emulator():emulating_(false)
{
    vcpu_ = new Vcpu();
    decoder_ = Decoder();
}

Emulator::~Emulator()
{
    delete vcpu_;
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
    this->dumpState();
    emulating_ = true;

    while (emulating_)
    {
        args_prototype_t args_prototype;
        args_t args;
        currentPointer = getVcpu()->getRegValue(7) / 2;
        uint16_t instr = getVcpu()->getMemValue(currentPointer);

        decoder_.defineArguments(&args_prototype, instr);
        fillArguments(&args, &args_prototype);

        decoder_.decodeAndExecute(vcpu_, opcode_t{ instr }, args);
        for (size_t i = 0; i < 100; i++)
        {
            QThread::msleep(1);
            QCoreApplication::processEvents();
            if (emulating_ == false)
                goto exit0;
        }
        *getVcpu()->getRegAddr(7) += 2;
        this->dumpState();
        this->showState();
        QCoreApplication::processEvents();
        if (emulating_ == false)
            goto exit0;
    }

exit0:
    return true;
}

bool Emulator::step()
{
    args_prototype_t args_prototype;
    args_t args;
    int currentPointer = getVcpu()->getRegValue(7) / 2;
    uint16_t instr = getVcpu()->getMemValue(currentPointer);

    decoder_.defineArguments(&args_prototype, instr);
    fillArguments(&args, &args_prototype);

    decoder_.decodeAndExecute(vcpu_, opcode_t{ instr }, args);
    *getVcpu()->getRegAddr(7) += 2;
    this->dumpState();
    this->showState();
    QCoreApplication::processEvents();
}

args_t Emulator::fillArguments(args_t* args, args_prototype_t* args_prototype)
{
    switch (args_prototype->instrType)
    {
        case SINGLE_OPERAND:
        {
            args->arg1 = getArgViaMode(args_prototype->arg1, args_prototype->mode1);
            break;
        }
        case CONDITIONAL:
        {
            args->offset = (int8_t)args_prototype->arg1;
            break;
        }
        case DOUBLE_OPERAND_REG:
        {
            args->arg1 = getArgViaMode(args_prototype->arg1, 0);
            args->arg2 = getArgViaMode(args_prototype->arg2, args_prototype->mode2);
            break;
        }
        case DOUBLE_OPERAND:
        {
            args->arg1 = getArgViaMode(args_prototype->arg1, args_prototype->mode1);
            args->arg2 = getArgViaMode(args_prototype->arg2, args_prototype->mode2);
            break;
        }
    }
}

uint16_t* Emulator::getArgViaMode(uint16_t arg, uint16_t mode)
{
    switch (mode)
    {
        case 0:
        {
            uint16_t* value = vcpu_->getRegAddr(arg);
            return value;
        }
        case 1:
        {
            uint16_t value = vcpu_->getRegValue(arg);
            return vcpu_->getMemAddr(value);
        }
        case 2:
        {
            uint16_t value = vcpu_->getRegValue(arg);
            vcpu_->setRegValue(arg, value + 1);
            return vcpu_->getMemAddr(value);
        }
        case 3:
        {
            uint16_t value = vcpu_->getRegValue(arg);
            uint16_t address = vcpu_->getMemValue(value);
            vcpu_->setRegValue(arg, value + 2);
            return vcpu_->getMemAddr(address);
        }
        case 4:
        {
            uint16_t value = vcpu_->getRegValue(arg) - 1;
            vcpu_->setRegValue(arg, value);
            return vcpu_->getMemAddr(value);
        }
        case 5:
        {
            uint16_t value = vcpu_->getRegValue(arg) - 2;
            vcpu_->setRegValue(arg, value);
            uint16_t address = vcpu_->getMemValue(value);
            return vcpu_->getMemAddr(address);
        }
        case 6:
        {
            uint16_t value = vcpu_->getRegValue(arg) + vcpu_->getMemValue(vcpu_->getRegValue(7));
            uint16_t* pc_addr = vcpu_->getMemAddr(vcpu_->getRegValue(7));
            *pc_addr += 2;
            return vcpu_->getMemAddr(value);
        }
        case 7:
        {
            uint16_t value = vcpu_->getRegValue(arg) + vcpu_->getMemValue(vcpu_->getRegValue(7));
            uint16_t* pc_addr = vcpu_->getMemAddr(vcpu_->getRegValue(7));
            *pc_addr += 2;
            uint16_t address = vcpu_->getMemValue(value);
            return vcpu_->getMemAddr(address);
        }
    };
}

bool Emulator::showState()
{
    ui->lineEdit_2->setText(QString::number(vcpu_->getRegValue(0)));
    ui->lineEdit_3->setText(QString::number(vcpu_->getRegValue(1)));
    ui->lineEdit_4->setText(QString::number(vcpu_->getRegValue(2)));
    ui->lineEdit_5->setText(QString::number(vcpu_->getRegValue(3)));
    ui->lineEdit_6->setText(QString::number(vcpu_->getRegValue(4)));
    ui->lineEdit_7->setText(QString::number(vcpu_->getRegValue(5)));
    ui->lineEdit_8->setText(QString::number(vcpu_->getRegValue(6)));
    ui->lineEdit_9->setText(QString::number(vcpu_->getRegValue(7)));

    QImage videoState(vcpu_->getFrameBuffer(), SCREEN_WIDTH, SCREEN_HEIGHT, QImage::Format_Indexed8);
    ui->screen->setPixmap(QPixmap::fromImage(videoState).scaled(WINDOW_WIDTH, WINDOW_HEIGHT));

    ui->lineEdit_10->setText(QString::number(vcpu_->getFlag(0)));
    ui->lineEdit_11->setText(QString::number(vcpu_->getFlag(1)));
    ui->lineEdit_12->setText(QString::number(vcpu_->getFlag(2)));
    ui->lineEdit_13->setText(QString::number(vcpu_->getFlag(3)));

    QString memDump("");
    vcpu_->getMemString(memDump);
    ui->textBrowser->setText(memDump);

    QString disasmDump("");
    getDisasmDump(disasmDump);
    ui->textBrowser_2->setText(disasmDump);
    qApp->processEvents();
    return true;
}

bool Emulator::getDisasmDump(QString &out_str)
{
    for (uint16_t currentPointer = 0; currentPointer < MEM_SIZE; currentPointer++)
    {
        args_prototype_t args_prototype;
        args_t args;
        uint16_t instr = vcpu_->getMemValue(currentPointer);
        QString disasmDelta("");

        decoder_.defineArguments(&args_prototype, instr);
        decoder_.decodeAndDisasm(vcpu_, opcode_t{ instr }, args_prototype, disasmDelta);
        disasmDelta = "0x" + QString("%1").arg(2*(currentPointer), 4, 16, QChar('0')) + ": " + disasmDelta;
        if (2*currentPointer == getVcpu()->getRegValue(7))
            disasmDelta += " <<<";
        out_str += disasmDelta + "\n";
    }

    return true;
}

bool Emulator::dumpState()
{
    std::cout << "============STATE DUMP============\n";
    std::cout << "  R0 = " << vcpu_->getRegValue(0) << std::endl;
    std::cout << "  R1 = " << vcpu_->getRegValue(1) << std::endl;
    std::cout << "  R2 = " << vcpu_->getRegValue(2) << std::endl;
    std::cout << "  R3 = " << vcpu_->getRegValue(3) << std::endl;
    std::cout << "  R4 = " << vcpu_->getRegValue(4) << std::endl;
    std::cout << "  R5 = " << vcpu_->getRegValue(5) << std::endl;
    std::cout << "  R6 = " << vcpu_->getRegValue(6) << std::endl;
    std::cout << "  R7 = " << vcpu_->getRegValue(7) << std::endl;
    std::cout << "============   END    ============\n";
}

bool Emulator::setUi(Ui::MainWindow* ui) {
    this->ui = ui;
    return true;
}

Vcpu* Emulator::getVcpu() {
    return (this->vcpu_);
}

bool Emulator::stop()
{
    emulating_ = false;
    return true;
}

bool Emulator::reset()
{
    getVcpu()->reset();
    showState();
    return true;
}
