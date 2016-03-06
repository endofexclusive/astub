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

#include <stdio.h>
#include <link.h>

#include <stub_arch.h>

static int putting = 0;

void link_put(int c)
{
        put_buf[put_i] = c;
        put_i = (put_i + 1) % PUTSZ;
        if (!putting) {
                printf("\n-> ");
        }
        putting = 1;
        putchar(c);
}

int link_get(void)
{
        int c = get_buf[get_i];
        get_i = (get_i + 1) % GETSZ;

        if (putting) {
                printf("\n<- ");
        }
        putting = 0;
        putchar(c);

        return c;
}

