#ifndef INCLUDE_RUNNER_H
#define INCLUDE_RUNNER_H

#include <vector>
#include "vm.h"

class Runner final {
public:
    Runner() = default;
    ~Runner() = default;

    bool Run(Instruction);

    std::vector<interpreter::IReg> &GetIRegs() {
        return iRegs_;
    }
    std::vector<interpreter::FReg> &GetFRegs() {
        return fRegs_;
    }

private:
    std::vector<interpreter::IReg> iRegs_;
    std::vector<interpreter::FReg> fRegs_;
};

#endif  // INCLUDE_RUNNER_H