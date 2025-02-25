//
// Created by os on 5/8/23.
//

#ifndef PROJ_SUSPENDEDLIST_HPP
#define PROJ_SUSPENDEDLIST_HPP
#include "list.hpp"
#include "memAllocator.hpp"
class TCB;
class SuspendedList : public List<TCB>{
public:
    void* operator new(size_t size){
        return MemoryAllocator::mem_alloc(MemoryAllocator::sizeToBlocks(size));
    }
    void operator delete(void* pok){
        MemoryAllocator::mem_free(pok);
    }
    //za Blocked
    void addFirstSorted(TCB* Thread);
    void decAll();
};

#endif //PROJ_SUSPENDEDLIST_HPP
