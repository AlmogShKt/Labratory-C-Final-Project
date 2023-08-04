.entry LENGTH

MAIN:,,mov @r3 , LENGTH
LOOP: jmp L1
prn a
bne @W
sub r1,, @r4
     L3
L1:  in K2
    .entry 1LOOP
jmp W and A
END: stop
STR:..string. "abcdef"
LENGTH:.data  6,,-9,15
K:.data  22, a
    .extern L3