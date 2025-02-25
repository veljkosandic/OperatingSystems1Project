#include "../h/syscall_c.h" //ideja preuzeta sa vjezbi 7,zadatak 6

void* operator new[](uint64 n){
    return mem_alloc(n);
}
void operator delete[](void *p)noexcept{
    mem_free(p);
}