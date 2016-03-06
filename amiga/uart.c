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

#include <stdint.h>

#include <link.h>

#define CUSTOMBASE 0x00dff000
static volatile const uint16_t *const SERDATR = (void *) (CUSTOMBASE + 0x018);
static volatile       uint16_t *const SERDAT  = (void *) (CUSTOMBASE + 0x030);
static volatile       uint16_t *const SERPER  = (void *) (CUSTOMBASE + 0x032);
static volatile       uint16_t *const INTENA  = (void *) (CUSTOMBASE + 0x09a);
static volatile       uint16_t *const INTREQ  = (void *) (CUSTOMBASE + 0x09c);

#define INTB_SETCLR     15
#define INTB_RBF        11
#define INTB_TBE        0
#define INTF_SETCLR     (1<<INTB_SETCLR)
#define INTF_RBF        (1<<INTB_RBF)
#define INTF_TBE        (1<<INTB_TBE)

#define SERDATB_RBF    14
#define SERDATB_TBE    13
#define SERDATB_STP8    8
#define SERDATB_DB      0
#define SERDATF_RBF     (1<<SERDATB_RBF)
#define SERDATF_TBE     (1<<SERDATB_TBE)
#define SERDATF_STP8    (1<<SERDATB_STP8)
#define SERDATF_DB      (0xff<<SERDATB_DB)

/*
 * On a PAL Amiga machine, the relation between SERPER register value and baud
 * is:
 *
 * SERPER = (3,546,895 / baud) - 1
 *
 * baud = 3,546,895 / (SERPER + 1)
 */
#define SERPER_BAUD9600         368
#define SERPER_BAUD38400         91
/* SERPER_BAUD230400 gives the baud 236 460, which is +2.6%. */
#define SERPER_BAUD230400        14
/* SERPER_BAUD460800 gives the baud 443 362, which is -3.8% */
#define SERPER_BAUD460800         7

void link_open(void)
{
        *SERPER = SERPER_BAUD230400;
        *INTREQ = INTF_RBF | INTF_TBE;
        /* Enable UART RX interrupt. */
        *INTREQ = INTF_RBF;
        *INTENA = INTF_SETCLR | INTF_RBF;
}

int link_get(void)
{
        uint16_t serdatr;

        do {
                serdatr = *SERDATR;
        } while (0 == (SERDATF_RBF & serdatr));

        *INTREQ = INTF_RBF;

        return serdatr & SERDATF_DB;
}

void link_put(int c)
{
        uint16_t serdatr;

        do {
                serdatr = *SERDATR;
        } while (0 == (SERDATF_TBE & serdatr));

        *INTREQ = INTF_TBE;
        *SERDAT = SERDATF_STP8 | ((uint8_t) c);
}

void link_close(void)
{
}

