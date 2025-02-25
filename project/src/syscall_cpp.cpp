//
// Created by os on 5/15/23.
//
#include "../h/syscall_cpp.hpp"
#include "../h/TCB.hpp"
void* operator new(size_t n){//vjezbe 7.,zadatak 6 - sinhrona promjena konteksta
    return mem_alloc(n);
}
void  operator delete(void* p ) {
    mem_free(p);
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&this->myHandle,init);
}

Semaphore::~Semaphore() {
    sem_close(this->myHandle);
}

int Semaphore::wait() {
    return sem_wait(this->myHandle);
}

int Semaphore::signal() {
    return sem_signal(this->myHandle);
}

Thread::Thread(void (*body)(void *), void *arg) {
    this->body=body;
    this->arg=arg;
    myHandle=nullptr;
    thread_create(&myHandle,body,arg);
}

Thread::~Thread() {
    join();
    delete this->myHandle;
}
int Thread::start() {
    if(!myHandle){
        return thread_create(&myHandle,body,arg);
    }
    return -5;
}

void Thread::join() {
    if(myHandle)thread_join(myHandle);
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t time) {
    return time_sleep(time);
}

Thread::Thread() {//slicno zadatku 4., april 2013.
    this->body=this->wrapper;
    this->arg=this;
    this->myHandle=nullptr;
}

void Thread::wrapper(void *arg) {
        if(arg)((Thread*)arg)->run();
}

void PeriodicThread::terminate() {
    this->period=0;
}

PeriodicThread::PeriodicThread(time_t period):Thread() {
    this->period=period;
}

void PeriodicThread::run() {
    while(true){
        PeriodicThread::sleep(period);
        if(!this->period)break;
        Thread* t = new Thread(perWrapper,this);
        t->join();
        delete t;
    }
}

void PeriodicThread::perWrapper(void *args) {
    if(args)((PeriodicThread*)args)->periodicActivation();
}

