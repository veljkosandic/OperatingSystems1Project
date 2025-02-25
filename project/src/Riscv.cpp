//
// Created by os on 4/23/23.
//
#include "../h/riscv.hpp"
#include "../h/kprint.h"
#include "../h/TCB.hpp"
#include "../h/sem.hpp"
#include "../h/_console.hpp"
//isto kao i Riscv.hpp, neke stvari preuzete sa vjezbi 7, zadaci 6 i 7 - sinhrona i asinhrona promjena konteksta
uint64* Riscv::pars=nullptr;
uint64 Riscv::ssp=0;
inline uint64 Riscv::handleSysCall(){
    //uint64 sysCall;
    //__asm__ volatile("mv %0, a0" : "=r" (sysCall));
    switch (pars[0]){
        case MEM_ALLOC: {
            return (uint64)(MemoryAllocator::mem_alloc(pars[1]));
        }
        case MEM_FREE: {
            return (uint64)(MemoryAllocator::mem_free((void*)pars[1]));
        }
        case THREAD_CREATE:{
            return (uint64)Riscv::thread_create((thread_t*)pars[1],(Body)pars[2],(void*)pars[3],(void*)pars[4]);
        }
        case THREAD_EXIT:{
            return (uint64)TCB::threadExit();
        }
        case THREAD_DISPATCH:{
            TCB::dispatch();return 0;
        }
        case THREAD_JOIN:{
            TCB::join((thread_t)pars[1]);return 0;
        }
        case SEM_OPEN:{
            return sem_create((sem_t*)pars[1],(unsigned )pars[2]);
        }
        case SEM_CLOSE:{
            return _sem::close((sem_t)pars[1]);
        }
        case SEM_SIGNAL:{
            return _sem::signal((sem_t)pars[1]);
        }
        case SEM_WAIT:{
            return _sem::wait((sem_t)pars[1]);
        }
        case TIME_SLEEP:{
            return TCB::sleep(pars[1]);
        }
        case GETC:{
            return _console::getc();
        }
        case PUTC:{
            _console::putc((char)pars[1]);return 0;
        }
        /*case SWITCH_USER:{ poprilicno zastarjelo i eksperimentalno
            uint64 spp;
            __asm__ volatile("csrr %0, sstatus" : "=r" (spp));
            if(spp&0x100)printStr("Sistemski rezim!");
            spp &= 0xff;
            __asm__ volatile("csrw sstatus, %0" : : "r" (spp));
            //  __asm__ volatile ("ld a0, %0" : : "r" (1));
            return 0;
        }
        case SWITCH_SUPERVISOR:{
            uint64 spp;
            __asm__ volatile("csrr %0, sstatus" : "=r" (spp));
            if(!(spp&0x100))printStr("Korisnicki rezim!");
            spp |= 0x100;
            __asm__ volatile("csrw sstatus, %0" : : "r" (spp));
            __asm__ volatile ("li a0, 0");return 0;
        }*/
        default:return -1;
    }
}
void Riscv::handleSysError() {
    Riscv::loadPars();
    switch(Riscv::r_scause()){
        case INT_ECALL_SYS:case INT_ECALL_USER : {
            //sistemski poziv
            uint64 volatile sepc=Riscv::r_sepc()+4;
            uint64 volatile sstatus=r_sstatus();
            __asm__ volatile("mv %0, a0" : : "r" (handleSysCall()));
            Riscv::w_sepc(sepc);
            Riscv::w_sstatus(sstatus);
        }break;
        default:{//ispis greske
            kprintStr("scause: ");
            kprintHex(r_scause());
            kprintStr("\n");
            kprintStr("sepc: ");
            kprintHex(r_sepc());
            kprintStr("\n");
            kprintStr("stval: ");
            kprintHex(r_stval());
            kprintStr("\n");
            boundedBuffer *buf = _console::getInst().putBuf;
            while(true){
                while(buf->elems && Riscv::consoleWriteReady()) {
                    char c = buf->buffer[buf->head];
                    buf->head = (buf->head + 1) % buf->len;
                    buf->elems--;
                    *_console::getInst().WR_REG = c;
                }
            }
        }break;
    }
}
void Riscv::handleConsole(){
        //printStr("Spoljasnji prekid od konzole\n");
        uint64 irq = plic_claim();
        if(irq==CONSOLE_IRQ){
            boundedBuffer* buf = _console::getInst().getBuf;
            while(Riscv::consoleReadReady() && buf->elems!=buf->len){
                char c = *_console::getInst().READ_REG;
                buf->buffer[buf->tail]=c;
                buf->tail=(buf->tail+1)%buf->len;
                buf->elems++;
                buf->spaceAvailable->val--;
                _sem::signal(buf->itemAvailable);
            }
        }
    plic_complete(irq);
};
void Riscv::handleTimer(){//preuzeto sa vjezbi 7, zadatak 7 - asinhrona promjena konteksta
        mc_sip(SIP_SSIP);
        TCB::timeSliceCounter++;
        TCB::suspended.decAll();
        while(TCB::suspended.peekFirst() && TCB::suspended.peekFirst()->getSuspendedTime()==0){
            TCB* temp = TCB::suspended.removeFirst();
            temp->setStatus(TCB::RUNNING);
            Scheduler::put(temp);
        }
        if(TCB::timeSliceCounter>=TCB::running->getTimeSlice()){
            uint64 volatile sepc=Riscv::r_sepc();
            uint64 volatile sstatus=r_sstatus();
            TCB::timeSliceCounter=0;
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
};
int Riscv::thread_create(thread_t* handle,void(*start_routine)(void*),void* arg,void* stack_space){
    thread_t* tempHandle=handle;//argumenti, odnosno registri u kojima se nalaze se zbog nekog razloga brisu
    Body tempBody=start_routine;
    void *tempArg=arg,*tempStack=stack_space;
    *tempHandle = new TCB((uint64*)tempStack,tempBody,tempArg);
    if(!*tempHandle)return -3;//nema mjesta za handle
    return 0;
}
int Riscv::sem_create(sem_t *handle, unsigned int init) {
    *handle = new _sem(init);
    if(*handle==nullptr)return -1;//nema mjesta za handle
    return 0;
}
void Riscv::popSppSpie(){//ne smije biti inline
    __asm__ volatile("csrw sepc, ra");//skok u ThreadWrapper
    if(!TCB::running->isKernel) Riscv::mc_sstatus(Riscv::SSTATUS_SPP);
    __asm__ volatile("sret");
}

void Riscv::deleteThreads() {
    //brisanje niti koje cekaju:
    while(true) {
        TCB* thread = Scheduler::get();
        while(thread->blockedByJoin.peekFirst()){//brisanje niti koje se joinuju
            delete thread->blockedByJoin.removeFirst();
        }
        if(thread==Scheduler::idle)break;
        delete thread;
    }
    //brisanje niti koje spavaju:
    while(TCB::suspended.peekFirst())
        delete TCB::suspended.removeFirst();
    //brisanje zavrsenih niti:
    while(TCB::finished.peekFirst()){
        delete TCB::finished.removeFirst();
    }
}
