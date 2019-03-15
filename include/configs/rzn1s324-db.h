/*
 * Renesas RZN1S-324 Demo board
 *
 * (C) Copyright 2017 Renesas Electronics Europe Ltd
 * Phil Edworthy <phil.edworthy@renesas.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __RZN1S324_DB_H
#define __RZN1S324_DB_H

#include "rzn1-common.h"
#include "renesas/rzn1-memory-map.h"
#include "renesas/rzn1-sysctrl.h"

#define CONFIG_BOARD_NAME	"Renesas RZN1S-DB"

/* The RZ/N1S can only support a 45MHz SDHCI clock on this board */
#undef RZN1_SDHC_PLL_DIV
#undef SDHC_CLK_MHZ
#define RZN1_SDHC_PLL_DIV		22
#define SDHC_CLK_MHZ			(1000 / RZN1_SDHC_PLL_DIV)

/*
 * U-Boot switches into NONSEC mode based on the "bootm_boot_mode" environment
 * variable and the CONFIG_ARMV7_BOOT_SEC_DEFAULT symbol.
 * However, RZ/N1 checks an additional "boot_hyp" environment variable to
 * decide whether to switch to NONSEC+HYP mode.
 */
#define CONFIG_SMP_PEN_ADDR		smp_secondary_bootaddr
#define CONFIG_OF_BOARD_SETUP
#define CONFIG_BOARD_LATE_INIT

#define CONFIG_SYS_THUMB_BUILD
#define CONFIG_SYS_LONGHELP
#define	CONFIG_CMDLINE_EDITING
#define CONFIG_VERSION_VARIABLE		/* include version env variable */
/* debug purpose */
#define CONFIG_MD5
#define CONFIG_CMD_MD5SUM

/* Local helper symbols to enable/disable functionality */
#define RZN1_ENABLE_I2C
#define RZN1_ENABLE_GPIO
#define RZN1_ENABLE_SDHC
#define RZN1_ENABLE_ETHERNET
#define RZN1_ENABLE_QSPI
#undef RZN1_ENABLE_NAND
#define RZN1_ENABLE_USBF
#define RZN1_ENABLE_USBH
#define RZN1_ENABLE_SPL

/* GPIOs */
#define CONFIG_RZN1_GPIO

/* FPGA reprogramming */
#define CONFIG_FPGA
#define CONFIG_FPGA_LATTICE
#define CONFIG_FPGA_COUNT	1

/* SRAM */
#define CONFIG_SYS_TEXT_BASE		0x200a0000
#define CONFIG_SYS_STAY_IN_SRAM
/* Very early stack, 320KB above start of U-Boot image */
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_TEXT_BASE + (320 * 1024) - 4)
#define CONFIG_SYS_MALLOC_LEN		(320 * 1024)

#define	CONFIG_SYS_ALT_MEMTEST
#define CONFIG_SYS_MEMTEST_SCRATCH	RZN1_SRAM_SYS_BASE

/* This is not used by uboot startup, just by qspi etc */
#define CONFIG_SYS_LOAD_ADDR		0x80000000

/* Serial */
#define CONFIG_BAUDRATE			115200


/* ENV settings.
 * To avoid U-Boot accessing NAND/QSPI, you might want to set NOWHERE instead.
 */
#if defined(RZN1_ENABLE_QSPI)
 #define CONFIG_ENV_IS_IN_SPI_FLASH
#else
 #define CONFIG_ENV_IS_NOWHERE		/* Store ENV in memory only */
#endif

/* The ENV offsets match the DFU offsets for *_env */
#if defined(CONFIG_ENV_IS_IN_SPI_FLASH)
 #define CONFIG_ENV_OFFSET		0xa0000
 #define CONFIG_ENV_SECT_SIZE		(64 * 1024)
#endif
#define CONFIG_ENV_SIZE			(8 * 1024)


/***** SPI Flash *****/
#if defined(RZN1_ENABLE_QSPI)
 #define CONFIG_SF_DEFAULT_SPEED	62500000
 #define CONFIG_ENV_SPI_MAX_HZ		62500000
