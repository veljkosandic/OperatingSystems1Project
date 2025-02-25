#include "../h/memAllocator.hpp"
void* MemoryAllocator::mem_alloc(size_t blocks){ //alocira se u blokovima
    if(blocks <= 0)return nullptr;
    size_t newSize = blocks * MEM_BLOCK_SIZE; //za svaki blok treba dodati i AllocMem
    FreeMem* tek = Inst().firstFit(newSize);
    if(!tek)return nullptr; //ako ne postoji segment za alokaciju
    if(tek->size> newSize + sizeof(FreeMem)){ // ostaje visak
        FreeMem* newTek = (FreeMem*)((char*)tek + newSize);
        newTek->next=tek->next;
        newTek->prev=tek->prev;
        newTek->size=tek->size-newSize;
        tek->size=newSize;
        if(tek==Inst().headFree)Inst().headFree=newTek;
        if(newTek->prev){
            newTek->prev->next=newTek;
        }else Inst().headFree=newTek;
        if(newTek->next)tek->next->prev=newTek;
    }else{ //nema viska
        if(tek->prev){
            tek->prev->next=tek->next;
        }else Inst().headFree=tek->next;
        if(tek->next)tek->next->prev=tek->prev;
    }
    tek->next=tek->prev=nullptr;
    AllocMem* newAlloc = (AllocMem*)tek;
    Inst().AllocInsert(newAlloc);
    return (void*)((char*)tek + sizeof(AllocMem));
}
MemoryAllocator::FreeMem* MemoryAllocator::firstFit(size_t size) {
    FreeMem* tek;
    for(tek = headFree;tek && tek->size<size + sizeof(FreeMem);tek=tek->next); //zbog zaglavlja
    return tek;
}
void MemoryAllocator::AllocInsert(AllocMem* node){
    AllocMem* after = this->headAlloc,*before=nullptr;
    if(!after){
        headAlloc=node;
        return;
    }
    while(after && (void*) after < (void*) node){
        before=after;
        after=after->next;
    }
    if(before) {
        before->next = node;
    }
        node->prev=before;
    if(after)after->prev=node;
    node->next=after;
    if(after==headAlloc)headAlloc=node;
}
int MemoryAllocator::connect(FreeMem* before,FreeMem* after){
    if(!before || !after || (char*)before + before->size!=(char*)after)return -1;
    before->size+=after->size;
    if(after->next)after->next->prev=before;
    before->next=after->next;
    after->next = after->prev = nullptr;
    after = 0;
    return 0;
}
void MemoryAllocator::FreeInsert(FreeMem* node){
    FreeMem* after = this->headFree,*before=nullptr;
    if(!after){
        headFree=node;
        return;
    }
    while(after && (void*) after < (void*) node){
        before=after;
        after=after->next;
    }
    if(before) {
        before->next = node;
    }
    node->prev=before;
    if(after)after->prev=node;
    node->next=after;
    if(after==headFree)headFree=node;
    if(!Inst().connect(before,node))node=before;
    Inst().connect(node,after);
}
int MemoryAllocator::mem_free(void* ptr){
    AllocMem* cur = Inst().headAlloc;
    while(cur && ((char*)cur+sizeof(AllocMem)) < ptr)cur=cur->next;
    if(!cur || ((char*)cur+sizeof(AllocMem))!=ptr){return -1;} //unijeta je ili slobodna adresa ili ona koja nije generisana iz mem_alloc
    if(cur->prev){
        cur->prev->next=cur->next;
    }else Inst().headAlloc=cur->next;
    if(cur->next)cur->next->prev=cur->prev;
    cur->next=cur->prev=nullptr;
    Inst().FreeInsert((FreeMem*)cur);
    return 0;
}

MemoryAllocator &MemoryAllocator::Inst() {
    static MemoryAllocator inst;
    return inst;
}
