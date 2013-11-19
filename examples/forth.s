|
| forth.s--a 68K forth interpreter
|
| Register allocation:
| A7--68K stack pointer
| A6--IP
| A5--SP
| A4--RSP
| A3..A0--General
| D7--Here
| D6--Input line pointer
| D5..D0--General
|

|
| Flag bits in status field
|
Priority = 1
Smudged	= 2

|
| control structure checking flags
|
FlgDef	= 1	| : .. ;
FlgBeg	= 2	| begin .. again, while, repeat
FlgWhi	= 5	|    the "while" part flag
FlgIf	= 3	| if .. endif
FlgDo	= 4	| do .. loop, +loop


|
| Other constants/offsets
|
stacksize = 100
umem      = 96			| K of dict. space for user
rstack: .space 	stacksize*4	| 100 Words for return stack
stack:	.space	stacksize*4	|  and 100 for user's stack
mstack:	.space	stacksize*4	|  and 100 for the 68K processor stack
Inbufsize  =	1024+4*3	| Input buffer record
InUnit	=	1024+1		|  Unix file descriptor number
InbufIdx   =	1024+4		|  Holds index into it for nesting of units
InbufPrev  =	1024+8		|  Pointer to previous input unit (nesting)
MaxIn	=	4		| Max # open input units
MaxOut	=	MaxIn		|  and output units
inbufs:
	.space	1024		| Input buffer
	.byte	0,0		|  <NULL>, <STDIN>
	.space	2		|  two bytes wasted
	.space 	1*4		|  holds index
	.long	0		|  ptr to prev--is NULL for first

	.space	Inbufsize*(MaxIn-1) | The rest of the input units
End_inbufs:

outfds:	.long	1		| <STDOUT>
	.space	(MaxOut-1)*4	|  The rest of the output units

ounit:	.long	outfds		| Current output unit
iunit:	.long	inbufs		|  and current input unit

|
| init--start up forth. Set up our dictionary & use ABORT
|
	.globl	init
init:	move.l	#udict,d7		| Set up HERE

|
| abort--clear I/O, reset stacks, clear state, enter INTERP
|
abort:	
	move.l	#rstack+stacksize,a4	| Initialize return stack
	move.l	#stack+stacksize,a5	|  and user stack
	move.l	#mstack+stacksize,a7	|  and processor stack
	clr.l	state1			| Set state back to interpretive

	move.l	#interp,a6		| Set IP to top of INTERP

	move.l	#inbufs,a0		| Set up & clear input buffer
	clr.b	(a0)
	clr.b	1024(a0)
	move.l	a0,d6
	move.l	d6,iunit
	clr.b	InUnit(a0)
	move.l	#outfds,a0		| Set up & clear output buffer
	move.l	#1,(a0)
	move.l	a0,ounit

	move.l	#3,-(a7)		| Close all open files
	clr.l	-(a7)			|   Dummy place holder
	move.l	#20,d3			|  How many units to close
abor1:	moveq	#6,d0			|  UNIX "close" system call
	trap	#0
	addq.l	#1,4(a7)		|  Move to next file descriptor
	dbra	d3,abor1
	add.l	#8,a7			| Remove arguments from stack
					| Fall into...
					|	     V
					|	     V
|
| Next--the "fetch/execute" code of FORTH
|
next:	move.l	(a6)+,a0		| Get CFA's addr, advance IP
next2:	move.l	(a0)+,a1		| Get contents of CFA
	jmp	(a1)			| Jump to that address

|
| interp--a high level definition
| : interp
|       getword lookup if
|               state @ 0= or if execute else [compile] (lit) , endif
|       else
| 	         number if
|                       state @ if , endif
|               else notfound abort endif
|       endif
| ;
|
interp:	.long	ckstack,getword,lookup,zbranch,inter1
	.long	state,fetch,zeq,l_or,zbranch,inter2
	.long	execute,branch,interp
inter2:	.long	comma,branch,interp
inter1:	.long	number,zbranch,inter3
	.long	state,fetch,zbranch,interp
	.long	plit,plit,comma,comma,branch,interp
inter3:	.long	notfound

|
| or--bitwise "or"
|
l_or2:	.long    0
l_or:   .long    l_or1,l_or1,0
        .ascii	"or      "
l_or1:	move.l  (a5)+,d0
        or.l    d0,(a5)
        jmp     next
 
|
| and--logical bit-wise AND
|
l_and2:	.long    l_or2,l_and1,l_and1,0
        .ascii	"and     "
l_and1:	move.l  (a5)+,d0
        and.l   d0,(a5)
        jmp     next
 
|
| 0<--push whether top is less than 0
|
zlt2:	.long    l_and2,zlt1,zlt1,0
        .ascii	"0<      "
zlt1:	tst.l   (a5)
        blt     puttrue
        bra     putfalse
 
|
| 0>--push whether top is greater than 0
|
zgt2:    .long    zlt2,zgt1,zgt1,0
        .ascii	"0>      "
zgt1:	tst.l   (a5)
        bgt     puttrue
        bra     putfalse
 
|
| u<--unsigned version of "less than"
|
ult2:    .long    zgt2,ult1,ult1,0
        .ascii	"u<      "
ult1:	move.l  (a5)+,d0
        cmp.l   (a5),d0
	beq	putfalse
        bcc     puttrue
        bra     putfalse
 
|
| 0=--a logical "not"
|
zeq2:	.long    ult2
zeq:	.long    zeq1,zeq1,0
        .ascii	"0=      "
zeq1:	tst.l   (a5)
        bne     putfalse
puttrue:
        move.l  #-1,(a5)
        jmp     next
putfalse:
        clr.l   (a5)
        jmp     next
 
|
| <--less than. Push whether second is less than top
|
lt2:	.long	zeq2,lt1,lt1,0
	.ascii	"<       "
lt1:	move.l	(a5)+,d0
	cmp.l	(a5),d0
	bgt	puttrue
	bra	putfalse

|
| >--greater than. Push whether second is greater than top
|
gt2:	.long	lt2,gt1,gt1,0
	.ascii	">       "
gt1:	move.l	(a5)+,d0
	cmp.l	(a5),d0
	blt	puttrue
	bra	putfalse

|
| =--push whether top and second are equal
|
equal2:	.long	gt2,equal1,equal1,0
	.ascii	"=       "
equal1:	move.l	(a5)+,d0
	cmp.l	(a5),d0
	beq	puttrue
	bra	putfalse

|
| ccomma--store a byte into the next location
|
ccomma2:	.long	equal2,ccomma1,ccomma1,0
	.ascii	"c,      "
ccomma1:move.l	d7,a0
	move.l	(a5)+,d0	| Get word off stack
	move.b	d0,(a0)		| Store its low byte
	addq.l	#1,d7		| Advance HERE
	jmp	next

|
| comma--store a word into the next free location, advancing the
| current location pointer
|
comma2:	.long	ccomma2
comma:	.long	comma1,comma1,0
	.ascii	",       "
comma1:	addq.l	#3,d7		| Word-align data
	and.l	#0xFFFFFFFC,d7
	move.l	d7,a0
	move.l	(a5)+,(a0)+
	move.l	a0,d7
	jmp	next

|
| !--store second at address pointed to by top
|
store2:	.long	comma2
store:	.long	store1,store1,0
	.ascii	"!       "
store1:	move.l	(a5)+,a0
	move.l	(a5)+,(a0)
	jmp	next

|
| @--replace top of stack with what it pointed to
|
fetch2:	.long	store2
fetch:	.long	fetch1,fetch1,0
	.ascii	"@       "
fetch1:	move.l	(a5),a0
	move.l	(a0),(a5)
	jmp	next

|
| branch--replace IP with next sequential word in execution
|
branch2:	.long	fetch2
branch:	.long	branch1,branch1,0
	.ascii	"branch  "
branch1:move.l	(a6),a6
	jmp	next

|
| zbranch--"branch" if top of stack is zero
|
zbran2:	.long	branch2
zbranch:.long	zbran1,zbran1,0
	.ascii	"zbranch "
zbran1:	move.l	(a6)+,d0	| Get the conditional destination
	tst.l	(a5)+		| Should we take it?
	beq	zbran3
	jmp	next
zbran3:	move.l	d0,a6		| Take the branch
	jmp	next

|
| run-time code to push the PFA to stack
|
getpfa:	move.l	(a0),-(a5)
	jmp	next

|
| state--variable which holds the state: 0 == interp, <>0 == compiling
|
state2:	.long	zbran2
state:	.long	getpfa,state1,0
	.ascii	"state   "
