//
// Created by os on 4/24/23.
//

#include "../h/TCB.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"
#include "../lib/console.h"
#include "../h/SuspendedList.hpp"

TCB* TCB::running=nullptr;//vjezbe 7,zadatak 7
List<TCB> TCB::finished;
uint64 TCB::timeSliceCounter=0;
bool TCB::createKernelThreads=true;
SuspendedList TCB::suspended;
 void TCB::threadWrapper() {
     //__asm__ volatile("mv sp, %0" : : "r" (running->context.ssp));
    // __asm__ volatile("csrr sp, sscratch");
     Riscv::popSppSpie();
    running->body(running->args);
    thread_exit();
}
int TCB::threadExit() {//raditi samo tokom sistemskog poziva
    running->stat=FINISHED;
    TCB* oldThread=running;
    while(running->blockedByJoin.peekLast()){
        TCB* temp = running->blockedByJoin.removeLast();
        temp->setStatus(TCB::RUNNING);
        Scheduler::put(temp);
    }
    if(mem_free(oldThread->stack))return -1;//greska u brisanju steka
    oldThread->stack=nullptr;
    TCB::finished.addFirst(oldThread);
    TCB::dispatch();
    return 0;
}
void TCB::dispatch(){
    TCB::timeSliceCounter=0;
    TCB* old = running;
    if(old->stat==RUNNING)Scheduler::put(old);
    running=Scheduler::get();
    //zamjena konteksta
    TCB::contextSwitch(&old->context, &running->context);
 }
 void TCB::join(thread_t thread) {
     if(thread && thread!=running && thread->stat!=FINISHED) {
         running->stat = BLOCKED;
         thread->blockedByJoin.addFirst(running);
         dispatch();
     }
 }
 int TCB::sleep(uint64 time){
     //if(time<0)return -1;//unijeto negativno vrijeme - ovo se zapravo ne moze desiti
     if(!time)return 0;//nema potrebe sa suspendovanjem
     TCB::running->setSuspendedTime(time);
     TCB::running->setStatus(SUSPENDED);
     suspended.addFirstSorted(TCB::running);
     TCB::dispatch();
     return 0;
 }