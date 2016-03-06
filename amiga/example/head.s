; This is the crt code for any program.

	section	".text", code
	public	_start
	cnop	0,4

_start:
        ; Interrupt priority level can be lowered here if needed.
	;move.w	#$2000, sr

	; Set stack pointer iff defined by linker.
	move.l	#__stack, d0
	beq	.keep_sp
	move.l	d0, sp
.keep_sp:

	; Create a stack frame.
	link	a6, #-8

	; .data section should already be loaded
.clear_bss:
	lea	__bss_start(pc), a0
	lea	__bss_end(pc), a1
	bra	.clear_bss_first
.clear_bss_loop:
	move.l	#0, (a0)+
.clear_bss_first:
	cmp.l	a0, a1
	bne	.clear_bss_loop

	; CUSTOMBASE in a5 in case we want to use the GCC 'global register
	; variable' extension.
	lea     $dff000, a5

	bsr	main

	; Enter debugger if main returns.
	trap	#1

