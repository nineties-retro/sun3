|--
|-- A simple factorial program based on that given on page 80 of 
|-- Gill&Corwin&Logar.
|--

posix_syscall		= 0
posix_exit		= 1
posix_exit_success	= 0

start:	move.l	#9, d1
	bsr	fact
	move.l	d2, d1
	jsr	writen
	jsr	newline
	move.l	#posix_exit_success, -(a7)
	clr.l	-(a7)
	move.l	#posix_exit, d0
	trap	#posix_syscall

fact:	tst.l	d1
	beq	basis
	move.l	d1, -(sp)
	subq.l	#1, d1
	bsr.s	fact
	move.l	(sp)+, d1
	mulu	d1, d2
	rts
basis:	moveq	#1, d2
	rts
