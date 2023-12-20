#include "interpreter.h"
#include <fstream>
#include <iostream>

int main() {
  std::cout << " " << std::endl;
  std::cout << " " << std::endl;
  std::cout << "Welcome to LiriKon VM    ᕙ(⇀‸↼‶)ᕗ" << std::endl;
  std::cout << " " << std::endl;
  std::cout << "Press 1 - square.bin" << std::endl;
  std::cout << "Press 2 - array.bin" << std::endl;
  std::cout << "Press 3 - call_test.bin" << std::endl;
  std::cout << "Press smth else to exit" << std::endl;
  std::cout << " " << std::endl;

  int prog_num;
  std::cin >> prog_num;

  std::string prog_name;

  switch (prog_num) {
  case 1:
    prog_name = "../examples/square.bin";
    std::cout << "Square solver. Enter a, b, c." << std::endl;
    break;
  case 2:
    prog_name = "../examples/array.bin";
    std::cout << "Array test." << std::endl;
    break;
  case 3:
    prog_name = "../examples/call_test.bin";
    std::cout << "Call test." << std::endl;
    break;
  default:
    std::cout << "Invalid symbols. Please try again." << std::endl;
    return 0;
  }

  Interpreter *interpreter = new Interpreter();
  interpreter->loadProgram(prog_name);
  std::vector<interpreter::Instr> program = interpreter->GetProgram();
  interpreter->executeProgram(
      reinterpret_cast<interpreter::Byte *>(program.data()));
  delete interpreter;

  return 0;
}