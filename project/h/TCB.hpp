//
// Created by os on 4/24/23.
//
//inspirisano vjezbama 7,zadatak 7 - asinhrona promjena konteksta
#ifndef PROJ_TCB_HPP
#define PROJ_TCB_HPP
#include "../lib/hw.h"
#include "../h/Scheduler.hpp"
#include "../h/memAllocator.hpp"
#include "../h/SuspendedList.hpp"
using Body = void(*)(void*);
class TCB{
public:
    enum ThreadStatus{
        RUNNING,BLOCKED,FINISHED,SUSPENDED
    };
    void* operator new(size_t size){
        return MemoryAllocator::mem_alloc(MemoryAllocator::sizeToBlocks(size));
    }
    void* operator new[](size_t size){
        return MemoryAllocator::mem_alloc(MemoryAllocator::sizeToBlocks(size));
    }
    TCB(uint64* stack,Body body,void* args):stack(stack),stat(ThreadStatus::RUNNING), timeSlice(DEFAULT_TIME_SLICE){
        context.ra=(uint64)&threadWrapper;//moram ovako jer zbog nekog razloga mi dibager puca kada hocu da inicijalizujem strukturu
        context.sp=(uint64)(stack != nullptr ? &stack[DEFAULT_STACK_SIZE] : nullptr),
        this->body=body;
        this->args=args;
        this->isKernel=createKernelThreads;
        if(body!=nullptr)Scheduler::put(this);else createKernelThreads=false;
    }
    ~TCB(){
        finished.deleteElem(this);
       if(stack) MemoryAllocator::mem_free(stack);
    }
    static TCB* running;//trenutno izvrsavana nit
    static void dispatch();
    static void threadDeleter(void* arg);
    void setStatus(ThreadStatus stat){
        this->stat=stat;
    }
    uint64 getTimeSlice() const{return timeSlice;}
    uint64 getSuspendedTime() const{return suspendedTime;}
    void setSuspendedTime(uint64 time){this->suspendedTime=time;}
    void decSuspendedTime(){if(suspendedTime>0)suspendedTime--;}
private:
   static bool createKernelThreads;
   List<TCB> blockedByJoin;
    static List<TCB> finished;
   static void join(thread_t thread);
   static int threadExit();
    friend class Riscv;
    uint64* stack;
    static void threadWrapper();
    struct Context{
        uint64 ra;//povratna adresa
        uint64 sp;//stack pointer
    };

    Context context;
    Body body;
    void* args;
    ThreadStatus stat;
    static void contextSwitch(Context *oldContext,Context* newContext);
    uint64 timeSlice;
    static uint64 timeSliceCounter;
    static int sleep(uint64 time);
    uint64 suspendedTime=0;
    bool isKernel;
    static SuspendedList suspended;
};
#endif //PROJ_TCB_HPP
