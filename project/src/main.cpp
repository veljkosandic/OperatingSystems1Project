//
// Created by os on 3/31/23.
//
#include "../h/syscall_c.h"
#include "../lib/hw.h"
#include "../h/riscv.hpp"
#include "../h/TCB.hpp"
#include "../h/kprint.h"
#include "../h/_console.hpp"
void userMain();
void userMainWrapper(void* args){
    userMain();
}
void idleFunc(void* arg){
    while(true) {
        thread_dispatch();
    }
}
int main(){ //sizeof(int) = 4,sizeof(long) = 8
    Riscv::w_stvec((uint64) (&Riscv::trapRoutine) | 0x01UL);//postavljanje prekidne rutine
    _console::getInst();//inicijalizacija konzole
    MemoryAllocator::Inst();
    Scheduler::getInst();
    TCB* kernelThread,*test,*idle,*writer;
    thread_create(&idle, &idleFunc, nullptr);
    Scheduler::idle=idle;
    Scheduler::get();//da izbacimo idle nit iz schedulera
    thread_create(&writer,_console::writer,nullptr);
    thread_create(&kernelThread,nullptr,nullptr);
    //thread_create(&reader,readFunc,nullptr)
     thread_create(&test,&userMainWrapper,nullptr);
    TCB::running=kernelThread;
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    thread_join(test);
    //brisanje niti
    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);//kraj asinhrone promjene konteksta
    Riscv::deleteThreads();
    delete idle;
    delete writer;
    delete kernelThread;
    return 0;
}