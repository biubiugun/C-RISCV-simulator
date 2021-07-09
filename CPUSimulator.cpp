#include "CPUSimulator.h"
using namespace std;

Simulator::Simulator():CycleNum(0),pc(0),reg(),If(),Id(),Ex(),Mem(),Wb(),stop(false),predictor()
{
    memory = new u_int8_t [500000000];
    for(auto &i : reg)i = 0;
    for(auto &i : modify)i = 0;
}

Simulator::~Simulator() {
    delete [] memory;
}

void Simulator::IF() {
    If.FetchInstruction(memory,pc,stop,Id.stall,Id.jump,Id.jump_to,Id.IsBubble);
}

void Simulator::ID() {
    Id.DecodeInstruction(If.inst,pc,If.working,modify,reg,Ex.nextIsBubble,If.pc,predictor);
}

void Simulator::EX() {
    Ex.Execute(Id,pc,reg,predictor);
}

void Simulator::MEM() {
    Mem.AccessMemory(Ex,memory);
}

void Simulator::WB() {
    Wb.WriteBack(Mem,reg,modify);
}

void Simulator::debug() const{
    cout << "step" << dec << CycleNum << ": pc=" << hex << pc;
    cout << dec;
    cout << ", immediate=" << Id.immediate << ", op=" << Id.op << ", rs1=" << Id.rs1 << ", rs2=" << Id.rs2 << ", rd=" << Id.rd;
    cout << ", result=" << Ex.result << ", memoryAddress=" << Ex.TarAddress;
    cout << ", rd=" << Wb.tarReg << ", resultLoad=" << Wb.result;
    cout << endl;
    cout << reg[1] << " " << reg[2] << " " << reg[10] << " " << reg[12] << " " << reg[14] << " " << reg[15] << endl;
}

void Simulator::run() {
    while(true){
        if(stop){
            WB();
            MEM();
            EX();
            WB();
            MEM();
            WB();
            break;
        }
        CycleNum++;
        WB();
//        for debug：
//        debug();
        MEM();
        EX();
        ID();
        IF();
    }
//    格式输出：
    printf("%d\n", (u_int32_t)reg[10] & 255u);
//    预测成功率：
//    printf("%.2lf%c\n",100 * ((double)predictor.success / predictor.total),'%');
}

