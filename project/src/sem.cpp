//
// Created by os on 4/30/23.
//
//inspirisano sintaksom iz knjige
#include "../h/sem.hpp"
#include "../h/Scheduler.hpp"
#include "../h/TCB.hpp"
int _sem::close(_sem *sem) {
    if(sem==nullptr)return -1;
    sem->left=-sem->val;
    if(sem->blocked.peekFirst()==nullptr){//ako nijedna nit ne ceka mozemo slobodno da obrisemo semafor
        delete sem;
        return 0;
    }
    while(sem->blocked.peekFirst()!=nullptr)
        signal(sem);
    sem->deleted=true;
    return 0;
}
int _sem::wait(_sem *sem) {
    if(sem==nullptr || sem->deleted)return -1;
    if(--sem->val<0){
        TCB::running->setStatus(TCB::BLOCKED);
        sem->blocked.addLast(TCB::running);
        TCB::dispatch();
    }
    if(sem->deleted){
        if(!--sem->left)delete sem;
        return -2;
    }
    return 0;
}
int _sem::signal(_sem *sem) {
    if(sem==nullptr || sem->deleted)return -1;
    if(++(sem->val)<=0){
        TCB* t = sem->blocked.removeFirst();
        t->setStatus(TCB::RUNNING);
        Scheduler::put(t);
    }
    return 0;
}