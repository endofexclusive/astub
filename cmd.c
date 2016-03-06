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
 * Handle gdb Remote Serial Protocol commands
 *
 * Implemented according to "Debugging with gdb", Tenth Edition, for GDB
 * version 7.10.1, appendix E gdb Remote Serial Protocol.
 */

#include <stub_arch.h>
#include <hex.h>
#include <pkt.h>
#include <cmd.h>

/* Send a Stop Reply Packet. */
static int send_stop_reply(int gdb_signal)
{
        static char pkt[] = {'S', 0, 0, '\0'};

        pkt[1] = nibbletohex[(gdb_signal / 0x10) % 0x10];
        pkt[2] = nibbletohex[gdb_signal % 0x10];
        putpkt(pkt);

        return 0;
}

/* pktbuf is used both as command and reply buffer. */
static char pktbuf[1 + REGSBYTES*2 + 1 + 2 + 1];
static int isattached = 0;

int stub_main(int arch_reason)
{
        int ret;
        int gdb_signal;

        gdb_signal = decode_signal(arch_reason);
        /* Do not announce stop reply if we were detached. */
        if (isattached) {
                send_stop_reply(gdb_signal);
        }
        /* We are now attached. */
        isattached = 1;

        do {
                int cmd;
                char *p;

                while (getpkt(&pktbuf[0], sizeof(pktbuf)) < 1);

                p = &pktbuf[0];
                cmd = *p;
                if ('?' == cmd) {
                        send_stop_reply(gdb_signal);
                        ret = 0;
                } else if ('c' == cmd) {
                        ret = handle_c(p);
                } else if ('s' == cmd) {
                        ret = handle_s(p);
                } else if ('D' == cmd) {
                        ret = handle_D(p);
                        if (ret & CMD_LEAVE) {
                                isattached = 0;
                        }
                } else if ('g' == cmd) {
                        ret = handle_g(p);
                } else if ('G' == cmd) {
                        ret = handle_G(p);
                } else if ('P' == cmd) {
                        ret = handle_P(p);
                } else if ('m' == cmd) {
                        ret = handle_m(p);
                } else if ('M' == cmd) {
                        ret = handle_M(p);
                } else {
                        /*
                         * For any command not supported by the stub, an empty
                         * response ('$#00') should be returned.
                         */
                        p = "";
                        ret = CMD_REPLY;
                }

                if (ret & CMD_REPLY) {
                        putpkt(p);
                }
        } while (!(ret & CMD_LEAVE));

        return ret;
}

