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

#include <hex.h>

const char nibbletohex[] = "0123456789abcdef";

int hextonibble(int c)
{
        int nibble;

        if ('a' <= c && c <= 'f') {
                nibble = 0xa + c - 'a';
        } else if ('0' <= c && c <= '9') {
                nibble = c - '0';
        } else {
                nibble = -1;
        }
        return nibble;
}

char *memtohex(char *hex, const void *mem, size_t n)
{
        size_t i;
        const unsigned char *cp;

        cp = mem;
        for (i = 0; i < n; i++) {
                int val;

                val = *cp++ % 0x100;
                *hex++ = nibbletohex[val / 0x10];
                *hex++ = nibbletohex[val % 0x10];
        }
        *hex = 0;

        return hex;
}

char *hextomem(void *mem, const char *hex, size_t n)
{
        unsigned char *dp;

        dp = mem;
        while (n--) {
                int c;
                int nibble;
                int val;

                c = *hex;
                nibble = hextonibble(c);
                if (nibble < 0) { break; }
                hex++;

                val = nibble << 4;

                c = *hex;
                nibble = hextonibble(c);
                if (nibble < 0) { break; }
                hex++;

                val += hextonibble(c);
                *dp++ = val;
        }

        return (char *) hex;
}

char *hextoul(unsigned long *mem, const char *hex)
{
        size_t n;
        unsigned long val;

        n = 2 * sizeof (unsigned long);
        val = 0;
        while (n--) {
                int nibble;
                int c;

                c = *hex;
                nibble = hextonibble(c);
                if (nibble < 0) { break; }
                hex++;

                val <<= 4;
                val += nibble;
        }
        *mem = val;

        return (char *) hex;
}
