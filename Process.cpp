#include "Process.h"

InstructionFetcher::InstructionFetcher()
:pc(0),inst(0),working(false){}

void InstructionFetcher::FetchInstruction(u_int8_t *memory, u_int32_t &_pc,bool &stop) {
    pc = _pc;
    memcpy(&inst,memory + pc,4);
    if(inst == 0x0ff00513) { stop = true;return; }
    _pc += 4;
    working = true;
}

InstructionDecoder::InstructionDecoder()
:working(false),immediate(0),rs1(),rs2(),rd(){}

void InstructionDecoder::DecodeInstruction(u_int32_t _inst, u_int32_t &_pc) {
    OPType type = getType(_inst);
    op = getName(type,_inst);
    if(type == U){
        immediate = int32_t (_inst >> 12) << 12;
        rd = (_inst >> 7) & 0b11111u;
    }else if(type == J){
        u_int32_t imm20 = _inst >> 31 << 20;
        u_int32_t imm10_1 =( (_inst >> 20 ) & 2046u);
        u_int32_t imm11 = ((_inst >> 19) & 2u) << 10;
        u_int32_t imm19_12 = ((_inst >> 12) & 0b11111111) << 12;
        immediate = int(_inst >> 31 << 20 | _inst << 1 >> 22 << 1 | _inst << 11 >> 31 << 11 | _inst << 12 >> 24 << 12) << 11 >> 11;
        rd = (_inst >> 7) & 0b11111u;
    }else if(type == I){
        rd = (_inst >> 7) & 0b11111u;
        rs1 = (_inst >> 15) & 0b11111u;
        immediate = int32_t (_inst >> 20) << 20 >> 20;
    }else if(type == B){
        rd = (_inst >> 7) & 0b11111u;
        rs1 = (_inst >> 15) & 0b11111u;
        rs2 = (_inst >> 20) & 0b11111u;
        immediate =  int(_inst >> 31 << 12 | _inst << 1 >> 26 << 5 | _inst << 20 >> 28 << 1 | _inst << 24 >> 31 << 11) << 19 >> 19;
    }else if(type == S){
        rs1 = (_inst >> 15) & 0b11111u;
        rs2 = (_inst >> 20) & 0b11111u;
        u_int32_t imm11_5 = ((_inst >> 25) & 0x3fu) << 5;
        u_int32_t imm4_0 = (_inst >> 7) & 0x1fu;
        immediate = int(_inst >> 25 << 5 | _inst << 20 >> 27) << 20 >> 20;
     } else if(type == R){
        rd = (_inst >> 7) & 0b11111u;
        rs1 = (_inst >> 15) & 0b11111u;
        rs2 = (_inst >> 20) & 0b11111u;
        immediate = 0;
    }
    working = true;
}

Executer::Executer()
:working(false),rd(),rs1(),rs2(),immediate(),op(),result(),TarAddress(),pc() {}

