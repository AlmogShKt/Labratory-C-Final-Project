MAI:    mov  @r5, LENGTH gg
LOOP:   jmp  L1
mcro m1
sub  @r3, @r4
	cmp @r1 , @r4 2
	bne  END
endmcro
prn  @r2
bne  LOOP
m1
L1:     inc  K
jsr  @r2
END:    stop 55
STR:    .string "abcdef"
LENGTH: .data 1,-9,15
K:      .data 22