state1:	.long	0

|
| getword--get the next word from the input stream, put it in "pad".
|
getw2:	.long	state2
getword:.long	getwo1,getwo1,0
	.ascii	"getword "
getwo1:	jsr	getw1
	jmp	next

getw1:	move.l	d6,a0		| A0 will be our line pointer
	jsr	skipwhite	| Skip leading white space
	move.l	#pad1,a1	| Build into "pad" via A1
	clr.b	8(a1)		|  Put in Null-termination
	move.l	#8,d1		| Count # chars stored
getw3:	move.b	(a0)+,(a1)+	| Get next char
	bne.s	getw10		| Need to read in a new buffer?

	subq.l	#1,a1		|  Back up destination ptr
getw20:	movem.l	a1/d1,-(a7)	|  Save registers
	jsr	getline		|  Get new line
	movem.l	(a7)+,a1/d1	|  Restore registers
	move.l	d6,a0		|  Update input line pointer
	bra.s	getw4

getw10:	subq.l	#1,d1		| Decrement character count
	beq	getw5		|  If run out, truncate rest of word
getw4:	jsr	iswhite		| See if at end of word
	bne	getw3
	tst.b	(a0)		| At end of buffer?
	beq.s	getw20

	tst.l	d1		| Blank-fill word
	beq	getw6
getw7:	move.b	#32,(a1)+
	subq.l	#1,d1
	bne	getw7
getw6:	move.l	a0,d6		| Save input pointer
	rts

getw5:	tst.b	(a0)		| Get new buffer at end of current
	bne.s	getw11
	jsr	getline
	move.l	d6,a0
	bra.s	getw5
getw11:	jsr	iswhite		| Quit when get white space
	beq.s	getw6
	addq.l	#1,a0		|  Skip over characters
	bra.s	getw5

|
| skipwhite--skip over white space.  For a number of bizarre reasons,
| this is also the best place to read in a new buffer if we run
| off the end of the current one. It is expected that all input lines
| will end in NEWLINE--if they don't, you're taking a chance.
|
skipwhite:
	jsr	iswhite		| Check next char:
	bne	skipw2		|  No white space, return
	tst.b	(a0)+		| At end of input buffer?
	bne	skipwhite	|  No--continue
	jsr	getline		|  Yes--get a fresh buffer
	move.l	d6,a0		|   update our line buffer pointer
	bra	skipwhite
skipw2:	rts

|
| iswhite--return via the Z flag whether the char pointed to by A0
| is a white space character. Uses D3 to hold the char.
|
iswhite:	move.b	(a0),d3		| Get the char
	cmp.b	#32,d3		| Check space
	beq	iswh2
	cmp.b	#9,d3		|  ..Tab
	beq	iswh2
	cmp.b	#10,d3		|  ..Newline
	beq	iswh2
	tst.b	d3		|  ..NULL
iswh2:	rts

|
| getline--get another buffer-full from the current input unit. If no
| more input is available on it, pop back a level. If there are
| no more levels (i.e., the user typed ^D), exit. If the input is
| TTY, prompt.
|
ok_msg:	.asciz	"Ok\n> "
	.even
getline:move.l	iunit,a0	| Get ptr to head of current input record
	cmp.l	#inbufs,a0	| See if it's the TTY
	bne	getl9
	move.l	#ok_msg,a0	| Print "Ok"
	jsr	prstr
	move.l	iunit,a0	| restore A0

getl9:	move.l	a0,d6		|  Set up our input line pointer

getl4:	move.b	InUnit(a0),d0	| Get file descriptor
	ext.w	d0
        ext.l   d0              | Turn file descriptor into longword
        move.l  #1024,-(a7)     |  Third arg: # bytes
        move.l  a0,-(a7)        |  Second: store buffer
        move.l  d0,-(a7)        |  First arg is file descriptor
        clr.l   -(a7)           |  Dummy space holder
        moveq   #3,d0		|  UNIX READ syscall
        trap    #0
        bcc     getl2           | On carry set, abort on an I/O error
        jmp     io_err
getl2:	add.l   #16,a7          | Pop off arguments
        tst.l   d0              | Zero bytes read means EOF--pop up a unit!
        beq.s	getl3
        add.l   d0,a0           | Tack on the trailing NULL
        clr.b   (a0)
        rts                     |  and return
 
getl3:                           | Hit EOF--pop back a unit, or exit
        move.l  InbufPrev(a0),d0 | Get previous record
        beq     leave           |  STDIN at EOF--exit
	move.l	d0,a0
        move.l  a0,iunit        |  Update current unit
        move.l  InbufIdx(a0),d6 |  Get the old line index
        rts
 
|
| leave--do an "exit" syscall
|
leave:   move.l  #1,d0           | Request 1 means "exit"
        clr.l   -(a7)           |  We will give a return code of 0
        clr.l   -(a7)
        trap    #0
	trap	#1		| Shouldn't reach here!

|
| pad--an area of storage to use
|
pad2:	.long	getw2
pad:	.long	getpfa,pad1,0
	.ascii	"pad     "
pad1:	.space	84

|
| lookup--search for the word represented by the first 8 bytes of PAD
| in the dictionary. If it's not found, push FALSE. Otherwise,
| push the CFA, the priority, and TRUE.
|
look2:	.long	pad2
lookup:	.long	look1,look1,0
	.ascii	"lookup  "
look1:	jsr	look99
	jmp	next

look99:	move.l	latest+4,a0		| Get pointer to latest definition
	move.l	pad1,d3			| Get search string
	move.l	pad1+4,d4
look5:	cmp.l	16(a0),d3		| Compare first 4 bytes
	bne	look3
	cmp.l	20(a0),d4		| Compare second 4 bytes
	bne	look3
	move.l	12(a0),d5		| See if smudged
	and.l	#Smudged,d5
	bne	look3
	add.l	#4,a0			| turn A0 into CFA addr and push
	move.l	a0,-(a5)
	move.l	8(a0),d0		| Get status field
	and.l	#Priority,d0		| Push flag for priority
	move.l	d0,-(a5)
	move.l	#-1,-(a5)		| Push true flag--word found
	rts

look3:	move.l	(a0),d0			| Move to next entry
	tst.l	d0			| Check null ptr (end of chain)
	beq	look4
	move.l	d0,a0			| Move back to A0
	bra	look5
look4:	clr.l	-(a5)			| Not found--push false
	rts

|
| execute--pop a CFA off the stack & invoke that word
|
exec2:	.long	look2
execute:.long	exec1,exec1,0
	.ascii	"execute "
exec1:	move.l	(a5)+,a0
	jmp	next2

|
| number--if the string in PAD is not a legal number, push FALSE.
| If it is, push the value and TRUE.
|
num2:	.long	exec2
number:	.long	num1,num1,0
	.ascii	"number  "
num1:	move.l	#pad1,a0		| This is where our number is
	jsr	num99
	jmp	next

num99:	clr.l	d0			| D0 accumulates the result
	move.l	base,d5			| D5 is the current base
	cmp.b	#45,(a0)		| Flag negation if leading '-' there
	seq	d3
	bne	num3
	add.l	#1,a0

num3:	move.b	(a0)+,d1		| Get next char
	tst.b	d1			| At end of string?
	beq	num4
	cmp.b	#32,d1			| At the trailing blanks?
	beq	num4
	jsr	isdig			| Legal numeric digit?
	bne	num6			|  No, this isn't a number
	muls	d5,d0			|  Yes, shift and add
	add.l	d1,d0			|   ("isdigit" converts it)
	bra	num3

num4:	tst.b	d3			| See if it should be negated
	beq	num5
	neg.l	d0
num5:	move.l	d0,-(a5)		| Push number
	move.l	#-1,-(a5)		|  and true flag
	rts

num6:	clr.l	-(a5)			| Not number, push false
	rts

|
| isdig--check whether the character in D1 is a legal digit. If it is,
| return its value in D2, and Z set. Otherwise, return with
| Z cleared. We assume that BASE has already been put in D5,
|
isdig:	sub.l	#48,d1		| Shift '0' down to 0
	blt	isdi1		|  Was lower than '0'--can't be a digit
	cmp.b	#10,d1		| Was it 0..9?
	blt	isdi2
	sub.b	#7,d1		| Map 'A'..'F' down to 10..15
	blt	isdi1
	cmp.b	#16,d1		| Was it in range 10..15?
	blt	isdi2
	sub.b	#32,d1		| Finally, map 'a'..'f' down to 10..15
	blt	isdi1
	cmp.b	#16,d1		| Was it in range 10..15?
	bge	isdi1

