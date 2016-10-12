Mesg     ORI 13
         ENT Displ,X
NOUT     EQU 2
;
Txt      NMD 42336      also represents "\nV"
         CCD al
         CCD ue
         CCD = 
X        NMD 6
SavR     RES 1
Pairs    RES 1
;
Displ    ST  3,SavR     M[SavR] <- r3
         LD  0,X        r0 <- M[X]
         BRN 0,Done     if (r0 < 0) goto Done
Loop     LDI 1,4        Display text "\nValue= " (4 pairs of chars)
TLP      BRZ 1,V     Terminate Text LooP
         LDI 3,4        Calc. offset in r3
         ST  1,Pairs    prepare for subtraction
         SUB 3,Pairs    offset = 4 - r1
         LD  2,Txt(3)   r2 <- M[Txt + r3]
         PTC 2          output first char. of pair
         SHL 2,8        get second char. ready
         PTC 2          output second char.
         SUB 1,=1       r1--
         BR  3,TLP      goto TLP
V        IO  NOUT,X     Display value in M[X]
         SUB 0,=1       r0--
         ST  0,X        M[X] <- r0
         BRN 0,Done     if (r0 < 0) goto Done
         BR  3,Loop     goto Loop
Done     LD  3,SavR     r3 <- M[SavR]
         BR  3,0(3)     return
         END   