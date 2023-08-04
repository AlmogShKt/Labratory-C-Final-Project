.entry START
    .extern Z
MAIN: mov @r2, START
LOOP1: jmp L2
prn -4
bne Z
sub @r1, @r3
    bne L4
L2:  inc J
    .entry LOOP1
jmp Z
END1: stop
STR1: .string "ghijkl"
START: .data 8,-12,20
J: .data  33
    .extern L4

; Using rest of the opcodes
    LOOP2: cmp @r5, @r6
    dec @r5
    not @r2
    clr @r6
    add @r4, @r5
    red @r1
    jsr LOOP2
rts
END2: stop
STR2: .string "mnopqr"
DATA2: .data 10,-15,25
L: .data  44
    .extern L5
