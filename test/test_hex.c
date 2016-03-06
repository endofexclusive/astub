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
#include <hex.h>

void test_nibbletohex(void)
{
        for (int i = 0; i < 0x10; i++) {
                if (i < 0xa) {
                        vtest('0'+i == nibbletohex[i]);
                } else {
                        vtest('a'+i-0xa == nibbletohex[i]);
                }
        }
}

void test_hextonibble(void)
{
        for (int i = -128; i <= 127; i++) {
                if (isdigit(i)) {
                        vtest(i-'0' == hextonibble(i));
                } else if (isxdigit(i) && islower(i)) {
                        vtest(i-'a'+10 == hextonibble(i));
                } else {
                        vtest(-1 == hextonibble(i));
                }
        }
}

void test_memtohex(void)
{
        char hex[128];
        uint8_t mem[32];
        char *ret;

        memset(&mem[0], 0xff, sizeof mem);
        memset(&hex[0], 0x77, sizeof hex);

        for (int i = 0; i < (int) sizeof mem; i++) {
                mem[i] = (uint8_t) (i+0x56);
        }

        ret = memtohex(&hex[0], &mem[0], sizeof mem);
        vtest(&hex[2 * (sizeof mem)] == ret);
        vtest(hex[2 * (sizeof mem)] == '\0');
        vtest(hex[2 * (sizeof mem)+1] == 0x77);
        vtest(hex[2 * (sizeof mem)+2] == 0x77);

        for (int i = 0; i < (int) sizeof mem; i++) {
                int nibble;

                nibble = hextonibble(hex[2*i]);
                vtest(-1 != nibble);
                vtest(nibble == (0xf & (mem[i]>>4)));

                nibble = hextonibble(hex[2*i+1]);
                vtest(-1 != nibble);
                vtest(nibble == (0xf & mem[i]));
        }
        ret = memtohex(&hex[1], &mem[2], 0);
        vtest(ret == &hex[1]);

        ret = memtohex(&hex[2], &mem[3], 1);
        vtest(hex[2] == '5');
        vtest(hex[3] == '9');
        vtest(ret == &hex[4]);
}

void test_hextomem(void)
{
        uint8_t mem[32];

        memset(&mem[0], 0xff, sizeof mem);
        
        char hex0[] = "";
        char hex1[] = "1";
        char hex2[] = "23";
        char hex3[] = "456";
        char hex4[] = "abcd";
        char *hex;

        hex = hextomem(mem, hex0, 0x100);
        vtest(hex == hex0);
        for (size_t i = 0; i < sizeof (mem); i++) { vtest(mem[i] == 0xff); }

        hex = hextomem(mem, hex1, 0);
        vtest(hex == hex1);
        for (size_t i = 0; i < sizeof (mem); i++) { vtest(mem[i] == 0xff); }

        hex = hextomem(mem, hex1, 1);
        vtest(hex == &hex1[1]);
        /* Should not update mem because no full byte decoded. */
        for (size_t i = 0; i < sizeof (mem); i++) { vtest(mem[i] == 0xff); }

        /* Should stop after the lone nibble and don't write anything. */
        hex = hextomem(mem, hex1, 2);
        vtest(hex == &hex1[1]);
        /* Should not update mem because no full byte decoded. */
        for (size_t i = 0; i < sizeof (mem); i++) { vtest(mem[i] == 0xff); }

        hex = hextomem(mem, hex2, 1);
        vtest(hex == &hex2[2]);
        vtest(0x23 == mem[0]);
        for (size_t i = 1; i < sizeof (mem); i++) { vtest(mem[i] == 0xff); }
        hex = hextomem(mem, hex2, 23);
        vtest(hex == &hex2[2]);

        hex = hextomem(mem, hex3, 1);
        vtest(hex == &hex3[2]);
        vtest(0x45 == mem[0]);
        hex = hextomem(mem, hex3, 123);
        /* Should pass hex index 2 because it is ok, but don't write. */
        vtest(hex == &hex3[3]);
        vtest(0x45 == mem[0]);
        for (size_t i = 1; i < sizeof (mem); i++) { vtest(mem[i] == 0xff); }

        hex = hextomem(mem, hex4, 2);
        vtest(hex == &hex4[4]);
        vtest(0xab == mem[0]); vtest(0xcd == mem[1]);
        for (size_t i = 2; i < sizeof (mem); i++) { vtest(mem[i] == 0xff); }
}

void test_hextoul(void)
{
        unsigned long ul = -1;
        char *ret;

        char *hex0 = "";
        ret = hextoul(&ul, hex0); vtest(ret == &hex0[0]); vtest(ul == 0x0);

        char *hex1 = "1";
        ret = hextoul(&ul, hex1); vtest(ret == &hex1[1]); vtest(ul == 0x1);

        char *hex2 = "2;";
        ret = hextoul(&ul, hex2); vtest(ret == &hex2[1]); vtest(ul == 0x2);

        char *hex3 = "X3";
        ret = hextoul(&ul, hex3); vtest(ret == &hex3[0]); vtest(ul == 0x0);

        char *hex4 = "4e";
        ret = hextoul(&ul, hex4); vtest(ret == &hex4[2]); vtest(ul == 0x4e);

        char *hex5 = "56e";
        ret = hextoul(&ul, hex5); vtest(ret == &hex5[3]); vtest(ul == 0x56e);

        char *hex6 = "678e";
        ret = hextoul(&ul, hex6); vtest(ret == &hex6[4]); vtest(ul == 0x678e);

        char *hex7 = "789ab";
        ret = hextoul(&ul, hex7); vtest(ret == &hex7[5]); vtest(ul == 0x789ab);

        char *hex8 = "89abcd";
        ret = hextoul(&ul, hex8); vtest(ret == &hex8[6]); vtest(ul == 0x89abcd);

        char *hex9 = "9abcdef";
        ret = hextoul(&ul, hex9); vtest(ret == &hex9[7]); vtest(ul == 0x9abcdef);

        char *hexa = "a1234567";
        ret = hextoul(&ul, hexa); vtest(ret == &hexa[8]); vtest(ul == 0xa1234567);

        /* Depends on compiler! */
        char *hexb;
        size_t z = sizeof (unsigned long);
        if (8 == z) {
                hexb = "123456789abcdef56";
                ret = hextoul(&ul, hexb); vtest(ret == &hexb[16]); vtest(ul == 0x123456789abcdef5);
        } else if (4 == z) {
                hexb = "123456789";
                ret = hextoul(&ul, hexb); vtest(ret == &hexb[8]); vtest(ul == 0x12345678);
        }
}


int main(void)
{
        vtest_init();

        test_nibbletohex();
        test_hextonibble();
        test_memtohex();
        test_hextomem();
        test_hextoul();

        vtest_report();
        vtest_end();

        return 0;
}

