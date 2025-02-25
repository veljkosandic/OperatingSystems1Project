//
// Created by os on 4/17/23.
//
#include "../h/Scheduler.hpp"
#include "../h/TCB.hpp"
List<TCB> Scheduler::ready;
TCB* Scheduler::idle=nullptr;
TCB *Scheduler::get()
{
    if(!ready.peekFirst())return idle;
    return ready.removeFirst();
}

void Scheduler::put(TCB *ccb)
{
    if(ccb!=idle)
    ready.addLast(ccb);
}

