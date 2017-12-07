	.file "music.s"
	.arch msp430g2553
	.p2align 1,0

	.data
state:	.byte 0

	.text
curr:
	.word note0
	.word note1
	.word note2
	.word note3
	.word note4
	.word note5
	.word note5
	.word note7

	.global victory_theme

victory_theme:

	    mov #7, r12
	    cmp.b &state, r12	; is state less than 7
	    jl default		; 5 - state < 0

	    mov.b &state, r12	; reading current state
	    add r12, r12		; r12 = 2*state
	    mov curr(r12), r0		; jmp curr[state]

note0: 	    mov #880, r12
	    CALL #buzzer_set_period
	    mov.b #1, &state
	    jmp end

note1: 	    mov #1500, r12
	    CALL #buzzer_set_period
	    mov.b #2, &state
	    jmp end

note2:	    mov #500, r12
	    CALL #buzzer_set_period
	    mov.b #3, &state
	    jmp end

note3: 	    mov #700, r12
	    CALL #buzzer_set_period
	    mov.b #4, &state
	    jmp end

note4: 	    mov #2000, r12
	    CALL #buzzer_set_period
	    mov.b #5, &state
	    jmp end

note5:	    mov #2200, r12
	    CALL #buzzer_set_period
	    mov.b #6, &state
	    jmp end
note6:	    mov #3000, r12
	    CALL #buzzer_set_period
	    mov.b #7, &state
	    jmp end
note7:	    mov #2300, r12
	    CALL #buzzer_set_period
	    mov.b #8, &state
	    jmp end
default:    mov #0, r12
	    CALL #buzzer_set_period

end:	    pop r0
