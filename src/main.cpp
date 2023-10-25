#include "../include/interpreter.h"

int main() {
    int prog_num;
    std::string prog_name;
    std::cout << "Enter the number of executing program. "
                 "Press 1 to run examples/square.bin. "
              << "Press 2 to quit."
              << std::endl;
    std::cin >> prog_num;
    if (prog_num == 2) {
        std::cout << "Goodbye!" << std::endl;
        return 0;
    } else if (prog_num != 1) {
        std::cout << "Invalid symbols." << std::endl;
        return 1;
    }

    prog_name = "examples/square.bin";

    Interpreter interpreter;
    interpreter.loadProgram(prog_name);
    interpreter.executeProgram();

    return 0;
}