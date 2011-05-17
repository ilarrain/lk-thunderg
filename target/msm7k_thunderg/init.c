/*
 * Copyright (c) 2009, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name of Google, Inc. nor the names of its contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <debug.h>
#include <dev/keys.h>
#include <dev/gpio_keypad.h>
#include <lib/ptable.h>
#include <dev/flash.h>
#include <smem.h>

#define LINUX_MACHTYPE  3014

#define VARIABLE_LENGTH        0x10101010
#define DIFF_START_ADDR        0xF0F0F0F0
#define NUM_PAGES_PER_BLOCK    0x40

static struct ptable flash_ptable;

/* for these partitions, start will be offset by either what we get from
 * smem, or from the above offset if smem is not useful. Also, we should
 * probably have smem_ptable code populate our flash_ptable.
 *
 * When smem provides us with a full partition table, we can get rid of
 * this altogether.
 *
 */
static struct ptentry board_part_list[] = {
	{
		.start = 304,
		.length = 34,
		.name = "boot",
	},
	{
		.start = 338,
		.length = 1520,
		.name = "system",
	},
	{
		.start = 1858,
		.length = 40,
		.name = "recovery",
	},
	{
		.start = 1898,
		.length = 22,
		.name = "lgdrm",
	},
	{
		.start = 1920,
		.length = 8,
		.name = "splash",
	},
	{
		.start = 1928,
		.length = 12,
		.name = "FOTABIN",
	},
	{
		.start = 1940,
		.length = 46,
		.name = "FOTA",
	},
	{
		.start = 1986,
		.length = 2,
		.name = "misc",
	},
	{
		.start = 1988,
		.length = 512,
		.name = "cache",
	},
	{
		.start = 2500,
		.length = 1596,
		.name = "userdata",
	},
};
static int num_parts = sizeof(board_part_list)/sizeof(struct ptentry);

void smem_ptable_init(void);
unsigned smem_get_apps_flash_start(void);

void keypad_init(void);

int target_is_emmc_boot(void);

void target_init(void)
{
	unsigned offset;
	struct flash_info *flash_info;
	unsigned total_num_of_blocks;
	bool  start_addr_changed = false;
	unsigned next_ptr_start_adr = 0;
	int i;

	dprintf(INFO, "target_init()\n");

	keys_init();
	keypad_init();

	if (target_is_emmc_boot())
		return;

	ptable_init(&flash_ptable);
	smem_ptable_init();

	flash_init();
	flash_info = flash_get_info();
	ASSERT(flash_info);

	offset = smem_get_apps_flash_start();
	if (offset == 0xffffffff)
	        while(1);

	total_num_of_blocks = (flash_info->block_size)/NUM_PAGES_PER_BLOCK;

	for (i = 0; i < num_parts; i++) {
		struct ptentry *ptn = &board_part_list[i];
		unsigned len = ptn->length;

		if ((len == 0) && (i == num_parts - 1))
			len = flash_info->num_blocks - offset - ptn->start;
		ptable_add(&flash_ptable, ptn->name, offset + ptn->start,
			   len, ptn->flags);
	}

	smem_add_modem_partitions(&flash_ptable);

	ptable_dump(&flash_ptable);
	flash_set_ptable(&flash_ptable);
}

unsigned board_machtype(void)
{
    return LINUX_MACHTYPE;
}

void reboot_device(unsigned reboot_reason)
{
    reboot(reboot_reason);
}

unsigned check_reboot_mode(void)
{
    unsigned mode[2] = {0, 0};
    unsigned int mode_len = sizeof(mode);
    unsigned smem_status;

    smem_status = smem_read_alloc_entry(SMEM_APPS_BOOT_MODE,
					&mode, mode_len );
    if(smem_status)
    {
      dprintf(CRITICAL, "ERROR: unable to read shared memory for reboot mode\n");
      return 0;
    }
    return mode[0];
}

void target_battery_charging_enable(unsigned enable, unsigned disconnect)
{
}