isdi2:	ext.w	d1		| Turn the number into a longword
	ext.l	d1
	cmp.l	d5,d1		| See if it's within the base
	bge	isdi1
	ori.b	#4,ccr		| Set Z--we have a legal number
	rts

isdi1:	andi	#0xFB,ccr	| Clear Z--not a digit!
	rts

|
| (lit)--run-time word to push a literal onto the stack
|
plit2:	.long	num2
plit:	.long	plit1,plit1,0
	.ascii	"(lit)   "
plit1:	move.l	(a6)+,-(a5)
	jmp	next

base2:	.long	plit2,getpfa		| Current base for numbers
	.long	base,0
	.ascii	"base    "
base:	.long	10

|
| prstr--print a string to the current output unit. No management of the
| TTY is implied here--it just writes to the current output unit.
| The string to print is pointed to by A0.
|
prstr:	clr.l	d0		| String length counter
	move.l	a0,a1		| Local copy of the pointer
prst1:	tst.b	(a1)+		| At end of string?
	beq	prst2
	add.l	#1,d0		| No, increment count
	bra	prst1		|  and loop
prst2:	move.l	ounit,a1	| Build syscall parameters
	move.l	d0,-(a7)	|  Number of bytes
	move.l	a0,-(a7)	|  Buffer
	move.l	(a1),-(a7)	|  File descriptor
	clr.l	-(a7)		|  Dummy place holder
	move.l	#4,d0		| A write syscall
	trap	#0		|  Do the call
	add.l	#16,a7		| Remove the arguments
	bcc	prst3
	jmp	io_err		| Complain if the I/O failed
prst3:	rts

|
| io_err--complain about an I/O error
|
io_err:	move.l	#io_err_msg,a0	| The error message
	jsr	prstr
	jmp	abort
io_err_msg:
	.asciz	"\nI/O error!\n"
	.even

|
| notfound--routine to call when the compiler gets a word it
| doesn't know.
|
notf2:	.long	base2
notfound:
	.long	notf1,notf1,0
	.ascii	"notfound"
notf1:	move.l	#pad1,a0	| Print the word
	jsr	prstr
	move.l	#notf_msg,a0	| Print ": not found"
	jsr	prstr
	jmp	abort
notf_msg:
	.asciz	": not found\n"
	.even

|
| The match primitives--+, -, *, /
|
plus2:	.long	notf2,plus1,plus1,0
	.ascii	"+       "
plus1:	move.l	(a5)+,d0
	add.l	d0,(a5)
	jmp	next
sub2:	.long	plus2,sub1,sub1,0
	.ascii	"-       "
sub1:	move.l	(a5)+,d0
	sub.l	d0,(a5)
	jmp	next

	.globl	_lrem
mod2:	.long	sub2,mod1,mod1,0
	.ascii	"mod     "
mod1:	move.l	(a5)+,-(sp)
	move.l	(a5),-(sp)
	jbsr	_lrem
	addq.l	#8,sp
	move.l	d0,(a5)
	jmp	next

	.globl	_ldiv
div2:	.long    mod2,div1,div1,0
        .ascii	"/       "
div1:	move.l  (a5)+,-(sp)        | Divisor
        move.l  (a5),-(sp)         | Dividend
	jbsr	_ldiv
	addq	#8,sp
	move.l	d0,(a5)
	jmp     next

tdm2:	.long    div2,tdm1,tdm1,0
        .ascii	"*/mod   "
tdm1:	move.l  (a5)+,d0        | Hold divisor
        move.l  (a5)+,d1        | Get two multipliers
        move.l  (a5),d2
        muls    d1,d2
        divs    d0,d2           | Divide into the product
        move.l  d2,d3           | push remainder
        swap    d3
        ext.l   d3
        move.l  d3,(a5)
        ext.l   d2              | now push quotient
        move.l  d2,-(a5)
        jmp     next
td2:	.long    tdm2,td1,td1,0
        .ascii	"*/      "
td1:    move.l  (a5)+,d0        | Divisor
        move.l  (a5)+,d1        | Two multipliers
        move.l  (a5),d2
        muls    d1,d2
        divs    d0,d2           | divide into product
        ext.l   d2              | Extend quotient to longword and push
        move.l  d2,(a5)
        jmp     next
 
divmod2:.long    td2,divmod1,divmod1,0
        .ascii	"/mod    "
divmod1:move.l  (a5)+,d0        | Divisor
        move.l  (a5),d1         | Dividend
        divs    d0,d1
        move.l  d1,d0
        swap    d0              | Put remainder in low word
        ext.l   d0              |  fill remainder to longword quantity
        move.l  d0,(a5)
        ext.l   d1              | Now fill quotient to longword
        move.l  d1,-(a5)
        jmp     next

mul2:	.long    divmod2,mul1,mul1,0
        .ascii	"*       "
mul1:	move.l  (a5)+,d0
	move.w	d0,d1
	move.w	(a5)+,d0
	tst.l	d0
	beq.s	timesl1
	move.w	d1,a0
	mulu	d0,d1
	swap	d0
	mulu	(a5),d0
	add.w	d1,d0
	swap	d0
	clr.w	d0
	move.w	a0,d1
	mulu	(a5)+,d1
	add.l	d1,d0
	bra.s	timesl2
timesl1:	move.w	(a5)+,d0
	mulu	d1,d0
timesl2:	move.l  d0,-(a5)
        jmp     next
 
|
| u.--due to the stupidity of the 68K divide instructions, this has
| to be just an alias for ".".
|
udot2:	.long	mul2,dot1,dot1,0
	.ascii	"u.      "

|
| .--pop and print the top of stack in the current base
|
dot2:	.long    udot2,dot1,dot1,0
        .ascii	".       "
dot1:    move.l	(a5)+,d0	| The number to print
	move.l	base,d2		|  In this base
	move.l	#pad1+20,a0	| Where to build the number
	clr.b	(a0)		|  A terminating NULL
	move.b	#32,-(a0)	|  Add a trailing blank
	tst.l	d0		| Handle negative numbers
	slt	d1		| Flag a negative
	move.l	d1,-(sp)
	bge	dot3
	neg.l	d0		| Negate a negative

dot3:	move.l	d2,-(sp)
	move.l	d0,-(sp)
	jbsr	_lrem		| divide, getting the next digit
	addq.l	#8,sp
	add.b	#48,d0		| Move 0..9 to '0'..'9'
	cmp.b	#58,d0		| Hex digit?
	blt	dot4
	addq.b	#7,d0
dot4:	move.b	d0,-(a0)	| Store the digit
	move.l	d1,d0		| Get quotient
	tst.l	d0		| All of the number printed?
	bne	dot3

	move.l	(sp)+,d2
	tst.b	d2		| Tack on a leading '-' if it's needed
	beq	dot7
	move.b	#45,-(a0)
dot7:	jsr	prstr
dot9:	jmp	next

|
| ckstack--check the user's stack for underflow
|
cks_msg:.asciz	"? Stack empty\n"
	.even
cks2:	.long	dot2
ckstack:.long	cks1,cks1,0
	.ascii	"?stack  "
cks1:	cmp.l	#stack+stacksize,a5
	ble	dot9
	move.l	#cks_msg,a0	| Underflowed--complain
	jsr	prstr
	jmp	abort

|
| words--list contents of dictionary
|
wrdpad:	.ascii	"    "
word2:	.long	cks2,word1,word1,0
	.ascii	"words   "

word1:	move.l	late1,a2	| For following the dictionary chain

word3:	move.l	#pad1,a1	| Set up for next line
	moveq	#6,d0		| Number of entries per line
word4:	cmp.l	#0,a2		| See if at end of chain
	beq	word5
	move.l	16(a2),(a1)+	| Copy string
	move.l	20(a2),(a1)+
	move.l	wrdpad,(a1)+	| Pad with 4 spaces
	move.l	(a2),a2		| Advance to next entry
	subq.l	#1,d0
	bne	word4
word5:	move.b	#10,(a1)+	| Trailing newline
	clr.b	(a1)		|  and NULL
	move.l	#pad1,a0	| Write it
	jsr	prstr
	cmp.l	#0,a2		| All done?
	bne	word3
	jmp	next

