/*
 * Cadence DDR Controller
 *
 * Copyright (C) 2015 Renesas Electronics Europe Ltd
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 */

/*
 * The Cadence DDR Controller has a huge number of registers that principally
 * cover two aspects, DDR specific timing information and AXI bus interfacing.
 * Cadence's TCL script generates all of the register values for specific
 * DDR devices operating at a specific frequency. The TCL script uses Denali
 * SOMA files as inputs. The tool also generates the AXI bus register values as
 * well, however this driver assumes that users will want to modifiy these to
 * meet a specific application's needs.
 * Therefore, this driver is passed two arrays containing register values for
 * the DDR device specific information, and explicity sets the AXI registers.
 *
 * AXI bus interfacing:
 *  The controller has four AXI slaves connections, and each of these can be
 * programmed to accept requests from specific AXI masters (using their IDs).
 * The regions of DDR that can be accessed by each AXI slave can be set such
 * as to isolate DDR used by one AXI master from another. Further, the maximum
 * bandwidth allocated to each AXI slave can be set.
 */

#include <common.h>
#include <linux/sizes.h>
#include <asm/io.h>
#include "cadence_ddr_ctrl.h"

#if 0
	#define pr_debug(fmt, args...)	printf(fmt, ##args)
	#define pr_debug2(fmt, args...)	printf(fmt, ##args)
#else
	#define pr_debug(fmt, args...)
	#define pr_debug2(fmt, args...)
#endif

#define DDR_NR_AXI_PORTS		4
#define DDR_NR_ENTRIES			16

#define DDR_START_REG			(0)		/* DENALI_CTL_00 */
#define DDR_CS0_MR1_REG			(32 * 4)	/* DENALI_CTL_32 */
#define DDR_CS0_MR2_REG			(32 * 4 + 2)	/* DENALI_CTL_32 */
#define DDR_CS1_MR1_REG			(34 * 4 + 2)	/* DENALI_CTL_34 */
#define DDR_CS1_MR2_REG			(35 * 4)	/* DENALI_CTL_35 */
#define DDR_ECC_ENABLE_REG		(36 * 4 + 2)	/* DENALI_CTL_36 */
#define DDR_ECC_DISABLE_W_UC_ERR_REG	(37 * 4 + 2)	/* DENALI_CTL_37 */
#define DDR_HALF_DATAPATH_REG		(54 * 4)	/* DENALI_CTL_54 */
#define DDR_INTERRUPT_STATUS		(56 * 4)	/* DENALI_CTL_56 */
#define DDR_INTERRUPT_ACK		(57 * 4)	/* DENALI_CTL_57 */
#define DDR_INTERRUPT_MASK		(58 * 4)	/* DENALI_CTL_58 */
#define DDR_CS0_ODT_MAP_REG		(62 * 4 + 2)	/* DENALI_CTL_62 */
#define DDR_CS1_ODT_MAP_REG		(63 * 4)	/* DENALI_CTL_63 */
#define DDR_ODT_TODTL_2CMD		(63 * 4 + 2)	/* DENALI_CTL_63 */
#define DDR_ODT_TODTH_WR		(63 * 4 + 3)	/* DENALI_CTL_63 */
#define DDR_ODT_TODTH_RD		(64 * 4 + 0)	/* DENALI_CTL_64 */
#define DDR_ODT_EN			(64 * 4 + 1)	/* DENALI_CTL_64 */
#define DDR_ODT_WR_TO_ODTH		(64 * 4 + 2)	/* DENALI_CTL_64 */
#define DDR_ODT_RD_TO_ODTH		(64 * 4 + 3)	/* DENALI_CTL_64 */
#define DDR_DIFF_CS_DELAY_REG		(66 * 4)	/* DENALI_CTL_66 */
#define DDR_SAME_CS_DELAY_REG		(67 * 4)	/* DENALI_CTL_67 */
#define DDR_RW_PRIORITY_REGS		(87 * 4 + 2)	/* DENALI_CTL_87 */
#define DDR_AXI_ALIGN_STRB_DISABLE_REG	(90 * 4 + 2)	/* DENALI_CTL_90 */
#define DDR_AXI_PORT_PROT_ENABLE_REG	(90 * 4 + 3)	/* DENALI_CTL_90 */
#define DDR_ADDR_RANGE_REGS		(91 * 4)	/* DENALI_CTL_91 */
#define DDR_RANGE_PROT_REGS		(218 * 4 + 2)	/* DENALI_CTL_218 */
#define  RANGE_PROT_BITS_PRIV_SECURE	0
#define  RANGE_PROT_BITS_SECURE		1
#define  RANGE_PROT_BITS_PRIV		2
#define  RANGE_PROT_BITS_FULL		3
#define DDR_ARB_CMD_Q_THRESHOLD_REG	(346 * 4 + 2)	/* DENALI_CTL_346 */
#define DDR_AXI_PORT_BANDWIDTH_REG	(346 * 4 + 3)	/* DENALI_CTL_346 */

