#pragma once
#include "gtest/gtest.h"
#include "Processor.h"


class InstructionSetTest : public ::testing::Test {
protected:
    // You can remove any or all of the following functions if their bodies would
    // be empty.
    InstructionSetTest():p_Processor(std::make_shared<Processor>("InstructionSet.csv")), m_State(p_Processor->getState()), m_MemoryMap(p_Processor->getMemoryMap())
    {
    }

    ~InstructionSetTest() override {
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
    std::shared_ptr<Processor> p_Processor;
    State& m_State;
    MemoryMap& m_MemoryMap;
};