|
| make_head--build a FORTH header, return its address in
| register A0.
|
make_head:
	move.l	d7,a0		| For returning it
	move.l	d7,a1		| For storing sequentially
	move.l	late1,(a1)+	| Build this def into the chain
	move.l	d7,late1
	clr.l	(a1)+		| Empty CFA
	lea	24(a0),a2	| Point PFA to the def body
	move.l	a2,(a1)+
	clr.l	(a1)+
	movem.l	a0/a1,-(a5)	| Stash our work reg
	jsr	getw1		| Build the name in-line
	movem.l	(a5)+,a0/a1	| Stash our work reg
	move.l	pad1,(a1)+
	move.l	pad1+4,(a1)+
	move.l	a1,d7		| Reset D7
	rts

|
| variable--allocate a variable in the dictionary
|
var2:	.long	word2,var1,var1,0
	.ascii	"variable"
var1:	addq.l	#3,d7		| Word-align HERE
	and.l	#0xFFFFFFFC,d7
	jsr	make_head	| Build a header
	move.l	#getpfa,4(a0)	| Our run-time code will push the PFA
	addq.l	#4,d7		| Our body starts with one word
	jmp	next

|
| constant--allocate a constant in the dictionary
|
const2:	.long	var2,const1,const1,0
	.ascii	"constant"
const1:	addq.l	#3,d7		| Word-align HERE
	and.l	#0xFFFFFFFC,d7
	jsr	make_head	| Build header
	move.l	#getpfa,4(a0)	| run-time code pushes PFA
	move.l	(a5)+,8(a0)	| Our PFA is the number on-stack
	jmp	next

|
| colon--go into compilation mode
|
colon2:	.long	const2,colon1,colon1,0
	.ascii	":       "
colon1:	addq.l	#3,d7		| Word-align definitions
	and.l	#0xFFFFFFFC,d7
	move.l	#1,state1	| Go into compilation state
	jsr	make_head	| Build our header
	move.l	#hilev,4(a0)	| our CFA invokes a high-level def
	move.l	#Smudged,12(a0)	|  and we start Smudged
	move.l	#FlgDef,-(a5)	| Push our flag for a definition
	jmp	next

|
| semicolon--come out of compilation mode
|
semi_msg:
	.asciz	"control structure not matched\n"
	.even
semi2:	.long	colon2,semi1,semi1,Priority
	.ascii	";       "
semi1:	clr.l	state1		| Back to interpretive state
	move.l	late1,a0	| Turn off the smudge bit
	clr.l	12(a0)
	move.l	d7,a0		| Compile in a trailing ';s'
	move.l	#popup,(a0)+
	move.l	a0,d7
	cmp.l	#FlgDef,(a5)+	| See if control structures matched
	bne	semi3
	jmp	next
semi3:	move.l	#semi_msg,a0	| Complain
	jsr	prstr
	jmp	abort

|
| hilev--the machine code which sets off a high-level definition
|
hilev:	move.l	a6,-(a4)	| Save old IP
	move.l	(a0),a6		| Get new IP
	jmp	next

|
| popup--aka ';s'. Pop the IP from the return stack. For exiting
| a high-level word.
|
pop2:	.long	semi2
popup:	.long	pop1,pop1,0
	.ascii	";s      "
pop1:	move.l	(a4)+,a6
	jmp	next

|
| do--build the opening part of a do..loop
|
do2:	.long	pop2,do1,do1,Priority
	.ascii	"do      "
do1:	move.l	d7,a0
	move.l	#pushr,(a0)+	| Generate code to get the loop parameters
	move.l	#pushr,(a0)+
	move.l	a0,-(a4)	| Save this place for backbranching
	move.l	#pdo,(a0)+	| compile (do)
	clr.l	(a0)+		| Leave room for our forward branch
	move.l	#FlgDo,-(a5)	| Flag our control structure
	move.l	a0,d7
do3:	jmp	next

|
| (do)--run-time word to set off a do..loop
|
pdo2:	.long	do2
pdo:	.long	pdo1,pdo1,0
	.ascii	"(do)    "
pdo1:	move.l	4(a4),d0	| Check for exit condition
	cmp.l	(a4),d0		| Check for exit condition
	blt	pdo3
	addq.l	#8,a4		| Clear the loop parameters
	move.l	(a6),a6		| Jump out of loop
	jmp	next

pdo3:	addq.l	#4,a6		| Loop's not done--advance IP
	jmp	next		|  and continue

|
| loop--compile in the closing part of a loop
|
loop2:	.long	pdo2,loop1,loop1,Priority
	.ascii	"loop    "
loop1:	cmp.l	#FlgDo,(a5)	| See if they botched
	bne	loop3
	addq.l	#4,a5		| Free the flag
	move.l	d7,a0
	move.l	#ploop,(a0)+	| Compile (loop)
	move.l	(a4)+,a1	| Get address of "loop"
	move.l	a1,(a0)+	| This is our backbranch address
	move.l	a0,4(a1)	| Give them the forward branch address
	move.l	a0,d7		| Restore HERE
	jmp	next
loop3:	move.l	#loop_msg,a0
	jsr	prstr
	jmp	abort
loop_msg:
	.asciz	"\ndo not matched by loop\n"
	.even

|
| +loop--compile in the closing part of a loop
|
aloop2:	.long	loop2,aloop1,aloop1,Priority
	.ascii	"+loop   "
aloop1:	cmp.l	#FlgDo,(a5)	| See if they botched
	bne	aloop3
	addq.l	#4,a5		| Free the flag
	move.l	d7,a0
	move.l	#paloop,(a0)+	| Compile (loop)
	move.l	(a4)+,a1	| Get address of "loop"
	move.l	a1,(a0)+	| This is our backbranch address
	move.l	a0,4(a1)	| Give them the forward branch address
	move.l	a0,d7		| Restore HERE
	jmp	next
aloop3:	move.l	#loop_msg,a0
	jsr	prstr
	jmp	abort
aloop_msg:
	.ascii	"\ndo not matched by +loop\n"
	.even

|
| (+loop)--run-time loop execution
|
paloop2:.long	aloop2
paloop:	.long	paloop1,paloop1,0
	.ascii	"(+loop) "
paloop1:move.l	(a5)+,d0	| Add on number from user's stack
	add.l	d0,4(a4)
	move.l	(a6),a6		| branch back
	jmp	next

|
| (loop)--run-time loop execution
|
ploop2:	.long	paloop2
ploop:	.long	ploop1,ploop1,0
	.ascii	"(loop)  "
ploop1:	addq.l	#1,4(a4)	| Increment the run-time index
	move.l	(a6),a6		| branch back
	jmp	next

|
| >r--pop top of operand stack & push on return stack
|
pushr2:	.long	ploop2
pushr:	.long	pushr1,pushr1,0
	.ascii	">r      "
pushr1:	move.l	(a5)+,-(a4)
	jmp	next

|
| r>--pop top of return stack & push on operand stack
|
popr2:   .long    pushr2
popr:    .long    popr1,popr1,0
        .ascii	"r>      "
popr1:   move.l  (a4)+,-(a5)
        jmp     next
 
|
| r@--copy top of return stack to user stack
|
rget2:	.long    popr2,rget1,rget1,0
        .ascii	"r@      "
rget1:	move.l  (a4),-(a5)
        jmp     next
 
|
| depth--tell how many elements are on user stack
|
depth2:	.long    rget2,depth1,depth1,0
        .ascii	"depth   "
depth1:	move.l  #stack+stacksize,d0
        sub.l   a5,d0
        asr.l   #2,d0
        move.l  d0,-(a5)
        jmp     next
 
|
| i--push index of innermost do..loop context
|
push_i2:.long    depth2,push_i1,push_i1,0
        .ascii	"i       "
push_i1:move.l  4(a4),-(a5)
        jmp     next
|
| j--like i, but second most-innermost
|
push_j2:.long    push_i2,push_j1,push_j1,0
        .ascii	"j       "
push_j1:move.l  12(a4),-(a5)
        jmp     next

|
| leave--jump out of the innermost loop structure. Note that control
| structure matching isn't done here, since we will probably be
| inside of multiple if..endif contexts--meaningful error checking
| would be very difficult to provide.
|
leave2:	.long	push_j2,leave1,leave1,Priority
	.ascii	"leave   "
leave1:	move.l	(a4),a1		| This is the address of the (do) part
	move.l	d7,a0		| We will be compiling some stuff in:
	move.l	#pleave,(a0)+	|  (leave)
	addq.l	#4,a1		|  addr of the exit location--(do)+1
	move.l	a1,(a0)+
	move.l	a0,d7
	jmp	next

|
| (leave)--fetch via the word which follows us, and make that the IP
|
pleave2:.long	leave2
pleave:	.long	pleave1,pleave1,0
	.ascii	"(leave) "
