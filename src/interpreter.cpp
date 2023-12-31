#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "array.h"
#include "frame.h"
#include "instructions.h"
#include "interpreter.h"

std::vector<interpreter::Instr> Interpreter::GetProgram() {
  return parser_->GetProgram();
}

Interpreter::Interpreter() {
  decoder_ = Decoder::Init();
  runner_ = Runner::Init();
  parser_ = new Parser();
  allocator_ = std::make_unique<ArenaAllocator>(vm_numbers::VM_DEFAULT_ARENA);
  allocator_->allocate(vm_numbers::VM_DEFAULT_ARENA);
  char *const_pool = new char[32];
  pool_.push_back(std::make_tuple(interpreter::Types::FLOAT, 8,
                                  reinterpret_cast<int *>(const_pool)));
  pool_.push_back(std::make_tuple(interpreter::Types::FLOAT, 8,
                                  reinterpret_cast<int *>(const_pool + 8)));
  pool_.push_back(std::make_tuple(interpreter::Types::FLOAT, 8,
                                  reinterpret_cast<int *>(const_pool + 16)));
}

Interpreter::~Interpreter() {
  delete decoder_;
  delete runner_;
}

void Interpreter::loadProgram(const std::string filename) {

  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: Unable to open the program file." << std::endl;
    exit(1);
  }

  parser_->parseProgram(file);
}

interpreter::Instr Interpreter::executeInstruction(interpreter::Byte *bytecode,
                                                   interpreter::IReg pc) {
  return *reinterpret_cast<interpreter::Instr *>(&bytecode[pc]);
}

