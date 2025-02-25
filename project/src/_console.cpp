//
// Created by os on 5/15/23.
//

#include "../h/_console.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"
#include "../h/boundedBuffer.hpp"
void _console::putc(char c) {
    getInst().putBuf->append(c);
}
char _console::getc(){
    char c = getInst().getBuf->get();
    return c;
}
void _console::writer(void *args) {//u odnosu na procesor

    while(true) {
        while(Riscv::consoleWriteReady()) {
            char c = getInst().putBuf->get();
            *getInst().WR_REG = c;
        }
        thread_dispatch();
    }
}

_console::_console(int size) : putBuf(new boundedBuffer(size)), getBuf((new boundedBuffer(size))),READ_REG((char*)CONSOLE_RX_DATA),WR_REG((char*)CONSOLE_TX_DATA){
}

_console &_console::getInst() {
    static _console inst;
    return inst;
}
