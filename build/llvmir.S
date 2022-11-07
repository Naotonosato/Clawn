	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 17
	.globl	"_[GLOBAL]/Various#0"   ## -- Begin function [GLOBAL]/Various#0
	.p2align	4, 0x90
"_[GLOBAL]/Various#0":                  ## @"[GLOBAL]/Various#0"
	.cfi_startproc
## %bb.0:                               ## %"[GLOBAL]/Various#0"
	pushq	%r15
	.cfi_def_cfa_offset 16
	pushq	%r14
	.cfi_def_cfa_offset 24
	pushq	%r12
	.cfi_def_cfa_offset 32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	subq	$24, %rsp
	.cfi_def_cfa_offset 64
	.cfi_offset %rbx, -40
	.cfi_offset %r12, -32
	.cfi_offset %r14, -24
	.cfi_offset %r15, -16
	movq	%rsi, %r15
	movq	%rdi, %rbx
	callq	_get_unique_number
	movq	%rax, %r14
	movl	$128, %edi
	callq	_use_heap
	movq	%rax, %r12
	movq	%rbx, (%rax)
	movq	%r15, 8(%rax)
	movl	$1, %esi
	movq	%rbx, %rdi
	callq	*"_addr:op_equal_int"(%rip)
	movl	%eax, %ebx
	movq	%r14, %rdi
	callq	_register_calling_id_to_free
	movl	$128, %edi
	callq	_use_heap
	testb	$1, %bl
	jne	LBB0_1
## %bb.2:                               ## %else
	movq	(%r12), %rdi
	movl	$1, %esi
	callq	*"_addr:op_equal_int"(%rip)
	movl	%eax, %ebx
	movq	%r14, %rdi
	callq	_register_calling_id_to_free
	movl	$128, %edi
	callq	_use_heap
	testb	$1, %bl
	jne	LBB0_1
## %bb.3:                               ## %else2
	movq	(%r12), %rdi
	movl	$1, %esi
	callq	*"_addr:op_equal_int"(%rip)
	movl	%eax, %ebx
	movq	%r14, %rdi
	callq	_register_calling_id_to_free
	movl	$128, %edi
	callq	_use_heap
	testb	$1, %bl
	je	LBB0_4
LBB0_1:                                 ## %then
	movq	8(%r12), %rdi
	callq	*"_addr:[GLOBAL]/Various#0_1368"(%rip)
	movq	%rax, %rbx
	movq	%rdx, %r15
	callq	_register_calling_id
	movl	$192, %edi
	callq	_use_heap
	movq	$1, (%rax)
	movq	%r15, 16(%rax)
	movq	%rbx, 8(%rax)
	movq	%r14, %rdi
	callq	_free_heaps_associated_calling_id
	movl	$1, %eax
	movq	%rbx, %rdx
LBB0_5:                                 ## %else5
	movq	%r15, %rcx
	addq	$24, %rsp
	popq	%rbx
	popq	%r12
	popq	%r14
	popq	%r15
	retq
LBB0_4:                                 ## %else5
	movq	16(%rsp), %r15
	movq	(%rsp), %rbx
	movq	8(%rsp), %r12
	movq	%r14, %rdi
	callq	_free_heaps_associated_calling_id
	movq	%rbx, %rax
	movq	%r12, %rdx
	jmp	LBB0_5
	.cfi_endproc
                                        ## -- End function
	.globl	"_[GLOBAL]/Various#0_1368" ## -- Begin function [GLOBAL]/Various#0_1368
	.p2align	4, 0x90
"_[GLOBAL]/Various#0_1368":             ## @"[GLOBAL]/Various#0_1368"
	.cfi_startproc
## %bb.0:                               ## %"[GLOBAL]/Various#0_1368"
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$32, %rsp
	.cfi_def_cfa_offset 48
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	callq	_get_unique_number
	movq	%rbx, 24(%rsp)
	movq	%rbx, 16(%rsp)
	movq	$1, 8(%rsp)
	movq	%rax, %rdi
	callq	_free_heaps_associated_calling_id
	movl	$1, %eax
	movq	%rbx, %rdx
	addq	$32, %rsp
	popq	%rbx
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	"_[GLOBAL]/Various#1"   ## -- Begin function [GLOBAL]/Various#1
	.p2align	4, 0x90
"_[GLOBAL]/Various#1":                  ## @"[GLOBAL]/Various#1"
	.cfi_startproc
## %bb.0:                               ## %"[GLOBAL]/Various#1"
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%r15
	.cfi_def_cfa_offset 24
	pushq	%r14
	.cfi_def_cfa_offset 32
	pushq	%r12
	.cfi_def_cfa_offset 40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	subq	$32, %rsp
	.cfi_def_cfa_offset 80
	.cfi_offset %rbx, -48
	.cfi_offset %r12, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	.cfi_offset %rbp, -16
	movq	%rsi, %r15
	movq	%rdi, %r12
	callq	_get_unique_number
	movq	%rax, %r14
	movl	$128, %edi
	callq	_use_heap
	movq	%rax, %rbx
	movq	%r12, (%rax)
	movq	%r15, 8(%rax)
	movl	$1, %esi
	movq	%r12, %rdi
	callq	*"_addr:op_equal_int"(%rip)
	movl	%eax, %ebp
	movq	%r14, %rdi
	callq	_register_calling_id_to_free
	movl	$128, %edi
	callq	_use_heap
	testb	$1, %bpl
	je	LBB2_2
## %bb.1:                               ## %then
	movq	8(%rbx), %rdi
	callq	*"_addr:[GLOBAL]/Various#1_1435"(%rip)
	movq	%rax, %rbx
	movq	%rdx, %rbp
	callq	_register_calling_id
	movl	$192, %edi
	callq	_use_heap
	movq	$1, (%rax)
	movq	%rbp, 16(%rax)
	movq	%rbx, 8(%rax)
	movq	%r14, %rdi
	callq	_free_heaps_associated_calling_id
	movl	$1, %eax
	movq	%rbx, %rdx
	movq	%rbp, %rcx
	jmp	LBB2_3
LBB2_2:                                 ## %else
	movq	24(%rsp), %r15
	movq	8(%rsp), %rbp
	movq	16(%rsp), %rbx
	movq	%r14, %rdi
	callq	_free_heaps_associated_calling_id
	movq	%rbp, %rax
	movq	%rbx, %rdx
	movq	%r15, %rcx
LBB2_3:                                 ## %else
	addq	$32, %rsp
	popq	%rbx
	popq	%r12
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	"_[GLOBAL]/Various#1_1435" ## -- Begin function [GLOBAL]/Various#1_1435
	.p2align	4, 0x90
"_[GLOBAL]/Various#1_1435":             ## @"[GLOBAL]/Various#1_1435"
	.cfi_startproc
## %bb.0:                               ## %"[GLOBAL]/Various#1_1435"
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$32, %rsp
	.cfi_def_cfa_offset 48
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	callq	_get_unique_number
	movq	%rbx, 24(%rsp)
	movq	%rbx, 16(%rsp)
	movq	$2, 8(%rsp)
	movq	%rax, %rdi
	callq	_free_heaps_associated_calling_id
	movl	$2, %eax
	movq	%rbx, %rdx
	addq	$32, %rsp
	popq	%rbx
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	"_[GLOBAL]/Various#2"   ## -- Begin function [GLOBAL]/Various#2
	.p2align	4, 0x90
"_[GLOBAL]/Various#2":                  ## @"[GLOBAL]/Various#2"
	.cfi_startproc
## %bb.0:                               ## %"[GLOBAL]/Various#2"
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%r15
	.cfi_def_cfa_offset 24
	pushq	%r14
	.cfi_def_cfa_offset 32
	pushq	%r12
	.cfi_def_cfa_offset 40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	subq	$32, %rsp
	.cfi_def_cfa_offset 80
	.cfi_offset %rbx, -48
	.cfi_offset %r12, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	.cfi_offset %rbp, -16
	movq	%rsi, %r15
	movq	%rdi, %r12
	callq	_get_unique_number
	movq	%rax, %r14
	movl	$128, %edi
	callq	_use_heap
	movq	%rax, %rbx
	movq	%r12, (%rax)
	movq	%r15, 8(%rax)
	movl	$1, %esi
	movq	%r12, %rdi
	callq	*"_addr:op_equal_int"(%rip)
	movl	%eax, %ebp
	movq	%r14, %rdi
	callq	_register_calling_id_to_free
	movl	$128, %edi
	callq	_use_heap
	testb	$1, %bpl
	je	LBB4_2
