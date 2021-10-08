	.file	"sum.c"
	.text
	.globl	plus
	.type	plus, @function
plus:
.LFB0:
	.cfi_startproc
	leaq	(%rdi,%rsi), %rax
	ret
	.cfi_endproc
.LFE0:
	.size	plus, .-plus
	.globl	sumstore
	.type	sumstore, @function
sumstore:
.LFB1:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdx, %rbx
	call	plus
	movq	%rax, (%rbx)
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE1:
	.size	sumstore, .-sumstore
	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	movl	$0, %eax
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.5.0-6ubuntu2) 7.5.0"
	.section	.note.GNU-stack,"",@progbits
