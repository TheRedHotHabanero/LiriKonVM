#ifndef INCLUDE_RUNNER_H
#define INCLUDE_RUNNER_H

#include <vector>
#include "instruction.h"
#include "../isa/instructions.h"
#include "vm.h"

class Runner final {
public:
    Runner();
    ~Runner();
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