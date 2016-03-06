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

#ifndef STUB_ARCH_
#define STUB_ARCH_

/*
 * This file describes the Motorola 68000 specific parts of the GDB stub.
 *
 * Currently only MC68000 is supported.
 */

#include <stdint.h>

struct mc68000_regs {
        uint32_t d0, d1, d2, d3, d4, d5, d6, d7;
        uint32_t a0, a1, a2, a3, a4, a5, a6, a7;
        uint16_t _srpad, sr;
        uint32_t pc;
};

extern struct mc68000_regs mc68000_regs;

#define REGSBYTES (sizeof (struct mc68000_regs))

extern const void *exception_entry_int;
extern const void *exception_entry_ill;
extern const void *exception_entry_trap;
extern const void *exception_entry_fpe;
extern const void *exception_entry_bus;
extern const void *exception_entry_segv;

#endif

