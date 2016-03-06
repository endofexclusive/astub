;  Copyright 2016 Martin Aberg
; 
;  This file is part of astub.
; 
;  astub is free software: you can redistribute it and/or modify it under the
;  terms of the GNU General Public License as published by the Free Software
;  Foundation, either version 3 of the License, or (at your option) any later
;  version.
; 
;  astub is distributed in the hope that it will be useful, but WITHOUT ANY
;  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
;  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
;  details.
; 
;  You should have received a copy of the GNU General Public License along with
;  this program. If not, see <http://www.gnu.org/licenses/>.
; 

; This file implements low-level GDB stub exception handling for the M68000
; architecture. Only Motorola MC68000 is supported at the moment.
;
; Motorola assembly syntax is used. vasm 1.7d can be used to assemble:
; $ vasmm68k_mot -quiet -phxass -m68000 -Felf
;
; All code in this file is written PC relative, which means the code may run
; from any location. If your own code is also PC relative, then take care when
; setting up the handlers in the exception vector table.
;

; Status Register bit definitions: Trace, Supervisor, Interrupt level
SRB_T	EQU 15
SRB_S	EQU 13
SRB_I	EQU  8

SRF_T	EQU ($1<<SRB_T)
SRF_S	EQU ($1<<SRB_S)
SRF_I	EQU ($7<<SRB_I)

	section	".text", code
	public	breakpoint
	public	exception_entry_int
	public	exception_entry_ill
	public	exception_entry_trap
	public	exception_entry_fpe
	public	exception_entry_bus
	public	exception_entry_segv
	cnop	0,4

; NOTE: MC68000 and MC68008 exception stack frame differ from
; later processors. MC680010 and later always have the
; "Format | Vector Offset" word at 6(sp).
;
; Only 68000 is supported by the source code in this file.
;
; MC68000 Exception Frame (most common)
; +---------+----------------------+
; |  SP +04 | PC Low               |
; |  SP +02 | PC High              |
; |  SP     | Status Register      |
; +---------+----------------------+
;
; The exception entries below pushes one more word on the stack with an
; appropriate GDB signal number.
;

GDB_SIGNAL_INT 	EQU	 2 ; Interrupt
GDB_SIGNAL_ILL 	EQU	 4 ; Illegal instruction
GDB_SIGNAL_TRAP	EQU	 5 ; Trace/breakpoint trap
GDB_SIGNAL_FPE 	EQU	 8 ; Arithmetic exception (incl. integer-arith)
GDB_SIGNAL_BUS 	EQU	10 ; Bus error
GDB_SIGNAL_SEGV	EQU	11 ; Segmentation fault
; Offsets from m68000_regs
REGS_D0         EQU	$00
REGS_A6         EQU	$38
REGS_A7         EQU	$3c
REGS_SR         EQU	$42
REGS_PC         EQU	$44

exception_entry_int:
	move.w	#GDB_SIGNAL_INT, -(sp)
	bra	exception_entry
exception_entry_ill:
	move.w	#GDB_SIGNAL_ILL, -(sp)
	bra	exception_entry
exception_entry_trap:
	move.w	#GDB_SIGNAL_TRAP, -(sp)
	bra	exception_entry
exception_entry_fpe:
	move.w	#GDB_SIGNAL_FPE, -(sp)
	bra	exception_entry
exception_entry_bus:
	move.w	#GDB_SIGNAL_BUS, -(sp)
	bra	exception_entry
exception_entry_segv:
	move.w	#GDB_SIGNAL_SEGV, -(sp)
	bra	exception_entry

exception_entry:
	; Raise interrupt priority level early.
	move.w	#(SRF_I | SRF_S), sr
	; Save registers: PC relative!
	move.l	a6, -(sp)
	lea	mc68000_regs(pc), a6
	movem.l	d0-d7/a0-a5, (a6)
	move.l	(sp)+, REGS_A6(a6)

	; Take off the signal number set by exception_entry_<signal>.
	move.w	(sp)+, d6
	; We might have come from user mode so load USP.
	move.l	usp, a5

	; Did we come from supervisor mode? Check SR on exception
	; frame.
	move.w	(sp)+, d7
	; Save SR
	move.w	d7, REGS_SR(a6)
	; Save PC
	move.l	(sp)+, REGS_PC(a6)
	; Save SSP
	move.l	sp, a4

	btst.w	#SRB_S, d7
	beq	.entry_sp_store

.entry_mode_s:
	; We came from supervisor mode so SSP was used.
	move.l	sp, a5

.entry_sp_store:
	; Pre-exception stack pointer is loaded in a5, store it so GBB can
	; use it.
	move.l	a5, REGS_A7(a6)

	; Signal number is in d6.
	; NOTE: Assuming the C int type is 16 bit!!!
	move.w	d6, -(sp)
	bsr	stub_main
	; Pop parameter
	addq.l	#2, sp

.trap_exit:
	; We still have mc68000_regs in a6, since it is preserved by
	; the ABI.
	; Read stack pointer
	move.l	REGS_A7(a6), a5

	; Inspect updated status register
	move.w	REGS_SR(a6), d7
	; Shall we load USP?
	btst.w	#SRB_S, d7
	beq	.trap_exit_user_mode

.trap_exit_supervisor_mode:
	; We used SSP so load it.
	move.l	a5, sp
	bra	.trap_exit_any_mode

.trap_exit_user_mode:
	move.l	a5, usp
	; SSP not under stub control so just load the value saved before
	move.l	a4, sp

.trap_exit_any_mode:
	; Restore PC to stack
	move.l	REGS_PC(a6), -(sp)
	; Restore status register to stack
	move.w	d7, -(sp)
	movem.l	(a6), d0-d7/a0-a6
	rte

	; User function for entering the debugger.
breakpoint:
	trap	#1
	rts

