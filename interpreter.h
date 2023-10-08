#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <vector>
#include <fstream>
#include <iostream>

class Interpreter final {
public:
    Interpreter();

    void loadProgram(const std::string &filename);
    void executeProgram();
    ~Interpreter();

private:
    std::vector<int> registers;
    std::vector<int> program;
    int accumulator;

    void executeInstruction(int &pc);
};

#endif // INTERPRETER_H
