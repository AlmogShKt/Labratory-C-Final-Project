MAIN:   mov  @r3, LENGTH
LOOP:   jmp  L1
mcro m1
sub  @r1, @r4
	bne  END
endmcro
prn  -5
,bne  LOOP
m1
L1:     inc  K
bne  LOOP
END:    stop
STR:    .string "abcdef"
LENGTH: .data 6,-9,15
K:      .data 22
