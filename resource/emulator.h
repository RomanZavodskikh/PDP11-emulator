#pragma once
#include <vector>
#include "vcpu.h"
#include "decode.h"

namespace Ui {
class MainWindow;
}

class Emulator
{
    public:
        Emulator();
        ~Emulator ();

        bool loadBin(std::string pathToBin);
        bool tryToEmulate();
        bool stop();
        bool step();
        bool reset();
        bool showState();
        bool dumpState();
        bool setUi(Ui::MainWindow* ui);

        Vcpu* getVcpu();

    private:
        Vcpu* vcpu_;
        Decoder decoder_;

        Ui::MainWindow* ui;
        bool emulating_;

        args_t fillArguments(args_t* args, args_prototype_t* args_prototype, int* currentPointer);
        uint16_t* getArgViaMode(uint16_t arg, uint16_t mode, int* currentPointer);
};
