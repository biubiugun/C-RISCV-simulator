#ifndef RISCV_PROCESS_H
#define RISCV_PROCESS_H
#include <cstdio>
#include <iostream>
#include "order.h"

using namespace RISCV;
class InstructionFetcher{
public:
    u_int32_t pc;
    int32_t inst;
    bool working;

    InstructionFetcher();

    void FetchInstruction(u_int8_t *memory,u_int32_t &_pc,bool &stop);
};

class InstructionDecoder{
public:
    bool working;
    OPName op;
    int32_t immediate;
    u_int32_t rs1,rs2,rd;

    InstructionDecoder();

    void DecodeInstruction(u_int32_t _inst,u_int32_t &_pc);


};

class Executer{
public:
    bool working;
    OPName op;
    int32_t immediate;
    u_int32_t rs1,rs2,rd;
    int32_t result;
    u_int32_t TarAddress;
    u_int32_t pc;

    Executer();

    void Execute(InstructionDecoder x,u_int32_t &pc,int32_t rg[]);

};

class MemoryAccessor{
public:
    bool  working;
    OPName op;
    u_int32_t address;
    int32_t result;

    MemoryAccessor();

    void AccessMemory(Executer x,u_int8_t *memory);
};

class Writer{
public:
    bool working;
    OPName op;
    int32_t result;
    u_int32_t tarReg;

    Writer();

    void WriteBack(MemoryAccessor x,int32_t rg[],u_int32_t tar);
};

#endif //RISCV_PROCESS_H
