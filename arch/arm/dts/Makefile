# SPDX-License-Identifier: GPL-2.0+


dtb-$(CONFIG_TARGET_RENESAS_RZN1D400_DB) += rzn1d400-db-no-cm3.dtb
dtb-$(CONFIG_TARGET_RENESAS_RZN1S324_DB) += rzn1s324-db.dtb
dtb-$(CONFIG_TARGET_RENESAS_RZN1S_IO_LINK) += rzn1s-io-link.dtb
dtb-$(CONFIG_TARGET_RENESAS_RZN1L_DB) += rzn1l-db.dtb




targets += $(dtb-y)

# Add any required device tree compiler flags here
DTC_FLAGS +=

PHONY += dtbs
dtbs: $(addprefix $(obj)/, $(dtb-y))
	@:

clean-files := *.dtb
