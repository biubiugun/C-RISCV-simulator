//created by biubiugun at 2021.7.8
//二位饱和计数器预测
#ifndef RISCV_BRANCHPREDICTOR_H
#define RISCV_BRANCHPREDICTOR_H

class BranchPredictor{
    enum state{
        STRONG_TAKEN = 3,
        WEAK_TAKEN = 2,
        STRONG_NOT_TAKEN = 0,
        WEAK_NOT_TAKEN = 1
    };

    static unsigned int hash(unsigned int pc);
public:
    unsigned int success,total;
    state record[2048]{};

    BranchPredictor();
    ~BranchPredictor() = default;

    bool ifBranch(unsigned int pc);

    void update(bool ifSuccess,unsigned int pc);
};

#endif //RISCV_BRANCHPREDICTOR_H
