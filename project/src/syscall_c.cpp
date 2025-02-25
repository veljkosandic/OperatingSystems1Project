//
// Created by os on 4/16/23.
//
#include "../h/syscall_c.h"
#include "../h/TCB.hpp"
uint64 retValue();
void moveRegs();
//pakovanje u sistemske pozive
void *mem_alloc (size_t size){
    volatile size_t blocks = MemoryAllocator::sizeToBlocks(size);
    __asm__ volatile("mv a1, %0" : : "r" (blocks));
    __asm__ volatile("li a0, 0x01");
    __asm__ volatile("ecall");
    return (void*)retValue();
}
int mem_free(void* pok){
    moveRegs();
    __asm__ volatile("li a0, 0x02");
    __asm__ volatile("ecall");
    return (int)retValue();
}

int thread_create(thread_t* handle,void (*start_routine)(void*),void* arg){
    thread_t* tempHandle=handle;Body temproutine=start_routine;void* temparg=arg;
    uint64* stack = temproutine!=nullptr ? (uint64*)mem_alloc(DEFAULT_STACK_SIZE*sizeof(uint64)) : nullptr;
    if(temproutine!=nullptr && stack==nullptr)return -1;//nema memorije za stek;
    __asm__ volatile ("li a6,0x11");
    __asm__ volatile ("mv a1, %0" : : "r" (tempHandle));
    __asm__ volatile ("mv a2, %0" : : "r" (temproutine));
    __asm__ volatile ("mv a3, %0" : : "r" (temparg));
    __asm__ volatile ("mv a4, %0" : : "r" (stack));
    __asm__ volatile ("li a0,0x11");
    __asm__ volatile("ecall");
    return (int)retValue();
}
int thread_exit(){
    moveRegs();
    __asm__ volatile ("li a0,0x12");
    __asm__ volatile("ecall");
    return (int)retValue();
}

void thread_dispatch(){
    moveRegs();
    __asm__ volatile ("li a0,0x13");
    __asm__ volatile("ecall");
}

void thread_join(thread_t handle){
    moveRegs();
    __asm__ volatile ("li a0, 0x14");
    __asm__ volatile("ecall");
}
int sem_open(sem_t* handle,unsigned init){
    moveRegs();
    __asm__ volatile ("li a0, 0x21");
    __asm__ volatile("ecall");
    return (int)retValue();
}
int sem_close(sem_t handle){
    moveRegs();
    __asm__ volatile ("li a0, 0x22");
    __asm__ volatile("ecall");
    return (int)retValue();
}
int sem_wait(sem_t handle){
    moveRegs();
    __asm__ volatile ("li a0, 0x23");
    __asm__ volatile("ecall");
    return (int)retValue();
}
int sem_signal(sem_t handle){
    moveRegs();
    __asm__ volatile ("li a0, 0x24");
    __asm__ volatile("ecall");
    return (int)retValue();
}
int time_sleep(time_t time){
    moveRegs();
    __asm__ volatile ("li a0, 0x31");
    __asm__ volatile("ecall");
    return (int)retValue();
}
char getc(){
    __asm__ volatile ("li a0, 0x41");
    __asm__ volatile("ecall");
    return (char)retValue();
}
void putc(char c){
    moveRegs();
    __asm__ volatile ("li a0, 0x42");
    __asm__ volatile("ecall");
}
//=============================================
inline uint64 retValue(){
    uint64 temp;
    __asm__ volatile ("mv a0, %0" : "=r" (temp));
    return temp;

}
inline void moveRegs(){
    __asm__ volatile("mv a7, a6");
    __asm__ volatile("mv a6, a5");
    __asm__ volatile("mv a5, a4");
    __asm__ volatile("mv a4, a3");
    __asm__ volatile("mv a3, a2");
    __asm__ volatile("mv a2, a1");
    __asm__ volatile("mv a1, a0");
}