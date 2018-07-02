/*
 * (C) Copyright 2015 Renesas Electronics Europe Ltd
 *
 * SPDX-License-Identifier:	BSD-2-Clause
 */
#include <asm/io.h>
#include <common.h>
#include "renesas/rzn1-memory-map.h"
#include "renesas/rzn1-utils.h"
#include "renesas/rzn1-clocks.h"

/* Generic register/bit group descriptor */
struct rzn1_onereg {
	uint16_t reg : 7,	/* Register number (word) */
		pos : 5,	/* Bit number */
		size : 4;	/* Optional: size in bits */
} __attribute__((packed));

struct rzn1_clkdesc {
	struct rzn1_onereg clock, reset, ready, masteridle, scon, mirack, mistat;
} __attribute__((packed));

#define _BIT(_r, _p) { .reg = _r, .pos = _p }

#define _CLK(_n, _clk, _rst, _rdy, _midle, _scon, _mirack, _mistat) \
	{ .clock = _clk, .reset = _rst, .ready = _rdy, .masteridle = _midle, \
	  .scon = _scon, .mirack = _mirack, .mistat = _mistat }

#include "renesas/rzn1-clkctrl-tables.h"

#define clk_readl readl
#define clk_writel writel

static void *clk_mgr_base_addr = (void *)RZN1_SYSTEM_CTRL_BASE;

static void clk_mgr_desc_set(const struct rzn1_onereg *one, int on)
{
	u32 *reg = ((u32 *)clk_mgr_base_addr) + one->reg;
	u32 val = clk_readl(reg);
	val = (val & ~(1 << one->pos)) | ((!!on) << one->pos);
	clk_writel(val, reg);
}

static u32 clk_mgr_desc_get(const struct rzn1_onereg *one)
{
	u32 *reg = ((u32 *)clk_mgr_base_addr) + one->reg;
	u32 val = clk_readl(reg);
	val = (val >> one->pos) & 1;
	return val;
}

int rzn1_clk_set_gate(int clkdesc_id, int on)
{
	const struct rzn1_clkdesc *g = &rzn1_clock_list[clkdesc_id];
	u32 timeout;

#if defined (CONFIG_ARCH_RZN1L)
	/* On RZ/N1L, don't change these clocks */
	if (clkdesc_id == RZN1_CLK_CM3_ID ||
	    clkdesc_id == RZN1_HCLK_CM3_ID ||
	    clkdesc_id == RZN1_HCLK_ROM_ID)
		return 0;
#endif


	BUG_ON(!clk_mgr_base_addr);
	if (!g->clock.reg)
		return -1;

	/* Set 'Master Idle Request' bit */
	if (!on && g->masteridle.reg)
		clk_mgr_desc_set(&g->masteridle, 1);

	/* Wait for Master Idle Request acknowledge */
	if (!on && g->mirack.reg) {
		for (timeout = 100000; timeout &&
			!clk_mgr_desc_get(&g->mirack); timeout--)
				;
		if (!timeout)
			return -1;
	}

	/* Wait for Master Idle Status signal */
	if (!on && g->mistat.reg) {
		for (timeout = 100000; timeout &&
			!clk_mgr_desc_get(&g->mistat); timeout--)
				;
		if (!timeout)
			return -1;
	}

	/* Enable/disable the clock */
	clk_mgr_desc_set(&g->clock, on);

	/* De-assert reset */
	if (g->reset.reg)
		clk_mgr_desc_set(&g->reset, 1);

	/* Hardware manual recommends 5us delay after enabling clock & reset */
	udelay(5);

	/* If the peripheral is memory mapped (i.e. an AXI slave), there is an
	 * associated SLVRDY bit in the System Controller that needs to be set
	 * so that the FlexWAY bus fabric passes on the read/write requests.
	 */
	if (g->ready.reg)
		clk_mgr_desc_set(&g->ready, on);

	/* Wait for FlexWAY Socket Connection signal */
	if (on && g->scon.reg) {
		for (timeout = 100000; timeout &&
			!clk_mgr_desc_get(&g->scon); timeout--)
				;
		if (!timeout)
			return -1;
	}

	/* Clear 'Master Idle Request' bit */
	if (on && g->masteridle.reg)
		clk_mgr_desc_set(&g->masteridle, 0);


	/* Wait for Master Idle Status signal */
	if (on && g->mistat.reg) {
		for (timeout = 100000; timeout &&
			clk_mgr_desc_get(&g->mistat); timeout--)
				;
		if (!timeout)
			return -1;
	}

	return 0;
}

void rzn1_clk_reset(int clkdesc_id)
{
	const struct rzn1_clkdesc *g = &rzn1_clock_list[clkdesc_id];

	BUG_ON(!clk_mgr_base_addr);

	if (g->reset.reg) {
		clk_mgr_desc_set(&g->reset, 0);
		udelay(5);
		clk_mgr_desc_set(&g->reset, 1);
	}
}

void rzn1_clk_reset_state(int clkdesc_id, int level)
{
	const struct rzn1_clkdesc *g = &rzn1_clock_list[clkdesc_id];

	BUG_ON(!clk_mgr_base_addr);

	if (g->reset.reg)
		clk_mgr_desc_set(&g->reset, level);
}
