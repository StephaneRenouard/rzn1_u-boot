/*
 * arch/arm/include/asm/arch-rch1/cpu.h
 *
 * Copyright (C) 2011-2012 Renesas Electronics Corporation
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

/* Common-PF@H1 */

/* MERAM */
#define	ICB_SWAP	0xfffe1004
#define	ICB_TRNCTRL	0xfffe100c
#define VPC_TRNCTRL 0xfffe1014

/* SYSC */
#define SYSCSR		0xffd85000
#define SYSCISR		0xffd85004
#define SYSCISCR	0xffd85008
#define SYSCIER		0xffd8500c
#define SYSCIMR		0xffd85010
#define SYSCOFSR	0xffd85020
#define SYSCOFSCR	0xffd85024
#define SYSCOFSER	0xffd85028

#define PWRSR0		0xffd85040
#define PWROFFCR0	0xffd85044
#define PWROFFSR0	0xffd85048
#define PWRONCR0	0xffd8504c
#define PWRONSR0	0xffd85050
#define PWRER0		0xffd85054

#define PWRSR2		0xffd850c0
#define PWROFFCR2	0xffd850c4
#define PWROFFSR2	0xffd850c8
#define PWRONCR2	0xffd850cc
#define PWRONSR2	0xffd850d0
#define PWRER2		0xffd850d4

#define PWRSR3		0xffd85100
#define PWROFFCR3	0xffd85104
#define PWROFFSR3	0xffd85108
#define PWRONCR3	0xffd8510c
#define PWRONSR3	0xffd85110
#define PWRER3		0xffd85114

#define PWRSR4		0xffd85140
#define PWROFFCR4	0xffd85144
#define PWROFFSR4	0xffd85148
#define PWRONCR4	0xffd8514c
#define PWRONSR4	0xffd85150
#define PWRER4		0xffd85154

/* cpg */
#define	FRQMR		0xffc80014
#define	MSTPCR0		0xffc80030
#define	MSTPCR1		0xffc80034
#define	MSTPCR3		0xffc8003c
#define	MSTPSR1		0xffc80044
#define	MSTPSR4		0xffc80048
#define	MSTPSR6		0xffc8004c
#define	MSTPCR4		0xffc80050
#define	MSTPCR5		0xffc80054
#define	MSTPCR6		0xffc80058
#define	MSTPCR7		0xffc80040
#define	SH4AIFC		0xfe400000
#define	SH4ASTBCR	0xfe400004

/* pfc */
#define	PMMR		0xfffc0000
#define	GPSR0		0xfffc0004
#define	GPSR1		0xfffc0008
#define	GPSR2		0xfffc000c
#define	GPSR3		0xfffc0010
#define	GPSR4		0xfffc0014
#define	GPSR5		0xfffc0018
#define	GPSR6		0xfffc001c
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
#define	IPSR11		0xfffc004c
#define	IPSR12		0xfffc0050
#define	IOCTRL0		0xfffc0060
#define	IOCTRL1		0xfffc0064
#define	IOCTRL2		0xfffc0068
#define	IOCTRL3		0xfffc006c
#define	MOD_SEL		0xfffc0090
#define	MOD_SEL2	0xfffc0094
#define	PUPR0		0xfffc0100
#define	PUPR1		0xfffc0104
#define	PUPR2		0xfffc0108
#define	PUPR3		0xfffc010c
#define	PUPR4		0xfffc0110
#define	PUPR5		0xfffc0114
#define	PUPR6		0xfffc0118
#define	ARMTREN		0xfffc0200
#define	RGWD0		0xfffc0210
#define	RGWD1		0xfffc0214
#define	RGWD2		0xfffc0218
#define	RGWD3		0xfffc021c
#define	RGWD4		0xfffc0220
#define	RGWD5		0xfffc0224
#define	RGRD0		0xfffc0230
#define	RGRD1		0xfffc0234
#define	RGRD2		0xfffc0238

/* reset/wdt */
#define	MODEMR	0xffcc0020
#define		MD1	(1 << 1)
#define		MD2	(1 << 2)

#define WDT_BASE	0xffcc0000
#define		WDTST		0x000
#define		WDTCSR		0x004
#define		WDTST		0x000

#define	WDTST_CODE	0x5a000000

#define	WDTCSR_CODE	0xa5000000
#define	WDTCSR_WT	(WDTCSR_CODE | 0x40)
#define	WDTCSR_TME	(WDTCSR_CODE | 0x80)

/* dbsc */
#define	MEMC_BASE	0xfe800000
#define	MEMC2_BASE	0xfec00000
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
#define		DBBL		0x0b0
#define		DBADJ0		0x0c0
#define		DBADJ1		0x0c4
#define		DBADJ2		0x0c8
#define		DBADJ3		0x0cc
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
#define	MEMC_W(base, offset, val)	writel(val, (base) + (offset))
#define	MEMC_R(base, offset)		readl((base) + (offset))

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
#define		EXDMAWCR0	0x250
#define		EXDMAWCR1	0x254
#define		EXDMAWCR2	0x258
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
#define		EXDMASET1	0x2f4
#define		EXDMASET2	0x2f8
#define		EXDMACR0	0x310
#define		EXDMACR1	0x314
#define		EXDMACR2	0x318
#define		BCINTSR		0x330
#define		BCINTCR		0x334
#define		BCINTMR		0x338
#define		EXBATLV		0x340
#define		EXWTSTS		0x344
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
#if defined(CONFIG_CONS_SCIF4)
#define	SCIF_BASE	SCIF4_BASE
#elif defined(CONFIG_CONS_SCIF2)
#define	SCIF_BASE	SCIF2_BASE
#endif
#define		SCIF_DL		0x30
#define		SCIF_CKS	0x34
#define	CKS_EXTERNAL	0	/* CKS: external clock */
