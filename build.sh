#!/bin/bash

# Copyright (C) 2024 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m'

# Kernel details
KERNEL_NAME="OnePlus 13R"
KERNEL_DEFCONFIG="vendor/sm8650_defconfig"
KERNEL_DIR="$(pwd)"
OUT_DIR="$KERNEL_DIR/out"

# Toolchain paths
CLANG_DIR="$KERNEL_DIR/../../prebuilts/clang/host/linux-x86/clang-r450784e"
GCC_DIR="$KERNEL_DIR/../../prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9"
GCC_32_DIR="$KERNEL_DIR/../../prebuilts/gcc/linux-x86/arm/arm-linux-androideabi-4.9"

# Export paths
export PATH="$CLANG_DIR/bin:$GCC_DIR/bin:$GCC_32_DIR/bin:$PATH"
export ARCH="arm64"
export SUBARCH="arm64"
export CROSS_COMPILE="aarch64-linux-android-"
export CROSS_COMPILE_ARM32="arm-linux-androideabi-"
export CLANG_TRIPLE="aarch64-linux-gnu-"

# Clean build directory
echo -e "${BLUE}Cleaning build directory...${NC}"
rm -rf "$OUT_DIR"
mkdir -p "$OUT_DIR"

# Build kernel
echo -e "${BLUE}Building kernel for $KERNEL_NAME...${NC}"
make O="$OUT_DIR" \
    ARCH=arm64 \
    CC=clang \
    CLANG_TRIPLE=aarch64-linux-gnu- \
    CROSS_COMPILE=aarch64-linux-android- \
    CROSS_COMPILE_ARM32=arm-linux-androideabi- \
    -j$(nproc --all) \
    $KERNEL_DEFCONFIG

if [ $? -ne 0 ]; then
    echo -e "${RED}Failed to configure kernel!${NC}"
    exit 1
fi

make O="$OUT_DIR" \
    ARCH=arm64 \
    CC=clang \
    CLANG_TRIPLE=aarch64-linux-gnu- \
    CROSS_COMPILE=aarch64-linux-android- \
    CROSS_COMPILE_ARM32=arm-linux-androideabi- \
    -j$(nproc --all)

if [ $? -ne 0 ]; then
    echo -e "${RED}Failed to build kernel!${NC}"
    exit 1
fi

# Copy kernel image
cp "$OUT_DIR/arch/arm64/boot/Image" "$KERNEL_DIR/Image"
if [ $? -ne 0 ]; then
    echo -e "${RED}Failed to copy kernel image!${NC}"
    exit 1
fi

# Copy DTB files
cp "$OUT_DIR/arch/arm64/boot/dts/vendor/qcom/*.dtb" "$KERNEL_DIR/"
if [ $? -ne 0 ]; then
    echo -e "${RED}Failed to copy DTB files!${NC}"
    exit 1
fi

# Copy DTBO image
cp "$OUT_DIR/arch/arm64/boot/dtbo.img" "$KERNEL_DIR/"
if [ $? -ne 0 ]; then
    echo -e "${RED}Failed to copy DTBO image!${NC}"
    exit 1
fi

echo -e "${GREEN}Kernel build completed successfully!${NC}"
echo -e "${BLUE}Kernel image: ${NC}$KERNEL_DIR/Image"
echo -e "${BLUE}DTB files: ${NC}$KERNEL_DIR/*.dtb"
echo -e "${BLUE}DTBO image: ${NC}$KERNEL_DIR/dtbo.img"