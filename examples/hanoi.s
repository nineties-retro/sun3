|--
|-- A solution to the Towers of Hanoi problem based on the sample answer
|-- to exercise exercise 4.8 in King&Knight.
|--
|-- Needs to be linked against libkk or an equivalent to work.

posix_syscall		= 0
posix_exit		= 1
posix_exit_success	= 0

start:	moveq.l	#5, d1
	moveq.l	#'A, d2
	moveq.l	#'B, d3
	moveq.l	#'C, d4
	jsr	hanoi
	move.l	#posix_exit_success, -(a7)
	clr.l	-(a7)
	move.l	#posix_exit, d0
	trap	#posix_syscall

|--
|-- name:    hanoi
|-- purpose: Displays a solution to the Towers of Hanoi problem on the
|--          standard/default output.
|-- in:      d1.l - number of discs to transfer
|--          d2.b - name of the from pile (as a single character)
|--          d3.b - name of the to pile (as a single character)
|--          d4.b - name of the intermediate pile (as a single character)
|-- pre:     lots of stack space!
|--
hanoi:
	movem.l	d0-d5, -(sp)
	tst.w	d1
	beq.s	hanoi_exit
	move.w	d1, d5
	dbra	d1, 1f

1:	move.w	d3, -(sp)
	move.b	d4, d3
	move.w	(sp)+, d4
	bsr.s	hanoi

	move.w	d1, -(sp)
	lea.l	move_disc, a1
	jsr	writes

	clr.l	d1
	move.w	d5, d1
	jsr	writen
	lea.l	from_pile, a1
	jsr	writes
	move.b	d2, d0
	jsr	writech
	lea.l	to_pile, a1
	jsr	writes
	move.b	d4, d0
	jsr	writech
	jsr	newline
	move.w	(sp)+, d1

	move.w	d2, -(sp)
	move.w	d3, d2
	move.w	d4, d3
	move.w	(sp)+, d4
	bsr.s	hanoi
hanoi_exit:
	movem.l	(sp)+, d0-d5
	rts

move_disc:
	.asciz "Move disc "
from_pile:
	.asciz " from pile "
to_pile:
	.asciz " to pile "
	.even