## %bb.1:                               ## %then
	movq	8(%rbx), %rdi
	callq	*"_addr:[GLOBAL]/Various#2_1502"(%rip)
	movq	%rax, %rbx
	movq	%rdx, %rbp
	callq	_register_calling_id
	movl	$192, %edi
	callq	_use_heap
	movq	$1, (%rax)
	movq	%rbp, 16(%rax)
	movq	%rbx, 8(%rax)
	movq	%r14, %rdi
	callq	_free_heaps_associated_calling_id
	movl	$1, %eax
	movq	%rbx, %rdx
	movq	%rbp, %rcx
	jmp	LBB4_3
LBB4_2:                                 ## %else
	movq	24(%rsp), %r15
	movq	8(%rsp), %rbp
	movq	16(%rsp), %rbx
	movq	%r14, %rdi
	callq	_free_heaps_associated_calling_id
	movq	%rbp, %rax
	movq	%rbx, %rdx
	movq	%r15, %rcx
LBB4_3:                                 ## %else
	addq	$32, %rsp
	popq	%rbx
	popq	%r12
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	"_[GLOBAL]/Various#2_1502" ## -- Begin function [GLOBAL]/Various#2_1502
	.p2align	4, 0x90
"_[GLOBAL]/Various#2_1502":             ## @"[GLOBAL]/Various#2_1502"
	.cfi_startproc
## %bb.0:                               ## %"[GLOBAL]/Various#2_1502"
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$32, %rsp
	.cfi_def_cfa_offset 48
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	callq	_get_unique_number
	movq	%rbx, 24(%rsp)
	movq	%rbx, 16(%rsp)
	movq	$3, 8(%rsp)
	movq	%rax, %rdi
	callq	_free_heaps_associated_calling_id
	movl	$3, %eax
	movq	%rbx, %rdx
	addq	$32, %rsp
	popq	%rbx
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	"_[GLOBAL]/Various#3"   ## -- Begin function [GLOBAL]/Various#3
	.p2align	4, 0x90
"_[GLOBAL]/Various#3":                  ## @"[GLOBAL]/Various#3"
	.cfi_startproc
## %bb.0:                               ## %"[GLOBAL]/Various#3"
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	callq	_get_unique_number
	movq	$0, 8(%rsp)
	movq	%rax, %rdi
	callq	_free_heaps_associated_calling_id
	addq	$24, %rsp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	"_[GLOBAL]/Various#4"   ## -- Begin function [GLOBAL]/Various#4
	.p2align	4, 0x90
"_[GLOBAL]/Various#4":                  ## @"[GLOBAL]/Various#4"
	.cfi_startproc
## %bb.0:                               ## %"[GLOBAL]/Various#4"
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	callq	_get_unique_number
	movq	$0, 8(%rsp)
	movq	%rax, %rdi
	callq	_free_heaps_associated_calling_id
	addq	$24, %rsp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	"_[GLOBAL]/Various#5"   ## -- Begin function [GLOBAL]/Various#5
	.p2align	4, 0x90
"_[GLOBAL]/Various#5":                  ## @"[GLOBAL]/Various#5"
	.cfi_startproc
## %bb.0:                               ## %"[GLOBAL]/Various#5"
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	callq	_get_unique_number
	movq	$0, 8(%rsp)
	movq	%rax, %rdi
	callq	_free_heaps_associated_calling_id
	addq	$24, %rsp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	"_[GLOBAL]/Various#6"   ## -- Begin function [GLOBAL]/Various#6
	.p2align	4, 0x90
"_[GLOBAL]/Various#6":                  ## @"[GLOBAL]/Various#6"
	.cfi_startproc
## %bb.0:                               ## %"[GLOBAL]/Various#6"
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	callq	_get_unique_number
	movq	$0, 8(%rsp)
	movq	%rax, %rdi
	callq	_free_heaps_associated_calling_id
	addq	$24, %rsp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	"_[GLOBAL]/Various#7"   ## -- Begin function [GLOBAL]/Various#7
	.p2align	4, 0x90
"_[GLOBAL]/Various#7":                  ## @"[GLOBAL]/Various#7"
	.cfi_startproc
## %bb.0:                               ## %"[GLOBAL]/Various#7"
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	callq	_get_unique_number
	movq	$0, 8(%rsp)
	movq	%rax, %rdi
	callq	_free_heaps_associated_calling_id
	addq	$24, %rsp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	"_[GLOBAL]/Various#8"   ## -- Begin function [GLOBAL]/Various#8
	.p2align	4, 0x90
"_[GLOBAL]/Various#8":                  ## @"[GLOBAL]/Various#8"
	.cfi_startproc
## %bb.0:                               ## %"[GLOBAL]/Various#8"
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	callq	_get_unique_number
	movq	$0, 8(%rsp)
	movq	%rax, %rdi
	callq	_free_heaps_associated_calling_id
	addq	$24, %rsp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	"_[GLOBAL]/Various#9"   ## -- Begin function [GLOBAL]/Various#9
	.p2align	4, 0x90
"_[GLOBAL]/Various#9":                  ## @"[GLOBAL]/Various#9"
	.cfi_startproc
## %bb.0:                               ## %"[GLOBAL]/Various#9"
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	callq	_get_unique_number
	movq	$0, 8(%rsp)
	movq	%rax, %rdi
	callq	_free_heaps_associated_calling_id
	addq	$24, %rsp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	"_[GLOBAL]/constructor@S" ## -- Begin function [GLOBAL]/constructor@S
	.p2align	4, 0x90
"_[GLOBAL]/constructor@S":              ## @"[GLOBAL]/constructor@S"
	.cfi_startproc
## %bb.0:                               ## %"[GLOBAL]/constructor@S"
	pushq	%r15
	.cfi_def_cfa_offset 16
	pushq	%r14
	.cfi_def_cfa_offset 24
	pushq	%r12
	.cfi_def_cfa_offset 32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	subq	$24, %rsp
	.cfi_def_cfa_offset 64
	.cfi_offset %rbx, -40
	.cfi_offset %r12, -32
	.cfi_offset %r14, -24
	.cfi_offset %r15, -16
	movq	%rsi, %r15
	movq	%rdi, %rbx
	callq	_get_unique_number
	movq	%rax, %r14
	movl	$128, %edi
	callq	_use_heap
	movq	%rax, %r12
	movq	%rbx, (%rax)
	movq	%r15, 8(%rax)
	movl	$1, %esi
	movq	%rbx, %rdi
	callq	*"_addr:op_equal_int"(%rip)
	movl	%eax, %ebx
	movq	%r14, %rdi
	callq	_register_calling_id_to_free
	movl	$64, %edi
	callq	_use_heap
	testb	$1, %bl
	jne	LBB13_1
## %bb.2:                               ## %else
	movq	(%r12), %rdi
	movl	$1, %esi
	callq	*"_addr:op_equal_int"(%rip)
	movl	%eax, %ebx
	movq	%r14, %rdi
	callq	_register_calling_id_to_free
	movl	$64, %edi
	callq	_use_heap
	testb	$1, %bl
	je	LBB13_3
LBB13_1:                                ## %then
	movq	8(%r12), %rdi
	callq	*"_addr:[GLOBAL]/constructor@S_1300"(%rip)
	movq	%rax, %rbx
	callq	_register_calling_id
	movl	$128, %edi
	callq	_use_heap
	movq	$1, (%rax)
	movq	%rbx, 8(%rax)
	movq	%r14, %rdi
	callq	_free_heaps_associated_calling_id
	movl	$1, %eax
	movq	%rbx, %rdx
	jmp	LBB13_4
LBB13_3:                                ## %else2
	movq	8(%rsp), %rbx
	movq	16(%rsp), %r15
	movq	%r14, %rdi
	callq	_free_heaps_associated_calling_id
	movq	%rbx, %rax
	movq	%r15, %rdx
LBB13_4:                                ## %else2
	addq	$24, %rsp
	popq	%rbx
	popq	%r12
	popq	%r14
	popq	%r15
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	"_[GLOBAL]/constructor@S_1300" ## -- Begin function [GLOBAL]/constructor@S_1300
	.p2align	4, 0x90
"_[GLOBAL]/constructor@S_1300":         ## @"[GLOBAL]/constructor@S_1300"
	.cfi_startproc
## %bb.0:                               ## %"[GLOBAL]/constructor@S_1300"
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	callq	_get_unique_number
	movq	%rbx, 8(%rsp)
	movq	%rbx, (%rsp)
	movq	%rax, %rdi
	callq	_free_heaps_associated_calling_id
	movq	%rbx, %rax
	addq	$16, %rsp
	popq	%rbx
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	"_[GLOBAL]/f"           ## -- Begin function [GLOBAL]/f
	.p2align	4, 0x90
