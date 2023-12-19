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
  ACC = 0x9,
  R0 = 0x10,
  R1 = 0x1a,
  R2 = 0x1b,
  R3 = 0x1c,
  R4 = 0x1d,
  R5 = 0x1e,
  R6 = 0x1f,
  R7 = 0x20,
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