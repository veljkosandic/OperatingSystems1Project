//
// Created by os on 4/30/23.
//

#ifndef PROJ_SEM_HPP
#define PROJ_SEM_HPP
#include "memAllocator.hpp"
#include "list.hpp"
class boundedBuffer;
class _sem{
public:
    void* operator new(size_t size){
        return MemoryAllocator::mem_alloc(MemoryAllocator::sizeToBlocks(size));
    }
    void* operator new[](size_t size){
        return MemoryAllocator::mem_alloc(MemoryAllocator::sizeToBlocks(size));
    }
    void operator delete(void* p){
        MemoryAllocator::mem_free(p);
    }
    void operator delete[](void* p){
        MemoryAllocator::mem_free(p);
    }
    _sem(int val):val(val),left(0),deleted(false){};
    ~_sem(){}
private:
    friend class Riscv;
    friend class boundedBuffer;
    int val,left;
    bool deleted;
    List<TCB> blocked;
    static int close(_sem *sem);
    static int wait(_sem *sem);
    static int signal(_sem *sem);
};
#endif //PROJ_SEM_HPP