static void ddrc_writeb(u8 val, void *p)
{
	pr_debug2("DDR: %p = 0x%02x\n", p, val);
	writeb(val, p);
}

static void ddrc_writew(u16 val, void *p)
{
	pr_debug2("DDR: %p = 0x%04x\n", p, val);
	writew(val, p);
}

static void ddrc_writel(u32 val, void *p)
{
	pr_debug2("DDR: %p = 0x%08x\n", p, val);
	writel(val, p);
}

void cdns_ddr_set_mr1(void *base, int cs, u16 odt_impedance, u16 drive_strength)
{
	void *reg;
	u16 tmp;

	if (cs == 0)
		reg = (u8 *)base + DDR_CS0_MR1_REG;
	else
		reg = (u8 *)base + DDR_CS1_MR1_REG;

	tmp = readw(reg);

	tmp &= ~MODE_REGISTER_MASK;
	tmp |=  MODE_REGISTER_MR1;

	tmp &= ~MR1_ODT_IMPEDANCE_MASK;
	tmp |=  odt_impedance;

	tmp &= ~MR1_DRIVE_STRENGTH_MASK;
	tmp |=  drive_strength;

	writew(tmp, reg);
}

void cdns_ddr_set_mr2(void *base, int cs, u16 dynamic_odt, u16 self_refresh_temp)
{
	void *reg;
	u16 tmp;

	if (cs == 0)
		reg = (u8 *)base + DDR_CS0_MR2_REG;
	else
		reg = (u8 *)base + DDR_CS1_MR2_REG;

	tmp = readw(reg);

	tmp &= ~MODE_REGISTER_MASK;
	tmp |=  MODE_REGISTER_MR2;

	tmp &= ~MR2_DYNAMIC_ODT_MASK;
	tmp |=  dynamic_odt;

	tmp &= ~MR2_SELF_REFRESH_TEMP_MASK;
	tmp |=  self_refresh_temp;

	writew(tmp, reg);
}

void cdns_ddr_set_odt_map(void *base, int cs, u16 odt_map)
{
	void *reg;

	if (cs == 0)
		reg = (u8 *)base + DDR_CS0_ODT_MAP_REG;
	else
		reg = (u8 *)base + DDR_CS1_ODT_MAP_REG;

	writew(odt_map, reg);
}

void cdns_ddr_set_odt_times(void *base, u8 TODTL_2CMD, u8 TODTH_WR, u8 TODTH_RD,
			    u8 WR_TO_ODTH, u8 RD_TO_ODTH)
{
	writeb(TODTL_2CMD,	(u8 *)base + DDR_ODT_TODTL_2CMD);
	writeb(TODTH_WR,	(u8 *)base + DDR_ODT_TODTH_WR);
	writeb(TODTH_RD,	(u8 *)base + DDR_ODT_TODTH_RD);
	writeb(1,		(u8 *)base + DDR_ODT_EN);
	writeb(WR_TO_ODTH,	(u8 *)base + DDR_ODT_WR_TO_ODTH);
	writeb(RD_TO_ODTH,	(u8 *)base + DDR_ODT_RD_TO_ODTH);
}

void cdns_ddr_set_same_cs_delays(void *base, u8 r2r, u8 r2w, u8 w2r, u8 w2w)
{
	u32 val = (w2w << 24) | (w2r << 16) | (r2w << 8) | r2r;

	writel(val, (u8 *)base + DDR_SAME_CS_DELAY_REG);
}

void cdns_ddr_set_diff_cs_delays(void *base, u8 r2r, u8 r2w, u8 w2r, u8 w2w)
{
	u32 val = (w2w << 24) | (w2r << 16) | (r2w << 8) | r2r;

	writel(val, (u8 *)base + DDR_DIFF_CS_DELAY_REG);
}

