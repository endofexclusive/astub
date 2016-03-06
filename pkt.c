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

/*
 * Handle gdb Remote Serial Protocol packets
 * 
 * Implemented according to "Debugging with gdb", Tenth Edition, for GDB
 * version 7.10.1, appendix E gdb Remote Serial Protocol.
 *
 * E.1 Overview
 */

#include <link.h>
#include <hex.h>
#include <pkt.h>

static int cksum(int chk, int data)
{
        return (chk + data) & 0xff;
}

int getpkt(char *buf, int len)
{
        char *p;
        int count;
        int c;
        int rxcksum;
        int txcksum;

        /* Synchronize on start character */
        while ('$' != (c = link_get()));
gotsync:
        /* NOTE: '$' not written to buf. */
        p = buf;
        count = 0;
        rxcksum = 0;

        /* Get characters until a # or end of buffer. */
        while (count < len-1) {
                c = link_get();
                if ('$' == c) {
                        goto gotsync;
                } else if ('#' == c) {
                        /*
                         * End of packet. NOTE: '#' not written to buf.
                         */
                        break;
                }
                rxcksum = cksum(rxcksum, c);
                *p++ = c;
                count++;
        }
        *p = 0;

        if ('#' != c) {
                /*
                 * Buffer overflow. ACK: Request retranmission (because it
                 * could be a transmission error).
                 */
                link_put('-');
                return -1;
        }
        /* We have a full packet in buffer: get checksum. */
        c = link_get();
        txcksum = hextonibble(c) << 4;
        c = link_get();
        txcksum += hextonibble(c);

        if (rxcksum == txcksum) {
                /* ACK: Packet received correctly. */
                link_put('+');
        } else {
                /* Checksum error. ACK: Request retransmission. */
                link_put('-');
                count = -1;
        }

        return count;
}

void putpkt(const char *buf)
{
        do {
                const char *p;
                int c;
                int txcksum;

                link_put('$');
                txcksum = 0;
                p = buf;
                while ('\0' != (c = *p++)) {
                        link_put(c);
                        txcksum = cksum(txcksum, c);
                }

                link_put('#');
                link_put(nibbletohex[txcksum / 0x10]);
                link_put(nibbletohex[txcksum % 0x10]);
                /* Loop until packet received correctly. */
        } while ('+' != link_get());
        /* Got response from host. */
}

