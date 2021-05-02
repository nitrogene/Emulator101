#pragma once
#include "gtest/gtest.h"
#include <filesystem>
#include "Processor.h"
#include "Intel-8080-Emulator/MachineTemplate.h"

const static std::vector<std::filesystem::path> roms
{
    "cpudiag.bin"
};

const static std::filesystem::path instructions = "instuctions.set";

class CPUDIAGTest : public ::testing::Test {
protected:
    // You can remove any or all of the following functions if their bodies would
    // be empty.
    std::shared_ptr<Processor> p_Processor;

    CPUDIAGTest()
    {
        // You can do set-up work for each test here.
        p_Processor = std::make_shared<Processor>(instructions);

        std::vector<uint8_t> bytes(0x100,0);

        p_Processor->Initialize(roms, 0xFFFF, 0x2000, 0x2400, 0x4000,bytes);
    }

    ~CPUDIAGTest() override {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }

    // Class members declared here can be used by all tests in the test suite
    // for Foo.
};