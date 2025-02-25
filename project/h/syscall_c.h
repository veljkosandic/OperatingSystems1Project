//
// Created by os on 4/10/23.
//

#ifndef PROJ_SYSCALL_C_H
#define PROJ_SYSCALL_C_H
#include "../lib/hw.h"
//rad sa memorijom
const int EOF = -1;
void* mem_alloc(size_t size);//0x01
int mem_free (void *);//0x02
//rad sa nitima
class TCB;
typedef TCB* thread_t;
int thread_create(thread_t* handle,void (*start_routine)(void*),void* arg);//0x11
int thread_exit();//0x12
void thread_dispatch();//0x13
void thread_join(thread_t handle);//0x14
//rad sa semaforima
class _sem;
typedef _sem* sem_t;
int sem_open(sem_t* handle,unsigned int);
int sem_close(sem_t handle);
int sem_wait(sem_t id);
int sem_signal(sem_t id);
//asinhrona promjena konteksta
typedef unsigned long time_t;
int time_sleep(time_t);
//konzola
char getc();
void putc(char);

#endif //PROJ_SYSCALL_C_H
