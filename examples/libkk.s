|--
|-- Various library routines that are used by King&Knight in their
|-- examples and sample answers.  The following IO implementation is built
|-- on the POSIX IO provided by the simulator which means it is not a 
|-- particularly good (read efficient) match.
|--

posix_syscall	= 0
posix_read	= 3
posix_write	= 4
posix_stdout	= 1
posix_stdin	= 0


|--
|-- name:    writes
|-- purpose: write a null-terminated string to the default/standard output.
|-- in:      a1.l - address of start of null-terminated string to output
|-- pre:     stack has enough space for 5 long words.
|--
	.global	writes
writes:
	move.l	d0, -(sp)
	move.l	a1, -(sp)
	moveq	#-1, d0
0:	addq.l	#1, d0
	tst.b	(a1)+
	bne	0b
	move.l	(sp)+, a1
	move.l	d0, -(sp)
	move.l	a1, -(sp)
	move.l	#posix_stdout, -(sp)
	clr.l	-(sp)
	move.l	#posix_write, d0
	trap	#posix_syscall
	add.l	#4*4, sp
	move.l	(sp)+, d0
	rts

|-
|-- name:    writech
|-- purpose: write a character to the default/standard output.
|-- in:      d0.b - character to write out.
|-- pre:     stack has enough space for 5 long words.
|--
	.global	writech
writech:
	move.l	d0, -(sp)
	move.l	#1, -(sp)
	pea	7(sp)
	move.l	#posix_stdout, -(sp)
	clr.l	-(sp)
	move.l	#posix_write, d0
	trap	#posix_syscall
	add.l	#4*4, sp
	move.l	(sp)+, d0
	rts

|--
|-- name:    newline
|-- purpose: starts a fresh line on the standard/default output.
|-- trashes: d0.b
|-- pre:     stack has enough space for 6 long words.
|--
	.global	newline
newline:
	move.b	#'\n, d0
	bra	writech


|--
|-- name:    writen
|-- purpose: writes out a decimal representation of the number in d1.l
|--          to the standard/default output.  Leading zeros are suppressed,
|--          and a minus sign is included if the number is negative.
|-- in:      d1.l - the number to be output
|-- pre:     stack has enough space for 13 long words.
|-- comment: This is the sample answer to exercise 5.8 in King&Knight.
|--          Since writech is not buffered, calling it for each character
|--          in the number is not particularly efficient -- it would be better
|--          to convert the number to a string and write that instead.
|--
	.global	writen
writen:
	movem.l	d0-d5/a0, -(sp)
	clr.l	d3
	lea.l	writen_table, a0
| If the number is negative, then print '-', else make it negative
	tst.l	d1
	bpl.s	wrnpos
	moveq	#'-, d0
	jsr	writech
	bra.s	wrnnext
wrnpos:	neg.l	d1
wrnnext:move.l	(a0)+, d2
	beq.s	wrndone
	clr.b	d0
wrnloop:add.l	d2, d1
	bgt.s	wrndig
	addq.b	#1, d0
	bra.s	wrnloop
wrndig:	sub.l	d2, d1
	tst.b	d0
	bne.s	wrnconv
	cmpi.l	#1, d2
	beq.s	wrnconv
	bra.s	wrnnext
wrnnot0:moveq.l	#1, d3
wrnconv:addi.b	#'0, d0
	jsr	writech
	bra.s	wrnnext
wrndone:movem.l	(sp)+, d0-d5/a0
	rts
writen_table:
	.long 1000000000, 100000000, 10000000, 1000000
	.long 100000, 10000, 1000, 100, 10, 1, 0


|-
|-- name:    readch
|-- purpose: read a character from the default/standard output.
|-- out:     d0.b - character read in.
|-- pre:     stack has enough space for 5 long words.
|--
	.global	readch
readch:
	sub.l	#4, sp
	move.l	#1, -(sp)
	pea	7(sp)
	move.l	#posix_stdin, -(sp)
	clr.l	-(sp)
	move.l	#posix_read, d0
	trap	#posix_syscall
	add.l	#5*4, sp
	move.l	-4(sp), d0
	rts


|--
|-- name:    mul10
|-- purpose: multiplies a 32-bit signed number by 10
|-- in:      d1.l - the number to be multiplied.
|-- out:     d0.l - d1.l*10
|-- flags:   N, Z and V are set correctly.
|-- comment: This is the sample answer to exercise 5.4 in King&Knight.
|--
mul10:
	move.l	d1, d0
	add.l	d0, d0
	bvs.s	0f
	add.l	d0, d0
	bvs.s	0f
	add.l	d1, d0
	bvs.s	0f
	add.l	d0, d0
0:	rts



|--
|-- name:    chtobin
|-- purpose: converts a number in characters into binary.
|--          to the standard/default output.  Leading zeros are suppressed,
|--          and a minus sign is included if the number is negative.
|-- in:      a1.l - the address of a number as a null-terminated string.
|-- out:     d0.l - 0 if d1.l contains the number, non-zero if bad number
|--          d1.l - the binary value of the number if d0.l == 0
|-- pre:     stack has enough space for 13 long words.
|-- comment: This is the sample answer to exercise 5.9 in King&Knight.
|--
	.global	chtobin
chtobin:
	movem.l	a1/d2/d3, -(sp)
	clr.l	d1
	moveq.l	#1, d2
	cmpi.b	#'-, (a1)
	bne.s	0f
	neg.l	d2
	addq.l	#1, a1
0:	bra.s	chtobin_loop_end
|--
|-- The main loop where the digits of the number are collected.
|--         result := old result * 10 + new digit
|--
chtobin_loop:
	bsr	mul10
	bvs.s	chtobin_bad
	move.l	d0, d1
	add.l	d3, d1
	bvs.s	chtobin_bad
chtobin_loop_end:
	clr.l	d3
	move.b	(a1)+, d3
	beq.s	chtobin_done
	subi.b	#'0, d3
	blt.s	chtobin_bad
	cmp.b	#9, d3
	ble.s	chtobin_loop

|--
|-- Invalid number: contains non-digit, or too big for 32 bits.
|--
chtobin_bad:
	moveq.l	#1, d0
	bra.s	chtobin_exit

|--
|-- Number converted, indicate that the result is legal and correct the sign.
|--
chtobin_done:
	clr.l	d0
	tst.l	d2
	bpl.s	chtobin_exit
	neg.l	d1

chtobin_exit:
	movem.l	(sp)+, a1/d2/d3
	rts
