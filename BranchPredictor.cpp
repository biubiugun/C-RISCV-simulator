#include "BranchPredictor.h"

#include <iostream>

unsigned int BranchPredictor::hash(unsigned int pc) {
    if(pc > 2047)return (pc / 2 + 1) % 2039;
    else return pc;
}

BranchPredictor::BranchPredictor():success(0),total(0) {
    for(auto &i : record)
        i = WEAK_NOT_TAKEN;
}

bool BranchPredictor::ifBranch(unsigned int pc) {
    switch (record[hash(pc)]) {
        case STRONG_TAKEN:
        case WEAK_TAKEN:
            return true;
        case STRONG_NOT_TAKEN:
        case WEAK_NOT_TAKEN:
            return false;
    }
}

void BranchPredictor::update(bool ifSuccess, unsigned int pc) {
    unsigned index = hash(pc);
    switch (record[index]) {
        case STRONG_TAKEN:
            if(!ifSuccess)record[index] = WEAK_TAKEN;
            break;
        case WEAK_TAKEN:
            if(ifSuccess)record[index] = STRONG_TAKEN;
            else record[index] = WEAK_NOT_TAKEN;
            break;
        case STRONG_NOT_TAKEN:
            if(!ifSuccess)record[index] = WEAK_NOT_TAKEN;
            break;
        case WEAK_NOT_TAKEN:
            if(ifSuccess)record[index] = STRONG_NOT_TAKEN;
            else record[index] = WEAK_TAKEN;
            break;
    }
}