#define ISA(_)                                                                 \
  _(ADD, "ADD")                                                                \
  _(SUB, "SUB")                                                                \
  _(MUL, "MUL")                                                                \
  _(DIV, "DIV")                                                                \
  _(FADD, "FADD")                                                              \
  _(FSUB, "FSUB")                                                              \
  _(FMUL, "FMUL")                                                              \
  _(FDIV, "FDIV")                                                              \
  _(AND, "AND")                                                                \
  _(OR, "OR")                                                                  \
  _(XOR, "XOR")                                                                \
  _(NEG, "NEG")                                                                \
  _(FNEG, "FNEG")                                                              \
  _(SHL, "SHL")                                                                \
  _(SHR, "SHR")                                                                \
  _(MOV_IMM_TO_ACC, "MOV_IMM_TO_ACC")                                          \
  _(MOV_REG_TO_REG, "MOV_REG_TO_REG")                                          \
  _(FMOV_IMM_TO_ACC, "FMOV_IMM_TO_ACC")                                        \
  _(FMOV_REG_TO_REG, "FMOV_REG_TO_REG")                                        \
  _(MOV_ACC_TO_REG, "MOV_ACC_TO_REG")                                          \
  _(FMOV_ACC_TO_REG, "FMOV_ACC_TO_REG")                                        \
  _(INPUT, "INPUT")                                                            \
  _(OUTPUT, "OUTPUT")                                                          \
  _(FINPUT, "FINPUT")                                                          \
  _(FOUTPUT, "FOUTPUT")                                                        \
  _(RETURN, "RETURN")                                                          \
  _(SIN, "SIN")                                                                \
  _(COS, "COS")                                                                \
  _(SQRT, "SQRT")                                                              \
  _(POW, "POW")                                                                \
  _(NEWARR, "NEWARR")                                                          \
  _(LOADARR, "LOADARR")                                                        \
  _(STOREARR, "STOREARR")                                                      \
  _(INVALID, "INVALID")

#define INT_REGS(_)                                                            \
  _(R0, "r0")                                                                  \
  _(R1, "r1")                                                                  \
  _(R2, "r2")                                                                  \
  _(R3, "r3")                                                                  \
  _(R4, "r4")                                                                  \
  _(R5, "r5")                                                                  \
  _(R6, "r6")                                                                  \
  _(R7, "r7")                                                                  \
  _(ACC, "acc")

#define FLOAT_REGS(_)                                                          \
  _(FR0, "fr0")                                                                \
  _(FR1, "fr1")                                                                \
  _(FR2, "fr2")                                                                \
  _(FR3, "fr3")                                                                \
  _(FR4, "fr4")                                                                \
  _(FR5, "fr5")                                                                \
  _(FR6, "fr6")                                                                \
  _(FR7, "fr7")                                                                \
  _(FACC, "facc")
