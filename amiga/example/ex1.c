/*
 * Implemented according to Photon/Scoopex video "Amiga Hardware Programming 3
 * - Move that rasterline"
 */

#include <stdint.h>

static volatile       uint16_t *const COLOR0 = (void *) 0xdff180;
static volatile const uint16_t *const VHPOSR = (void *) 0xdff006;

/* Get vertical screen position, 8-bit value. */
static inline uint8_t getvpos8(void)
{
        volatile const uint8_t *const vlo = (uint8_t *) VHPOSR;
        return *vlo;
}

/* Wait for vertical screen position, 16-bit value. */
static inline void waitvpos8(uint8_t vpos)
{
        while (getvpos8() != vpos);
}

int main(void)
{
        uint8_t v = 0xac;
        int8_t vadd = 1;

        while (1) {
                waitvpos8(0x2c);
                /* Frame loop start */
                *COLOR0 = 0x218;

                v += vadd;

                /* Bottom check */
                if (0xf0 <= v) {
                        /* Change direction */
                        vadd = -vadd;
                }
                /* Top check */
                if (v <= 0x40) {
                        /* Change direction */
                        vadd = -vadd;
                }

                while (getvpos8() != v);
                *COLOR0 = 0x0f77;
                while (getvpos8() == v);
                *COLOR0 = 0x0128;

                /* Frame loop end */
        }

        return 0;
}

