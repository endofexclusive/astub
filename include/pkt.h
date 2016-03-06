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

#ifndef PKT_H_
#define PKT_H_

/*
 * getpkt - Receive the next packet string.
 *
 * buf: Packet from $ to # (both exclusive).
 * len: Size of buf.
 * return: Length of packet string, or negative on failure.
 */
int getpkt(char *buf, int len);

/*
 * petpkt - Transmit a packet string.
 *
 * buf: Packet string excluding $, # and checksum.
 */
void putpkt(const char *buf);

#endif