pleave1:move.l	(a6),a0		| Addr of exit address
	move.l	(a0),a6		| Set IP to it
	addq.l	#8,a4		| Clear the do..loop's parameters of rstack
	jmp	next

|
| if--starting part of a conditional
|
if2:	.long	pleave2,if1,if1,Priority
	.ascii	"if      "
if1:	move.l	d7,a0
	move.l	#zbranch,(a0)+	| If false, branch around
	move.l	a0,-(a5)	|  save this place for back-branch
	clr.l	(a0)+		|  leave room for it
	move.l	a0,d7
	move.l	#FlgIf,-(a5)	| Flag the control structure
	jmp	next

|
| else--optional middle part of a conditional
|
else2:	.long	if2,else1,else1,Priority
	.ascii	"else    "
else1:	cmp.l	#FlgIf,(a5)	| Check control structure
	bne	else3
	move.l	d7,a0
	move.l	4(a5),a1	| Save location to backpatch
	move.l	#branch,(a0)+	| Patch in a branch out of the conditional
	move.l	a0,4(a5)	|  the new back-patch location
	clr.l	(a0)+
	move.l	a0,(a1)		| Now patch in address of false part of cond.
	move.l	a0,d7
	jmp	next

else3:	move.l	#else_msg,a0	| Complain about bad control structure
	jsr	prstr
	jmp	abort
else_msg:
	.asciz	"\nelse does not match an if\n"
	.even

|
| endif--ending part of a conditional
|
endif2:	.long	else2,endif1,endif1,Priority
	.ascii	"endif   "
endif1:	cmp.l	#FlgIf,(a5)	| Check control strucure
	bne	endif3
	addq.l	#4,a5		| Pop off flag
	move.l	(a5)+,a0	| Get address to back-patch
	move.l	d7,(a0)		|  backpatch it
	jmp	next

endif3:	move.l	#endif_msg,a0	| complain
	jsr	prstr
	jmp	abort
endif_msg:
	.asciz	"\nendif does not match if/else\n"
	.even

|
| stack manipulation words--dup, swap, rot, -rot, drop, over
|
over2:	.long    endif2,over1,over1,0
        .ascii	"over    "
over1:	move.l  4(a5),-(a5)
        jmp     next
pick2:	.long    over2,pick1,pick1,0
        .ascii	"pick    "
pick1:	move.l  (a5)+,d0
        asl.l   #2,d0           | Scale D0 for a word offset
        move.l  0(a5,d0.l),-(a5)
        jmp     next
roll2:	.long    pick2,roll1,roll1,0
        .ascii	"roll    "
roll1:	move.l  (a5)+,d0
        asl.l   #2,d0
        move.l  0(a5,d0.l),d1           | Save word rolling into
roll3:	tst.l   d0                      | While not to top of stack...
        beq     roll4
        move.l  -4(a5,d0.l),0(a5,d0.l)  | Copy down a word
        subq.l  #4,d0                   | Advance a word
        bra     roll3
roll4:	move.l  d1,(a5)                 | Replace top with word
        jmp     next
dup2:	.long    roll2,dup1,dup1,0
        .ascii	"dup     "
dup1:	move.l  (a5),-(a5)
        jmp     next
qdup2:	.long    dup2,qdup1,qdup1,0
        .ascii	"?dup    "
qdup1:	move.l  (a5),d0
        beq     qdup3
        move.l  d0,-(a5)
qdup3:	jmp    next
swap2:	.long    qdup2,swap1,swap1,0
        .ascii	"swap    "
swap1:	move.l  (a5)+,d0
        move.l  (a5),d1
        move.l  d0,(a5)
        move.l  d1,-(a5)
        jmp     next
rot2:	.long    swap2,rot1,rot1,0
        .ascii	"rot     "
rot1:	move.l  (a5)+,d0
        move.l  (a5)+,d1
        move.l  (a5),d2
        move.l  d1,(a5)
        move.l  d0,-(a5)
        move.l  d2,-(a5)
        jmp     next
drot2:	.long    rot2,drot1,drot1,0
        .ascii	"-rot    "
drot1:	move.l	(a5)+,d0
	move.l	(a5)+,d1
	move.l  (a5),d2
        move.l  d0,(a5)
        move.l  d2,-(a5)
        move.l  d1,-(a5)
        jmp     next
drop2:	.long    drot2,drop1,drop1,0
        .ascii	"drop    "
drop1:	addq.l  #4,a5
        jmp     next
 
|
| begin--start a structured loop
|
beg2:	.long    drop2,beg1,beg1,Priority
        .ascii	"begin   "
beg1:	move.l  d7,-(a5)
        move.l  #FlgBeg,-(a5)
        jmp     next
 
|
| again--unconditional branch back; an infinite loop
|
again2:	.long    beg2,again1,again1,Priority
        .ascii	"again   "
again1:	cmp.l   #FlgBeg,(a5)
        bne     again3          
        addq.l  #4,a5
        move.l  d7,a0
        move.l  #branch,(a0)+
        move.l  (a5)+,(a0)+
        move.l  a0,d7
        jmp     next
again3:  move.l  #again_msg,a0
        jsr     prstr
        jmp     abort
again_msg:
        .asciz	"\nagain does not match a begin\n"
        .even
 
|
| until--branch back until condition becomes true
|
until2:	.long    again2,until1,until1,Priority
        .ascii	"until   "
until1:	cmp.l   #FlgBeg,(a5)
        bne     until3
        addq.l  #4,a5
        move.l  d7,a0
        move.l  #zbranch,(a0)+
        move.l  (a5)+,(a0)+
        move.l  a0,d7
        jmp     next
until3:	move.l  #until_msg,a0
        jsr     prstr
        jmp     abort
until_msg:
        .asciz	"\nuntil does not match a begin\n"
        .even
 
|
| while..repeat: loop with exit check up front
|
while2:	.long	until2,while1,while1,Priority
	.ascii	"while   "
while1:	cmp.l	#FlgBeg,(a5)		| Check control structure
	bne	while3
	move.l	d7,a0
	move.l	#zbranch,(a0)+		| Branch out on false
	move.l	a0,(a5)			|  save where to backpatch
	clr.l	(a0)+
	move.l	a0,d7
	move.l	#FlgWhi,-(a5)		| And place our own flag
	jmp	next
while3:	move.l	#while_msg,a0		| Complain
	jsr	prstr
	jmp	abort
while_msg:
	.asciz	"\nwhile does not match a begin\n"
	.even

|
| repeat--the closing part of a begin..while..repeat structure
|
rep2:	.long	while2,rep1,rep1,Priority
	.ascii	"repeat  "
rep1:	cmp.l	#FlgWhi,(a5)		| Check control structure
	bne	rep3
        addq.l  #4,a5
        move.l  (a5)+,a1                | Save where to backpatch
        move.l  d7,a0
        move.l  #branch,(a0)+           | Generate a backbranch
        move.l  (a5)+,(a0)+             |  to top of loop
        move.l  a0,d7
        move.l  d7,(a1)                 | Backpatch exit location, HERE
        jmp     next
rep3:	move.l  #rep_msg,a0             | Complain
        jsr     prstr
        jmp     abort
rep_msg:.asciz	"\nrepeat does not match a while\n"
        .even
 
|
| xor--exclusive OR
|
xor2:	.long    rep2,xor1,xor1,0
        .ascii	"xor     "
xor1:	move.l  (a5)+,d0
        eor.l	d0,(a5)
        jmp     next
 
|
| not--one's complement
|
not2:	.long    xor2,not1,not1,0
        .ascii	"not     "
not1:	eor.l	#0xFFFFFFFF,(a5)
        jmp     next
 
|
| 1+, 1-, 2+, 2-, 2*, 2/--common, quick math operations
|
onep2:	.long    not2,onep1,onep1,0
        .ascii	"1+      "
onep1:	addq.l  #1,(a5)
        jmp     next
onem2:	.long    onep2,onem1,onem1,0
        .ascii	"1-      "
onem1:	subq.l  #1,(a5)
        jmp     next
twop2:	.long    onem2,twop1,twop1,0
        .ascii	"2+      "
twop1:	addq.l  #2,(a5)
        jmp     next
twom2:	.long    twop2,twom1,twom1,0
        .ascii	"2-      "
twom1:	subq.l  #2,(a5)
        jmp     next
twot2:	.long    twom2,twot1,twot1,0
        .ascii	"2*      "
twot1:	move.l  (a5),d0
        asl.l   #1,d0
        move.l  d0,(a5)
        jmp     next
