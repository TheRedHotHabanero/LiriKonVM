#include "interpreter.h"
#include <fstream>
#include <iostream>

int main()
{

  int prog_num;
  std::cout << "Enter the number of executing program. Press 1 to run "
               "square.bin. Press 2 to run call_test.bin. Press 3 to quit."
            << std::endl;
  std::cin >> prog_num;

  if (prog_num == 3) {
    std::cout << "Goodbye!" << std::endl;
    return 0;
  } 
  else if (prog_num == 2) {
    const std::string prog_name = "../examples/array.bin";
    Interpreter *interpreter = new Interpreter();
    interpreter->loadProgram(prog_name);
    std::vector<interpreter::Instr> program = interpreter->GetProgram();
    interpreter->executeProgram(
          reinterpret_cast<interpreter::Byte *>(program.data()));
    delete interpreter;

      return 0;

  }
  else if (prog_num == 1)
  {
    const std::string prog_name = "../examples/square.bin";
    std::cout << "Welcome to quare solver. Enter a, b, c." << std::endl;
    Interpreter *interpreter = new Interpreter();
    interpreter->loadProgram(prog_name);
    std::vector<interpreter::Instr> program = interpreter->GetProgram();
    interpreter->executeProgram(
        reinterpret_cast<interpreter::Byte *>(program.data()));
    delete interpreter;

    return 0;
  }
  else
  {
    std::cout << "Invalid symbols." << std::endl;
    return 1;
  }
}