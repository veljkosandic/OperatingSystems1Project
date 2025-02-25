//
// Created by os on 4/17/23.
//

#ifndef PROJ_RISCV_HPP
#define PROJ_RISCV_HPP
#include "../lib/hw.h"
#include "../h/syscall_c.h"
#include "TCB.hpp"
#include "../h/memAllocator.hpp"
class Riscv {//dosta stvari preuzeto sa vjezbi 7, zadatak 6 i vjezbi 7, zadatak 7
public:
    static void popSppSpie();//vadi spp i spie bite sa sstatus
    static void pushRegisters();//cuvanje registara na stek
    static void popRegisters();//vadjenje registara sa steka
    static uint64 r_scause();//cita registar scause
    static void w_scause(uint64 scause); //upisuje scause u registar scause
    static uint64 r_sepc();//cita registar sepc
    static void w_sepc(uint64 sepc);//upisuje sepc u registar sepc
    static uint64 r_stvec();//cita registar stvec
    static void w_stvec(uint64 stvec);//upisuje stvec u registar stvec
    static uint64 r_stval();//cita registar stval
    static void w_stval(uint64 stval);//upisuje stval u registar stval
    static uint64 ssp;
    enum BitMaskSStatus{
        SSTATUS_SPP=1 << 8,//skok iz sistemskog ili korisnickog rezima
        SSTATUS_SIE=1 << 1,//maskiranje spoljasnjih prekida(samo u sistemskom modu)
        SSTATUS_SPIE=1<< 5//prijethodna vrijednost SIE
    };
    static void ms_sstatus(uint64 mask);//postavlja vrijednost 1 na dati bit sstatus
    static void mc_sstatus(uint64 mask);//postavlja vrijednost 0 na dati bit sstatus
    enum BitMaskSip{
        SIP_SSIP = ( 1 << 1),
        SIP_STIP = (1 << 5),
        SIP_SEIP = (1 << 9)
    };
    static void ms_sip(uint64 mask);//postavlja vrijednost 1 na dati bit sip
    static void mc_sip(uint64 mask);//postavlja vrijednost 0 na dati bit sip
    static uint64 r_sstatus();//cita registar sstatus
    static void w_sstatus(uint64 sstatus);//upisuje sstatus u registar sstatus
    static void trapRoutine();//skok u prekidnu rutinu
    static bool consoleReadReady();
    static bool consoleWriteReady();

    static void deleteThreads();
private:
    static void handleSysError();//opsluzivanje sistemskog poziva ili greske
    static uint64 handleSysCall();//obrada sistemskog poziva
    static void handleConsole();//opsluzivanje konzole
    static void handleTimer();//opsluzivanje tajmera
    enum SyscallList{
        MEM_ALLOC=0x01,MEM_FREE=0x02,
        THREAD_CREATE=0x11,THREAD_EXIT=0x12,THREAD_DISPATCH=0x13,THREAD_JOIN=0x14,
        SEM_OPEN=0x21,SEM_CLOSE=0x22,SEM_WAIT=0x23,SEM_SIGNAL=0x24,
        TIME_SLEEP=0x31,
        GETC=0x41,PUTC=0x42,
        SWITCH_USER=0x51,SWITCH_SUPERVISOR=0x52//dodatne metode, za eksperimentalnu upotrebu
    };
    enum TrapList{
        INT_TIMER=0x8000000000000001UL,
        INT_CONSOLE=0x8000000000000009UL,
        INT_INSTRUCTION=0x0000000000000002UL,
        INT_READ=0x0000000000000005UL,
        INT_WRITE=0x0000000000000007UL,
        INT_ECALL_USER=0x0000000000000008UL,
        INT_ECALL_SYS=0x0000000000000009UL
    };
    static uint64* pars; //parametri registara a1-a7
    static void loadPars();
    static int thread_create(thread_t* handle,void(*start_routine)(void*),void* arg,void* stack_space);
    static int sem_create(sem_t * handle,unsigned init);
};


inline uint64 Riscv::r_scause(){
    uint64 scause;
    __asm__ volatile("csrr %0, scause" : "=r" (scause));
    return scause;
}
inline void Riscv::w_scause(uint64 scause) {
    __asm__ volatile("csrw scause, %0" : : "r" (scause));
}
inline uint64 Riscv::r_sepc(){
    uint64 sepc;
    __asm__ volatile("csrr %0, sepc" : "=r" (sepc));
    return sepc;
}
inline void Riscv::w_sepc(uint64 sepc) {
    __asm__ volatile("csrw sepc, %0" : : "r" (sepc));
}
inline uint64 Riscv::r_stvec(){
    uint64 stvec;
    __asm__ volatile("csrr %0, stvec" : "=r" (stvec));
    return stvec;
}
inline void Riscv::w_stvec(uint64 stvec) {
    __asm__ volatile("csrw stvec, %0" : : "r" (stvec));
}
inline void Riscv::ms_sstatus(uint64 mask) {
    __asm__ volatile ("csrs sstatus, %0" : : "r" (mask));
}
inline void Riscv::mc_sstatus(uint64 mask){
    __asm__ volatile ("csrc sstatus, %0" : : "r" (mask));
}
inline uint64 Riscv::r_stval(){
    uint64 stval;
    __asm__ volatile("csrr %0, stval" : "=r" (stval));
    return stval;
}
inline void Riscv::w_stval(uint64 stval) {
    __asm__ volatile("csrw stval, %0" : : "r" (stval));
}
inline uint64 Riscv::r_sstatus(){
    uint64 sstatus;
    __asm__ volatile("csrr %0, sstatus" : "=r" (sstatus));
    return sstatus;
}
inline void Riscv::w_sstatus(uint64 sstatus) {
    __asm__ volatile("csrw sstatus, %0" : : "r" (sstatus));
}

inline void Riscv::ms_sip(uint64 mask) {
    __asm__ volatile ("csrs sip, %0" : : "r" (mask));
}
inline void Riscv::mc_sip(uint64 mask){
    __asm__ volatile ("csrc sip, %0" : : "r" (mask));
}
inline void Riscv::loadPars() {
    __asm__ volatile("mv %0, a0" : "=r" (pars));
}
inline bool Riscv::consoleReadReady() {
    char temp = *((char*)CONSOLE_STATUS);
    return temp & CONSOLE_RX_STATUS_BIT;
}
inline bool Riscv::consoleWriteReady() {
    char temp = *((char*)CONSOLE_STATUS);
    return temp & CONSOLE_TX_STATUS_BIT;
}
#endif //PROJ_RISCV_HPP