void cdns_ddr_set_port_rw_priority(void *base, int port,
			  u8 read_pri, u8 write_pri, u8 fifo_type)
{
	u8 *reg8 = (u8 *)base + DDR_RW_PRIORITY_REGS;

	reg8 += (port * 3);
	pr_debug("%s port %d (reg8=%p, DENALI_CTL_%d)\n",
			__func__, port, reg8, (reg8-(u8 *)base)/4);

	ddrc_writeb(read_pri, reg8++);
	ddrc_writeb(write_pri, reg8++);
	ddrc_writeb(fifo_type, reg8++);
}

/* The DDR Controller has 16 entries. Each entry can specify an allowed address
 * range (with 16KB resolution) for one of the 4 AXI slave ports.
 */
void cdns_ddr_enable_port_addr_range_x(void *base, int port, int entry,
			      u32 addr_start, u32 size)
{
	u32 addr_end;
	u32 *reg32 = (u32 *)((u8 *)base + DDR_ADDR_RANGE_REGS);
	u32 tmp;

	reg32 += (port * DDR_NR_ENTRIES * 2);
	reg32 += (entry * 2);
	pr_debug("%s port %d, entry %d (reg32=%p, DENALI_CTL_%d)\n",
			__func__, port, entry, reg32, ((u8 *)reg32-(u8 *)base)/4);

	/* These registers represent 16KB address blocks */
	addr_start /= SZ_16K;
	size /= SZ_16K;
	if (size)
		addr_end = addr_start + size - 1;
	else
		addr_end = addr_start;

	ddrc_writel(addr_start, reg32++);

	/* end_addr: Ensure we only set the bottom 18-bits as DENALI_CTL_218
	 * also contains the AXI0 range protection bits. */
	tmp = readl(reg32);
	tmp &= ~(BIT(18) - 1);
	tmp |= addr_end;
	ddrc_writel(tmp, reg32);
}

void cdns_ddr_enable_port_prot_x(void *base, int port, int entry,
	u8 range_protection_bits,
	u16 range_RID_check_bits,
	u16 range_WID_check_bits,
	u8 range_RID_check_bits_ID_lookup,
	u8 range_WID_check_bits_ID_lookup)
{
	/*
	 * Technically, the offset here points to the byte before the start of
	 * the range protection registers. However, all entries consist of 8
	 * bytes, except the first one (which is missing a padding byte) so we
	 * work around that subtlely.
	 */
	u8 *reg8 = (u8 *)base + DDR_RANGE_PROT_REGS;

	reg8 += (port * DDR_NR_ENTRIES * 8);
	reg8 += (entry * 8);
	pr_debug("%s port %d, entry %d (reg8=%p, DENALI_CTL_%d)\n",
			__func__, port, entry, reg8, (reg8-(u8 *)base)/4);

	if (port == 0 && entry == 0)
		ddrc_writeb(range_protection_bits, reg8 + 1);
	else
		ddrc_writeb(range_protection_bits, reg8);

	ddrc_writew(range_RID_check_bits, reg8 + 2);
	ddrc_writew(range_WID_check_bits, reg8 + 4);
	ddrc_writeb(range_RID_check_bits_ID_lookup, reg8 + 6);
	ddrc_writeb(range_WID_check_bits_ID_lookup, reg8 + 7);
}

void cdns_ddr_set_port_bandwidth(void *base, int port,
		        u8 max_percent, u8 overflow_ok)
{
	u8 *reg8 = (u8 *)base + DDR_AXI_PORT_BANDWIDTH_REG;

	reg8 += (port * 3);
	pr_debug("%s port %d, (reg8=%p, DENALI_CTL_%d)\n",
			__func__, port, reg8, (reg8-(u8 *)base)/4);

	ddrc_writeb(max_percent, reg8++);	/* Maximum bandwidth percentage */
	ddrc_writeb(overflow_ok, reg8++);	/* Bandwidth overflow allowed */
}

