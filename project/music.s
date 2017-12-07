	.file "music.s"
	.arch msp430g2553
	.p2align 1,0

	.data
state:	.byte 0

	.text
curr_state:	.word case0
	.word case1
	.word case2
	.word case3
	.word case4
	.word case5
	.word case5
	.word case7

	.global victory_theme
victory_theme:

	    mov #5, r12
	    cmp.b &state, r12	;5 - state
	    jl default		; 5 - state < 0

	    mov.b &state, r12
	    add r12, r12		; r12 = 2*state
	    mov curr_state(r12), r0		; jmp curr_state[state]

case0: 	    mov #2000, r12
	    CALL #buzzer_set_period
	   	;mov.b #1, &state
			CALL #case1
	    jmp end

case1: 	    mov #1500, r12
	    CALL #buzzer_set_period
	    ;mov.b #2, &state
			CALL #case2
	    jmp end

case2:	    mov #500, r12
	    CALL #buzzer_set_period
	    ;mov.b #3, &state
			CALL #case3
	    jmp end

case3: 	    mov #700, r12
	    CALL #buzzer_set_period
	    ;mov.b #4, &state
			CALL #case4
	    jmp end

case4: 	    mov #2000, Fr12
	    CALL #buzzer_set_period
	    ;mov.b #5, &state
			CALL #case5
	    jmp end

case5:	    mov #2200, r12
	    CALL #buzzer_set_period
	    ;mov.b #6, &state
			CALL #case6
	    jmp end
case6:	    mov #3000, r12
	    CALL #buzzer_set_period
	    ;mov.b #7, &state
			CALL #case7
	    jmp end
case7:	    mov #2300, r12
	    CALL #buzzer_set_period
	    ;mov.b #8, &state
			CALL #default
	    jmp end
default:    mov #0, r12
	    CALL #buzzer_set_period

end:	    pop r0