"_[GLOBAL]/f":                          ## @"[GLOBAL]/f"
	.cfi_startproc
## %bb.0:                               ## %"[GLOBAL]/f"
	pushq	%r15
	.cfi_def_cfa_offset 16
	pushq	%r14
	.cfi_def_cfa_offset 24
	pushq	%r12
	.cfi_def_cfa_offset 32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	subq	$24, %rsp
	.cfi_def_cfa_offset 64
	.cfi_offset %rbx, -40
	.cfi_offset %r12, -32
	.cfi_offset %r14, -24
	.cfi_offset %r15, -16
	movq	%rsi, %r15
	movq	%rdi, %rbx
	callq	_get_unique_number
	movq	%rax, %r14
	movl	$128, %edi
	callq	_use_heap
	movq	%rax, %r12
	movq	%rbx, (%rax)
	movq	%r15, 8(%rax)
	movl	$1, %esi
	movq	%rbx, %rdi
	callq	*"_addr:op_equal_int"(%rip)
	movl	%eax, %ebx
	movq	%r14, %rdi
	callq	_register_calling_id_to_free
	movl	$64, %edi
	callq	_use_heap
	testb	$1, %bl
	jne	LBB15_1
## %bb.2:                               ## %else
	movq	(%r12), %rdi
	movl	$1, %esi
	callq	*"_addr:op_equal_int"(%rip)
	movl	%eax, %ebx
	movq	%r14, %rdi
	callq	_register_calling_id_to_free
	movl	$64, %edi
	callq	_use_heap
	testb	$1, %bl
	je	LBB15_3
LBB15_1:                                ## %then
	movq	8(%r12), %rdi
	callq	*"_addr:[GLOBAL]/f_1621"(%rip)
	movq	%rax, %rbx
	callq	_register_calling_id
	movl	$128, %edi
	callq	_use_heap
	movq	$1, (%rax)
	movq	%rbx, 8(%rax)
	movq	%r14, %rdi
	callq	_free_heaps_associated_calling_id
	movl	$1, %eax
	movq	%rbx, %rdx
	jmp	LBB15_4
LBB15_3:                                ## %else2
	movq	8(%rsp), %rbx
	movq	16(%rsp), %r15
	movq	%r14, %rdi
	callq	_free_heaps_associated_calling_id
	movq	%rbx, %rax
	movq	%r15, %rdx
LBB15_4:                                ## %else2
	addq	$24, %rsp
	popq	%rbx
	popq	%r12
	popq	%r14
	popq	%r15
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	"_[GLOBAL]/f_1621"      ## -- Begin function [GLOBAL]/f_1621
	.p2align	4, 0x90
"_[GLOBAL]/f_1621":                     ## @"[GLOBAL]/f_1621"
	.cfi_startproc
## %bb.0:                               ## %"[GLOBAL]/f_1621"
	pushq	%r14
	.cfi_def_cfa_offset 16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	subq	$24, %rsp
	.cfi_def_cfa_offset 48
	.cfi_offset %rbx, -24
	.cfi_offset %r14, -16
	movq	%rdi, %r14
	callq	_get_unique_number
	movq	%rax, %rbx
	movq	%r14, 8(%rsp)
	leaq	L___unnamed_1(%rip), %rdi
	movl	$1, %esi
	callq	*"_addr:string_constructor"(%rip)
	movq	%rax, %r14
	movq	%rbx, %rdi
	callq	_register_calling_id_to_free
	movq	%r14, %rdi
	callq	*"_addr:[GLOBAL]/print"(%rip)
	movq	%rbx, %rdi
	callq	_register_calling_id_to_free
	movq	8(%rsp), %rdi
	movl	$10, %esi
	callq	*"_addr:add_int"(%rip)
	movq	%rax, %r14
	callq	_register_calling_id
	movq	%rbx, %rdi
	callq	_free_heaps_associated_calling_id
	movq	%r14, %rax
	addq	$24, %rsp
	popq	%rbx
	popq	%r14
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	"_[GLOBAL]/g"           ## -- Begin function [GLOBAL]/g
	.p2align	4, 0x90
"_[GLOBAL]/g":                          ## @"[GLOBAL]/g"
	.cfi_startproc
## %bb.0:                               ## %"[GLOBAL]/g"
	pushq	%r15
	.cfi_def_cfa_offset 16
	pushq	%r14
	.cfi_def_cfa_offset 24
	pushq	%r12
	.cfi_def_cfa_offset 32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	subq	$24, %rsp
	.cfi_def_cfa_offset 64
	.cfi_offset %rbx, -40
	.cfi_offset %r12, -32
	.cfi_offset %r14, -24
	.cfi_offset %r15, -16
	movq	%rsi, %r15
	movq	%rdi, %rbx
	callq	_get_unique_number
	movq	%rax, %r14
	movl	$128, %edi
	callq	_use_heap
	movq	%rax, %r12
	movq	%rbx, (%rax)
	movq	%r15, 8(%rax)
	movl	$1, %esi
	movq	%rbx, %rdi
	callq	*"_addr:op_equal_int"(%rip)
	movl	%eax, %ebx
	movq	%r14, %rdi
	callq	_register_calling_id_to_free
	movl	$64, %edi
	callq	_use_heap
	testb	$1, %bl
	jne	LBB17_1
## %bb.2:                               ## %else
	movq	(%r12), %rdi
	movl	$1, %esi
	callq	*"_addr:op_equal_int"(%rip)
	movl	%eax, %ebx
	movq	%r14, %rdi
	callq	_register_calling_id_to_free
	movl	$64, %edi
	callq	_use_heap
	testb	$1, %bl
	je	LBB17_3
LBB17_1:                                ## %then
	movq	8(%r12), %rdi
	callq	*"_addr:[GLOBAL]/g_1792"(%rip)
	movq	%rax, %rbx
	callq	_register_calling_id
	movl	$128, %edi
	callq	_use_heap
	movq	$1, (%rax)
	movq	%rbx, 8(%rax)
	movq	%r14, %rdi
	callq	_free_heaps_associated_calling_id
	movl	$1, %eax
	movq	%rbx, %rdx
	jmp	LBB17_4
LBB17_3:                                ## %else2
	movq	8(%rsp), %rbx
	movq	16(%rsp), %r15
	movq	%r14, %rdi
	callq	_free_heaps_associated_calling_id
	movq	%rbx, %rax
	movq	%r15, %rdx
LBB17_4:                                ## %else2
	addq	$24, %rsp
	popq	%rbx
	popq	%r12
	popq	%r14
	popq	%r15
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	"_[GLOBAL]/g_1792"      ## -- Begin function [GLOBAL]/g_1792
	.p2align	4, 0x90
"_[GLOBAL]/g_1792":                     ## @"[GLOBAL]/g_1792"
	.cfi_startproc
