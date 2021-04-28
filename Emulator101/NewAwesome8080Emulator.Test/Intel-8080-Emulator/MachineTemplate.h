#pragma once
extern "C" {
#include "i8080.h"
}

#include <chrono>

//REMINDER
//When implementing a specific machine
//execute CPU and platform window (graphics) on separate threads
//appropriately sleep the CPU thread to have it run at 2MHz
class MachineTemplate
{
    //TODO
    //Define machine specific input and output ports

private:
    //CPU
    State8080* state;

    //Timer & Interrupt variables
    long long now;
    long long lastTimer;
    double nextInterrupt;
    double sinceLastCycle;
    int whichInterrupt;

public:
    //Set to True if you want CPU to output trace of execution
    bool printTrace;
    MachineTemplate(bool printTraceSet);

    void* FrameBuffer();
    void MachineOUT(uint8_t port, uint8_t value);
    uint8_t MachineIN(uint8_t port);
    long long timeusec();
    void doCPU();
    void doCPUStep(int& cycles);

    const State8080& getState() const;
};