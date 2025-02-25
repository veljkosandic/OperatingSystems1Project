//
// Created by os on 3/31/23.
//
//Kontinualna alokacija - first fit algoritam
//singleton klasa
#ifndef PROJEKAT_MEMALLOCATOR_HPP
#define PROJEKAT_MEMALLOCATOR_HPP
#include "syscall_c.h"
    class MemoryAllocator {
    private:
        struct FreeMem { //velicina : 24
            size_t size; //velicina sa FreeMem struct
            FreeMem *next, *prev;
        };
        struct AllocMem{ //velicina : 24
            size_t size; //velicina sa AllocMem struct
            AllocMem *next, *prev;
        };
        FreeMem* firstFit(size_t size);
        FreeMem *headFree;
        AllocMem *headAlloc;
        MemoryAllocator() : headFree((FreeMem *) HEAP_START_ADDR),headAlloc(nullptr) {
            headFree->next = headFree->prev = nullptr;
            headFree->size = (size_t) HEAP_END_ADDR - (size_t) HEAP_START_ADDR;
        }
        void AllocInsert(AllocMem*);
        void FreeInsert(FreeMem*);
        int connect(FreeMem*, FreeMem*);
    public:
        static const size_t HEADER_SIZE=sizeof(AllocMem);
        static MemoryAllocator& Inst();
        MemoryAllocator(const MemoryAllocator &m) = delete; //jer je Singleton
        MemoryAllocator& operator=(const MemoryAllocator &m) = delete;
        static void* mem_alloc(size_t size);
        static int mem_free(void*);
        static inline size_t sizeToBlocks(size_t size);
};
inline size_t MemoryAllocator::sizeToBlocks(size_t size) {
    return ((size+MemoryAllocator::HEADER_SIZE)/MEM_BLOCK_SIZE + ((size+MemoryAllocator::HEADER_SIZE)%MEM_BLOCK_SIZE!=0));
}
#endif //PROJEKAT_MEMALLOCATOR_HPP
