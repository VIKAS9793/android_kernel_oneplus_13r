# Copyright (C) 2024 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# Kernel configuration
KERNEL_DEFCONFIG := vendor/kalama_defconfig
KERNEL_ARCH := arm64
KERNEL_CROSS_COMPILE := aarch64-linux-android-
KERNEL_CLANG_COMPILE := true
KERNEL_CLANG_VERSION := r416183b
KERNEL_CLANG_PATH := prebuilts/clang/host/linux-x86/clang-r416183b

# Kernel build flags
KERNEL_CFLAGS := \
    -Wall \
    -Werror \
    -Wno-format-security \
    -Wno-unused-variable \
    -Wno-unused-function \
    -Wno-unused-label \
    -Wno-unused-parameter \
    -Wno-unused-value \
    -Wno-unused-but-set-variable \
    -Wno-maybe-uninitialized \
    -Wno-array-bounds \
    -Wno-stringop-overflow \
    -Wno-restrict \
    -Wno-format-truncation \
    -Wno-format-overflow \
    -Wno-stringop-truncation \
    -Wno-misleading-indentation \
    -Wno-int-in-bool-context \
    -Wno-address-of-packed-member \
    -Wno-attribute-alias \
    -Wno-packed-not-aligned \
    -Wno-stringop-overread \
    -Wno-stringop-overflow \
    -Wno-restrict \
    -Wno-format-truncation \
    -Wno-format-overflow \
    -Wno-stringop-truncation \
    -Wno-misleading-indentation \
    -Wno-int-in-bool-context \
    -Wno-address-of-packed-member \
    -Wno-attribute-alias \
    -Wno-packed-not-aligned \
    -Wno-stringop-overread

# Kernel build targets
KERNEL_BUILD_TARGETS := Image.gz-dtb
KERNEL_BUILD_TARGETS += dtbo.img

# Include kernel build rules
include $(LOCAL_PATH)/build_kernel.mk 