/* Reading using QuadIO can achieve 20% better throughput compared to QuadOutput
 * on this board. However, the if you want to use the same U-Boot binary on
 * boards with a second source SPI Flash, check that all use the same number of
 * dummy cycles in this mode. If not, you may want to use the QuadOuput read
 * command instead */
/* Macronix MX25L25635F : 8 dummy cycles for Quad Output Fast,  6 for QuadIO */
/* Micron   N25Q128     : 8 dummy cycles for Quad Output Fast, 10 for QuadIO */
 #define CONFIG_SPI_FLASH_READ_QUAD_CMD	CMD_READ_QUAD_IO_FAST
 #define CONFIG_SPI_FLASH_DUMMY_CYCLES	6
 #define CONFIG_CQSPI_DECODER		0	/* i.e. don't use 4-to-16 CS decoder */
#endif /* RZN1_ENABLE_QSPI */

#define CONFIG_SYS_MAX_FLASH_BANKS	1


/***** Ethernet *****/
#if defined(RZN1_ENABLE_ETHERNET)
 #define CONFIG_MII
 #define CONFIG_PHY_GIGE		/* Include GbE speed/duplex detection */
 #define CONFIG_PHY_MICREL
 #define CONFIG_PHY_MARVELL
 #define CONFIG_PHY_RESET_DELAY		1000	/* PHY RESET recovery delay in usec */

/*
 * GMAC1 is connected to a Marvell PHY on the Extension board
 * RGMII/GMII Conv   PHY Addr    PHY     Connector
 *        1             8       Marvell  J22 (Ext Board)
 */
 #define CONFIG_PHY_ADDR		8

/*
 * GMAC2 is connected to the 5-port Switch and all ports are enabled.
 * Two Micrel PHYs are on the CPU board, two additional Marvell PHYs are on the
 * Extension board. The board setup code specifies the MII interface type used,
 * see call to designware_initialize_fixed_link().
 * We can only use one PHY in U-Boot.
 * Switch Port   RGMII/GMII Conv   PHY Addr    PHY     Connector
 *      3               2             1       Marvell  J23 (Ext Board)
 *      2               3            10       Marvell  J24 (Ext Board)
 *      1               4             4       Micrel   CN1
 *      0               5             5       Micrel   CN4
 */
 #define CONFIG_HAS_ETH1
 #define CONFIG_PHY1_ADDR		4
#endif /* RZN1_ENABLE_ETHERNET */

/***** SDHC *****/
#if defined(RZN1_ENABLE_SDHC)
 #define CONFIG_SDHCI_ARASAN_QUIRKS	SDHCI_QUIRK_WAIT_SEND_CMD
 #define CONFIG_GENERIC_MMC
 #define CONFIG_DOS_PARTITION
 #define CONFIG_FS_FAT_MAX_CLUSTSIZE (8 * 1024)
#endif /* RZN1_ENABLE_SDHC */

#if defined(RZN1_ENABLE_USBH)
 #define CONFIG_USB_EHCI
 #define CONFIG_USB_EHCI_RMOBILE
 #define CONFIG_USB_MAX_CONTROLLER_COUNT	1
 #define CONFIG_SYS_USB_OHCI_MAX_ROOT_PORTS	1
#endif /* RZN1_ENABLE_USBH */

/***** USB Device aka Gadget aka Function *****/
#if defined(RZN1_ENABLE_USBF)
 /* Default is... 8 MB !! Note that it needs to be AT LEAST the size of
  * a single erase block of the nand/qspi, otherwise the operation will fail */
 #define CONFIG_SYS_DFU_DATA_BUF_SIZE	(64 * 1024)
 #define DFU_DEFAULT_POLL_TIMEOUT	100

 #define DFU_EXT_INFO \
	"dfu_ext_info=" \
	"sf sf_spl raw 0 10000;" \
	"sf sf_rpkgt raw 10000 10000;" \
	"sf sf_uboot raw 20000 80000;" \
	"sf sf_env raw a0000 10000;" \
	"sf sf_dtb raw b0000 20000;" \
	"sf sf_cm3 raw d0000 100000;" \
	"sf sf_kernel raw 1d0000 600000;" \
	"sf sf_data raw 7d0000 0;" \
	"sf sf_vxworks raw d0000 600000;" \
	"ram r_kernel ram 80000000 400000;" \
	"sf 1:0 qspi1 raw 0 0\0"
