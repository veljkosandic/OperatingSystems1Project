//
// Created by os on 4/24/23.
//
#include "../h/kprint.h"
#include "../h/_console.hpp"
void kprintStr(const char* string){
    while(*string != '\0'){
        _console::putc(*(string++));
    }
}
void kprintInt(uint64 integer){//uvijek pozitivni brojevi
    if(!integer){
        _console::putc('0');return;
    }
    char arr[20];
    int i=15;
    while(integer>0){
        arr[i--] = integer%10 + '0';
        integer=integer/10;
    }
    i++;
    while(i!=16){
        _console::putc(arr[i++]);
    }
}
void kprintHex(uint64 integer){
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
    _console::putc('0');_console::putc('x');
    for(int k=0;k<16;k++){
        _console::putc(arr[k]);
    }
}