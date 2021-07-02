#ifndef RISCV_CPU_SIMULATOR_H
#define RISCV_CPU_SIMULATOR_H
#include <cstdio>
#include <iostream>
#include "Process.h"
using namespace std;



class Simulator{
public:
    u_int32_t pc;
    int32_t reg[33];
    size_t CycleNum;
    u_int8_t *memory;
    bool stop = false;

    InstructionFetcher If;
    InstructionDecoder Id;
    Executer Ex;
    MemoryAccessor Mem;
    Writer Wb;

    void IF();

    void ID();

    void EX();

    void MEM();

    void WB();

    void run();

public:
    explicit Simulator();

    ~Simulator();
};

#endif //RISCV_CPU_SIMULATOR_H
