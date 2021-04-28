#pragma once
#include "gtest/gtest.h"
#include <filesystem>
#include "Processor.h"

const static std::vector<std::filesystem::path> roms
{
    "invaders.h",
    "invaders.g",
    "invaders.f",
    "invaders.e"
};

const static std::filesystem::path instructions = "instuctions.set";

class ProcessorTest : public ::testing::Test {
protected:
    // You can remove any or all of the following functions if their bodies would
    // be empty.
    std::shared_ptr<Processor> p_Processor;

    ProcessorTest() 
    {
        // You can do set-up work for each test here.
        p_Processor = std::make_shared<Processor>(instructions);
        p_Processor->Initialize(roms, 0xFFFF, 0x2000, 0x2400, 0x4000);
    }

    ~ProcessorTest() override {
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