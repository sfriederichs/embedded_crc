/**
 * \file crc_tbl_pgm.h
 * Functions and types for CRC checks.
 *
 * Generated on Tue Oct 06 20:47:44 2015,
 * by pycrc v0.8.3, https://pycrc.org
 * using the configuration:
 *    Width        = 32
 *    Poly         = 0x04c11db7
 *    XorIn        = 0xffffffff
 *    ReflectIn    = False
 *    XorOut       = 0x00000000
 *    ReflectOut   = False
 *    Algorithm    = table-driven
 *****************************************************************************/
#ifndef __CRC_TBL_PGM_H__
#define __CRC_TBL_PGM_H__

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Calculate the initial crc value.
 *
 * \return     The initial crc value.
 *****************************************************************************/
static inline crc_t crc_init_tbl_pgm(void)
{
    return 0xffffffff;
}


/**
 * Update the crc value with new data.
 *
 * \param crc      The current crc value.
 * \param data     Pointer to a buffer of \a data_len bytes.
 * \param data_len Number of bytes in the \a data buffer.
 * \return         The updated crc value.
 *****************************************************************************/
crc_t crc_update_tbl_pgm(crc_t crc, const void *data, size_t data_len);


/**
 * Calculate the final crc value.
 *
 * \param crc  The current crc value.
 * \return     The final crc value.
 *****************************************************************************/
static inline crc_t crc_finalize_tbl_pgm(crc_t crc)
{
    return crc ^ 0x00000000;
}


#ifdef __cplusplus
}           /* closing brace for extern "C" */
#endif

#endif      /* __CRC_TBL_PGM_H__ */
