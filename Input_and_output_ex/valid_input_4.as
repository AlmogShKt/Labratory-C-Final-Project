.extern XYZ
    .entry MAIN

KINITIALVALUE: sub @r4   ,    @r3

MAIN:   mov @r3, LENGTH

LOOP:   jmp L1

    mcro M1
sub @r1, @r4
    bne END
    endmcro

    .entry GGG
prn -5
bne LOOP

M1

L1:     inc K
bne LOOP

END:    stop

STR:    .string "abcdef"

LENGTH: .data 6, -9, 15

K:      .data 4    ,    -55,4,4,4,6

    mcro M2
mov reg1, val
add reg2, reg1
    endmcro

M2
ABC: mov XYZ, @r3
reg1: .data 6,5,-555,66
reg2: .data 6,5,-555,66
val: .string "asfas   %%dfjk"
