# SPDX-License-Identifier: GPL-2.0
VERSION = 6
PATCHLEVEL = 1
SUBLEVEL = 75

EXTRAVERSION = -sm8650

NAME = OnePlus 13R Kernel

# *DOCUMENTATION*
# To see a list of typical targets execute "make help"
# More info can be located in ./README
# Comments in this file are targeted only to the developer, do not
# expect to learn how to build the kernel reading this file.

# Do not print "Entering directory ..."
MAKEFLAGS += --no-print-directory

# We are using a recursive build, so we need to do a little thinking
# to get the ordering right.
#
# Most importantly: sub-Makefiles should only ever modify files in
# their own directory. If in some directory we have a dependency on
# a file in another dir (which doesn't happen often, but it's often
# unavoidable when linking the built-in.a targets which finally
# turn into vmlinux), we will call a sub make in that other dir, and
# after that we are sure that everything which is in that other dir
# is now up to date.
#
# The only cases where we need to modify files which have global
# effects are thus separated out and done before the recursive
# descending is started. They are now explicitly listed as the
# prepare rule.

# To put more focus on warnings, be less verbose as default
# Use 'make V=1' to see the full commands

ifeq ("$(origin V)", "command line")
  KBUILD_VERBOSE = $(V)
endif
ifndef KBUILD_VERBOSE
  KBUILD_VERBOSE = 0
endif

# kbuild supports saving output files in a separate directory.
# To locate output files in a separate directory two syntaxes are supported.
# In both cases the working directory must be the root of the kernel src.
# 1) O=
# Use "make O=dir/to/store/output/files/"
#
# 2) Set KBUILD_OUTPUT
# Set the environment variable KBUILD_OUTPUT to point to the directory
# where the output files shall be placed.
# export KBUILD_OUTPUT=dir/to/store/output/files/
# make
#
# The O= assignment takes precedence over the KBUILD_OUTPUT environment
# variable.

# ARCH can be set on the command line, in the environment, or
# in the config file. ARM64 is the default.
ARCH ?= arm64

# CROSS_COMPILE can be set on the command line, in the environment, or
# in the config file. aarch64-linux-android- is the default.
CROSS_COMPILE ?= aarch64-linux-android-

# CROSS_COMPILE_ARM32 is used for CONFIG_COMPAT
CROSS_COMPILE_ARM32 ?= arm-linux-androideabi-

# Architecture specific flags
KBUILD_DEFCONFIG := vendor/sm8650_defconfig

# Kernel binary name
KBUILD_IMAGE := Image.gz-dtb

export ARCH CROSS_COMPILE CROSS_COMPILE_ARM32 KBUILD_DEFCONFIG KBUILD_IMAGE

# Read .config if it exists, otherwise read arch/$(ARCH)/configs/$(KBUILD_DEFCONFIG)
-include include/config/auto.conf

# Include the make variables (CC, etc...)
AS      = $(CROSS_COMPILE)as
LD      = $(CROSS_COMPILE)ld
CC      = $(CROSS_COMPILE)gcc
CPP     = $(CC) -E
AR      = $(CROSS_COMPILE)ar
NM      = $(CROSS_COMPILE)nm
STRIP   = $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump

export AS LD CC CPP AR NM STRIP OBJCOPY OBJDUMP

# Basic helpers built in scripts/
HOSTCC  = gcc
HOSTCXX = g++
HOSTCFLAGS  = -Wall -Wmissing-prototypes -Wstrict-prototypes -O2 -fomit-frame-pointer
HOSTCXXFLAGS = -O2

export HOSTCC HOSTCXX HOSTCFLAGS HOSTCXXFLAGS

# Files to ignore in find ... statements
RCS_FIND_IGNORE := \( -name SCCS -o -name BitKeeper -o -name .svn -o -name CVS -o -name .pc -o -name .hg -o -name .git \) -prune -o
export RCS_FIND_IGNORE

# ===========================================================================
# Rules shared between *config targets and build targets

# Basic helpers built in scripts/
PHONY += scripts_basic
scripts_basic:
	$(Q)$(MAKE) $(build)=scripts/basic
	$(Q)rm -f .tmp_quiet_recordmcount

# To avoid any implicit rule to kick in, define an empty command
.PHONY: $(PHONY) 