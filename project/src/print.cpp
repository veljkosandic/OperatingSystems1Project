//
// Created by os on 4/24/23.
//
#include "../h/print.h"
#include "../h/syscall_c.h"
uint64 lockPrinta=0;
extern "C" uint64 copy_and_swap(uint64 &lock, uint64 expected, uint64 desired);
#define LOCK() while(copy_and_swap(lockPrinta, 0, 1))
#define UNLOCK() while(copy_and_swap(lockPrinta, 1, 0))
void printStr(const char* string){
    LOCK();
    while(*string != '\0'){
        putc(*(string++));
    }
    UNLOCK();
}
void printInt(uint64 integer){//uvijek pozitivni brojevi
    LOCK();
    if(!integer){
        putc('0');
        UNLOCK();
        return;
    }
    char arr[20];
    int i=15;
    while(integer>0){
        arr[i--] = integer%10 + '0';
        integer=integer/10;
    }
    i++;
    while(i!=16){
        putc(arr[i++]);
    }
    UNLOCK();
}
void printHex(uint64 integer){
    LOCK();
    char arr[16]={'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
    int i=15;
    while(integer>0){
        uint8 par =integer%16;
        if(par<10)arr[i--]=par + '0';
        else{
            switch(par){
                case 10:arr[i--]='A';break;
                case 11:arr[i--]='B';break;
                case 12:arr[i--]='C';break;
                case 13:arr[i--]='D';break;
                case 14:arr[i--]='E';break;
                case 15:arr[i--]='F';break;
            }
        }
        integer=integer/16;
    }
    putc('0');putc('x');
    for(int k=0;k<16;k++){
        putc(arr[k]);
    }
    UNLOCK();
}