twod2:	.long    twot2,twod1,twod1,0 
        .ascii	"2/      "
twod1:	move.l  (a5),d0
        asr.l   #1,d0
        move.l  d0,(a5)
        jmp     next
 
|
| c@, c!--character fetch/store
|
cfetch2:.long    twod2,cfetch1,cfetch1,0
        .ascii	"c@      "
cfetch1:move.l  (a5),a0
        move.b  (a0),d0
        ext.w   d0
        ext.l   d0
        move.l  d0,(a5)
        jmp     next
cstore2:.long    cfetch2,cstore1,cstore1,0
        .ascii	"c!      "
cstore1:move.l  (a5)+,a0
        move.l  (a5)+,d0
        move.b  d0,(a0)
        jmp     next
pstore2:.long    cstore2,pstore1,pstore1,0
        .ascii	"+!      "
pstore1:move.l  (a5)+,a0
        move.l  (a5)+,d0
        add.l   d0,(a0)
        jmp     next
 
|
| min and max--push greater or less of two numbers
|
min2:	.long    pstore2,min1,min1,0
        .ascii	"min     "
min1:	move.l  (a5)+,d0
        cmp.l   (a5),d0
        bge     min3
min4:	move.l  d0,(a5)
min3:	jmp     next
max2:	.long    min2,max1,max1,0
        .ascii	"max     "
max1:	move.l  (a5)+,d0
        cmp.l   (a5),d0
        ble     min3
        bra     min4
 
|
| abs, negate--replace number with its absolute value or negation
|
abs2:	.long    max2,abs1,abs1,0
        .ascii	"abs     "
abs1:	move.l  (a5),d0
        bge     min3
        neg.l   (a5)
        jmp     next
neg2:	.long    abs2,neg1,neg1,0
        .ascii	"negate  "
neg1:	neg.l   (a5)
        jmp     next
 
|
| cmove--move a range of bytes
|
cmov2:	.long	neg2,cmov1,cmov1,0
	.ascii	"cmove   "
cmov1:	move.l	(a5)+,d0	| Count
	move.l	(a5)+,a0	| Destination
	move.l	(a5)+,a1	| Source
	tst.l	d0		| Catch case of zero-length
	beq	cmov4
cmov3:	move.b	(a1)+,(a0)+	| Move bytes
	dbra	d0,cmov3
cmov4:	jmp	next

|
| cmove>--like cmove, but set up to guard against the "ripple" effect
|
cmovu2:	.long	cmov2,cmovu1,cmovu1,0
	.ascii	"cmove>  "
cmovu1:	move.l	(a5)+,d0	| Count
	move.l	(a5)+,a0	| Destination
	move.l	(a5)+,a1	| Source
	tst.l	d0		| Zero-length?
	beq	cmov4
	add.l	d0,a0		| Point to end of destination
	add.l	d0,a1		|  same for source
cmovu3:	move.b	-(a1),-(a0)	| Move bytes
	dbra	d0,cmovu3
	jmp	next

|
| fill--fill a range of bytes with a constant
|
fill2:	.long	cmovu2,fill1,fill1,0
	.ascii	"fill    "
fill1:	move.l	(a5)+,d0	| Get byte constant to use
	move.l	(a5)+,d1	| # Bytes to fill
	move.l	(a5)+,a0	| Where to start
	tst.l	d0		| Avoid zero-length
	beq	cmov4
fill3:	move.b	d0,(a0)+	| Fill bytes
	subq.l	#1,d1
	bne	fill3
	jmp	next

|
| count--get byte at addr, advance addr
|
count2:	.long	fill2,count1,count1,0
	.ascii	"count   "
count1:	move.l	(a5),a0		| Get addr
	move.b	(a0)+,d0	| Get byte at addr, advance
	move.l	a0,(a5)		| Store back addr
	ext.w	d0		|  and extended byte
	ext.l	d0
	move.l	d0,-(a5)
	jmp	next

|
| -trailing--trim trailing spaces
|
dtrail2:.long	count2,dtrail1,dtrail1,0
	.ascii	"-trailin"
dtrail1:move.l	(a5)+,d0	| Current count
	beq	dtrail4		|  handle zero-length
	move.l	(a5),a0		| Address of string
	add.l	d0,a0		| Get address of current end of string
dtrail3:	cmp.b	#32,-(a0)	| Check next char
	beq	dtrail4
	subq.l	#1,d0
	bne	dtrail3
dtrail4:	move.l	d0,-(a5)	| Push back count
	jmp	next

|
| decimal, hex, octal--set BASE
|
deci2:	.long	dtrail2,deci1,deci1,0
	.ascii	"decimal "
deci1:	move.l	#10,base
	jmp	next
hexa2:	.long	deci2,hexa1,hexa1,0
	.ascii	"hex     "
hexa1:	move.l	#16,base
	jmp	next
octa2:	.long	hexa2,octa1,octa1,0
	.ascii	"octal   "
octa1:	move.l	#8,base
	jmp	next

|
| The number printing words--<# # #> #s hold sign
|
lsh_pos:.space	1*4			| Position in output buffer

lsh2:	.long	octa2,lsh1,lsh1,0
	.ascii	"<#      "		| Prepare for conversion
lsh1:	move.l	#pad1+70,lsh_pos
	jmp	next

sh2:	.long	lsh2,sh1,sh1,0
	.ascii	"#       "		| Convert next digit
sh1:	jsr	sh99
	jmp	next

sh99:	move.l	base,-(sp)		|  get BASE--format is wrong in mem.
	move.l	(a5),-(sp)
	jbsr	_lrem
	move.l	d1,(a5)			|  put quotient back to stack
	add.l	#48,d0			|  Remainder: map 0 to '0'
	cmp.l	#58,d0			|  Check for HEX digits
	blt	sh3
	addq.l	#7,d0			|  Map 10 to 'A'
sh3:	move.l	lsh_pos,a0		|  Store character into PAD, advance
	move.b	d0,-(a0)
	move.l	a0,lsh_pos
	rts

shg2:	.long	sh2,shg1,shg1,0
	.ascii	"#>      "		| End conversion
shg1:	move.l	lsh_pos,d0
	move.l	d0,(a5)			| Push address
	move.l	#pad1+70,d1		| Calculate count
	sub.l	d0,d1
	move.l	d1,-(a5)		| Push count
	jmp	next

shs2:	.long	shg2,shs1,shs1,0
	.ascii	"#s      "		| Convert all remaining digits
shs1:	jsr	sh99			| Do a digit
	tst.l	(a5)			| See if done
	bne	shs1
	jmp	next

hold2:	.long	shs2,hold1,hold1,0
	.ascii	"hold    "		| Put a char into the string
hold1:	move.l	lsh_pos,a0
	move.l	(a5)+,d0
	move.b	d0,-(a0)
	move.l	a0,lsh_pos
hold3:	jmp	next

sign2:	.long	hold2,sign1,sign1,0
	.ascii	"sign    "		| Add a '-' if sign negative
sign1:	tst.l	(a5)+
	bge	hold3
	move.l	#45,-(a5)
	bra	hold1

|
| ."--generate code to print a string at run-time
|
dotq2:	.long	sign2,dotq1,dotq1,Priority
	.ascii	".\"      "
dotq1:	move.l	d7,a0
	move.l	#pdotq,(a0)+	| Compile (.")
	move.l	d6,a1		| Get line pointer
	addq.l	#1,a1		|  advance past current word delimiter
dotq3:
	move.b	(a1)+,d0	| Get next char
	beq	dotq5		|  read a new buffer if we run out
	cmp.b	#34,d0		| End when we find the closing "
	beq	dotq4
	move.b	d0,(a0)+	| Add the character
	bra	dotq3

dotq5:	move.l	a0,-(sp)
	jsr	getline		| Get new buffer
	move.l	(sp)+,a0
	move.l	d6,a1
	bra	dotq3

dotq4:	clr.b	(a0)+		| Terminating NULL
	move.l	a1,d6		| Update line pointer
	move.l	a0,d7
	addq.l	#3,d7		| Longword-align DP
	and.l	#0xFFFFFFFC,d7
	jmp	next

