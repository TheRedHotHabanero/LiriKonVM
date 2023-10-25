#include "interpreter.h"

int main() {
    std::string prog_name;
    std::cout << "Enter the name of executing program, for example, examples/square.bin." << std::endl;
    std::cin >> prog_name;
    if (prog_name.empty()) {
        std::cout << "No name entered. Fuck off" << std::endl;
        return 1;
    }

    Interpreter interpreter;
    interpreter.loadProgram(prog_name);
    interpreter.executeProgram();

    return 0;
}