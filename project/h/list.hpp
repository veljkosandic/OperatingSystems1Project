//
// Created by os on 4/17/23.
//

#ifndef PROJ_LIST_HPP
#define PROJ_LIST_HPP
#include "../lib/hw.h"
#include "memAllocator.hpp"
template<typename T>//interfejs preuzet sa vjezbi 7.,zadatak 6 - sinhrona promjena konteksta
class List{
protected:
    struct Node{
        T* data;
        Node* next;
        Node(T* data):data(data),next(nullptr){};
        void* operator new(size_t size){
            return MemoryAllocator::mem_alloc(MemoryAllocator::sizeToBlocks(size));
        }
        void operator delete(void* pok){
            MemoryAllocator::mem_free(pok);
        }
    };

    Node *first,*last;
public:
    List() : first(nullptr),last(nullptr){};
    List(const List<T>&) = delete;
    List<T>& operator=(const List<T>&) = delete;
    void addFirst(T* data){
        Node* node = new Node(data);
        if(last==nullptr)
            last = node;
        else node->next=first;
        first=node;
    }
    void addLast(T* data){
        Node* node = new Node(data);
        if(first==nullptr)
            first=node;
        else last->next=node;
        last=node;
    }
    T* removeFirst(){
        if(!first) return nullptr;
        T* res = first->data;
        if(last==first)last=nullptr;
        Node* temp = first;
        first=first->next;
        delete temp;
        return res;
    }
    T* peekFirst() const{
        if(first==nullptr)return nullptr;
        else return first->data;
    }
    T* removeLast(){
        if(!last)return nullptr;
        T* res = last->data;
        Node* tek = first;
        while( tek!= last && tek->next!=last)tek=tek->next;
        if(tek==last)first=tek=nullptr;
        delete last;
        last=tek;
        return res;
    }
    T* peekLast() const{
        if(last==nullptr)return nullptr;
        else return last->data;
    }
    void deleteElem(T* elem){
        Node* node = first,*pret=nullptr;
        while(node){
            if(node->data==elem)break;
            pret=node;
            node=node->next;
        }
        if(!node)return;
        if(!pret){removeFirst();return;}
        pret->next=node->next;
        if(node==last)last=pret;
        delete node;
    }
};
#endif //PROJ_LIST_HPP