|
| (.")--run-time word to print a string
|
pdotq2:	.long	dotq2
pdotq:	.long	pdotq1,pdotq1,0
	.ascii	"(.\")    "
pdotq1:	move.l	a6,a0
	jsr	prstr
pdotq3:	tst.b	(a6)+		| Skip past text
	bne	pdotq3
	move.l	a6,d0
	addq.l	#3,d0		| Align IP
	and.l	#0xFFFFFFFC,d0
	move.l	d0,a6
	jmp	next

|
| .(--print a message to the terminal from the input stream
|
dotp2:	.long	pdotq2,dotp1,dotp1,Priority
	.ascii	".(      "
dotp1:	move.l	d6,a1		| Get line pointer
	addq.l	#1,a1		|  advance past current word delimiter
	move.l	#pad1,a0	| Build message into PAD

dotp3:	move.b	(a1)+,d0	| Get next char
	beq	dotp5		|  read a new buffer if we run out
	cmp.b	#41,d0		| End when we find the closing "
	beq	dotp4
	move.b	d0,(a0)+	| Add the character
	bra	dotp3

dotp5:	jsr	getline		| Get new buffer
	move.l	d6,a1
	bra	dotp3

dotp4:	clr.b	(a0)+		| Terminating NULL
	move.l	a1,d6		| Update line pointer
	move.l	#pad1,a0	| Print the message
	jsr	prstr
	jmp	next

|
| cr--print newline
|
cr_msg:	.byte	10,0
cr2:	.long	dotp2,cr1,cr1,0
	.ascii	"cr      "
cr1:	move.l	#cr_msg,a0
	jsr	prstr
	jmp	next

|
| emit--print out a character
|
emit_buf:
	.space	1
	.byte	0,0,0		| Terminating NULL, 2 wasted
emit2:	.long	cr2,emit1,emit1,0
	.ascii	"emit    "
emit1:	move.l	(a5)+,d0
	move.b	d0,emit_buf
	move.l	#emit_buf,a0
	jsr	prstr
	jmp	next

|
| type--print out a string given a count & a pointer
|
type2:	.long	emit2,type1,type1,0
	.ascii	"type    "
type1:	move.l	(a5)+,d0	| Count
	move.l	(a5)+,a0	| Addr
	move.l	#pad1,a1	| Where to buffer to
type3:	tst.l	d0		| Out of chars?
	beq	type4
	move.b	(a0)+,(a1)+	| Store a char
	subq.l	#1,d0		| Decrement count
	bra	type3
type4:	clr.b	(a1)		| Terminating NULL
	move.l	#pad1,a0
	jsr	prstr
	jmp	next

|
| space--emit a space
|
space2:	.long	type2,space1,space1,0
	.ascii	"space   "
space1:	move.l	#32,-(a5)
	bra	emit1

|
| spaces--emit N spaces
|
spac_buf:			| A printable space
	.byte	32,0,0,0
spaces2:.long	space2,spaces1,spaces1,0
	.ascii	"spaces  "
spaces1:tst.l	(a5)		| Enough spaces?
	beq	spaces3
	move.l	#spac_buf,a0
	jsr	prstr
	sub.l	#1,(a5)		| Decrement count
	bra	spaces1
spaces3:	addq.l	#4,a5		| Pop count
	jmp	next

|
| key--get a key from STDIN. Normally, this will block until a whole
| line is entered. However, if the TTY is put into RAW mode,
| this will respond on a key-by-key basis.
|
keybuf:	.space 	1*4		| Holds the keystroke
key2:	.long	spaces2,key1,key1,0
	.ascii	"key     "
key1:	move.l	#1,-(a7)	| Build READ syscall parameters--1 byte
	move.l	#keybuf,-(a7)	|  buffer address
	clr.l	-(a7)		|  0--STDIN
	clr.l	-(a7)		|  dummy
        moveq   #3,d0		|  UNIX READ syscall
	trap	#0
	add.l	#16,a7		| Remove the parameters from stack
	move.b	keybuf,d0	| Push byte
	ext.w	d0
	ext.l	d0
	move.l	d0,-(a5)
	jmp	next

|
| expect--read a number of chars from the terminal
|
expect2:.long	key2,expect1,expect1,0
	.ascii	"expect  "
expect1:move.l	(a5)+,-(a7)	| UNIX syscall: N bytes
	move.l	(a5)+,-(a7)	|  to buffer
	clr.l	-(a7)		|  STDIN
	clr.l	-(a7)		|  dummy
        moveq   #3,d0		|  UNIX READ syscall
	trap	#0
	move.l	d0,span		| Store # bytes read
	add.l	#16,a7		| Remove the parameters from stack
	jmp	next
span2:	.long	expect2,getpfa,span,0
	.ascii	"span    "
span:	.space	1*4

|
| abort--jump to abort
|
abort2:	.long	span2
do_abort:.long	abort,abort,0
	.ascii	"abort   "

|
| abort"--if top is true, print a message and abort
|
qabort2:.long	abort2,qabort1,qabort1,Priority
	.ascii	"abort\"  "
qabort1:move.l	d7,a0
	move.l	#zbranch,(a0)+	| Skip the whole shebang on false
	move.l	a0,a2		| Mark where to backpatch
	clr.l	(a0)+		| Leave room for the branch address

	move.l	#pdotq,(a0)+	| Compile (.")
	move.l	d6,a1		| Get line pointer
	addq.l	#1,a1		|  advance past current word delimiter
qabort3:
	move.b	(a1)+,d0	| Get next char
	beq	qabort5		|  read a new buffer if we run out
	cmp.b	#34,d0		| End when we find the closing "
	beq	qabort4
	move.b	d0,(a0)+	| Add the character
	bra	qabort3

qabort5:jsr	getline		| Get new buffer
	move.l	d6,a1
	bra	qabort3

qabort4:clr.b	(a0)+		| Terminating NULL
	move.l	a1,d6		| Update line pointer
	move.l	a0,d7
	addq.l	#3,d7		| Longword-align DP
	and.l	#0xFFFFFFFC,d7
	move.l	d7,a0
	move.l	#do_abort,(a0)+	| Put in ABORT
	move.l	a0,d7
	move.l	d7,(a2)		| Backpatch false case
	jmp	next


|
| quit--leave parameter stack alone, but return to INTERP
|
quit2:	.long	qabort2,quit1,quit1,0
	.ascii	"quit    "
quit1:	move.l	#rstack+stacksize,a4	| Clear return stack
	move.l	#interp,a6
	jmp	next

|
| here--push address of next free location
|
here2:	.long	quit2,here1,here1,0
	.ascii	"here    "
here1:	move.l	d7,-(a5);
	jmp	next

|
| tib--address of text input buffer
|
tib2:	.long	here2,tib1,tib1,0
	.ascii	"tib     "
tib1:	move.l	iunit,-(a5)
	jmp	next

|
| >body--turn pointer to CFA field into pointer to parameter field
|
gbod2:	.long	tib2,gbod1,gbod1,0
	.ascii	">body   "
gbod1:	move.l	(a5),a0
	move.l	4(a0),(a5)
	jmp	next

|
| (--start a forth comment )
|
paren2:	.long	gbod2,paren1,paren1,Priority
	.ascii	"(       "	| )
paren1:	move.l	d6,a0
paren4:	move.b	(a0)+,d0	| Get next char
	cmp.b	#41,d0		| End on closing paren
	beq	paren3
	tst.b	d0		| Get new buffer on end of current
	bne	paren4
	jsr	getline
	bra	paren1
paren3:	move.l	a0,d6		| Restore line pointer
	jmp	next

|
| allot--allocate N bytes off end of dictionary
|
allot2:	.long	paren2,allot1,allot1,0
	.ascii	"allot   "
allot1:	move.l	(a5)+,d0
	add.l	d0,d7
	jmp	next

|
| does>--terminate execution of word which calls this, but also set it up
| so that the LATEST word has its PFA directed to after this word.
| : definer create ...1... does> ...2... ;
| Will be used as: definer <word>
| <word> will be added to the dictionary, and ...1... may do any
| actions it wishes. When <word> is later executed, it will run
| the code ...2...
|
does2:	.long	allot2,does1,does1,Priority
	.ascii	"does>   "
does1:	move.l	d7,a0
	move.l	#pdoes,(a0)+	| Compile in (does)
	move.l	a0,d7
	jmp	next
pdoes2:	.long	does2
pdoes:	.long	pdoes1,pdoes1,0
	.ascii	"(does)  "
pdoes1:	move.l	late1,a0	| Get LFA of latest definition
	move.l	#hilev,4(a0)	| Make this execute as a high-level def
	move.l	a6,8(a0)	| Fill in PFA with rest of this word's body
	move.l	(a4)+,a6	| Return from this word
	jmp	next

|
| immediate--set the Priority bit of the latest definition
|
immed2:	.long	pdoes2,immed1,immed1,0
	.ascii	"immediat"
immed1:	move.l	late1,a0
	or.l	#Priority,12(a0)	| Set Priority in SFA word
	jmp	next

|
| [compile], compile--immediate & non-immediate versions of compile
|
bcomp2:	.long	immed2,bcomp1,bcomp1,Priority
	.ascii	"[compile"
bcomp1:	jsr	getw1		| Fetch next word from stream
	jsr	look99		| See if it can be found
	tst.l	(a5)+		| Error if it couldn't
	beq	bcomp3
	addq.l	#4,a5		| Drop the priority field
	move.l	d7,a0		| Compile in CFA
	move.l	(a5)+,(a0)+
	move.l	a0,d7
	jmp	next
bcomp3:	jmp	notf1		| Not found: complain

comp2:	.long	bcomp2,bcomp1,bcomp1,0
	.ascii	"compile "

|
| literal--compile a literal
|
lit2:	.long	comp2,lit1,lit1,Priority
	.ascii	"literal "
lit1:	move.l	d7,a0
	move.l	#plit,(a0)+
	move.l	(a5)+,(a0)+
	move.l	a0,d7
	jmp	next

|
| [, ]--turn compilation off & on, respectively
|
compon2:.long	lit2,compon1,compon1,0
	.ascii	"]       "
compon1:move.l	#-1,state1
	jmp	next
compof2:.long	compon2,compof1,compof1,Priority
	.ascii	"[       "
compof1:clr.l	state1
	jmp	next

|
| word--get a word from the input stream, put in string
|
word_buf:.space	84
gword2:	.long	compof2,gword1,gword1,0
	.ascii	"word    "
gword1:	move.l	(a5)+,d0	| Delimiter char
	move.l	#word_buf+1,a0	| Where to put the chars
	move.l	d6,a1		| Input line buffer
	clr.l	d2		| Count # chars received
gword3:	move.b	(a1)+,d1	| Get next char
	beq	gword4		|  get new bufferfull if current empty
	cmp.b	d0,d1		| Found delimiter?
	beq	gword5
	move.b	d1,(a0)+	| Store char
	addq.l	#1,d2		| Increment count
	bra	gword3
gword4:
	movem.l	d0/a0,-(a7)	| Save d0 and a0
	jsr	getline		| Get next line
	movem.l	(a7)+,d0/a0
	move.l	d6,a1
	bra	gword3
gword5:
	clr.b	(a0)		| Add NULL termination
	move.b	d2,word_buf	| Store count in first byte
	move.l	a1,d6		| Update line pointer
	move.l	#word_buf,-(a5)	| Return pointer to it
	jmp	next

|
| >in--give a byte offset into current buffer
|
to_in2:	.long	gword2,to_in1,to_in1,0
	.ascii	">in     "
to_in1:	move.l	d6,d0
	sub.l	iunit,d0
	move.l	d0,-(a5)
	jmp	next

|
| #tib--length of current input buffer
|
ntib2:	.long	to_in2,ntib1,ntib1,0
	.ascii	"#tib    "
ntib1:	move.l	iunit,a0	| Ptr into buf
	clr.l	d1		| Counter of # chars
ntib3:	tst.b	(a0)+		| Check next byte
	beq	ntib4
	addq.l	#1,d1
	bra	ntib3
ntib4:	move.l	d1,-(a5)	| Push count
	jmp	next

|
| create--create a dictionary entry
|
creat2:	.long	ntib2,creat1,creat1,0
	.ascii	"create  "
creat1:	jsr	make_head	| Build the header
	move.l	#getpfa,4(a0)	| Set it up to be variable/constant
	jmp	next

|
| '--push address of CFA
|
tick2:	.long	creat2,tick1,tick1,0
	.ascii	"'       "
tick1:	jsr	getw1		| Get word
	jsr	look99		| Look up word
	tst.l	(a5)+		| Abort on error
	beq	tick3
	addq.l	#4,a5		| Drop priority flag
	jmp	next
tick3:
	jmp	notf1

|
| [']--for compiling in a compilation address as a literal
|
btick2:	.long	tick2,btick1,btick1,Priority
	.ascii	"[\"]     "
btick1:	jsr	getw1		| Get word
	jsr	look99		| Look up word
	tst.l	(a5)+		| Abort on error
	beq	tick3
	addq.l	#4,a5		| Drop priority flag
	move.l	d7,a0		| Compile in (lit)
	move.l	#plit,(a0)+
	move.l	(a5)+,(a0)+	|  <compilation addr>
	move.l	a0,d7
	jmp	next

|
| find--find a string in the dictionary
|
find2:	.long	btick2,find1,find1,0
	.ascii	"find    "
find1:	move.l	latest+4,a0		| Get pointer to latest definition
	move.l	(a5),a1			| Get search string
	move.l	(a1),d3
	move.l	4(a1),d4
	jsr	look5			| Go find the string
	tst.l	(a5)			| See if it was found
	beq	find3			|  wasn't, can just return
	addq.l	#4,a5			| Was, pop boolean flag
	tst.l	(a5)+			| Change priority flag
	bne	find4

	move.l	(a5),4(a5)		| Move comp addr over string addr
	move.l	#-1,(a5)		|  not priority, flag -1
	bra	find3

find4:	move.l	(a5),4(a5)		| Move comp addr over string addr
	move.l	#1,(a5)			|  was priority, flag 1

find3:	jmp	next

|
| forget--find a word in the dictionary, and remove it
|
forg2:	.long	find2,forg1,forg1,0
	.ascii	"forget  "
forg1:	jsr	getw1		| Get the name to forget
	jsr	look99		| Find it in the dictionary
	tst.l	(a5)+		| Found it?
	beq	forg3		|  nope...
	addq.l	#4,a5		| Drop priority flag
	move.l	(a5)+,a0	| Put CFA into A0
	subq.l	#4,a0		| Put A0 back to LFA
	move.l	(a0),late1	| Point LATEST to previous word
	move.l	a0,d7		| Free memory back to here
	jmp	next

forg3:	jmp	notf1		| Forget WHO?

|
| input <file>--redirect input from a file
|
input2:	.long	forg2,input1,input1,0
	.ascii	"input   "
input1:	move.l	iunit,a0	| Room for more nesting?
	add.l	#Inbufsize,a0
	cmp.l	#End_inbufs,a0
	beq	input4
	move.l	a0,-(a7)	| Save address of new buffer

	move.l	d6,a0		| Read in until end of word
	jsr	skipwhite
	lea	pad1,a1		| Where to build into
input10:jsr	iswhite		| While not at end of word
	bne.s	input11
	tst.b	(a0)		| At end of input buffer?
	bne.s	input12
	move.l	a1,-(a7)	| Get new buffer-full
	jsr	getline
	move.l	(a7)+,a1
	move.l	d6,a0
	bra.s	input10

input11:move.b	(a0)+,(a1)+	| Store next char
	bra.s	input10

input12:clr.b	(a1)		| Trailing NULL
	move.l	a0,d6		|  update input pointer
	clr.l	-(a7)		| Mode 0=read
	pea	pad1		| Pointer to file name
	clr.l	-(a7)		| dummy space
	moveq	#5,d0		| Open request
	trap	#0
	bcs	input3
	add.l	#12,a7		| Get rid of parameters
	move.l	(a7)+,a0	| Get new buffer addr again
	move.l	iunit,a1	| Get previous
	move.l	a1,InbufPrev(a0) |  Save
	move.l	d6,InbufIdx(a1)	|   Save index into old buffer
	move.l	a0,InbufIdx(a0)	| Clear the buffer
	move.b	d0,InUnit(a0)	| Save UNIX FD to use
	clr.b	(a0)
	move.l	a0,d6
	move.l	a0,iunit	| Update current input unit
	jmp	next

input3:	lea	input_msg,a0
input5:	jsr	prstr
	jmp	abort
input4:	lea	input_msg2,a0
	bra.s	input5
input_msg:	.asciz "Could not open file for input"
input_msg2:	.asciz "Too many files nested"
	.even

|
| exit--return from the current high-level word
|
exit2:	.long	input2,pop1,pop1,0
	.ascii	"exit    "

|||||||||||||||||||||||||||||||||||||
| Insert new definitions above here |
|||||||||||||||||||||||||||||||||||||
|
| latest--pointer to most current LFA defined
|
late2:	.long    exit2
latest:	.long    getpfa,late1,0
	.ascii	"latest  "
late1:	.long    late2
 
|
| The user dictionary space
|
	.comm	udict,umem*1024	| User dictionary space
 
|
| The End!
|
