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

#include <m680x0.h>
#include <stub_arch.h>
#include <link.h>

/*
 * Vector table
 *
 * The table consists of 256 pointers. exception_vector_table is a pointer to
 * the table.
 *
 * NOTE: C may not allow dereferencing address 0.
 */
static void **const exception_vector_table = (void *) 0;

/*
 * Initialization of VEC_INIT5 is Amiga specific.
 *
 * NOTE: The exception entry installation into the exception vector table could
 * be table based instead, but its tricky when doing PC relative code.
 */
void a500_init(void)
{
        /* Initializes Amiga UART hardware. */
        link_open();

        exception_vector_table[VEC_BUSERR]      = &exception_entry_bus;
        exception_vector_table[VEC_ADDRERR]     = &exception_entry_bus;
        exception_vector_table[VEC_ILLEGAL]     = &exception_entry_ill;
        exception_vector_table[VEC_ZERODIV]     = &exception_entry_fpe;
        exception_vector_table[VEC_CHK]         = &exception_entry_fpe;
        exception_vector_table[VEC_TRAPV]       = &exception_entry_fpe;
        exception_vector_table[VEC_PRIV]        = &exception_entry_segv;
        exception_vector_table[VEC_TRACE]       = &exception_entry_trap;
        exception_vector_table[VEC_LINE10]      = &exception_entry_ill;
        exception_vector_table[VEC_LINE11]      = &exception_entry_ill;
        exception_vector_table[VEC_INT5]        = &exception_entry_int;
        exception_vector_table[VEC_INT7]        = &exception_entry_int;
        exception_vector_table[VEC_TRAP1]       = &exception_entry_trap;
        exception_vector_table[VEC_TRAP15]      = &exception_entry_trap;
}

/*
 * arch_signal is expected to be the actual GDB signal for this particular
 * implementation.
 */
int decode_signal(int arch_signal)
{
        return arch_signal;
}

