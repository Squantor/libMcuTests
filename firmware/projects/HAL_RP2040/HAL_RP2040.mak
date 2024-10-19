# SPDX-License-Identifier: MIT
# Copyright (c) 2021 Bart Bilos
# For conditions of distribution and use, see LICENSE file

# Version: 20230101

# current makefile base dir relative to Makefile
PROJ_DIR := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))

# project settings
NAME := test_HAL_RP2040
BOARD := nuclone_RP2040_HAL
$(NAME)_TARGET := MCU
$(NAME)_MCU := RP2040
$(NAME)_FLASH := generic
$(NAME)_DEFINES := -DMINUNIT_MAX_TESTS=100
$(NAME)_LIBS := libMcu squantorLibC squantorMinUnit
$(NAME)_FILES := $(PROJ_DIR)/src/main.cpp \
$(PROJ_DIR)/src/test_RP2040_HAL_pins.cpp \
$(PROJ_DIR)/src/test_RP2040_HAL_gpio.cpp \
common/src/$(BOARD).cpp \
common/src/RP2040_HAL_teardown.cpp
$(NAME)_INCLUDES := -I$(PROJ_DIR)/inc -Icommon/inc

# --- nothing user definable below ---
PROJECTS += $(NAME)
$(NAME)_PROJECT_ROOT := $(PROJ_DIR)
$(NAME)_BOARD := $(BOARD)
$(NAME)_DEFINES += -D$(BOARD)
