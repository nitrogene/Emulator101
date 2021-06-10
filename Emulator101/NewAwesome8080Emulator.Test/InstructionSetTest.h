#pragma once
#include "gtest/gtest.h"
#include <filesystem>
#include "InstructionSet.h"


const static std::filesystem::path instructions = "instructions.set";

class InstructionSetTest : public ::testing::Test {
protected:
    // You can remove any or all of the following functions if their bodies would
    // be empty.
    std::shared_ptr<InstructionSet> p_InstructionSet;

    InstructionSetTest()
    {
        p_InstructionSet = std::make_shared<InstructionSet>(instructions);
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
};