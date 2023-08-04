MAIN:mov @r3 , @r2
LOOP: jmp L1
prn -5
bne W
sub @r1, @r4
    bne L3
L1:  inc K
    .entry LOOP
jmp W
END: stop
STR:.string "abcdef"
LENGTH:.data  6,-9,15
K:.data  10,12,13,14