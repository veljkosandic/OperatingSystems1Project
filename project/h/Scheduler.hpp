//
// Created by os on 4/17/23.
//

#ifndef PROJ_SCHEDULER_HPP
#define PROJ_SCHEDULER_HPP
#include "list.hpp"
class TCB; //dio interfejsa preuzet sa vjezbi 7, zadatak 6 - sinhrona promjena konteksta

class Scheduler{
private:
    static List<TCB> ready;
    static TCB* idle;
    friend int main();
    friend class Riscv;
    friend class TCB;
public:
    static Scheduler& getInst(){
        static Scheduler inst;
        return inst;
    }
    static TCB* get();
    static void put(TCB *tcb);
};
#endif //PROJ_SCHEDULER_HPP