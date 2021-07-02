#ifndef RISCV_ORDER_H
#define RISCV_ORDER_H
namespace RISCV{

    enum OPType{
        R,
        I,
        S,
        B,
        U,
        J,
        UNKNOWN
    };

    enum OPName{
        //R
        ADD,SUB,SLL,SLT,SLTU,XOR,SRL,SRA,OR,AND,
        //I
        JALR,
        LB,LH,LW,LBU,LHU,
        ADDI,SLTI,SLTIU,XORI,ORI,ANDI,SLLI,SRAI,SRLI,
        //S
        SB,SH,SW,
        //B
        BEQ,BNE,BLT,BGE,BLTU,BGEU,
        //U
        LUI,AUIPC,
        //J
        JAL,
        //OTHER
        INVALID
    };

    inline OPType getType(u_int32_t inst){
        u_int32_t opcode = inst << 25 >> 25;
        if(opcode == 0b0110111 || opcode ==  0b0010111)return U;
        else if(opcode == 0b1101111)return J;
        else if(opcode == 0b1100011)return B;
        else if(opcode == 0b1100111 || opcode == 0b0000011 || opcode == 0b0010011
                || opcode == 0b0001111 || opcode == 0b1110011)return I;
        else if(opcode == 0b0110011)return R;
        else if(opcode == 0b0100011)return S;
        else return UNKNOWN;
    }

    inline OPName   getName(OPType type,u_int32_t inst){
        if(type == U){
            u_int32_t opcode = inst << 25 >> 25;
            if(opcode == 0b0110111)return LUI;
            else if(opcode == 0b0010111)return AUIPC;
            else return INVALID;
        }else if(type == J)return JAL;
        else if(type == B){
            u_int32_t func3 = (inst >> 12) & 7u;
            if(func3 == 0b000)return BEQ;
            else if(func3 == 0b001)return BNE;
            else if(func3 == 0b100)return BLT;
            else if(func3 == 0b101)return BGE;
            else if(func3 == 0b110)return BLTU;
            else if(func3 == 0b111)return BGEU;
            else return INVALID;
        }else if(type == S){
            u_int32_t func3 = (inst >> 12) & 7u;
            if(func3 == 0b000)return SB;
            else if(func3 == 0b001)return SH;
            else if(func3 == 0b010)return SW;
            else return INVALID;
        }else if(type == R){
            u_int32_t func3 = (inst >> 12) & 7u;
            u_int32_t func7 = inst >> 25;
            if(func3 == 0b000){
                if(func7 == 0b0000000)return ADD;
                else if(func7 == 0b0100000)return SUB;
                else return INVALID;
            }else if(func3 == 0b001)return SLL;
            else if(func3 == 0b010)return SLT;
            else if(func3 == 0b011)return SLTU;
            else if(func3 == 0b100)return XOR;
            else if(func3 == 0b101){
                if(func7 == 0b0000000)return SRL;
                else if(func7 == 0b0100000)return SRA;
                else return INVALID;
            }else if(func3 == 0b110)return OR;
            else if(func3 == 0b111)return AND;
            else return INVALID;
        }else if (type == I){
            u_int32_t opcode = inst << 25 >> 25;
            u_int32_t func3 = (inst >> 12) & 7u;
            if(opcode == 0b1100111)return JALR;
            else if(opcode == 0b0000011){
                if(func3 == 0b000)return LB;
                else if(func3 == 0b001)return LH;
                else if(func3 == 0b010)return LW;
                else if(func3 == 0b100)return LBU;
                else if(func3 == 0b101)return LHU;
                else return INVALID;
            }else if(opcode == 0b0010011){
                if(func3 == 0b000)return ADDI;
                else if(func3 == 0b010)return SLTI;
                else if(func3 == 0b011)return SLTIU;
                else if(func3 == 0b100)return XORI;
                else if(func3 == 0b110)return ORI;
                else if(func3 == 0b111)return ANDI;
                else if(func3 == 0b001)return SLLI;
                else if(func3 == 0b101){
                    u_int32_t func7 = inst >> 25;
                    if(func7 == 0b0000000)return SRLI;
                    else if(func7 == 0b0100000)return SRAI;
                    else return INVALID;
                }else return INVALID;
            }else return INVALID;
        }else if(type == UNKNOWN)return INVALID;
        return INVALID;
    }
}

#endif //RISCV_ORDER_H
