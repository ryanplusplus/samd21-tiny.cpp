__arm_none_eabi_gcc_tools_path := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

TOOLCHAIN_PREFIX ?= arm-none-eabi-

include $(__arm_none_eabi_gcc_tools_path)gcc/tools.mk
include $(__arm_none_eabi_gcc_tools_path)debug-adapter.mk
