/**
 * \file crc_bbbf.c
 * Functions and types for CRC checks.
 *
 * Generated on Tue Oct 06 20:49:21 2015,
 * by pycrc v0.8.3, https://pycrc.org
 * using the configuration:
 *    Width        = 32
 *    Poly         = 0x04c11db7
 *    XorIn        = 0xffffffff
 *    ReflectIn    = False
 *    XorOut       = 0x00000000
 *    ReflectOut   = False
 *    Algorithm    = bit-by-bit-fast
 *****************************************************************************/
 #include <stdint.h>
 
 /**
 * The type of the CRC values.
 *
 * This type must be big enough to contain at least 32 bits.
 *****************************************************************************/
typedef uint_fast32_t crc_t;

#include "crc_bbbf.h"     /* include the header file generated with pycrc */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * Update the crc value with new data.
 *
 * \param crc      The current crc value.
 * \param data     Pointer to a buffer of \a data_len bytes.
 * \param data_len Number of bytes in the \a data buffer.
 * \return         The updated crc value.
 *****************************************************************************/
crc_t crc_update_bbbf(crc_t crc, const void *data, size_t data_len)
{
    const unsigned char *d = (const unsigned char *)data;
    unsigned int i;
    bool bit;
    unsigned char c;

    while (data_len--) {
        c = *d++;
        for (i = 0x80; i > 0; i >>= 1) {
            bit = crc & 0x80000000;
            if (c & i) {
                bit = !bit;
            }
            crc <<= 1;
            if (bit) {
                crc ^= 0x04c11db7;
            }
        }
        crc &= 0xffffffff;
    }
    return crc & 0xffffffff;
}


