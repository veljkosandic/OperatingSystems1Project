//
// Created by os on 5/8/23.
//
#include "../h/SuspendedList.hpp"
#include "../h/TCB.hpp"
void SuspendedList::addFirstSorted(TCB *thread) {
    if(!first){addFirst(thread);return;}//prazna lista
    Node* cur= first,*prev=nullptr;
    while(cur){
        if(cur->data->getSuspendedTime()>thread->getSuspendedTime())break;
        prev=cur;
        cur=cur->next;
    }
    if(!cur){addLast(thread);return;}//najduze suspendovan
    if(!prev){addFirst(thread);return;}//najkrace suspendovan
    Node* node = new Node(thread);
    node->next=prev->next;
    prev->next=node;
}

void SuspendedList::decAll() {
    Node* tek = first;
    while(tek) {
    tek->data->decSuspendedTime();
    tek=tek->next;
    }
}