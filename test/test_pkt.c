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

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <vtest.h>
#include <pkt.h>
#include <stub_arch.h>

int get_i;
char get_buf[GETSZ];
int put_i;
char put_buf[PUTSZ];

void buf_init(void)
{
        get_i = 0;
        memset(get_buf, 0, sizeof get_buf);
        put_i = 0;
        memset(put_buf, 0, sizeof put_buf);
}

void test_putpkt(void)
{
        buf_init();
        get_buf[0] = '+';

        putpkt("abc");
        vtest(0 == strcmp(put_buf, "$abc#26"));

        get_i = 0;
        putpkt("");
        vtest(0 == strcmp(&put_buf[7], "$#00"));


        /* Force retransmit */
        buf_init();
        get_buf[0] = '-';
        get_buf[1] = '+';
        putpkt("OK");
        vtest(0 == strcmp(&put_buf[0], "$OK#9a$OK#9a"));
}

void test_getpkt(void)
{
}

int main(void)
{
        vtest_init();

        test_putpkt();
        test_getpkt();

        vtest_report();
        vtest_end();

        return 0;
}