## %bb.0:                               ## %"[GLOBAL]/g_1792"
	pushq	%r14
	.cfi_def_cfa_offset 16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	subq	$24, %rsp
	.cfi_def_cfa_offset 48
	.cfi_offset %rbx, -24
	.cfi_offset %r14, -16
	movq	%rdi, %r14
	callq	_get_unique_number
	movq	%rax, %rbx
	movq	%r14, 8(%rsp)
	leaq	L___unnamed_2(%rip), %rdi
	movl	$1, %esi
	callq	*"_addr:string_constructor"(%rip)
	movq	%rax, %r14
	movq	%rbx, %rdi
	callq	_register_calling_id_to_free
	movq	%r14, %rdi
	callq	*"_addr:[GLOBAL]/print"(%rip)
	movq	%rbx, %rdi
	callq	_register_calling_id_to_free
	movq	8(%rsp), %rdi
	movl	$10, %esi
	callq	*"_addr:sub_int"(%rip)
	movq	%rax, %r14
	callq	_register_calling_id
	movq	%rbx, %rdi
	callq	_free_heaps_associated_calling_id
	movq	%r14, %rax
	addq	$24, %rsp
	popq	%rbx
	popq	%r14
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:                               ## %"global:initialize"
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r15
	pushq	%r14
	pushq	%r12
	pushq	%rbx
	subq	$80, %rsp
	.cfi_offset %rbx, -48
	.cfi_offset %r12, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	callq	_get_unique_number
	movq	%rax, %r14
	leaq	"_[GLOBAL]/f"(%rip), %rax
	movq	%rax, "_addr:[GLOBAL]/f"(%rip)
	leaq	"_[GLOBAL]/g"(%rip), %rax
	movq	%rax, "_addr:[GLOBAL]/g"(%rip)
	leaq	"_[GLOBAL]/constructor@S"(%rip), %rax
	movq	%rax, "_addr:[GLOBAL]/constructor@S"(%rip)
	leaq	"_[GLOBAL]/Various#0"(%rip), %rax
	movq	%rax, "_addr:[GLOBAL]/Various#0"(%rip)
	leaq	"_[GLOBAL]/Various#1"(%rip), %rax
	movq	%rax, "_addr:[GLOBAL]/Various#1"(%rip)
	leaq	"_[GLOBAL]/Various#2"(%rip), %rax
	movq	%rax, "_addr:[GLOBAL]/Various#2"(%rip)
	leaq	"_[GLOBAL]/Various#3"(%rip), %rax
	movq	%rax, "_addr:[GLOBAL]/Various#3"(%rip)
	leaq	"_[GLOBAL]/Various#4"(%rip), %rax
	movq	%rax, "_addr:[GLOBAL]/Various#4"(%rip)
	leaq	"_[GLOBAL]/Various#5"(%rip), %rax
	movq	%rax, "_addr:[GLOBAL]/Various#5"(%rip)
	leaq	"_[GLOBAL]/Various#6"(%rip), %rax
	movq	%rax, "_addr:[GLOBAL]/Various#6"(%rip)
	leaq	"_[GLOBAL]/Various#7"(%rip), %rax
	movq	%rax, "_addr:[GLOBAL]/Various#7"(%rip)
	leaq	"_[GLOBAL]/Various#8"(%rip), %rax
	movq	%rax, "_addr:[GLOBAL]/Various#8"(%rip)
	leaq	"_[GLOBAL]/Various#9"(%rip), %rax
	movq	%rax, "_addr:[GLOBAL]/Various#9"(%rip)
	leaq	"_[GLOBAL]/constructor@S_1300"(%rip), %rax
	movq	%rax, "_addr:[GLOBAL]/constructor@S_1300"(%rip)
	leaq	"_[GLOBAL]/Various#0_1368"(%rip), %rax
	movq	%rax, "_addr:[GLOBAL]/Various#0_1368"(%rip)
	leaq	"_[GLOBAL]/Various#1_1435"(%rip), %rax
	movq	%rax, "_addr:[GLOBAL]/Various#1_1435"(%rip)
	leaq	"_[GLOBAL]/Various#2_1502"(%rip), %rax
	movq	%rax, "_addr:[GLOBAL]/Various#2_1502"(%rip)
	leaq	"_[GLOBAL]/f_1621"(%rip), %rax
	movq	%rax, "_addr:[GLOBAL]/f_1621"(%rip)
	leaq	"_[GLOBAL]/g_1792"(%rip), %rax
	movq	%rax, "_addr:[GLOBAL]/g_1792"(%rip)
	movq	_get_unique_number@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:get_unique_number"(%rip)
	movq	%rax, "_addr:[GLOBAL]/get_unique_number"(%rip)
	movq	_use_heap@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:use_heap"(%rip)
	movq	%rax, "_addr:[GLOBAL]/use_heap"(%rip)
	movq	_register_calling_id@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:register_calling_id"(%rip)
	movq	%rax, "_addr:[GLOBAL]/register_calling_id"(%rip)
	movq	_register_calling_id_to_free@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:register_calling_id_to_free"(%rip)
	movq	%rax, "_addr:[GLOBAL]/register_calling_id_to_free"(%rip)
	movq	_free_heaps_associated_calling_id@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:free_heaps_associated_calling_id"(%rip)
	movq	%rax, "_addr:[GLOBAL]/free_heaps_associated_calling_id"(%rip)
	movq	_free_heaps@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:free_heaps"(%rip)
	movq	%rax, "_addr:[GLOBAL]/free_heaps"(%rip)
	movq	_malloc@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:malloc"(%rip)
	movq	%rax, "_addr:[GLOBAL]/malloc"(%rip)
	movq	_memcpy@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:memcpy"(%rip)
	movq	%rax, "_addr:[GLOBAL]/memcpy"(%rip)
	movq	_exit@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:exit"(%rip)
	movq	%rax, "_addr:[GLOBAL]/exit"(%rip)
	movq	_printf@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:printf"(%rip)
	movq	%rax, "_addr:[GLOBAL]/printf"(%rip)
	movq	_snprintf@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:snprintf"(%rip)
	movq	%rax, "_addr:[GLOBAL]/snprintf"(%rip)
	movq	_realloc@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:realloc"(%rip)
	movq	%rax, "_addr:[GLOBAL]/realloc"(%rip)
	movq	_puts@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:puts"(%rip)
	movq	%rax, "_addr:[GLOBAL]/puts"(%rip)
	movq	_strcat@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:strcat"(%rip)
	movq	%rax, "_addr:[GLOBAL]/strcat"(%rip)
	movq	_strcmp@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:strcmp"(%rip)
	movq	%rax, "_addr:[GLOBAL]/strcmp"(%rip)
	movq	_clawn_realloc@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:clawn_realloc"(%rip)
	movq	%rax, "_addr:[GLOBAL]/clawn_realloc"(%rip)
	movq	_clawn_memcpy@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:clawn_memcpy"(%rip)
	movq	%rax, "_addr:[GLOBAL]/clawn_memcpy"(%rip)
	movq	_clawn_index@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:clawn_index"(%rip)
	movq	%rax, "_addr:[GLOBAL]/clawn_index"(%rip)
	movq	_clawn_set_element@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:clawn_set_element"(%rip)
	movq	%rax, "_addr:[GLOBAL]/clawn_set_element"(%rip)
	movq	_list_constructor@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:list_constructor"(%rip)
	movq	%rax, "_addr:[GLOBAL]/list_constructor"(%rip)
	movq	_append@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:append"(%rip)
	movq	%rax, "_addr:[GLOBAL]/append"(%rip)
	movq	_get@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:get"(%rip)
	movq	%rax, "_addr:[GLOBAL]/get"(%rip)
	movq	_print@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:print"(%rip)
	movq	%rax, "_addr:[GLOBAL]/print"(%rip)
	movq	_string_constructor@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:string_constructor"(%rip)
	movq	%rax, "_addr:[GLOBAL]/string_constructor"(%rip)
	movq	_input@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:input"(%rip)
	movq	%rax, "_addr:[GLOBAL]/input"(%rip)
	movq	_get_appended_string@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:get_appended_string"(%rip)
	movq	%rax, "_addr:[GLOBAL]/get_appended_string"(%rip)
	movq	_to_char_ptr@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:to_char_ptr"(%rip)
	movq	%rax, "_addr:[GLOBAL]/to_char_ptr"(%rip)
	movq	_append_string@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:append_string"(%rip)
	movq	%rax, "_addr:[GLOBAL]/append_string"(%rip)
	movq	_int_to_str@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:int_to_str"(%rip)
	movq	%rax, "_addr:[GLOBAL]/int_to_str"(%rip)
	movq	_float_to_str@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:float_to_str"(%rip)
	movq	%rax, "_addr:[GLOBAL]/float_to_str"(%rip)
	movq	_to_real_number@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:to_real_number"(%rip)
	movq	%rax, "_addr:[GLOBAL]/to_real_number"(%rip)
	movq	_to_integer@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:to_integer"(%rip)
	movq	%rax, "_addr:[GLOBAL]/to_integer"(%rip)
	movq	_op_and@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:op_and"(%rip)
	movq	%rax, "_addr:[GLOBAL]/op_and"(%rip)
	movq	_op_or@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:op_or"(%rip)
	movq	%rax, "_addr:[GLOBAL]/op_or"(%rip)
	movq	_op_equal_string@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:op_equal_string"(%rip)
	movq	%rax, "_addr:[GLOBAL]/op_equal_string"(%rip)
	movq	_add_int@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:add_int"(%rip)
	movq	%rax, "_addr:[GLOBAL]/add_int"(%rip)
	movq	_sub_int@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:sub_int"(%rip)
	movq	%rax, "_addr:[GLOBAL]/sub_int"(%rip)
	movq	_mul_int@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:mul_int"(%rip)
	movq	%rax, "_addr:[GLOBAL]/mul_int"(%rip)
	movq	_op_greater_than_int@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:op_greater_than_int"(%rip)
	movq	%rax, "_addr:[GLOBAL]/op_greater_than_int"(%rip)
	movq	_op_greater_than_or_equal_int@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:op_greater_than_or_equal_int"(%rip)
	movq	%rax, "_addr:[GLOBAL]/op_greater_than_or_equal_int"(%rip)
	movq	_op_less_than_int@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:op_less_than_int"(%rip)
	movq	%rax, "_addr:[GLOBAL]/op_less_than_int"(%rip)
	movq	_op_less_than_or_equal_int@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:op_less_than_or_equal_int"(%rip)
	movq	%rax, "_addr:[GLOBAL]/op_less_than_or_equal_int"(%rip)
	movq	_op_equal_int@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:op_equal_int"(%rip)
	movq	%rax, "_addr:[GLOBAL]/op_equal_int"(%rip)
	movq	_op_not_equal_int@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:op_not_equal_int"(%rip)
	movq	%rax, "_addr:[GLOBAL]/op_not_equal_int"(%rip)
	movq	_add_double@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:add_double"(%rip)
	movq	%rax, "_addr:[GLOBAL]/add_double"(%rip)
	movq	_sub_double@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:sub_double"(%rip)
	movq	%rax, "_addr:[GLOBAL]/sub_double"(%rip)
	movq	_mul_double@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:mul_double"(%rip)
	movq	%rax, "_addr:[GLOBAL]/mul_double"(%rip)
	movq	_div_double_double@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:div_double_double"(%rip)
	movq	%rax, "_addr:[GLOBAL]/div_double_double"(%rip)
	movq	_div_double_int@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:div_double_int"(%rip)
	movq	%rax, "_addr:[GLOBAL]/div_double_int"(%rip)
	movq	_div_int_double@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:div_int_double"(%rip)
	movq	%rax, "_addr:[GLOBAL]/div_int_double"(%rip)
	movq	_div_int_int@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:div_int_int"(%rip)
	movq	%rax, "_addr:[GLOBAL]/div_int_int"(%rip)
	movq	_op_greater_than_double@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:op_greater_than_double"(%rip)
	movq	%rax, "_addr:[GLOBAL]/op_greater_than_double"(%rip)
	movq	_op_greater_than_or_equal_double@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:op_greater_than_or_equal_double"(%rip)
	movq	%rax, "_addr:[GLOBAL]/op_greater_than_or_equal_double"(%rip)
	movq	_op_less_than_double@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:op_less_than_double"(%rip)
	movq	%rax, "_addr:[GLOBAL]/op_less_than_double"(%rip)
	movq	_op_less_than_or_equal_double@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:op_less_than_or_equal_double"(%rip)
	movq	%rax, "_addr:[GLOBAL]/op_less_than_or_equal_double"(%rip)
	movq	_op_equal_double@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:op_equal_double"(%rip)
	movq	%rax, "_addr:[GLOBAL]/op_equal_double"(%rip)
	movq	_op_not_equal_double@GOTPCREL(%rip), %rax
	movq	%rax, "_addr:op_not_equal_double"(%rip)
	movq	%rax, "_addr:[GLOBAL]/op_not_equal_double"(%rip)
	leaq	L___unnamed_3(%rip), %rdi
	movl	$3, %esi
	callq	*"_addr:string_constructor"(%rip)
	movq	%rax, %rbx
	movq	%r14, %rdi
	callq	_register_calling_id_to_free
	movq	%rbx, "_addr:[GLOBAL]/a"(%rip)
	leaq	L___unnamed_4(%rip), %rdi
	movl	$1, %esi
	callq	*"_addr:string_constructor"(%rip)
	movq	%rax, %rbx
	movq	%r14, %rdi
	callq	_register_calling_id_to_free
	movq	%rbx, -88(%rbp)
	movq	$1, -96(%rbp)
	movl	$1, %edi
	movq	%rbx, %rsi
	callq	*"_addr:[GLOBAL]/constructor@S"(%rip)
	movq	%rax, -80(%rbp)
	movq	%rdx, -72(%rbp)
	movq	%rdx, -104(%rbp)
	movq	$1, -112(%rbp)
	movl	$1, %edi
	movq	%rdx, %rsi
	callq	*"_addr:[GLOBAL]/Various#0"(%rip)
	movq	%rdx, -56(%rbp)
	movq	%rcx, -48(%rbp)
	movq	%rax, -64(%rbp)
	movq	%rdx, "_addr:[GLOBAL]/s"(%rip)
	movq	%rcx, "_addr:[GLOBAL]/s"+8(%rip)
	movq	%rdx, "_addr:[GLOBAL]/164"(%rip)
	movq	%rcx, "_addr:[GLOBAL]/164"+8(%rip)
	cmpq	$1, %rdx
	jne	LBB19_2
