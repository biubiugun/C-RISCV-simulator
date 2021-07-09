#ifndef RISCV_PROCESS_H
#define RISCV_PROCESS_H
#include <cstdio>
#include <iostream>
#include "order.h"
#include "BranchPredictor.h"
#include <cstring>

using namespace RISCV;
class InstructionFetcher{
public:
    u_int32_t pc;
    int32_t inst;
    bool working = false;

    InstructionFetcher();

    void FetchInstruction(u_int8_t *memory,u_int32_t &_pc,bool &stop,bool ID_stall,bool jump,u_int32_t jump_to,bool isBubble);
};

class InstructionDecoder{
public:
    bool working = false;
    bool stall = false;
    bool jump = false;
    bool IsBubble = false;
    u_int32_t jump_to;
    OPName op;
    int32_t immediate;
    u_int32_t rs1,rs2,rd,pc;

    InstructionDecoder();

    void DecodeInstruction(u_int32_t _inst,u_int32_t &_pc,bool IF_working,int modify[],int rg[],bool &isBubble,u_int32_t IF_pc,BranchPredictor &predictor);


};

class Executer{
public:
    bool working = false;
    bool stall = false;
    bool nextIsBubble = false;
    OPName op;
    int32_t immediate;
    u_int32_t rs1,rs2,rd;
    int32_t result;
    u_int32_t TarAddress;
    u_int32_t pc;

    Executer();

    void Execute(InstructionDecoder &x,u_int32_t &pc,int32_t rg[],BranchPredictor &predictor);

};

class MemoryAccessor{
public:
    bool  working = false;
    bool stall = false;
    OPName op;
    u_int32_t address;
    int32_t result;
    u_int32_t rd;

    MemoryAccessor();

    void AccessMemory(Executer &x,u_int8_t *memory);
};

class Writer{
public:
    bool stall = false;
    OPName op;
    int32_t result;
    u_int32_t tarReg;

    Writer();

    void WriteBack(MemoryAccessor &x,int32_t rg[],int modify[]);
};

#endif //RISCV_PROCESS_H
