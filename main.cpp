#include "interpreter.h"
#include <iostream>

int main() {
    Interpreter interpreter;
    interpreter.loadProgram("examples/square.bin");
    interpreter.executeProgram();
    return 0;
}