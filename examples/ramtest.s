|--
|-- A simple RAM test based on that given on page 48 of King&Knight.
|-- It has been extended to use sun3 calls to find out the upper and lower
|-- bounds of read/write area and POSIX calls to report the success/failure.

.set	test_pattern, 0xaa
.set	stack_space, 10*4

.set	posix_syscall, 0
.set	posix_exit, 1
.set	posix_write, 4
.set	posix_exit_success, 0
.set	posix_exit_failure, 1
.set	posix_stderr, 2
.set	posix_stdout, 1

.set	svm_syscall, 8
.set    svm_vma_start, 1
.set    svm_vma_end, 2
.set	svm_vma_rw_start, 3

start:
|-- Locate the vma upper and lower bounds.
	move.l	#svm_vma_rw_start, d0
	trap	#svm_syscall
	move.l	d0, a0
	move.l	d0, a3
	move.l	#svm_vma_end, d0
	trap	#svm_syscall
	sub.l	#stack_space, d0
	move.l	d0, a1

|-- Fill memory with required value
0:	move.b	#test_pattern, (a0)+          
	cmpa.l	a1, a0            
	bne.s	0b

|-- Check memory has kept that value
	movea.l	a3, a0
0:	cmpi.b	#test_pattern, (a0)+
	bne	mem_test_fail
	cmpa.l	a1, a0
	bne.s	0b

mem_test_succ:
	move.l	#20, -(a7)
	move.l	#succ_msg, -(a7)
	move.l	#posix_stdout, -(a7)
	clr.l	-(a7)
	move.l	#posix_write, d0
	trap	#posix_syscall
	add.l	#4*4, a7

	move.l	#posix_exit_success, -(a7)
	clr.l	-(a7)
	move.l	#posix_exit, d0
	trap	#posix_syscall

mem_test_fail:
	move.l	#20, -(a7)
	move.l	#fail_msg, -(a7)
	move.l	#posix_stderr, -(a7)
	clr.l	-(a7)
	move.l	#posix_write, d0
	trap	#posix_syscall
	add.l	#4*4, a7

	move.l	#posix_exit_failure, -(a7)
	clr.l	-(a7)
	move.l	#posix_exit, d0
	trap	#posix_syscall

fail_msg:.asciz "Memory test failure\n"
succ_msg:.asciz "Memory test success\n"