void Executer::Execute(InstructionDecoder x,u_int32_t &_pc,int32_t rg[]) {
    if(!x.working)return;
    immediate = x.immediate;
    rs1 = x.rs1,rs2 = x.rs2,rd = x.rd;
    op = x.op;
    pc = _pc;
    switch (op) {
        case ADD:
            result = rg[rs1] + rg[rs2];
            break;
        case SUB:
            result = rg[rs1] - rg[rs2];
            break;
        case SLL: {
            u_int32_t mov = rg[rs2] & 0b00011111;
            result = rg[rs1] << mov;
            break;
        }
        case SLT:
            if(rg[rs1] < rg[rs2])result = 1;
            else result = 0;
            break;
        case SLTU:
            if(u_int32_t(rg[rs1]) < u_int32_t(rg[rs2]))result = 1;
            else result = 0;
            break;
        case XOR:
            result = rg[rs1] ^ rg[rs2];
            break;
        case SRL: {
            u_int32_t mov = rg[rs2] & 0b00011111;
            result = rg[rs1] >> mov;
            break;
        }
        case SRA: {
            u_int32_t mov = rg[rs2] & 0b00011111;
            result = rg[rs1] >> mov;
            break;
        }
        case OR:
            result = rg[rs1] | rg[rs2];
            break;
        case AND:
            result = rg[rs1] & rg[rs2];
            break;
        case JALR: {
            result = pc;
            if(rd == 0)rd = 1;
            pc = (rg[rs1] + immediate) & ~1;
            break;
        }
        case LB: {
            TarAddress = rg[rs1] + immediate;
            break;
        }
        case LH:
            TarAddress = rg[rs1] + immediate;
            break;
        case LW:
            TarAddress = rg[rs1] + immediate;
            break;
        case LBU:
            TarAddress = rg[rs1] + immediate;
            break;
        case LHU:
            TarAddress = rg[rs1] + immediate;
            break;
        case ADDI:
            result = rg[rs1] + immediate;
            break;
        case SLTI:
            if(rg[rs1] < immediate)result = 1;
            else result = 0;
            break;
        case SLTIU:
            if(u_int32_t(rg[rs1]) < u_int32_t(immediate))result = 1;
            else result = 0;
            break;
        case XORI:
            result = rg[rs1] ^ immediate;
            break;
        case ORI:
            result = rg[rs1] | immediate;
            break;
        case ANDI:
            result = rg[rs1] & immediate;
            break;
        case SLLI:
            result = rg[rs1] <<  (immediate & 0x1f);
            break;
        case SRAI:
            result = rg[rs1] >>  (immediate & 0x1f);
            break;
        case SRLI:
            result = (u_int32_t)rg[rs1] >> (immediate & 0x1f);
            break;
        case SB: {
            result = rg[rs2] & 0xff;
            TarAddress = rg[rs1] + immediate;
            break;
        }
        case SH:{
            result = rg[rs2] & 0xffff;
            TarAddress = rg[rs1] + immediate;
            break;
        }
        case SW:{
            result = rg[rs2];
            TarAddress = rg[rs1] + immediate;
            break;
        }
        case BEQ:
            if(rg[rs1] == rg[rs2])pc += immediate - 4;
            break;
        case BNE:
            if(rg[rs1] != rg[rs2])pc += immediate - 4;
            break;
        case BLT:
            if(rg[rs1] < rg[rs2])pc += immediate - 4;
            break;
        case BGE:
            if(rg[rs1] >= rg[rs2])pc += immediate - 4;
            break;
        case BLTU:
            if((u_int32_t)rg[rs1] < (u_int32_t)rg[rs2])pc += immediate - 4;
            break;
        case BGEU:
            if((u_int32_t)rg[rs1] >= (u_int32_t)rg[rs2])pc += immediate - 4;
            break;
        case LUI:
            result = immediate;
            break;
        case AUIPC:
            result = immediate + pc - 4;
            break;
        case JAL:
            result = pc;
            pc += immediate - 4;
            break;
        case INVALID:
            break;
    }
    _pc = pc;
}

MemoryAccessor::MemoryAccessor():working(false),op(),address(),result() {}

void MemoryAccessor::AccessMemory(Executer x, u_int8_t *memory) {
    op = x.op,address = x.TarAddress,result = x.result;
    switch (op) {
        case LB: {
            int8_t tmp;
            memcpy(&tmp,memory + address,sizeof(int8_t));
            result = tmp;
            break;
        }
        case LH:{
            int16_t tmp;
            memcpy(&tmp,memory + address,sizeof(int16_t));
            result = tmp;
            break;
        }
        case LW:{
            memcpy(&result,memory + address,sizeof(u_int32_t));
            break;
        }
        case LBU:{
            u_int8_t tmp;
            memcpy(&tmp,memory + address,sizeof(u_int8_t));
            result = tmp;
            break;
        }
        case LHU:{
            u_int16_t tmp;
            memcpy(&tmp,memory + address,sizeof(u_int16_t));
            result = tmp;
            break;
        }
        case SB: {
            u_int8_t tmp = result;
            memcpy(memory + address, &tmp, sizeof(u_int8_t));
            break;
        }
        case SH: {
            u_int16_t tmp = result;
            memcpy(memory + address, &tmp, sizeof(u_int16_t));
            break;
        }
        case SW: {
            u_int32_t tmp = result;
            memcpy(memory + address, &tmp, sizeof(u_int32_t));
            break;
        }
        default: break;
    }
}

Writer::Writer():op(),result(),tarReg(),working(false){}

void Writer::WriteBack(MemoryAccessor x, int32_t rg[],u_int32_t tar) {
    rg[0] = 0;
    op = x.op;
    if(op == SB || op == SW || op == SH || op == BEQ || op == BNE || op == BGE || op == BLT || op == BLTU || op == BGEU)return;
    result = x.result;
    tarReg = tar;
    if(tar == 0)return;
    rg[tarReg] = result;
}