#else
 #define DFU_EXT_INFO
#endif /* RZN1_ENABLE_USBF */

#define CONFIG_BOOTDELAY 1
#define CONFIG_BOOTCOMMAND ""
/* Default environment variables */
#define CONFIG_EXTRA_ENV_SETTINGS \
	DFU_EXT_INFO \
	"bootdelay=1\0" \
	"loadaddr=80000000\0" \
	"ethaddr=00:00:0a:02:57:CD\0" \
	"eth1addr=00:00:0a:02:57:CE\0" \
	"netmask=255.255.255.0\0" \
	"serverip=192.168.1.30\0" \
	"ipaddr=192.168.1.60\0"


/***** SPL (Secondary Program Loader) *****/
#if defined(RZN1_ENABLE_SPL)

/* U-Boot/SPL is at the top of UA SRAM, out of the way of CM3 code which has to
 * be at the start of UA SRAM. These definitions setup SPL so that code, heap,
 * and stack at located together. */
#define CONFIG_SPL_MAX_FOOTPRINT	(55 * 1024)
#define CONFIG_SYS_SPL_MALLOC_SIZE	(38 * 1024)
#define RZN1_SPL_STACK_SIZE		(3 * 1024)

#define RZN1_SPL_SRAM_SIZE		(CONFIG_SPL_MAX_FOOTPRINT + \
						CONFIG_SYS_SPL_MALLOC_SIZE + \
						RZN1_SPL_STACK_SIZE)
#define CONFIG_SPL_TEXT_BASE		0x040e0000

#define CONFIG_SYS_SPL_MALLOC_START	(CONFIG_SPL_TEXT_BASE + CONFIG_SPL_MAX_FOOTPRINT)
#define CONFIG_SPL_STACK		(CONFIG_SYS_SPL_MALLOC_START + CONFIG_SYS_SPL_MALLOC_SIZE + RZN1_SPL_STACK_SIZE)

#if CONFIG_SPL_STACK >= (RZN1_SRAM_ID_BASE + RZN1_SRAM_ID_SIZE - 1024)
 #error "On RZ/N1, the top 1KB is reserved for the PKG Table. This needs a\
 fixed location when wrapped in an SPKG, due to signature verification."
#endif

/* If QSPI is enabled, default to loading the image or Package Table from QSPI */
#if defined(RZN1_ENABLE_QSPI)
 #define CONFIG_SPL_SPI_LOAD
#endif

#include "rzn1-spl.h"
/* Offset corresponds to DFU sf_rpkgt */
#define CONFIG_SYS_SPI_U_BOOT_OFFS      0x10000

/*
 * This is the hash of the public key used to sign BLp wrapped images that is
 * loaded by U-Boot/SPL. If you are not verifying the signature of packages, it
 * will not be used.
 */
#define RZN1_SPL_RPKG_HASH \
	0x49, 0x7f, 0x19, 0x56, \
	0x62, 0x5a, 0x91, 0x7e, \
	0x4b, 0x61, 0x5c, 0x06, \
	0x5c, 0x8c, 0x39, 0x7c, \
	0xdb, 0xbf, 0x1e, 0x97, \
	0x9c, 0xa5, 0x7c, 0x84, \
	0xe3, 0x97, 0x3a, 0x85, \
	0xfb, 0x5a, 0x3f, 0x24

#endif /* RZN1_ENABLE_SPL */

#endif /* __RZN1S324_DB_H */