void cdns_ddr_ctrl_init(void *ddr_ctrl_basex, int async,
			const u32 *reg0, const u32 *reg350,
			u32 ddr_start_addr, u32 ddr_size)
{
	int i, axi, entry;
	u32 *ddr_ctrl_base = (u32 *)ddr_ctrl_basex;
	u8 *base8 = (u8 *)ddr_ctrl_basex;

	pr_debug("%s\n", __func__);
	ddrc_writel(*reg0, ddr_ctrl_base + 0);
	/* 1 to 6 are read only */
	for (i = 7; i <= 26; i++)
		ddrc_writel(*(reg0 + i), ddr_ctrl_base + i);
	/* 27 to 29 are not changed */
	for (i = 30; i <= 87; i++)
		ddrc_writel(*(reg0 + i), ddr_ctrl_base + i);

	/* Enable/disable ECC */
#if defined(CONFIG_CADENCE_DDR_CTRL_ENABLE_ECC)
	pr_debug("%s enabling ECC\n", __func__);
	ddrc_writeb(1, base8 + DDR_ECC_ENABLE_REG);
#else
	ddrc_writeb(0, base8 + DDR_ECC_ENABLE_REG);
#endif

	/* ECC: Disable corruption for read/modify/write operations */
	ddrc_writeb(1, base8 + DDR_ECC_DISABLE_W_UC_ERR_REG);

	/* ECC: AXI aligned strobe disable => enable */
	ddrc_writeb(0, base8 + DDR_AXI_ALIGN_STRB_DISABLE_REG);

	/* Set 8/16-bit data width using reduc bit (enable half datapath)*/
#if defined(CONFIG_CADENCE_DDR_CTRL_8BIT_WIDTH)
	pr_debug("%s using 8-bit data\n", __func__);
	ddrc_writeb(1, base8 + DDR_HALF_DATAPATH_REG);
#else
	ddrc_writeb(0, base8 + DDR_HALF_DATAPATH_REG);
#endif

	/* Threshold for command queue */
	ddrc_writeb(0x03, base8 + DDR_ARB_CMD_Q_THRESHOLD_REG);

	/* AXI port protection => enable */
	ddrc_writeb(0x01, base8 + DDR_AXI_PORT_PROT_ENABLE_REG);

	/*
	 * We enable all AXI slave ports with the same settings that simply
	 * ensures all accesses to ports are in the valid address range.
	 */
	for (axi = 0; axi < DDR_NR_AXI_PORTS; axi++) {
		/* R/W priorities */
		if (async)
			cdns_ddr_set_port_rw_priority(ddr_ctrl_base, axi, 2, 2, 0);
		else
			cdns_ddr_set_port_rw_priority(ddr_ctrl_base, axi, 2, 2, 3);

		/* DDR address range */
		cdns_ddr_enable_port_addr_range_x(ddr_ctrl_base, axi, 0,
					 ddr_start_addr, ddr_size);

		/*
		 * The hardware requires that the address ranges must not
		 * overlap in order to trigger a valid address. So, we set all
		 * other address range entries to be above the DDR, length 0.
		 */
		for (entry = 1; entry < DDR_NR_ENTRIES; entry++) {
			cdns_ddr_enable_port_addr_range_x(ddr_ctrl_base, axi,
					entry, ddr_start_addr + ddr_size, 0);
		}

		/* Access rights, see manual for details */
		cdns_ddr_enable_port_prot_x(ddr_ctrl_base, axi, 0,
				   RANGE_PROT_BITS_FULL,
				   0xffff, 0xffff, 0x0f, 0x0f);

		/* AXI bandwidth */
		cdns_ddr_set_port_bandwidth(ddr_ctrl_base, axi, 50, 1);
	}

	for (i = 350; i <= 374; i++)
		ddrc_writel(*(reg350 - 350 + i), ddr_ctrl_base + i);

	/*
	 * Disable all interrupts, we are not handling them.
	 * For detail of the interrupt mask, ack and status bits, see the
	 * manual's description of the 'int_status' parameter.
	 */
	ddrc_writel(0, base8 + DDR_INTERRUPT_MASK);
}

void cdns_ddr_ctrl_start(void *ddr_ctrl_basex)
{
	u32 *ddr_ctrl_base = (u32 *)ddr_ctrl_basex;
	u8 *base8 = (u8 *)ddr_ctrl_basex;

	/* Start */
	ddrc_writeb(1, base8 + DDR_START_REG);

	/* Wait for controller to be ready (interrupt status) */
	while (!(readl(base8 + DDR_INTERRUPT_STATUS) & 0x100))
		;

	/* clear all interrupts */
	ddrc_writel(~0, base8 + DDR_INTERRUPT_ACK);

	/* Step 19 Wait 500us from MRESETB=1 */
	udelay(500);

	/* Step 20 tCKSRX wait (From supply stable clock for MCK) */
	/* DENALI_CTL_19 TREF_ENABLE=0x1(=1), AREFRESH=0x1(=1) */
	ddrc_writel(0x01000100, ddr_ctrl_base + 19);
}
