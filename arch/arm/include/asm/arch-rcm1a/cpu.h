/*
 * arch/arm/include/asm/arch-rcm1a/cpu.h
 *
 * Copyright (C) 2011 Renesas Electronics Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

extern void board_reset(void);
extern void invalidate_dcache(void);
extern void wait_usec(int);

#define	SetREG(x) \
	writel((readl((x)->addr) & ~((x)->mask)) | ((x)->val), (x)->addr)
#define	SetGuardREG(x) \
{ \
	u32	val; \
	val = (readl((x)->addr) & ~((x)->mask)) | ((x)->val); \
	writel(~val, PMMR); \
	writel(val, (x)->addr); \
}

/* pfc */
#define	PMMR		0xfffc0000
#define	GPSR0		0xfffc0004
#define	GPSR1		0xfffc0008
#define	GPSR2		0xfffc000c
#define	GPSR3		0xfffc0010
#define	GPSR4		0xfffc0014
#define	IPSR0		0xfffc0020
#define	IPSR1		0xfffc0024
#define	IPSR2		0xfffc0028
#define	IPSR3		0xfffc002c
#define	IPSR4		0xfffc0030
#define	IPSR5		0xfffc0034
#define	IPSR6		0xfffc0038
#define	IPSR7		0xfffc003c
#define	IPSR8		0xfffc0040
#define	IPSR9		0xfffc0044
#define	IPSR10		0xfffc0048
#define	MOD_SEL		0xfffc0050
#define	MOD_SEL2	0xfffc0054
#define	PUPR0		0xfffc0100
#define	PUPR1		0xfffc0104
#define	PUPR2		0xfffc0108
#define	PUPR3		0xfffc010c
#define	PUPR4		0xfffc0110
#define	PUPR5		0xfffc0114

/* reset/wdt */
#define	MODEMR	0xffcc0020
#define		MD1	(1 << 1)
#define		MD2	(1 << 2)
#define		MD18	(1 << 18)
#define		MD19	(1 << 19)

/* dbsc */
#define	MEMC_BASE	0xfe800000
#define		DBSTATE1	0x00c
#define		DBACEN		0x010
#define		DBRFEN		0x014
#define		DBCMD		0x018
#define		DBWAIT		0x01c
#define		DBKIND		0x020
#define		DBCONF0		0x024
#define		DBTR0		0x040
#define		DBTR1		0x044
#define		DBTR2		0x048
#define		DBTR3		0x050
#define		DBTR4		0x054
#define		DBTR5		0x058
#define		DBTR6		0x05c
#define		DBTR7		0x060
#define		DBTR8		0x064
#define		DBTR9		0x068
#define		DBTR10		0x06c
#define		DBTR11		0x070
#define		DBTR12		0x074
#define		DBTR13		0x078
#define		DBTR14		0x07c
#define		DBTR15		0x080
#define		DBTR16		0x084
#define		DBTR17		0x088
#define		DBTR18		0x08c
#define		DBTR19		0x090
#define		DBADJ0		0x0c0
#define		DBADJ1		0x0c4
#define		DBADJ2		0x0c8
#define		DBRFCNF0	0x0e0
#define		DBRFCNF1	0x0e4
#define		DBRFCNF2	0x0e8
#define		DBCALCNF	0x0f4
#define		DBCALTR		0x0f8
#define		DBRNK0		0x100
#define		DBPDNCNF	0x180
#define		DBPDCNT0	0x200
#define		DBPDCNT1	0x204
#define		DBPDCNT2	0x208
#define		DBPDCNT3	0x20c
#define		DBBS0CNT0	0x300
#define		DBBS0CNT1	0x304
#define	MEMC_W(offset, val)	writel(val, MEMC_BASE + (offset))
#define	MEMC_R(offset)		readl(MEMC_BASE + (offset))

/* lbsc */
#define	EXB_BASE	0xff800000
#define		CS0CTRL		0x200
#define		CS1CTRL		0x204
#define		ECS0CTRL	0x208
#define		ECS1CTRL	0x20c
#define		ECS2CTRL	0x210
#define		ECS3CTRL	0x214
#define		ECS4CTRL	0x218
#define		ECS5CTRL	0x21c
#define		CS0CTRL2	0x220
#define		CSWCR0		0x230
#define		CSWCR1		0x234
#define		ECSWCR0		0x238
#define		ECSWCR1		0x23c
#define		ECSWCR2		0x240
#define		ECSWCR3		0x244
#define		ECSWCR4		0x248
#define		ECSWCR5		0x24c
#define		CSPWCR0		0x280
#define		CSPWCR1		0x284
#define		EXSPWCR0	0x288
#define		EXSPWCR1	0x28c
#define		EXSPWCR2	0x290
#define		EXSPWCR3	0x294
#define		EXSPWCR4	0x298
#define		EXSPWCR5	0x29c
#define		EXWTSYNC	0x2a0
#define		CS0BSTCTL	0x2b0
#define		CS0BTPH		0x2b4
#define		CS1GDST		0x2c0
#define		ECS0GDST	0x2c4
#define		ECS1GDST	0x2c8
#define		ECS2GDST	0x2cc
#define		ECS3GDST	0x2d0
#define		ECS4GDST	0x2d4
#define		ECS5GDST	0x2d8
#define		EXDMASET0	0x2f0
#define		EXDMACR0	0x310
#define		EXDMACR1	0x314
#define		EXDMACR2	0x318
#define		BCINTMR		0x338
#define		EXBATLV		0x340
#define		ATACSCTRL	0x380
#define	EXB_W(offset, val)	writel(val, EXB_BASE + (offset))

/* tmu */
#define	TBASE	CONFIG_SYS_TIMERBASE
#define		TSTR0		0x04	/* timer start register (8bit) */
#define		TCOR0		0x08	/* timer constant register (32bit) */
#define		TCNT0		0x0c	/* timer counter (32bit) */
#define		TCR0		0x10	/* timer control register (16bit) */
#define	STR0	(1 << 0)	/* TSTR: start tcnt0 */
#define	TCLOCK	67		/* for wait_usec */

/* scif */
#define	SCIF_BASE	SCIF0_BASE
#define		SCIF_DL		0x30
#define		SCIF_CKS	0x34
#define	CKS_EXTERNAL	0	/* CKS: external clock */
