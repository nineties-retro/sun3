|--
|-- forth.s assumes it will link against libm on a Sun3 and so
|-- the following are the minimal additional functions necessary
|-- to support forth.s.  I have no idea whether I wrote the code
|-- or if it came from somewhere else.
|--
	.global lmodt, ulmodt

ulmodt_negative_dividend:
	neg.l	d0
	tst.l	d1
	bge.s	ulmodt_natural_divisor
	neg.l	d1
ulmodt_natural_divisor:
	bsr.s	lmodt_natural_divisor
	neg.l	d0
	rts

ulmodt_zero_divisor:
	link.w	fp, #0
	divu.w	d1, d0
	unlk	fp
	rts

|--
|-- name:    ulmodt
|-- purpose: unsigned modulus operation
|-- in:      d0.l - dividend
|--          d1.l - divisor
|-- out:     d0.l - d0.l%d1.l
|--
ulmodt:	tst.l	d1
	beq.s	ulmodt_zero_divisor
	bra.s	lmodt_natural_divisor

|--
|-- name:    lmodt
|-- purpose: signed modulus operation
|-- in:      d0.l - dividend
|--          d1.l - divisor
|-- out:     d0.l - d0.l%d1.l
|--
lmodt:	tst.l	d0
	blt.s	ulmodt_negative_dividend
	tst.l	d1
	beq.s	ulmodt_zero_divisor
	bge.s	lmodt_natural_divisor
	neg.l	d1
lmodt_natural_divisor:
	move.l	d2, -(sp)
	move.l	d1, d2
	clr.w	d2
	swap	d2
	bne.s	lmodt_32_bit_divisor
	move.l	d0, d2          |-\
	clr.w	d2              |--
	swap	d2              |-- if dividend >= divisor then goto lmodt2a
	cmp.l	d1, d2          |--
	bge.s	lmodt2a         |-/
	divu.w	d1, d0          |-\ d0[31:16]= d0/d1;  d0[0:15]= d0%d1
	clr.w	d0              |-- drop the quotient
	swap	d0              |-/
	move.l	(sp)+, d2
	rts

lmodt2a:move.l	d3, -(sp)
	divu.w	d1, d2
	move.l	d2, d3
	move.w	d0, d3
	divu.w	d1, d3
	clr.w	d3
	move.l	d3, d0
	swap	d0
	move.l	(sp)+, d3
	move.l	(sp)+, d2
	rts

lmodt_32_bit_divisor:
	move.l	(sp)+, d2
	movem.l	d2-d5/a0, -(sp)
	move.l	d1, d3
	sub.l	#1, d3
	and.l	d1, d3
	beq	lmodtaa
	moveq	#0, d4
	move.l	d0, d2
	lsr.l	#3, d2
	cmp.l	d1, d2
	bhi.s	lmodt6a
0:	addq.l	#1, d4
	sub.l	d1, d0
	bhi.s	0b
	beq.s	1f
	add.l	d1, d0
1:	movem.l	(sp)+, d2-d5/a0
	rts
lmodt6a:move.l	d0, d2
	move.l	d1, d3
	move.l	#65535, d5
	sub.l	#1, d3
	addq.l	#1, d4
	lsr.l	#1, d3
	cmp.l	d5, d3
	bhi.s	0b
	addq.l	#1, d3
	cmp.l	d5, d3
	bls.s	0f
	lsr.l	#1, d3
	addq.l	#1, d4
0:	lsr.l	d4, d2
	move.l	d1, d3
	move.l	d1, d5
	swap	d3
	mulu.w	d2, d5
	mulu.w	d2, d3
	swap	d3
	clr.w	d3
	add.l	d3, d5
	sub.l	d5, d0
	cmp.l	d0, d1
	bgt.s	0f
	sub.l	d1, d0
0:	movem.l	(sp)+, d2-d5/a0
	rts
lmodtaa:moveq	#0, d4
	tst.w	d1
	bne.s	0f
	swap	d1
	moveq	#16, d4
0:	lea	ulmodt_negative_dividend, a0
	tst.b	d1
	bne.s	0f
	addq.b	#8, d4
	lsr.w	#8, d1
0:	add.b	(a0, d1.w), d4
	moveq	#1, d2
	lsl.l	d4, d2
	sub.l	#1, d2
	and.l	d2, d0
	movem.l	(sp)+, d2-d5/a0
	rts


	.global uldivt
uldivt:	neg.l	d0
	tst.l	d1
	bge.s	0f
	neg.l	d1
	bra.s	ldivta
0:	bsr.s	ldivta
	neg.l	d0
	rts
uldivt1:neg.l	d1
	bsr.s	ldivta
	neg.l	d0
	rts
uldivt2:link.w	fp, #0
	divuw	d1, d0
	unlk	fp
	rts
	tst.l	d1
	beq.s	uldivt2
	bra.s	ldivta
ldivt:	tst.l	d0
	blt.s	uldivt
	tst.l	d1
	blt.s	uldivt1
	beq.s	uldivt2
ldivta:	move.l	d2, -(sp)
	move.l	d1, d2
	clr.w	d2
	swap	d2
	bne.s	1f
	move.l	d0, d2
	clr.w	d2
	swap	d2
	cmp.l	d1, d2
	bge.s	0f
	divu.w	d1, d0
	clr.w	d0
	swap	d0
	move.l	(sp)+, d2
	rts
0:	move.l	d3, -(sp)
	divu.w	d1, d2
	move.l	d2, d3
	move.w	d0, d3
	divuw	d1, d3
	move.l	d3, d0
	swap	d0
	clr.w	d0
	swap	d0
	swap	d2
	clr.w	d2
	add.l	d2, d0
	move.l	(sp)+, d3
	move.l	(sp)+, d2
	rts
1:	move.l	(sp)+, d2
	movem.l	d2-d5/a0, -(sp)
	move.l	d1, d3
	sub.l	#1, d3
	and.l	d1, d3
	beq	2f
	moveq	#0, d4
	move.l	d0, d2
	lsr.l	#3, d2
	cmp.l	d1, d2
	bhi.s	1f
0:	addq.l	#1, d4
	sub.l	d1, d0
	bhi.s	0b
	beq.s	0f
	sub.l	#1, d4
0:	move.l	d4, d0
	movem.l	(sp)+, d2-d5/a0
	rts
1:	move.l	d0, d2
	move.l	d1, d3
	move.l	#65535, d5
	sub.l	#1, d3
0:	addq.l	#1, d4
	lsr.l	#1, d3
	cmp.l	d5, d3
	bhi.s	0b
	addq.l	#1, d3
	cmp.l	d5, d3
	bls.s	0f
	lsr.l	#1, d3
	addq.l	#1, d4
0:	lsr.l	d4, d2
	divu.w	d3, d2
	move.l	d1, d3
	move.l	d1, d5
	swap	d3
	mulu.w	d2, d5
	mulu.w	d2, d3
	swap	d3
	clr.w	d3
	add.l	d3, d5
	sub.l	d5, d0
	cmp.l	d0, d1
	bgt.s	0f
	addq.l	#1, d2
0:	move.l	d2, d0
	swap	d0
	clr.w	d0
	swap	d0
	movem.l	(sp)+, d2-d5/a0
	rts
2:	moveq	#0, d4
	tst.w	d1
	bne.s	0f
	swap	d1
	moveq	#16, d4
0:	lea	uldivt, a0
	tst.b	d1
	bne.s	0f
	addq.b	#8, d4
	lsr.w	#8, d1
0:	add.b	(a0, d1.w), d4
	lsr.l	d4, d0
	moveml	(sp)+, d2-d5/a0
	rts

