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

#include <string.h>

#include <m680x0.h>
#include <stub_arch.h>
#include <hex.h>
#include <cmd.h>

struct mc68000_regs mc68000_regs;

int handle_c(char *buf)
{
        char *next;
        unsigned long addr;

        next = hextoul(&addr, &buf[1]);
        if (next != &buf[1]) {
                mc68000_regs.pc = (uint32_t) addr;
        }
        mc68000_regs.sr &= ~SRF_T;

        return CMD_LEAVE;
}

int handle_s(char *buf)
{
        int ret;

        ret = handle_c(buf);
        mc68000_regs.sr |= SRF_T;

        return ret;
}

int handle_D(char *buf)
{
        int ret;

        if ('\0' == buf[1]) {
                strcpy(buf, "OK");
                mc68000_regs.sr &= ~SRF_T;
                ret = CMD_REPLY | CMD_LEAVE;
        } else {
                strcpy(buf, "E00");
                ret = CMD_REPLY;
        }

        return ret;
}

int handle_g(char *buf)
{
        memtohex(buf, &mc68000_regs, sizeof(mc68000_regs));

        return CMD_REPLY;
}

int handle_G(char *buf)
{
        /* No need to check length. */
        hextomem(&mc68000_regs, &buf[1], sizeof(mc68000_regs));
        strcpy(buf, "OK");

        return CMD_REPLY;
}

int handle_P(char *buf)
{
        const char *p;
        unsigned long val;
        int n;
        uint32_t r;
        uint32_t *regs = (uint32_t *) &mc68000_regs;

        p = hextoul(&val, &buf[1]);
        n = (int) val;

        /* Check range. */
        if (
                (n < 0) ||
                ((int) ((sizeof (struct mc68000_regs))/sizeof (uint32_t)) <= n)
        ) {
                strcpy(buf, "E00");
                goto out;
        }

        /* Check format */
        if ('=' != *p) {
                strcpy(buf, "E01");
                goto out;
        }

        p++;
        p = hextoul(&val, p);
        r = (uint32_t) val;
        if ('\0' != *p) {
                strcpy(buf, "E02");
                goto out;
        }

        regs[n] = r;
        strcpy(buf, "OK");
out:
        return CMD_REPLY;
}

int handle_m(char *buf)
{
        const char *p;
        unsigned long val;
        const void *addr;
        size_t length;

        p = hextoul(&val, &buf[1]);
        addr = (const void *) val;
        if (',' != *p) {
                strcpy(buf, "E00");
                goto out;
        }

        p++;
        p = hextoul(&val, p);
        length = (size_t) val;
        if (REGSBYTES < length) {
                length = REGSBYTES;
        }

        memtohex(buf, addr, length);
out:
        return CMD_REPLY;
}

int handle_M(char *buf)
{
        const char *p;
        unsigned long val;
        void *addr;
        size_t length;

        p = hextoul(&val, &buf[1]);
        addr = (void *) val;
        if (',' != *p) {
                strcpy(buf, "E00");
                goto out;
        }

        p++;
        p = hextoul(&val, p);
        length = (size_t) val;
        if (':' != *p) {
                strcpy(buf, "E01");
                goto out;
        }

        p++;

        hextomem(addr, p, length);
        strcpy(buf, "OK");
out:
        return CMD_REPLY;
}

