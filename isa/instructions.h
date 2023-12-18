#pragma once
#include <iostream>
#include <unordered_map>

#include <string>
#include <vector>

#include "isa_for_parser.h"

enum OpCode : uint64_t {
#define OPLIST(code, mnemonic) code,
  ISA(OPLIST)
#undef OPLIST
};

enum IRegisters : uint8_t {
  ACC = 0x0,
  R0 = 0x1,
  R1 = 0x2,
  R2 = 0x3,
  R3 = 0x4,
  R4 = 0x5,
  R5 = 0x6,
  R6 = 0x7,
  R7 = 0x8,
};

enum FRegisters : uint8_t {
  FACC = 0x0,
  FR0 = 0x1,
  FR1 = 0x2,
  FR2 = 0x3,
  FR3 = 0x4,
  FR4 = 0x5,
  FR5 = 0x6,
  FR6 = 0x7,
  FR7 = 0x8,
};