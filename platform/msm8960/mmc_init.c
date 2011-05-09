/* Copyright (c) 2011, Code Aurora Forum. All rights reserved.

 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Code Aurora Forum, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <debug.h>
#include <reg.h>
#include <platform/iomap.h>
#include "mmc.h"

#define SDC_NS(n)       (CLK_CTL_BASE + 0x282C + 32*((n) - 1))

#define SDC_MD(n)       (CLK_CTL_BASE + 0x2828 + 32*((n) - 1))


void clock_set_enable (unsigned slot, unsigned int mclk)
{
	/*
	 * TODO: It must correspond the SDCx_BASE used in target/init.c
	 * Need a better way to base this off a single param. Shouldn't
	 * have to change at two places.
	 */
	unsigned char sdc_id = slot;

	if (mclk == MMC_CLK_400KHZ)
	{
		writel(0x100CF,  SDC_MD(sdc_id));
		writel(0xD00B40, SDC_NS(sdc_id));
		mdelay(10);
	}
	else if (mclk == MMC_CLK_20MHZ)
	{
		writel(0x00000,  SDC_MD(sdc_id));
		writel(0x000A00, SDC_NS(sdc_id));
		mdelay(10);
	}
	else if (mclk == MMC_CLK_25MHZ)
	{
		writel(0x100EF,  SDC_MD(sdc_id));
		writel(0xF00B47, SDC_NS(sdc_id));
		mdelay(10);
	}
	else if (mclk == MMC_CLK_50MHZ)
	{
		writel(0x100F7,  SDC_MD(sdc_id));
		writel(0xFC0B47, SDC_NS(sdc_id));
		mdelay(10);
	}
}

