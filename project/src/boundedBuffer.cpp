//
// Created by os on 5/12/23.
//

#include "../h/boundedBuffer.hpp"
#include "../h/sem.hpp"
boundedBuffer::boundedBuffer(int len):head(0),tail(0),len(len),elems(0) {
    itemAvailable = new _sem(0);
    spaceAvailable = new _sem(len);
    mutex = new _sem(1);
    buffer = new char[len];
}

boundedBuffer::~boundedBuffer() {
    _sem::close(itemAvailable);
    _sem::close(spaceAvailable);
    _sem::close(mutex);
    delete[] buffer;
}

void boundedBuffer::append(char c) {
    _sem::wait(spaceAvailable);
    _sem::wait(mutex);
    buffer[tail]=c;
    tail=(tail+1)%len;
    elems++;
    _sem::signal(mutex);
    _sem::signal(itemAvailable);
}

char boundedBuffer::get() {
    _sem::wait(itemAvailable);
    _sem::wait(mutex);
    char c = buffer[head];
    head = (head + 1) % len;
    elems--;
    _sem::signal(mutex);
    _sem::signal(spaceAvailable);
    return c;
}

/*void boundedBuffer::wrBuffer() {
    _sem::wait(mutex);
    int i = head;
    for(int j=0;j<elems;j++){
        putc(buffer[i]);
        i=(i+1)%len;
        putc(' ');
    }
    _sem::signal(mutex);
}*/
