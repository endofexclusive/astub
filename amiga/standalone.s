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

; This is the initialization code used if the GDB stub shall be used
; stand-alone. The code is Amiga specific

	section	".text", code
	public	_start
	cnop	0,4

_start:
	; .data section should already be loaded
.clear_bss:
	link	a6, #0

	lea	__bss_start(pc), a0
	lea	__bss_end(pc), a1
	bra	.clear_bss_first
.clear_bss_loop:
	move.l	#0, (a0)+
.clear_bss_first:
	cmp.l	a0, a1
	bne	.clear_bss_loop

	; Registers GDB stub in the exception vector table.
	bsr	a500_init

	; Reserve 512 byte Supervisor stack space for the stub. A program which
	; does not explicitly sets its Supervisor stack will use this area residing
	; under GDB stub stack.
	sub.l	#512, a7

	; The following code enters the debugger. It could be replaced with a
	; user application instead.
	move.l	#0, d0
.sa_forever:
	bsr	breakpoint
	addq.l	#1, d0
	bra	.sa_forever

