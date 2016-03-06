/*
 * Copyright 2016 Martin Aberg
 *
 * This file is part of astub.
 *
 * astub is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * astub is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CMD_H_
#define CMD_H_

/*
 * See include/gdb/signals.def.
 */
enum gdb_signal {
        GDB_SIGNAL_0    =  0, /* Signal 0 */
        GDB_SIGNAL_INT  =  2, /* Interrupt */
        GDB_SIGNAL_QUIT =  3, /* Quit */
        GDB_SIGNAL_ILL  =  4, /* Illegal instruction */
        GDB_SIGNAL_TRAP =  5, /* Trace/breakpoint trap */
        GDB_SIGNAL_ABRT =  6, /* Aborted */
        GDB_SIGNAL_EMT  =  7, /* Emulation trap */
        GDB_SIGNAL_FPE  =  8, /* Arithmetic exception (incl. integer-arith) */
        GDB_SIGNAL_KILL =  9, /* Killed */
        GDB_SIGNAL_BUS  = 10, /* Bus error */
        GDB_SIGNAL_SEGV = 11, /* Segmentation fault */
        GDB_SIGNAL_SYS  = 12, /* Bad system call */
        GDB_SIGNAL_TERM = 15  /* Terminated */
};


enum {
        CMD_REPLY = 1,
        CMD_LEAVE = 2
};

/*
 * This function indicates the reason for entering the GDB stub by returning a
 * GDB_SIGNAL_ as specified by the enum above.
 *
 * The input is an architecture/platform specific reason for entering the
 * debugger. The function may for example convert an exception vector number
 * into a corresponding GDB_SIGNAL_ number.
 */
int decode_signal(int arch_reason);

/*
 * GDB command processing main loop. It may return after processing a command
 * like 's' or 'c' etc. It should be called from the architecture specific code
 * after exception.
 */
int stub_main(int arch_reason);

/*
 * Command handlers
 *
 * - buf: Command and reply string. At entry, buf contains the command,
 *   including the command and NUL termination. At return, buf shall contain
 *   the NUL terminated reply packet, if any.
 * - return:
 *   - CMD_REPLY: buf shall be sent as reply.
 *   - CMD_LEAVE: Return from stub_main with return value of command handler.
 */

/*
 * 'c [addr]' - Continue at addr, which is the address to resume.
 *
 * If addr is omitted, resume at current address.
 *
 * Reply: [Stop Reply Packets]
 */
int handle_c(char *buf);

/*
 * 's [addr]' - Single step, resuming at addr. If addr is omitted, resume at
 * same address.
 *
 * If addr is omitted, resume at current address.
 *
 * Reply: [Stop Reply Packets]
 */
int handle_s(char *buf);

/*
 * 'D;pid' - Detach GDB from the remote system. It is sent to the remote target
 * before GDB disconnects via the detach command.
 *
 * Reply: [Stop Reply Packets]
 */
int handle_D(char *buf);


/*
 * 'g' - Read general registers.
 *
 * Reply:
 * - 'XX...' Each byte of register data is described by two hex digits
 *   (gdbarch_register_name).
 * - 'E NN' for an error
 */
int handle_g(char *buf);

/*
 * 'G XX...' - Write general registers.
 *
 * Reply:
 * - 'OK' for success
 * - 'E NN' for an error
 */
int handle_G(char *buf);

/*
 * 'P n...=r...' - Write register n... with value r... The register number n is
 * in hexadecimal (two hex digits).
 *
 * Reply:
 * - 'OK' for success
 * - 'E NN' for an error
 */
int handle_P(char *buf);

/*
 * 'm addr,length' - Read length addressable memory units starting at address
 * addr.
 *
 * Interpretation of access size and alignment is free.
 *
 * Reply:
 * - 'XX...' Memory contents; Each byte is transmitted as a two-digit
 *   hexadecimal number.
 * - 'E NN' for an error
 */
int handle_m(char *buf);

/*
 * 'M addr,length:XX...' - Write length addressable memory units starting at
 * address addr.
 *
 * The data is given by XX...; each byte is transmitted as a two-digit
 * hexadecimal number.
 *
 * Reply:
 * - 'OK' for success
 * - 'E NN' for an error (this includes the case where only part of the data
 *   was written).
 */
int handle_M(char *buf);

#endif

