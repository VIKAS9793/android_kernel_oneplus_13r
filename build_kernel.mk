# Copyright (C) 2024 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

# Kernel build rules
KERNEL_OUT := $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ
KERNEL_CONFIG := $(KERNEL_OUT)/.config
KERNEL_HEADERS_INSTALL := $(KERNEL_OUT)/usr
KERNEL_MODULES_INSTALL := system
KERNEL_MODULES_OUT := $(TARGET_OUT)/lib/modules

# Kernel build commands
KERNEL_MAKE_CMD := \
    PATH=$(KERNEL_CLANG_PATH)/bin:$$PATH \
    make -C $(TARGET_KERNEL_SOURCE) \
    O=$(KERNEL_OUT) \
    ARCH=$(KERNEL_ARCH) \
    CROSS_COMPILE=$(KERNEL_CROSS_COMPILE) \
    CLANG_TRIPLE=aarch64-linux-gnu- \
    CC=clang \
    LD=ld.lld \
    AR=llvm-ar \
    NM=llvm-nm \
    OBJCOPY=llvm-objcopy \
    OBJDUMP=llvm-objdump \
    STRIP=llvm-strip \
    HOSTCC=clang \
    HOSTCXX=clang++ \
    HOSTAR=llvm-ar \
    HOSTLD=ld.lld \
    CFLAGS_MODULE="-fno-pic" \
    CFLAGS_KERNEL="$(KERNEL_CFLAGS)"

# Kernel build targets
.PHONY: kernel
kernel: $(KERNEL_CONFIG)
	$(KERNEL_MAKE_CMD) $(KERNEL_BUILD_TARGETS)

# Kernel configuration
$(KERNEL_CONFIG): $(TARGET_KERNEL_SOURCE)/arch/$(KERNEL_ARCH)/configs/$(KERNEL_DEFCONFIG)
	$(KERNEL_MAKE_CMD) $(KERNEL_DEFCONFIG)

# Kernel modules
.PHONY: modules
modules: kernel
	$(KERNEL_MAKE_CMD) modules
	$(KERNEL_MAKE_CMD) INSTALL_MOD_PATH=../../$(KERNEL_MODULES_INSTALL) modules_install
	rm -f $(KERNEL_MODULES_OUT)/build
	rm -f $(KERNEL_MODULES_OUT)/source

# Kernel headers
.PHONY: headers
headers: kernel
	$(KERNEL_MAKE_CMD) headers_install

# Clean kernel
.PHONY: clean_kernel
clean_kernel:
	$(KERNEL_MAKE_CMD) clean
	rm -rf $(KERNEL_OUT)

# Include kernel build rules
include $(CLEAR_VARS)
LOCAL_MODULE := kernel
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_IS_HOST_MODULE := true
include $(BUILD_PREBUILT) 