## %bb.1:                               ## %on_0
	movq	"_addr:[GLOBAL]/164"+8(%rip), %rax
	movq	%rax, -40(%rbp)
LBB19_2:                                ## %last_merge
	movq	-40(%rbp), %rdi
	movq	%rdi, "_addr:[GLOBAL]/str"(%rip)
	callq	*"_addr:[GLOBAL]/print"(%rip)
	movq	%r14, %rdi
	callq	_register_calling_id_to_free
	movl	$128, %edi
	callq	_use_heap
	movq	"_addr:[GLOBAL]/f"(%rip), %rsi
	movq	%rsi, 8(%rax)
	movq	$1, (%rax)
	movl	$1, %edi
	callq	*"_addr:[GLOBAL]/Various#1"(%rip)
	movq	%rax, %r15
	movq	%rdx, %rbx
	movq	%rcx, %r12
	callq	_register_calling_id
	movl	$192, %edi
	callq	_use_heap
	movq	%rbx, 8(%rax)
	movq	%r12, 16(%rax)
	movq	%r15, (%rax)
	movq	%rbx, "_addr:[GLOBAL]/fo"(%rip)
	movq	%r12, "_addr:[GLOBAL]/fo"+8(%rip)
	movl	$128, %edi
	callq	_use_heap
	movq	"_addr:[GLOBAL]/g"(%rip), %rsi
	movq	%rsi, 8(%rax)
	movq	$1, (%rax)
	movl	$1, %edi
	callq	*"_addr:[GLOBAL]/Various#2"(%rip)
	movq	%rax, %r15
	movq	%rdx, %rbx
	movq	%rcx, %r12
	callq	_register_calling_id
	movl	$192, %edi
	callq	_use_heap
	movq	%rbx, 8(%rax)
	movq	%r12, 16(%rax)
	movq	%r15, (%rax)
	movq	%rbx, "_addr:[GLOBAL]/fo"(%rip)
	movq	%r12, "_addr:[GLOBAL]/fo"+8(%rip)
	movq	%rsp, %r15
	addq	$-16, %r15
	movq	%r15, %rsp
	movq	$10, "_addr:[GLOBAL]/214"(%rip)
	movq	"_addr:[GLOBAL]/fo"+8(%rip), %rax
	movq	"_addr:[GLOBAL]/fo"(%rip), %rbx
	movq	%rbx, "_addr:[GLOBAL]/212"(%rip)
	movq	%rax, "_addr:[GLOBAL]/212"+8(%rip)
	movq	%rsp, %r12
	addq	$-16, %r12
	movq	%r12, %rsp
	movq	%rbx, %rdi
	callq	*"_addr:[GLOBAL]/int_to_str"(%rip)
	movq	%rax, %rdi
	callq	*"_addr:[GLOBAL]/print"(%rip)
	cmpq	$1, %rbx
	je	LBB19_4
## %bb.3:                               ## %merge1
	cmpq	$2, %rbx
	jne	LBB19_5
LBB19_4:                                ## %on_2
	movq	%rsp, %rax
	leaq	-16(%rax), %rsp
	movq	"_addr:[GLOBAL]/214"(%rip), %rsi
	movq	%rsi, -8(%rax)
	movq	$1, -16(%rax)
	movl	$1, %edi
	callq	*"_addr:[GLOBAL]/212"+8(%rip)
	movq	%rsp, %rcx
	leaq	-16(%rcx), %rsp
	movq	%rdx, -8(%rcx)
	movq	%rax, -16(%rcx)
	movq	%rdx, (%r12)
