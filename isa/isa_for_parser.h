#define ISA(_)                                     \
    _(ADD, "ADD", INT)                             \
    _(SUB, "SUB", INT)                             \
    _(MUL, "MUL", INT_INT)                         \
    _(DIV, "DIV", INT_INT)                         \
    _(FADD, "FADD", INT)                           \
    _(FMUL, "FMUL", INT_INT)                       \
    _(FDIV, "FDIV", INT_INT)                       \
    _(AND, "AND", INT)                             \
    _(OR, "OR", INT)                               \
    _(XOR, "XOR", INT)                             \
    _(NEG, "NEG", INT_INT)                         \
    _(FNEG, "FNEG", INT_INT)                       \
    _(SHL, "SHL", INT)                             \
    _(SHR, "SHR", INT)                             \
    _(MOV_IMM_TO_ACC, "MOV_IMM_TO_ACC", INT)       \
    _(MOV_REG_TO_REG, "MOV_REG_TO_REG", INT)       \
    _(FMOV_IMM_TO_ACC, "FMOV_IMM_TO_ACC", INT_INT) \
    _(FMOV_REG_TO_REG, "FMOV_REG_TO_REG", INT_INT) \
    _(MOV_ACC_TO_REG, "MOV_ACC_TO_REG", INT_INT)   \
    _(FMOV_ACC_TO_REG, "FMOV_ACC_TO_REG", INT_INT) \
    _(INPUT, "INPUT", INT)                         \
    _(OUTPUT, "OUTPUT", INT)                       \
    _(FINPUT, "FINPUT", INT_INT)                   \
    _(FOUTPUT, "FOUTPUT", INT_INT)                 \
    _(RETURN, "RETURN", INT_INT)                   \
    _(SIN, "SIN", INT_INT)                         \
    _(COS, "COS", INT)                             \
    _(SQRT, "SQRT", INT)                           \
    _(POW, "POW", INT_INT)                         \
    _(INVALID, "INVALID", INT_INT)                 \
    _(FSUB, "FSUB", INT)

#define INT_REGS(_)  \
    _(R0, "r0", INT) \
    _(R1, "r1", INT) \
    _(R2, "r2", INT) \
    _(R3, "r3", INT) \
    _(R4, "r4", INT) \
    _(R5, "r5", INT) \
    _(R6, "r6", INT) \
    _(R7, "r7", INT) \
    _(ACC, "acc", INT)

#define FLOAT_REGS(_)  \
    _(FR0, "fr0", INT) \
    _(FR1, "fr1", INT) \
    _(FR2, "fr2", INT) \
    _(FR3, "fr3", INT) \
    _(FR4, "fr4", INT) \
    _(FR5, "fr5", INT) \
    _(FR6, "fr6", INT) \
    _(FR7, "fr7", INT) \
    _(FACC, "facc", INT)
