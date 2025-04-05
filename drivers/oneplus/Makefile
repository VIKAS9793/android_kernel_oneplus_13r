# SPDX-License-Identifier: GPL-2.0-only

# OnePlus drivers Makefile

obj-$(CONFIG_ONEPLUS_DRIVERS)		+= oneplus_drivers.o

# Alert Slider
obj-$(CONFIG_ONEPLUS_TRI_STATE_KEY)	+= tri-state-key/
oneplus_tri_state_key-y			:= tri-state-key/tri_state_key.o

# SUPERVOOC Fast Charging
obj-$(CONFIG_ONEPLUS_FASTCHG)		+= power/
oneplus_fastcharge-y			:= power/oplus_chg_core.o
oneplus_fastcharge-y			+= power/oplus_chg_wls.o
oneplus_fastcharge-y			+= power/oplus_chg_comm.o
oneplus_fastcharge-y			+= power/oplus_chg_voter.o
oneplus_fastcharge-y			+= power/oplus_chg_ic.o
oneplus_fastcharge-y			+= power/oplus_chg_strategy.o
oneplus_fastcharge-y			+= power/oplus_chg_gauge.o
oneplus_fastcharge-y			+= power/oplus_chg_track.o

# Goodix Fingerprint Sensor
obj-$(CONFIG_ONEPLUS_FINGERPRINT)	+= fingerprint/
oneplus_fingerprint-y			:= fingerprint/gf_spi.o
oneplus_fingerprint-y			+= fingerprint/gf_platform.o
oneplus_fingerprint-y			+= fingerprint/gf_netlink.o

# Ensure all directories are created
$(shell mkdir -p $(obj)/tri-state-key)
$(shell mkdir -p $(obj)/power)
$(shell mkdir -p $(obj)/fingerprint)

obj-$(CONFIG_ONEPLUS_DISPLAY_FEATURE) += display/
obj-$(CONFIG_ONEPLUS_MOTOR) += motor/
obj-$(CONFIG_ONEPLUS_WIRELESS_CHARGE) += power/

# Board support
obj-$(CONFIG_MACH_ONEPLUS_13R) += board-13r.o

# Touchscreen
obj-$(CONFIG_TOUCHSCREEN_GOODIX_GTX8) += touchscreen/goodix_gtx8.o
obj-$(CONFIG_TOUCHSCREEN_GOODIX_GTX8) += touchscreen/goodix_gtx8_update.o
obj-$(CONFIG_TOUCHSCREEN_GOODIX_GTX8) += touchscreen/goodix_gtx8_tools.o
obj-$(CONFIG_TOUCHSCREEN_GOODIX_GTX8) += touchscreen/goodix_gtx8_gesture.o 