void Interpreter::executeProgram(interpreter::Byte *bytecode) {
  static void *dispatch_table[] = {&&HANDLE_ADD,
                                   &&HANDLE_SUB,
                                   &&HANDLE_MUL,
                                   &&HANDLE_DIV,
                                   &&HANDLE_ADDF,
                                   &&HANDLE_SUBF,
                                   &&HANDLE_MULF,
                                   &&HANDLE_DIVF,
                                   &&HANDLE_AND,
                                   &&HANDLE_OR,
                                   &&HANDLE_XOR,
                                   &&HANDLE_NEG,
                                   &&HANDLE_NEGF,
                                   &&HANDLE_SHL,
                                   &&HANDLE_SHR,
                                   &&HANDLE_MOV_IMM_TO_ACC,
                                   &&HANDLE_MOV_REG_TO_REG,
                                   &&HANDLE_MOV_IMM_TO_ACCF,
                                   &&HANDLE_MOV_REG_TO_REGF,
                                   &&HANDLE_MOV_ACC_TO_REG,
                                   &&HANDLE_MOV_ACC_TO_REGF,
                                   &&HANDLE_INPUT,
                                   &&HANDLE_OUTPUT,
                                   &&HANDLE_INPUTF,
                                   &&HANDLE_OUTPUTF,
                                   &&HANDLE_RETURN,
                                   &&HANDLE_SIN,
                                   &&HANDLE_COS,
                                   &&HANDLE_SQRT,
                                   &&HANDLE_POW,
                                   &&HANDLE_NEWARR,
                                   &&HANDLE_LOADARR,
                                   &&HANDLE_STOREARR,
                                   &&HANDLE_CALL,
                                   &&HANDLE_RET,
                                   &&HANDLE_LABEL,
                                   &&HANDLE_INVALID};

  Frame *frame = new Frame(vm_numbers::VM_DEFAULT_FRAME, nullptr);
  std::unordered_map<int, int> labels;

  for (int i = 0; static_cast<uint64_t>(i) < vm_numbers::LABELS_COUNT; i++) {
    labels[i] = 0;
  }

  // auto frame = myFrame->getCurrentFramePtr();
  // void* current_array = nullptr;
  if (frame == nullptr) {
    std::cerr << "Error. Nullptr frame";
    std::abort();
  }

  interpreter::IReg pc = (frame->getPC());

  // 1st round
  Instruction *cur_instr = new Instruction();
  *cur_instr = decoder_->decodeInstruction(executeInstruction(bytecode, pc));

  while (cur_instr->GetInstOpcode() != OpCode::END) {
    if (cur_instr->GetInstOpcode() == OpCode::LABEL) {
      if (static_cast<uint64_t>(cur_instr->imm) > vm_numbers::LABELS_COUNT) {
        std::cerr << "Error. Exceeded maximum labels amount";
        std::abort();
      }
      labels[cur_instr->imm] = pc;
    }
    pc += 4;
    *cur_instr = decoder_->decodeInstruction(executeInstruction(bytecode, pc));
  }

  // 2nd round
  pc = (frame->getPC());
  *cur_instr = decoder_->decodeInstruction(executeInstruction(bytecode, pc));
  goto *dispatch_table[cur_instr->GetInstOpcode()];

#define NEXT()                                                                 \
  pc += 4;                                                                     \
  frame->setPC(pc);                                                            \
  *cur_instr = decoder_->decodeInstruction(executeInstruction(bytecode, pc));  \
  goto *dispatch_table[cur_instr->GetInstOpcode()];

HANDLE_ADD:
  *reinterpret_cast<interpreter::IReg *>(frame->regPtr(IRegisters::ACC)) =
      *reinterpret_cast<interpreter::IReg *>(frame->regPtr(cur_instr->reg_id)) +
      *reinterpret_cast<interpreter::IReg *>(
          frame->regPtr(cur_instr->GetSecondReg()));
  NEXT();
HANDLE_ADDF:
  *reinterpret_cast<interpreter::FReg *>(frame->regPtr(FRegisters::FACC)) =
      *reinterpret_cast<interpreter::FReg *>(frame->regPtr(cur_instr->reg_id)) +
      *reinterpret_cast<interpreter::FReg *>(
          frame->regPtr(cur_instr->GetSecondReg()));
  NEXT();
HANDLE_SUB:
  *reinterpret_cast<interpreter::IReg *>(frame->regPtr(IRegisters::ACC)) =
      *reinterpret_cast<interpreter::IReg *>(frame->regPtr(cur_instr->reg_id)) -
      *reinterpret_cast<interpreter::IReg *>(
          frame->regPtr(cur_instr->GetSecondReg()));
  NEXT();
HANDLE_SUBF:
  *reinterpret_cast<interpreter::FReg *>(frame->regPtr(FRegisters::FACC)) =
      *reinterpret_cast<interpreter::FReg *>(frame->regPtr(cur_instr->reg_id)) -
      *reinterpret_cast<interpreter::FReg *>(
          frame->regPtr(cur_instr->GetSecondReg()));
  NEXT();
HANDLE_MUL:
  *reinterpret_cast<interpreter::IReg *>(frame->regPtr(IRegisters::ACC)) =
      *reinterpret_cast<interpreter::IReg *>(frame->regPtr(cur_instr->reg_id)) *
      *reinterpret_cast<interpreter::IReg *>(
          frame->regPtr(cur_instr->GetSecondReg()));
  NEXT();
HANDLE_MULF:
  *reinterpret_cast<interpreter::FReg *>(frame->regPtr(FRegisters::FACC)) =
      *reinterpret_cast<interpreter::FReg *>(frame->regPtr(cur_instr->reg_id)) *
      *reinterpret_cast<interpreter::FReg *>(
          frame->regPtr(cur_instr->GetSecondReg()));
  NEXT();
HANDLE_DIV:
  if (*reinterpret_cast<interpreter::IReg *>(
          frame->regPtr(cur_instr->GetSecondReg())) == 0) {
    std::cerr << "Error. Devision by zero." << std::endl;
    return;
  }
  *reinterpret_cast<interpreter::IReg *>(frame->regPtr(IRegisters::ACC)) =
      *reinterpret_cast<interpreter::IReg *>(frame->regPtr(cur_instr->reg_id)) /
      *reinterpret_cast<interpreter::IReg *>(
          frame->regPtr(cur_instr->GetSecondReg()));
  NEXT();
HANDLE_DIVF:
  if (*reinterpret_cast<interpreter::FReg *>(
          frame->regPtr(cur_instr->GetSecondReg())) == 0) {
    std::cerr << "Error. Devision by zero." << std::endl;
    return;
  }
  *reinterpret_cast<interpreter::FReg *>(frame->regPtr(FRegisters::FACC)) =
      *reinterpret_cast<interpreter::FReg *>(frame->regPtr(cur_instr->reg_id)) /
      *reinterpret_cast<interpreter::FReg *>(
          frame->regPtr(cur_instr->GetSecondReg()));
  NEXT();
HANDLE_AND:
  *reinterpret_cast<interpreter::IReg *>(frame->regPtr(IRegisters::ACC)) =
      *reinterpret_cast<interpreter::IReg *>(frame->regPtr(cur_instr->reg_id)) &
      *reinterpret_cast<interpreter::IReg *>(
          frame->regPtr(cur_instr->GetSecondReg()));
  NEXT();
HANDLE_OR:
  *reinterpret_cast<interpreter::IReg *>(frame->regPtr(IRegisters::ACC)) =
      *reinterpret_cast<interpreter::IReg *>(frame->regPtr(cur_instr->reg_id)) |
      *reinterpret_cast<interpreter::IReg *>(
          frame->regPtr(cur_instr->GetSecondReg()));
  NEXT();
HANDLE_XOR:
  *reinterpret_cast<interpreter::IReg *>(frame->regPtr(IRegisters::ACC)) =
      *reinterpret_cast<interpreter::IReg *>(frame->regPtr(cur_instr->reg_id)) ^
      *reinterpret_cast<interpreter::IReg *>(
          frame->regPtr(cur_instr->GetSecondReg()));
  NEXT();
HANDLE_SHL:
  *reinterpret_cast<interpreter::IReg *>(frame->regPtr(IRegisters::ACC)) =
      *reinterpret_cast<interpreter::IReg *>(frame->regPtr(cur_instr->reg_id))
      << *reinterpret_cast<interpreter::IReg *>(
             frame->regPtr(cur_instr->GetSecondReg()));
  NEXT();
HANDLE_SHR:
  *reinterpret_cast<interpreter::IReg *>(frame->regPtr(IRegisters::ACC)) =
      *reinterpret_cast<interpreter::IReg *>(
          frame->regPtr(cur_instr->reg_id)) >>
      *reinterpret_cast<interpreter::IReg *>(
          frame->regPtr(cur_instr->GetSecondReg()));
  NEXT();
HANDLE_NEG:
  *reinterpret_cast<interpreter::IReg *>(frame->regPtr(IRegisters::ACC)) =
      -*reinterpret_cast<interpreter::IReg *>(frame->regPtr(cur_instr->reg_id));
  NEXT();
HANDLE_NEGF:
  *reinterpret_cast<interpreter::FReg *>(frame->regPtr(FRegisters::FACC)) =
      -*reinterpret_cast<interpreter::FReg *>(frame->regPtr(cur_instr->reg_id));
  NEXT();
HANDLE_MOV_IMM_TO_ACC:
  *reinterpret_cast<interpreter::IReg *>(frame->regPtr(IRegisters::ACC)) =
      static_cast<double>(cur_instr->imm);
  NEXT();
HANDLE_MOV_IMM_TO_ACCF:
  *reinterpret_cast<interpreter::FReg *>(frame->regPtr(FRegisters::FACC)) =
      static_cast<double>(cur_instr->imm);
  NEXT();
HANDLE_MOV_ACC_TO_REG:
  *reinterpret_cast<interpreter::IReg *>(frame->regPtr(cur_instr->reg_id)) =
      *reinterpret_cast<interpreter::IReg *>(frame->regPtr(IRegisters::ACC));
  NEXT();
HANDLE_MOV_ACC_TO_REGF:
  *reinterpret_cast<interpreter::FReg *>(frame->regPtr(cur_instr->reg_id)) =
      *reinterpret_cast<interpreter::FReg *>(frame->regPtr(FRegisters::FACC));
  NEXT();
HANDLE_MOV_REG_TO_REG:
  *reinterpret_cast<interpreter::IReg *>(frame->regPtr(cur_instr->reg_id)) =
      *reinterpret_cast<interpreter::IReg *>(
          frame->regPtr(cur_instr->GetSecondReg()));
  NEXT();
HANDLE_MOV_REG_TO_REGF:
  *reinterpret_cast<interpreter::FReg *>(frame->regPtr(cur_instr->reg_id)) =
      *reinterpret_cast<interpreter::FReg *>(
          frame->regPtr(cur_instr->GetSecondReg()));
  NEXT();
HANDLE_INPUT:
  std::cin >>
      *reinterpret_cast<interpreter::IReg *>(frame->regPtr(cur_instr->reg_id));
  NEXT();
HANDLE_INPUTF:
  std::cin >>
      *reinterpret_cast<interpreter::FReg *>(frame->regPtr(cur_instr->reg_id));
  NEXT();
HANDLE_OUTPUT:
  std::cout << "Output: "
            << *reinterpret_cast<interpreter::IReg *>(
                   frame->regPtr(IRegisters::ACC))
            << std::endl;
  NEXT();
HANDLE_OUTPUTF:
  std::cout << "Output: "
            << *reinterpret_cast<interpreter::FReg *>(
                   frame->regPtr(FRegisters::FACC))
            << std::endl;
  NEXT();
HANDLE_RETURN:
  delete frame;
  delete cur_instr;
  return;
HANDLE_SIN:
  *reinterpret_cast<interpreter::FReg *>(frame->regPtr(FRegisters::FACC)) =
      std::sin(*reinterpret_cast<interpreter::FReg *>(
          frame->regPtr(cur_instr->reg_id)));
  NEXT();
HANDLE_COS:
  *reinterpret_cast<interpreter::FReg *>(frame->regPtr(FRegisters::FACC)) =
      std::cos(*reinterpret_cast<interpreter::FReg *>(
          frame->regPtr(cur_instr->reg_id)));
  NEXT();
HANDLE_SQRT:
  if (*reinterpret_cast<interpreter::FReg *>(frame->regPtr(cur_instr->reg_id)) <
      0) {
    std::cerr << "Error. Expression for sqrt is < 0." << std::endl;
    return;
  }
  *reinterpret_cast<interpreter::FReg *>(frame->regPtr(FRegisters::FACC)) =
      std::sqrt(*reinterpret_cast<interpreter::FReg *>(
          frame->regPtr(cur_instr->reg_id)));
  NEXT();
HANDLE_POW:
  *reinterpret_cast<interpreter::FReg *>(frame->regPtr(FRegisters::FACC)) =
      std::pow(*reinterpret_cast<interpreter::FReg *>(
                   frame->regPtr(cur_instr->reg_id)),
               *reinterpret_cast<interpreter::FReg *>(
                   frame->regPtr(cur_instr->GetSecondReg())));
  NEXT();
HANDLE_NEWARR:
  auto arr_ptr = reinterpret_cast<Frame *>(
      frame->freeMemory(static_cast<size_t>(cur_instr->imm)));
  Array arr =
      Array(sizeof(interpreter::IReg), reinterpret_cast<void *>(arr_ptr));
  intptr_t arr_ptr_address = reinterpret_cast<intptr_t>(arr_ptr);
  *reinterpret_cast<interpreter::IReg *>(frame->regPtr(IRegisters::ACC)) =
      static_cast<interpreter::IReg>(arr_ptr_address);
  NEXT();
HANDLE_LOADARR:
  *reinterpret_cast<interpreter::IReg *>(frame->regPtr(IRegisters::ACC)) =
      arr.getValue(*reinterpret_cast<interpreter::IReg *>(
          frame->regPtr(cur_instr->reg_id)));
  NEXT();
HANDLE_STOREARR:
  arr.setValue(
      *reinterpret_cast<interpreter::IReg *>(frame->regPtr(cur_instr->reg_id)),
      *reinterpret_cast<interpreter::IReg *>(
          frame->regPtr(cur_instr->GetSecondReg())));

  NEXT();
HANDLE_INVALID:
  std::cerr << "Error: Unknown opcode " << std::endl;
  exit(1);
HANDLE_CALL:
  Frame *next_frame = new Frame(vm_numbers::VM_DEFAULT_FRAME, frame);
  next_frame->setPC(labels[cur_instr->imm]);
  std::memcpy(
      reinterpret_cast<void *>(next_frame->getStartMem()),
      reinterpret_cast<interpreter::FReg *>(frame->regPtr(FRegisters::FACC)),
      sizeof(interpreter::FReg));

  frame = next_frame;
  pc = frame->getPC();
  NEXT();
HANDLE_RET:
  if (frame->prev_frame_ == nullptr) {
    std::cerr << "Error. Nullptr return frame";
    std::abort();
  }
  std::memcpy(
      reinterpret_cast<void *>(frame->prev_frame_->getStartMem()),
      reinterpret_cast<interpreter::FReg *>(frame->regPtr(FRegisters::FACC)),
      sizeof(interpreter::FReg));

  Frame *temp_frame = frame->prev_frame_;
  delete (frame);
  frame = temp_frame;
  pc = frame->getPC();
  NEXT();
HANDLE_LABEL:
  if (static_cast<uint64_t>(cur_instr->imm) > vm_numbers::LABELS_COUNT) {
    std::cerr << "Error. Exceeded maximum labels amount";
    std::abort();
  }
  labels[cur_instr->imm] = pc;
  NEXT();
}