LBB19_5:                                ## %last_merge3
	movq	(%r12), %rdi
	movq	%rdi, (%r15)
	callq	*"_addr:[GLOBAL]/int_to_str"(%rip)
	movq	%rax, %rbx
	movq	%r14, %rdi
	callq	_register_calling_id_to_free
	movq	%rbx, %rdi
	callq	*"_addr:[GLOBAL]/print"(%rip)
	movq	%r14, %rdi
	callq	_register_calling_id_to_free
	movq	%r14, %rdi
	callq	_free_heaps_associated_calling_id
	xorl	%eax, %eax
	leaq	-32(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	"_addr:op_equal_int"    ## @"addr:op_equal_int"
.zerofill __DATA,__common,"_addr:op_equal_int",8,3
	.globl	"_addr:[GLOBAL]/Various#0_1368" ## @"addr:[GLOBAL]/Various#0_1368"
.zerofill __DATA,__common,"_addr:[GLOBAL]/Various#0_1368",8,3
	.globl	"_addr:[GLOBAL]/Various#1_1435" ## @"addr:[GLOBAL]/Various#1_1435"
.zerofill __DATA,__common,"_addr:[GLOBAL]/Various#1_1435",8,3
	.globl	"_addr:[GLOBAL]/Various#2_1502" ## @"addr:[GLOBAL]/Various#2_1502"
.zerofill __DATA,__common,"_addr:[GLOBAL]/Various#2_1502",8,3
	.globl	"_addr:[GLOBAL]/constructor@S_1300" ## @"addr:[GLOBAL]/constructor@S_1300"
.zerofill __DATA,__common,"_addr:[GLOBAL]/constructor@S_1300",8,3
	.globl	"_addr:[GLOBAL]/f_1621" ## @"addr:[GLOBAL]/f_1621"
.zerofill __DATA,__common,"_addr:[GLOBAL]/f_1621",8,3
	.globl	"_addr:string_constructor" ## @"addr:string_constructor"
.zerofill __DATA,__common,"_addr:string_constructor",8,3
	.section	__TEXT,__cstring,cstring_literals
L___unnamed_1:                          ## @0
	.asciz	"f"

	.globl	"_addr:[GLOBAL]/print"  ## @"addr:[GLOBAL]/print"
.zerofill __DATA,__common,"_addr:[GLOBAL]/print",8,3
	.globl	"_addr:add_int"         ## @"addr:add_int"
.zerofill __DATA,__common,"_addr:add_int",8,3
	.globl	"_addr:[GLOBAL]/g_1792" ## @"addr:[GLOBAL]/g_1792"
.zerofill __DATA,__common,"_addr:[GLOBAL]/g_1792",8,3
L___unnamed_2:                          ## @1
	.asciz	"g"

	.globl	"_addr:sub_int"         ## @"addr:sub_int"
.zerofill __DATA,__common,"_addr:sub_int",8,3
	.globl	"_addr:get_unique_number" ## @"addr:get_unique_number"
.zerofill __DATA,__common,"_addr:get_unique_number",8,3
	.globl	"_addr:use_heap"        ## @"addr:use_heap"
.zerofill __DATA,__common,"_addr:use_heap",8,3
	.globl	"_addr:register_calling_id" ## @"addr:register_calling_id"
.zerofill __DATA,__common,"_addr:register_calling_id",8,3
	.globl	"_addr:register_calling_id_to_free" ## @"addr:register_calling_id_to_free"
.zerofill __DATA,__common,"_addr:register_calling_id_to_free",8,3
	.globl	"_addr:free_heaps_associated_calling_id" ## @"addr:free_heaps_associated_calling_id"
.zerofill __DATA,__common,"_addr:free_heaps_associated_calling_id",8,3
	.globl	"_addr:free_heaps"      ## @"addr:free_heaps"
.zerofill __DATA,__common,"_addr:free_heaps",8,3
	.globl	"_addr:malloc"          ## @"addr:malloc"
.zerofill __DATA,__common,"_addr:malloc",8,3
	.globl	"_addr:memcpy"          ## @"addr:memcpy"
.zerofill __DATA,__common,"_addr:memcpy",8,3
	.globl	"_addr:exit"            ## @"addr:exit"
.zerofill __DATA,__common,"_addr:exit",8,3
	.globl	"_addr:printf"          ## @"addr:printf"
.zerofill __DATA,__common,"_addr:printf",8,3
	.globl	"_addr:snprintf"        ## @"addr:snprintf"
.zerofill __DATA,__common,"_addr:snprintf",8,3
	.globl	"_addr:realloc"         ## @"addr:realloc"
.zerofill __DATA,__common,"_addr:realloc",8,3
	.globl	"_addr:puts"            ## @"addr:puts"
.zerofill __DATA,__common,"_addr:puts",8,3
	.globl	"_addr:strcat"          ## @"addr:strcat"
.zerofill __DATA,__common,"_addr:strcat",8,3
	.globl	"_addr:strcmp"          ## @"addr:strcmp"
.zerofill __DATA,__common,"_addr:strcmp",8,3
	.globl	"_addr:clawn_realloc"   ## @"addr:clawn_realloc"
.zerofill __DATA,__common,"_addr:clawn_realloc",8,3
	.globl	"_addr:clawn_memcpy"    ## @"addr:clawn_memcpy"
.zerofill __DATA,__common,"_addr:clawn_memcpy",8,3
	.globl	"_addr:clawn_index"     ## @"addr:clawn_index"
.zerofill __DATA,__common,"_addr:clawn_index",8,3
	.globl	"_addr:clawn_set_element" ## @"addr:clawn_set_element"
.zerofill __DATA,__common,"_addr:clawn_set_element",8,3
	.globl	"_addr:list_constructor" ## @"addr:list_constructor"
.zerofill __DATA,__common,"_addr:list_constructor",8,3
	.globl	"_addr:append"          ## @"addr:append"
.zerofill __DATA,__common,"_addr:append",8,3
	.globl	"_addr:get"             ## @"addr:get"
.zerofill __DATA,__common,"_addr:get",8,3
	.globl	"_addr:print"           ## @"addr:print"
.zerofill __DATA,__common,"_addr:print",8,3
	.globl	"_addr:input"           ## @"addr:input"
.zerofill __DATA,__common,"_addr:input",8,3
	.globl	"_addr:get_appended_string" ## @"addr:get_appended_string"
.zerofill __DATA,__common,"_addr:get_appended_string",8,3
	.globl	"_addr:to_char_ptr"     ## @"addr:to_char_ptr"
.zerofill __DATA,__common,"_addr:to_char_ptr",8,3
	.globl	"_addr:append_string"   ## @"addr:append_string"
.zerofill __DATA,__common,"_addr:append_string",8,3
	.globl	"_addr:int_to_str"      ## @"addr:int_to_str"
.zerofill __DATA,__common,"_addr:int_to_str",8,3
	.globl	"_addr:float_to_str"    ## @"addr:float_to_str"
.zerofill __DATA,__common,"_addr:float_to_str",8,3
	.globl	"_addr:to_real_number"  ## @"addr:to_real_number"
.zerofill __DATA,__common,"_addr:to_real_number",8,3
	.globl	"_addr:to_integer"      ## @"addr:to_integer"
.zerofill __DATA,__common,"_addr:to_integer",8,3
	.globl	"_addr:op_and"          ## @"addr:op_and"
.zerofill __DATA,__common,"_addr:op_and",8,3
	.globl	"_addr:op_or"           ## @"addr:op_or"
.zerofill __DATA,__common,"_addr:op_or",8,3
	.globl	"_addr:op_equal_string" ## @"addr:op_equal_string"
.zerofill __DATA,__common,"_addr:op_equal_string",8,3
	.globl	"_addr:mul_int"         ## @"addr:mul_int"
.zerofill __DATA,__common,"_addr:mul_int",8,3
	.globl	"_addr:op_greater_than_int" ## @"addr:op_greater_than_int"
.zerofill __DATA,__common,"_addr:op_greater_than_int",8,3
	.globl	"_addr:op_greater_than_or_equal_int" ## @"addr:op_greater_than_or_equal_int"
.zerofill __DATA,__common,"_addr:op_greater_than_or_equal_int",8,3
	.globl	"_addr:op_less_than_int" ## @"addr:op_less_than_int"
.zerofill __DATA,__common,"_addr:op_less_than_int",8,3
	.globl	"_addr:op_less_than_or_equal_int" ## @"addr:op_less_than_or_equal_int"
.zerofill __DATA,__common,"_addr:op_less_than_or_equal_int",8,3
	.globl	"_addr:op_not_equal_int" ## @"addr:op_not_equal_int"
.zerofill __DATA,__common,"_addr:op_not_equal_int",8,3
	.globl	"_addr:add_double"      ## @"addr:add_double"
.zerofill __DATA,__common,"_addr:add_double",8,3
	.globl	"_addr:sub_double"      ## @"addr:sub_double"
.zerofill __DATA,__common,"_addr:sub_double",8,3
	.globl	"_addr:mul_double"      ## @"addr:mul_double"
.zerofill __DATA,__common,"_addr:mul_double",8,3
	.globl	"_addr:div_double_double" ## @"addr:div_double_double"
.zerofill __DATA,__common,"_addr:div_double_double",8,3
	.globl	"_addr:div_double_int"  ## @"addr:div_double_int"
.zerofill __DATA,__common,"_addr:div_double_int",8,3
	.globl	"_addr:div_int_double"  ## @"addr:div_int_double"
.zerofill __DATA,__common,"_addr:div_int_double",8,3
	.globl	"_addr:div_int_int"     ## @"addr:div_int_int"
.zerofill __DATA,__common,"_addr:div_int_int",8,3
	.globl	"_addr:op_greater_than_double" ## @"addr:op_greater_than_double"
.zerofill __DATA,__common,"_addr:op_greater_than_double",8,3
	.globl	"_addr:op_greater_than_or_equal_double" ## @"addr:op_greater_than_or_equal_double"
.zerofill __DATA,__common,"_addr:op_greater_than_or_equal_double",8,3
	.globl	"_addr:op_less_than_double" ## @"addr:op_less_than_double"
.zerofill __DATA,__common,"_addr:op_less_than_double",8,3
	.globl	"_addr:op_less_than_or_equal_double" ## @"addr:op_less_than_or_equal_double"
.zerofill __DATA,__common,"_addr:op_less_than_or_equal_double",8,3
	.globl	"_addr:op_equal_double" ## @"addr:op_equal_double"
.zerofill __DATA,__common,"_addr:op_equal_double",8,3
	.globl	"_addr:op_not_equal_double" ## @"addr:op_not_equal_double"
.zerofill __DATA,__common,"_addr:op_not_equal_double",8,3
	.globl	"_addr:[GLOBAL]/f"      ## @"addr:[GLOBAL]/f"
.zerofill __DATA,__common,"_addr:[GLOBAL]/f",8,3
	.globl	"_addr:[GLOBAL]/g"      ## @"addr:[GLOBAL]/g"
.zerofill __DATA,__common,"_addr:[GLOBAL]/g",8,3
	.globl	"_addr:[GLOBAL]/constructor@S" ## @"addr:[GLOBAL]/constructor@S"
.zerofill __DATA,__common,"_addr:[GLOBAL]/constructor@S",8,3
	.globl	"_addr:[GLOBAL]/Various#0" ## @"addr:[GLOBAL]/Various#0"
.zerofill __DATA,__common,"_addr:[GLOBAL]/Various#0",8,3
	.globl	"_addr:[GLOBAL]/Various#1" ## @"addr:[GLOBAL]/Various#1"
.zerofill __DATA,__common,"_addr:[GLOBAL]/Various#1",8,3
	.globl	"_addr:[GLOBAL]/Various#2" ## @"addr:[GLOBAL]/Various#2"
.zerofill __DATA,__common,"_addr:[GLOBAL]/Various#2",8,3
	.globl	"_addr:[GLOBAL]/Various#3" ## @"addr:[GLOBAL]/Various#3"
.zerofill __DATA,__common,"_addr:[GLOBAL]/Various#3",8,3
	.globl	"_addr:[GLOBAL]/Various#4" ## @"addr:[GLOBAL]/Various#4"
.zerofill __DATA,__common,"_addr:[GLOBAL]/Various#4",8,3
	.globl	"_addr:[GLOBAL]/Various#5" ## @"addr:[GLOBAL]/Various#5"
.zerofill __DATA,__common,"_addr:[GLOBAL]/Various#5",8,3
	.globl	"_addr:[GLOBAL]/Various#6" ## @"addr:[GLOBAL]/Various#6"
.zerofill __DATA,__common,"_addr:[GLOBAL]/Various#6",8,3
	.globl	"_addr:[GLOBAL]/Various#7" ## @"addr:[GLOBAL]/Various#7"
.zerofill __DATA,__common,"_addr:[GLOBAL]/Various#7",8,3
	.globl	"_addr:[GLOBAL]/Various#8" ## @"addr:[GLOBAL]/Various#8"
.zerofill __DATA,__common,"_addr:[GLOBAL]/Various#8",8,3
	.globl	"_addr:[GLOBAL]/Various#9" ## @"addr:[GLOBAL]/Various#9"
.zerofill __DATA,__common,"_addr:[GLOBAL]/Various#9",8,3
	.globl	"_addr:[GLOBAL]/get_unique_number" ## @"addr:[GLOBAL]/get_unique_number"
.zerofill __DATA,__common,"_addr:[GLOBAL]/get_unique_number",8,3
	.globl	"_addr:[GLOBAL]/use_heap" ## @"addr:[GLOBAL]/use_heap"
.zerofill __DATA,__common,"_addr:[GLOBAL]/use_heap",8,3
	.globl	"_addr:[GLOBAL]/register_calling_id" ## @"addr:[GLOBAL]/register_calling_id"
.zerofill __DATA,__common,"_addr:[GLOBAL]/register_calling_id",8,3
	.globl	"_addr:[GLOBAL]/register_calling_id_to_free" ## @"addr:[GLOBAL]/register_calling_id_to_free"
.zerofill __DATA,__common,"_addr:[GLOBAL]/register_calling_id_to_free",8,3
	.globl	"_addr:[GLOBAL]/free_heaps_associated_calling_id" ## @"addr:[GLOBAL]/free_heaps_associated_calling_id"
.zerofill __DATA,__common,"_addr:[GLOBAL]/free_heaps_associated_calling_id",8,3
	.globl	"_addr:[GLOBAL]/free_heaps" ## @"addr:[GLOBAL]/free_heaps"
.zerofill __DATA,__common,"_addr:[GLOBAL]/free_heaps",8,3
	.globl	"_addr:[GLOBAL]/malloc" ## @"addr:[GLOBAL]/malloc"
.zerofill __DATA,__common,"_addr:[GLOBAL]/malloc",8,3
	.globl	"_addr:[GLOBAL]/memcpy" ## @"addr:[GLOBAL]/memcpy"
.zerofill __DATA,__common,"_addr:[GLOBAL]/memcpy",8,3
	.globl	"_addr:[GLOBAL]/exit"   ## @"addr:[GLOBAL]/exit"
.zerofill __DATA,__common,"_addr:[GLOBAL]/exit",8,3
	.globl	"_addr:[GLOBAL]/printf" ## @"addr:[GLOBAL]/printf"
.zerofill __DATA,__common,"_addr:[GLOBAL]/printf",8,3
	.globl	"_addr:[GLOBAL]/snprintf" ## @"addr:[GLOBAL]/snprintf"
.zerofill __DATA,__common,"_addr:[GLOBAL]/snprintf",8,3
	.globl	"_addr:[GLOBAL]/realloc" ## @"addr:[GLOBAL]/realloc"
.zerofill __DATA,__common,"_addr:[GLOBAL]/realloc",8,3
	.globl	"_addr:[GLOBAL]/puts"   ## @"addr:[GLOBAL]/puts"
.zerofill __DATA,__common,"_addr:[GLOBAL]/puts",8,3
	.globl	"_addr:[GLOBAL]/strcat" ## @"addr:[GLOBAL]/strcat"
.zerofill __DATA,__common,"_addr:[GLOBAL]/strcat",8,3
	.globl	"_addr:[GLOBAL]/strcmp" ## @"addr:[GLOBAL]/strcmp"
.zerofill __DATA,__common,"_addr:[GLOBAL]/strcmp",8,3
	.globl	"_addr:[GLOBAL]/clawn_realloc" ## @"addr:[GLOBAL]/clawn_realloc"
.zerofill __DATA,__common,"_addr:[GLOBAL]/clawn_realloc",8,3
	.globl	"_addr:[GLOBAL]/clawn_memcpy" ## @"addr:[GLOBAL]/clawn_memcpy"
.zerofill __DATA,__common,"_addr:[GLOBAL]/clawn_memcpy",8,3
	.globl	"_addr:[GLOBAL]/clawn_index" ## @"addr:[GLOBAL]/clawn_index"
.zerofill __DATA,__common,"_addr:[GLOBAL]/clawn_index",8,3
	.globl	"_addr:[GLOBAL]/clawn_set_element" ## @"addr:[GLOBAL]/clawn_set_element"
.zerofill __DATA,__common,"_addr:[GLOBAL]/clawn_set_element",8,3
	.globl	"_addr:[GLOBAL]/list_constructor" ## @"addr:[GLOBAL]/list_constructor"
.zerofill __DATA,__common,"_addr:[GLOBAL]/list_constructor",8,3
	.globl	"_addr:[GLOBAL]/append" ## @"addr:[GLOBAL]/append"
.zerofill __DATA,__common,"_addr:[GLOBAL]/append",8,3
	.globl	"_addr:[GLOBAL]/get"    ## @"addr:[GLOBAL]/get"
.zerofill __DATA,__common,"_addr:[GLOBAL]/get",8,3
	.globl	"_addr:[GLOBAL]/string_constructor" ## @"addr:[GLOBAL]/string_constructor"
.zerofill __DATA,__common,"_addr:[GLOBAL]/string_constructor",8,3
	.globl	"_addr:[GLOBAL]/input"  ## @"addr:[GLOBAL]/input"
.zerofill __DATA,__common,"_addr:[GLOBAL]/input",8,3
	.globl	"_addr:[GLOBAL]/get_appended_string" ## @"addr:[GLOBAL]/get_appended_string"
.zerofill __DATA,__common,"_addr:[GLOBAL]/get_appended_string",8,3
	.globl	"_addr:[GLOBAL]/to_char_ptr" ## @"addr:[GLOBAL]/to_char_ptr"
.zerofill __DATA,__common,"_addr:[GLOBAL]/to_char_ptr",8,3
	.globl	"_addr:[GLOBAL]/append_string" ## @"addr:[GLOBAL]/append_string"
.zerofill __DATA,__common,"_addr:[GLOBAL]/append_string",8,3
	.globl	"_addr:[GLOBAL]/int_to_str" ## @"addr:[GLOBAL]/int_to_str"
.zerofill __DATA,__common,"_addr:[GLOBAL]/int_to_str",8,3
	.globl	"_addr:[GLOBAL]/float_to_str" ## @"addr:[GLOBAL]/float_to_str"
.zerofill __DATA,__common,"_addr:[GLOBAL]/float_to_str",8,3
	.globl	"_addr:[GLOBAL]/to_real_number" ## @"addr:[GLOBAL]/to_real_number"
.zerofill __DATA,__common,"_addr:[GLOBAL]/to_real_number",8,3
	.globl	"_addr:[GLOBAL]/to_integer" ## @"addr:[GLOBAL]/to_integer"
.zerofill __DATA,__common,"_addr:[GLOBAL]/to_integer",8,3
	.globl	"_addr:[GLOBAL]/op_and" ## @"addr:[GLOBAL]/op_and"
.zerofill __DATA,__common,"_addr:[GLOBAL]/op_and",8,3
	.globl	"_addr:[GLOBAL]/op_or"  ## @"addr:[GLOBAL]/op_or"
.zerofill __DATA,__common,"_addr:[GLOBAL]/op_or",8,3
	.globl	"_addr:[GLOBAL]/op_equal_string" ## @"addr:[GLOBAL]/op_equal_string"
.zerofill __DATA,__common,"_addr:[GLOBAL]/op_equal_string",8,3
	.globl	"_addr:[GLOBAL]/add_int" ## @"addr:[GLOBAL]/add_int"
.zerofill __DATA,__common,"_addr:[GLOBAL]/add_int",8,3
	.globl	"_addr:[GLOBAL]/sub_int" ## @"addr:[GLOBAL]/sub_int"
.zerofill __DATA,__common,"_addr:[GLOBAL]/sub_int",8,3
	.globl	"_addr:[GLOBAL]/mul_int" ## @"addr:[GLOBAL]/mul_int"
.zerofill __DATA,__common,"_addr:[GLOBAL]/mul_int",8,3
	.globl	"_addr:[GLOBAL]/op_greater_than_int" ## @"addr:[GLOBAL]/op_greater_than_int"
.zerofill __DATA,__common,"_addr:[GLOBAL]/op_greater_than_int",8,3
	.globl	"_addr:[GLOBAL]/op_greater_than_or_equal_int" ## @"addr:[GLOBAL]/op_greater_than_or_equal_int"
.zerofill __DATA,__common,"_addr:[GLOBAL]/op_greater_than_or_equal_int",8,3
	.globl	"_addr:[GLOBAL]/op_less_than_int" ## @"addr:[GLOBAL]/op_less_than_int"
.zerofill __DATA,__common,"_addr:[GLOBAL]/op_less_than_int",8,3
	.globl	"_addr:[GLOBAL]/op_less_than_or_equal_int" ## @"addr:[GLOBAL]/op_less_than_or_equal_int"
.zerofill __DATA,__common,"_addr:[GLOBAL]/op_less_than_or_equal_int",8,3
	.globl	"_addr:[GLOBAL]/op_equal_int" ## @"addr:[GLOBAL]/op_equal_int"
.zerofill __DATA,__common,"_addr:[GLOBAL]/op_equal_int",8,3
	.globl	"_addr:[GLOBAL]/op_not_equal_int" ## @"addr:[GLOBAL]/op_not_equal_int"
.zerofill __DATA,__common,"_addr:[GLOBAL]/op_not_equal_int",8,3
	.globl	"_addr:[GLOBAL]/add_double" ## @"addr:[GLOBAL]/add_double"
.zerofill __DATA,__common,"_addr:[GLOBAL]/add_double",8,3
	.globl	"_addr:[GLOBAL]/sub_double" ## @"addr:[GLOBAL]/sub_double"
.zerofill __DATA,__common,"_addr:[GLOBAL]/sub_double",8,3
	.globl	"_addr:[GLOBAL]/mul_double" ## @"addr:[GLOBAL]/mul_double"
.zerofill __DATA,__common,"_addr:[GLOBAL]/mul_double",8,3
	.globl	"_addr:[GLOBAL]/div_double_double" ## @"addr:[GLOBAL]/div_double_double"
.zerofill __DATA,__common,"_addr:[GLOBAL]/div_double_double",8,3
	.globl	"_addr:[GLOBAL]/div_double_int" ## @"addr:[GLOBAL]/div_double_int"
.zerofill __DATA,__common,"_addr:[GLOBAL]/div_double_int",8,3
	.globl	"_addr:[GLOBAL]/div_int_double" ## @"addr:[GLOBAL]/div_int_double"
.zerofill __DATA,__common,"_addr:[GLOBAL]/div_int_double",8,3
	.globl	"_addr:[GLOBAL]/div_int_int" ## @"addr:[GLOBAL]/div_int_int"
.zerofill __DATA,__common,"_addr:[GLOBAL]/div_int_int",8,3
	.globl	"_addr:[GLOBAL]/op_greater_than_double" ## @"addr:[GLOBAL]/op_greater_than_double"
.zerofill __DATA,__common,"_addr:[GLOBAL]/op_greater_than_double",8,3
	.globl	"_addr:[GLOBAL]/op_greater_than_or_equal_double" ## @"addr:[GLOBAL]/op_greater_than_or_equal_double"
.zerofill __DATA,__common,"_addr:[GLOBAL]/op_greater_than_or_equal_double",8,3
	.globl	"_addr:[GLOBAL]/op_less_than_double" ## @"addr:[GLOBAL]/op_less_than_double"
.zerofill __DATA,__common,"_addr:[GLOBAL]/op_less_than_double",8,3
	.globl	"_addr:[GLOBAL]/op_less_than_or_equal_double" ## @"addr:[GLOBAL]/op_less_than_or_equal_double"
.zerofill __DATA,__common,"_addr:[GLOBAL]/op_less_than_or_equal_double",8,3
	.globl	"_addr:[GLOBAL]/op_equal_double" ## @"addr:[GLOBAL]/op_equal_double"
.zerofill __DATA,__common,"_addr:[GLOBAL]/op_equal_double",8,3
	.globl	"_addr:[GLOBAL]/op_not_equal_double" ## @"addr:[GLOBAL]/op_not_equal_double"
.zerofill __DATA,__common,"_addr:[GLOBAL]/op_not_equal_double",8,3
	.globl	"_addr:[GLOBAL]/a"      ## @"addr:[GLOBAL]/a"
.zerofill __DATA,__common,"_addr:[GLOBAL]/a",8,3
L___unnamed_3:                          ## @2
	.asciz	"aaa"

	.globl	"_addr:[GLOBAL]/s"      ## @"addr:[GLOBAL]/s"
.zerofill __DATA,__common,"_addr:[GLOBAL]/s",16,3
L___unnamed_4:                          ## @3
	.asciz	"x"

	.globl	"_addr:[GLOBAL]/str"    ## @"addr:[GLOBAL]/str"
.zerofill __DATA,__common,"_addr:[GLOBAL]/str",8,3
	.globl	"_addr:[GLOBAL]/164"    ## @"addr:[GLOBAL]/164"
.zerofill __DATA,__common,"_addr:[GLOBAL]/164",16,3
	.globl	"_addr:[GLOBAL]/fo"     ## @"addr:[GLOBAL]/fo"
.zerofill __DATA,__common,"_addr:[GLOBAL]/fo",16,3
	.globl	"_addr:[GLOBAL]/214"    ## @"addr:[GLOBAL]/214"
.zerofill __DATA,__common,"_addr:[GLOBAL]/214",8,3
	.globl	"_addr:[GLOBAL]/212"    ## @"addr:[GLOBAL]/212"
.zerofill __DATA,__common,"_addr:[GLOBAL]/212",16,3

.subsections_via_symbols
