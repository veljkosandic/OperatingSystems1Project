//
// Created by os on 5/12/23.
//

#ifndef PROJ_BOUNDEDBUFFER_HPP
#define PROJ_BOUNDEDBUFFER_HPP
//inspirisano baferom iz knjige
class _sem;
typedef _sem* sem_t;
class boundedBuffer{
public:
    boundedBuffer(int len);
    boundedBuffer(const boundedBuffer& ) = delete;
    boundedBuffer& operator=(const boundedBuffer&)=delete;
    void append(char c);
    char get();
    ~boundedBuffer();
    //void wrBuffer();//poprilicno eksperimentalno
    bool isFull() const {return elems==len;}
private:
    friend class Riscv;
    char *buffer;
    sem_t itemAvailable,spaceAvailable,mutex;
    int head,tail,len,elems;

};
#endif //PROJ_BOUNDEDBUFFER_HPP
