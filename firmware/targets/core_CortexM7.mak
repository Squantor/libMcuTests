# SPDX-License-Identifier: MIT
# Copyright (c) 2021 Bart Bilos
# For conditions of distribution and use, see LICENSE file

# settings for the Cortex-M4 Core
#
# Version: 20220909

# Tool settings
include targets/TC_arm-none-eabi.mak

# Core flags
CFLAGS += -mcpu=cortex-m7 -mthumb
CXXFLAGS += -mcpu=cortex-m7 -mthumb
ASMFLAGS += -mcpu=cortex-m7 -mthumb
LDFLAGS += -mcpu=cortex-m7 -mthumb

