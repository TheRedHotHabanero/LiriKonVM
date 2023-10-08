#include "interpreter.h"

int main() {
    Interpreter interpreter;
    interpreter.loadProgram("examples/square.bin");
    interpreter.executeProgram();
    return 0;
}