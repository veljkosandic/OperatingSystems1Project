//preuzeto sa vjezbi 7.,zadatak 7.
.global _ZN3TCB13contextSwitchEPNS_7ContextES1_
.type _ZN3TCB13contextSwitchEPNS_7ContextES1_ @function
//P - pokazivac, S_ - prvi pokazan
_ZN3TCB13contextSwitchEPNS_7ContextES1_:
    //a0 - stari kontekst
    //a1 - novi kontekst
         sd ra,0 * 8(a0)
        sd sp, 1 * 8(a0)
        ld ra, 0 * 8(a1)
        ld sp, 1* 8(a1)
    ret
