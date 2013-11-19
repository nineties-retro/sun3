|--
|-- A simple reverse polish calculator based on the sample answer
|-- to exercise 5.8 in King&Knight.
|--
|-- Needs to be linked against libkk or an equivalent to work.

calc_bell	= 7

	.text
start:	lea.l	stack, a6
calc_loop:
	jsr	readch
calc_loop1:
	cmpi.b	#'0, d0
	blt.s	0f
	cmpi.b	#'9, d0
	ble.s	calc_number
0:	cmpi.b	#'*, d0
	beq	calc_mult
	cmpi.b	#'/, d0
	beq	calc_div
	cmpi.b	#'+, d0
	beq	calc_add
	cmpi.b	#'-, d0
	beq	calc_sub
	cmpi.b	#'=, d0
	beq	calc_print
	cmpi.b	#'\ , d0            |
	beq	calc_loop           | Ignore any spaces or newlines in
	cmpi.b	#'\n, d0            | the input
	beq	calc_loop           |
	cmpi.b	#'c, d0
	beq	calc_clear
	cmpi.b	#'d, d0
	beq	calc_delete
	cmpi.b	#'q, d0
	beq	calc_quit
|--
|-- The character typed is not a command or part of a number.
|-- Write out a BELL character by way of complaint.
|--
	moveq.l	#calc_bell, d0
	jsr	writech
	bra.s	calc_loop

|--
|--
|--
calc_number:
	lea.l	numbuf, a0
0:	move.b	d0, (a0)+
	jsr	readch
	cmpi.b	#'0, d0
	blt.s	1f
	cmpi.b	#'9, d0
	ble.s	0b
1:	move.b	d0, d2
	clr.b	(a0)
	lea.l	numbuf, a1
	jsr	chtobin
	move.l	d1, -(a6)
	move.b	d2, d0
	bra	calc_loop1

|--
|--
|--
|--
calc_mult:
	cmpa.l	#stack-8, a6
	bgt.s	0f
	move.l	(a6)+, d0
	move.l	(a6)+, d1
	muls	d0, d1
	move.l	d1, -(a6)
0:	bra	calc_loop


|--
|--
|--
|--
calc_div:
	cmpa.l	#stack-8, a6
	bgt.s	0f
	move.l	(a6)+, d0
	move.l	(a6)+, d1
	divs	d0, d1
	ext.l	d1
	move.l	d1, -(a6)
0:	bra	calc_loop


|--
|--
|--
|--
calc_add:
	cmpa.l	#stack-8, a6
	bgt.s	0f
	move.l	(a6)+, d1
	add.l	(a6)+, d1
	move.l	d1, -(a6)
	bra	calc_loop


|--
|--
|--
|--
calc_sub:
	cmpa.l	#stack-8, a6
	bgt.s	0f
	move.l	(a6)+, d0
	move.l	(a6)+, d1
	sub.l	d0, d1
	move.l	d1, -(a6)
0:	bra	calc_loop


|--
|--
|--
|--
calc_print:
	lea.l	stack, a5
	cmpa.l	a5, a6
	beq.s	0f
	move.l	(a6), d1
	jsr	writen
	jsr	newline
0:	bra	calc_loop


|--
|--
|--
|--
calc_clear:
	lea.l	stack, a6
	bra	calc_loop


|--
|--
|--
|--
calc_delete:
	lea.l	stack, a5
	cmpa.l	a5, a6
	beq.s	0f
	addq.l	#4, a6
0:	bra	calc_loop


.set	posix_syscall, 0
.set	posix_exit, 1
.set	posix_exit_success, 0

calc_quit:
	move.l	#posix_exit_success, -(a7)
	clr.l	-(a7)
	move.l	#posix_exit, d0
	trap	#posix_syscall

	.data
stackend:
	.space	100*4
stack:
numbuf:	.space	100
