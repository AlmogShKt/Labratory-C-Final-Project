.entry START
    .extern Z
MAIN!: mov @r9, START
LOOP1: jmp L11
prn -4
bne
sub ,@r1, @r3
    bne, L4
L2:  inc J
    .entry LOOP1
jmp Z
END1: stop
STR1: .string "ghijkl" 5
START: .data 8,-12,20 20 11 23
J: .data  33
    .extern L4

; Using rest of the opcodes !!
    bad comments
LOOP2: cmp @r5, @r6
    dec @r5 @r2
    not @r2
    clr @r6
    addi @r4, 4
    red @r9
    jsr LOOP2
rtss
END2: stop
STR2: .string "mn op q r
DATA2: .data 10,-15,25
