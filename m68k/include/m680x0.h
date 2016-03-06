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

#ifndef M680x0_H_
#define M680x0_H_

/*
 * This file describes the Motorola 68000 series microprocessors.
 *
 * Currently only MC68000 is described.
 */

/* Status register */
#define SRB_T   15
#define SRB_S   13
#define SRB_I    8

#define SRF_T   (0x1 << SRB_T)
#define SRF_S   (0x1 << SRB_S)
#define SRF_I   (0x7 << SRB_I)

/*
 * The exception vector table consists of 256 pointers. exception_vector_table
 * is a pointer to the table.
 */
#if 0
extern void **const exception_vector_table;
#endif

#define VEC_RESETSP (0)
#define VEC_RESETPC (1)
#define VEC_BUSERR  (2)
#define VEC_ADDRERR (3)
#define VEC_ILLEGAL (4)
#define VEC_ZERODIV (5)
#define VEC_CHK     (6)
#define VEC_TRAPV   (7)
#define VEC_PRIV    (8)
#define VEC_TRACE   (9)
#define VEC_LINE10  (10)
#define VEC_LINE11  (11)
#define VEC_FORMAT  (14)
#define VEC_UNINT   (15)
#define VEC_SPUR    (24)
#define VEC_INT1    (25)
#define VEC_INT2    (26)
#define VEC_INT3    (27)
#define VEC_INT4    (28)
#define VEC_INT5    (29)
#define VEC_INT6    (30)
#define VEC_INT7    (31)
#define VEC_TRAP0   (32)
#define VEC_TRAP1   (33)
#define VEC_TRAP2   (34)
#define VEC_TRAP3   (35)
#define VEC_TRAP4   (36)
#define VEC_TRAP5   (37)
#define VEC_TRAP6   (38)
#define VEC_TRAP7   (39)
#define VEC_TRAP8   (40)
#define VEC_TRAP9   (41)
#define VEC_TRAP10  (42)
#define VEC_TRAP11  (43)
#define VEC_TRAP12  (44)
#define VEC_TRAP13  (45)
#define VEC_TRAP14  (46)
#define VEC_TRAP15  (47)

#endif

