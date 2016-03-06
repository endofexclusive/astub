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

#ifndef HEX_H_
#define HEX_H_

#include <stddef.h>

extern const char nibbletohex[];

/*
 * hextonibble - Decode hex char to nibble
 *
 * c: Character to decode
 * return: 0..15, or -1 if decoding failed
 */
int hextonibble(int c);

/*
 * memtohex - Encode memory data to hex string
 *
 * hex: Destination hex string. A terminating NUL is added.
 * mem: Memory data to encode
 * n: Number of mem data bytes to encode
 * return: Pointer to hex NUL terminator.
 */
char *memtohex(char *hex, const void *mem, size_t n);

/*
 * hextomem - Decode hex string to memory data
 *
 * Decoding ends when n bytes has been written or when non-hex character is
 * observed at hex.
 *
 * mem: Destination memory address
 * hex: Hex string to decode
 * n: Number of mem bytes to write
 * return: Pointer to first character in hex not decoded
 */
char *hextomem(void *mem, const char *hex, size_t n);

/*
 * hextoul - Decode hex string to unsigned long
 *
 * mem: Destination memory address
 * hex: Hex string to decode
 * return: Pointer to first hex character not decoded
 */
char *hextoul(unsigned long *mem, const char *hex);

#endif

