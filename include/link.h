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

#ifndef LINK_H_
#define LINK_H_

/*
 * API for the communication channel between host and target. This API shall be
 * implemented by the platform dependent code. It is typically a UART
 * peripheral driver. The link API is used by the astub target independent
 * code.
 */

void link_open(void);
void link_close(void);
void link_put(int c);
int link_get(void);

#endif

