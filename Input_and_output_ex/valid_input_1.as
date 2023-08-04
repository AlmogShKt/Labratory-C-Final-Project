MS1:.string "Lets test"

LENGTH:.data  6,-9,15

    .extern Test

MAIN:        mov @r3 ,    @r2

        K:   .data  10,12  ,     13,    14
;This is a coment, !@#&%%#+!)+$
LOOP: jmp L1
prn -5
sub @r6    ,      @r4
    bne      Test

L1:  inc K


    .entry LOOP


END: stop
