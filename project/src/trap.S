.extern _ZN5Riscv14handleSysErrorEv,_ZN5Riscv13handleConsoleEv,_ZN5Riscv11handleTimerEv//inspirisano vjezbama 7,zadatak 7 - asinhrona promjena konteksta
.global _ZN5Riscv11trapRoutineEv
.long _ZN5Riscv4parsE, _ZN5Riscv3sspE
.align 4 //poravnato na 4
.type _ZN5Riscv11trapRoutineEv, @function
//konzola:0x09 - broj ulaza
//tajmer: 0x01 - broj ulaza
_ZN5Riscv11trapRoutineEv:
//https://inst.eecs.berkeley.edu/~cs61c/resources/RISCV_Calling_Convention.pdf
        jal x0, prepareSysError
        jal  x0, prepareTimer
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        jal x0, prepareConsole
prepareSysError:
         addi sp, sp, -256 //alokacija prostora na steku za 32 registra
        //cuvanje registara preko .irp - gnu assembler
        .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
        sd x\index, \index * 8(sp)
        .endr//0 se ne mora cuvati
        mv a0, sp
        addi a0, a0, 80//lokacija a0 u memoriji
        call _ZN5Riscv14handleSysErrorEv
        .irp index, 0,1,2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31//ne vracamo a0
        ld x\index, \index * 8(sp)
        .endr
        addi sp, sp, 256
        sret
prepareTimer:
        addi sp, sp, -256 //alokacija prostora na steku za 32 registra
        //cuvanje registara preko .irp - gnu assembler
        .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
        sd x\index, \index * 8(sp)
        .endr//0 se ne mora cuvati
        call _ZN5Riscv11handleTimerEv
        .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
        ld x\index, \index * 8(sp)
        .endr
        addi sp, sp, 256
        sret
prepareConsole:
        addi sp, sp, -256 //alokacija prostora na steku za 32 registra
        //cuvanje registara preko .irp - gnu assembler
        .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
        sd x\index, \index * 8(sp)
        .endr//0 se ne mora cuvati
        call _ZN5Riscv13handleConsoleEv
        .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
        ld x\index, \index * 8(sp)
        .endr
        addi sp, sp, 256
        sret