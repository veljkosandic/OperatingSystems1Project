//
// Created by os on 5/15/23.
//

#ifndef PROJ__CONSOLE_HPP
#define PROJ__CONSOLE_HPP
#include "boundedBuffer.hpp"
const int DEFAULT_BUFFER_SIZE=64;
class _console{
private:
    boundedBuffer* putBuf,*getBuf;
     char* READ_REG;
     char* WR_REG;
    _console(int size=DEFAULT_BUFFER_SIZE);

public:
    friend class Riscv;
    static _console& getInst();
    static char getc();
    static void putc(char c);
    static void writer(void* args);
};
#endif //PROJ__CONSOLE_HPP
