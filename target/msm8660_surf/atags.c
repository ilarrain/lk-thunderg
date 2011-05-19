/* Copyright (c) 2009-2010, Code Aurora Forum. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Code Aurora nor
 *       the names of its contributors may be used to endorse or promote
 *       products derived from this software without specific prior written
 *       permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <reg.h>
#include <debug.h>
#include <smem.h>

#define EBI1_SIZE1   0x02D00000 // 45M
#define EBI1_ADDR1   0x40200000

#define EBI1_SIZE2   0x08000000 // 128M
#define EBI1_ADDR2   0x48000000

#define EBI1_SIZE3   0x10000000 // 256M
#define EBI1_ADDR3   0x50000000

#define EBI1_CS1_ADDR_BASE    0x00A40024

unsigned* target_atag_mem(unsigned* ptr)
{
    unsigned value = 0;

    /* ATAG_MEM */
    *ptr++ = 4;
    *ptr++ = 0x54410002;
    *ptr++ = EBI1_SIZE1;
    *ptr++ = EBI1_ADDR1;

    *ptr++ = 4;
    *ptr++ = 0x54410002;
    *ptr++ = EBI1_SIZE2;
    *ptr++ = EBI1_ADDR2;

    /* For 512MB RAM*/
    value = readl(EBI1_CS1_ADDR_BASE);
    if (((value >> 8) & 0xFF)  == 0x50)
    {
        *ptr++ = 4;
        *ptr++ = 0x54410002;
        *ptr++ = EBI1_SIZE3;
        *ptr++ = EBI1_ADDR3;
    }

    return ptr;
}

void *target_get_scratch_address(void)
{
    return ((void *)SCRATCH_ADDR);
}
