/*
 * include/configs/bockw.h
 *
 * Copyright(c) 2011 Renesas Electronics Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef	__CONFIG_H
#define	__CONFIG_H

/* commands to include */
#define	CONFIG_CMD_EDITENV
#define	CONFIG_CMD_SAVEENV
#define	CONFIG_CMD_FLASH
#define	CONFIG_CMD_MEMORY
#define	CONFIG_CMD_MISC
#define	CONFIG_CMD_NET
#define	CONFIG_CMD_DHCP
#define	CONFIG_CMD_PING
#define	CONFIG_CMD_RUN

#define	CONFIG_CMDLINE_TAG
#define	CONFIG_SETUP_MEMORY_TAGS
#define	CONFIG_INITRD_TAG
#define	CONFIG_CMDLINE_EDITING

/* autoboot */
#define CONFIG_BOOTDELAY	3
#define CONFIG_BOOTCOMMAND	"bootp; bootm"
#define CONFIG_ZERO_BOOTDELAY_CHECK

/* high level configuration options */
#define	CONFIG_ARMV7	1
#define	CONFIG_BOCKW

/* keep l2 cache disabled */
#define	CONFIG_L2_OFF	1

/* ram memory map */
#define	CONFIG_SYS_SRAM_BASE	0x04000000
#define	CONFIG_SYS_SRAM_SIZE	0x00800000
#define	CONFIG_SYS_SRAM_BASE2	0x18000000
#define	CONFIG_SYS_SRAM_SIZE2	0x00200000
#define	CONFIG_NR_DRAM_BANKS	1
#define	CONFIG_SYS_SDRAM_BASE	0x60000000
#define	CONFIG_SYS_SDRAM_SIZE	(512 * 1024 * 1024)
#define	CONFIG_SYS_MALLOC_LEN	(CONFIG_ENV_SIZE + 16 * 1024)
#define	CONFIG_SYS_INIT_SP_ADDR	(0xfe798000 - 0x104)
#define	CONFIG_SYS_LOAD_ADDR	(CONFIG_SYS_SDRAM_BASE + 0x7fc0)

#define	CONFIG_SYS_MEMTEST_START	CONFIG_SYS_SDRAM_BASE
#define	CONFIG_SYS_MEMTEST_END \
	(CONFIG_SYS_SDRAM_BASE - 1 + CONFIG_SYS_SDRAM_SIZE)
#define	CONFIG_SYS_ALT_MEMTEST

/* serial port */
#define	CONFIG_BOARD_EARLY_INIT_F	1
#define	CONFIG_SCIF_CONSOLE	1
#define	CONFIG_CONS_SCIF0	1
#define	SCIF0_BASE		0xffe40000
#define	CONFIG_SYS_CLK_FREQ	14745600
#define	CONFIG_CPU_RCM1A	1

#define	CONFIG_BAUDRATE	115200
#define	CONFIG_SYS_BAUDRATE_TABLE	{ 115200 }

/* ethernet */
#define	CONFIG_NET_MULTI
#define	CONFIG_SMC911X		1
#define	CONFIG_SMC911X_16_BIT
#define	CONFIG_SMC911X_BASE	0x18300000

/* flash configuration */
#define	CONFIG_SYS_FLASH_CFI
#define	CONFIG_SYS_FLASH_CFI_WIDTH	FLASH_CFI_16BIT
#define	CONFIG_FLASH_CFI_DRIVER
#define	CONFIG_CFI_FLASH_USE_WEAK_ACCESSORS
#define	CONFIG_FLASH_SHOW_PROGRESS	45
#define	CONFIG_SYS_HZ		1000
#define	CONFIG_SYS_TIMERBASE	0xffd80000
#define	CONFIG_SYS_FLASH_BASE	0x00000000
#define	CONFIG_SYS_FLASH_SIZE	0x04000000	/* 64 MB */
#define	CONFIG_SYS_FLASH_BASE2	0x04000000
#define	CONFIG_SYS_FLASH_SIZE2	0x04000000	/* 64 MB */
#define	CONFIG_SYS_MAX_FLASH_BANKS	2
#define	CONFIG_SYS_FLASH_BANKS_LIST \
	{CONFIG_SYS_FLASH_BASE, CONFIG_SYS_FLASH_BASE2}
#define	CONFIG_SYS_FLASH_BANKS_SIZES \
	{CONFIG_SYS_FLASH_SIZE, CONFIG_SYS_FLASH_SIZE2}
#define	CONFIG_SYS_FLASH_QUIET_TEST
#define	CONFIG_SYS_MONITOR_BASE	CONFIG_SYS_FLASH_BASE
#define	CONFIG_SYS_MAX_FLASH_SECT	1024	/* sectors per dev */
#define	CONFIG_ENV_SECT_SIZE	0x40000
#define	CONFIG_ENV_SIZE		CONFIG_ENV_SECT_SIZE
#define	CONFIG_ENV_IS_IN_FLASH	1
#define	CONFIG_ENV_OFFSET	0x00040000
#define	CONFIG_ENV_ADDR	(CONFIG_SYS_FLASH_BASE + CONFIG_ENV_OFFSET)

/* misc */
#define	CONFIG_SYS_LONGHELP
#define CONFIG_SYS_PROMPT       "$ "
#define	CONFIG_SYS_CBSIZE	256
#define	CONFIG_SYS_PBSIZE \
	(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define	CONFIG_SYS_MAXARGS	16

#define	CONFIG_DISPLAY_BOARDINFO
#define	CONFIG_DISPLAY_CPUINFO
#define	CONFIG_EXTRA_ENV_SETTINGS	\
	"ethact=smc911x-0\0"		\
	"stderr=serial\0"		\
	"stdin=serial\0"		\
	"stdout=serial\0"

#define BOARD_LATE_INIT             /* call board_late_init during start up */

#define CONFIG_ARM_ERRATA_743622

#endif	/* __CONFIG_H */
