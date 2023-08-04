.entry START
.extern EXTERN
MAIN: mov @r1,@r2
LOOP: cmp -5,@r3
bne ENDLOOP1
add @r10,R0
jsr SUBROUTINE
prn STR
lea ARR,@r5
SUBROUTINE: bne EXTERNVAR
stop
ENDLOOP: dec K
jmp LOOP
STA$RT: sub @r2,@r7
clr STR
red @r7
stoppp
not @r2
inc R0
bne MAIN
EXTERNVAR: .data 100
STR: .string "Hello,World!"
ARR: .data 1,2,3,4,5
K: .data 10,gg
R0: .data 15,17
