#include "CPUSimulator.h"
using namespace std;

Simulator::Simulator():CycleNum(0),pc(0),reg(),If(),Id(),Ex(),Mem(),Wb(),stop(false)
{
    memory = new u_int8_t [0x200000000];
    for(auto &i : reg)i = 0;
}

Simulator::~Simulator() {
    delete [] memory;
}

void Simulator::IF() {
    If.FetchInstruction(memory,pc,stop);
}

void Simulator::ID() {
    Id.DecodeInstruction(If.inst,pc);
}

void Simulator::EX() {
    Ex.Execute(Id,pc,reg);
}

void Simulator::MEM() {
    Mem.AccessMemory(Ex,memory);
}

void Simulator::WB() {
    Wb.WriteBack(Mem,reg,Ex.rd);
}

void Simulator::run() {
    while(!stop){
//        if(CycleNum > 800)break;
        IF();
//        cout << "step" << CycleNum + 1 << ": pc=" << hex << pc - 4;
//        cout << ", inst= " << If.inst;
        if(stop)break;
        ID();
//        cout << dec;
//        cout << ", immediate=" << Id.immediate << ", op=" << Id.op << ", rs1=" << Id.rs1 << ", rs2=" << Id.rs2 << ", rd=" << Id.rd;
//        if(Id.op == INVALID)break;
        EX();
//        cout << ", result=" << Ex.result << ", memoryAddress=" << Ex.TarAddress;
        MEM();
//        cout << ", resultLoad=" << Mem.result;
        WB();
        CycleNum++;
//        cout << endl;
    }
    printf("%d\n", (u_int32_t)reg[10] & 255u);
}

