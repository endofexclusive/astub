; Minimal loader for binary image in AmigaOS
;
; It copies the data between labels binstart and binend to the memory area
; ending with address CHIPEND. Supervisor Stack Pointer is then set to the first
; destination word.
;
; Assembled with vasm:
; vasmm68k_mot -quiet -phxass -m68000 -Fhunkexe bload.s -o bload
;
; The program is self-standing, so let's define some magic here.
CHIPEND		EQU $80000
CUSTOM		EQU $dff000
INTENA		EQU $09A
DMACON		EQU $096
COLOR00		EQU $180
INTF_SETCLR	EQU $8000
INTF_INTEN	EQU $4000
INTF_ALL	EQU $3fff
DMAF_ALL	EQU $7fff
CIAB		EQU $bfd000
VO_ILLEGAL	EQU $10

	section code
	lea	CUSTOM, a0

	; Disable interrupts
	move.w	#INTF_ALL, INTENA(a0)
	move.w	#(INTF_SETCLR|INTF_INTEN), INTENA(a0)

	; Disable DMA
	move.w  #DMAF_ALL, DMACON(a0)
	move.w	#$0f00, COLOR00(a0)

	; Turn off disk motor.
	move.b	#$f1, CIAB+$100

	; Copy image - backwards is safe
	lea	CHIPEND, a1
	lea	binend(pc), a2
	lea	binstart(pc), a3
	bra	copy
copy_loop:
	move.w	#$0f0f, COLOR00(a0)
	move.l	-(a2), -(a1)
	move.w	#$00f0, COLOR00(a0)
copy:
	cmp.l	a2, a3
	bne	copy_loop

	; Install handler for illegal instruction
	lea	illegal_handler(pc), a4
	move.l	a4, VO_ILLEGAL

	; Trick CPU into Supervisor mode
	illegal
illegal_handler:
	move.w	#$2000, sr

	; Set Supervisor stack pointer
	move.l	a1, a7
	move.w	#$000f, COLOR00(a0)

	; Jump to entry point
	jmp	(a1)

	; Binary goes here.
	; NOTE: Be sure to include the .bss section in the binary, since the
	; loader does not allocate space for it.
	cnop	0,4
binstart:
	move.w	#$0000, COLOR00(a0)
	incbin	main.bin
	cnop	0,4
binend:

