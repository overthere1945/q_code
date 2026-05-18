/*
 * Copyright (c) 2024 Meta
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/linker/iterable_sections.h>

	SECTION_PROLOGUE(intc_table_area,,SUBALIGN(4))
	{ \
		Z_LINK_ITERABLE(intc_table); \
		. = ALIGN(0x1000);
	} GROUP_ROM_LINK_IN(RAMABLE_REGION, ROMABLE_REGION)
