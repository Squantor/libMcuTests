# SPDX-License-Identifier: MIT
# Copyright (c) 2021 Bart Bilos
# For conditions of distribution and use, see LICENSE file

# Version: 20230101

# current makefile base dir relative to Makefile
PROJ_DIR := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))

# project settings
NAME := test_HAL_LPC845BD48
BOARD := nuclone_LPC845BD48_small_HAL
$(NAME)_TARGET := MCU
$(NAME)_MCU := LPC845
$(NAME)_DEFINES += -DMINUNIT_MAX_TESTS=100 -DMINUNIT_REPORT_DISABLE # saves load of flash space
$(NAME)_LIBS := libMcu squantorLibC squantorLibEmbedded squantorMinUnit
$(NAME)_FILES := $(PROJ_DIR)/src/main.cpp \
$(PROJ_DIR)/src/test_LPC845M301_uart_int.cpp \
$(PROJ_DIR)/src/test_LPC845M301_i2c_int.cpp \
common/src/$(BOARD).cpp \
common/src/LPC845M301_teardown.cpp
$(NAME)_INCLUDES := -I$(PROJ_DIR)/inc -Icommon/inc

# --- nothing user definable below ---
PROJECTS += $(NAME)
$(NAME)_PROJECT_ROOT := $(PROJ_DIR)
$(NAME)_BOARD := $(BOARD)

