#pragma once
#include "gtest/gtest.h"
#include <filesystem>
#include "Processor.h"
#include "superzazu-8080/i8080.h"

const static std::vector<std::filesystem::path> roms
{
    "invaders.h",
    "invaders.g",
    "invaders.f",
    "invaders.e"
};

const static std::filesystem::path instructions = "InstructionSet.csv";

class SpaceInvadersTest : public ::testing::Test {
protected:
    // You can remove any or all of the following functions if their bodies would
    // be empty.
    SpaceInvadersTest()
    {
        // You can do set-up work for each test here.
        p_Processor = std::make_shared<Processor>(instructions);
        p_Processor->Initialize(roms, 0xFFFF);

        p_i8080State = std::make_shared<i8080>();
        i8080_init(p_i8080State.get());
        m_i8080Memory = p_Processor->getMemoryMap();
        p_i8080State->read_byte = [](void* mem, uint16_t addr)->uint8_t 
        {
            auto map = (MemoryMap*)mem;
            return map->Peek(addr);
        };
        p_i8080State->write_byte = [](void* mem, uint16_t addr, uint8_t value)
        {
            auto map = (MemoryMap*)mem;
            map->Poke(addr, value);
        };
        p_i8080State->port_in = [](void*, uint8_t)->uint8_t {return 0; };
        p_i8080State->port_out = [](void*, uint8_t, uint8_t) {};
        p_i8080State->userdata = &m_i8080Memory;
    }

    ~SpaceInvadersTest() override {
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
    //std::shared_ptr<MachineTemplate> p_MachineTemplate;

    std::shared_ptr<i8080> p_i8080State;
    MemoryMap m_i8080Memory;

    void fun();
};