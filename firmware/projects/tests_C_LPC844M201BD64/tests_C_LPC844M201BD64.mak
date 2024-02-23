# SPDX-License-Identifier: MIT
# Copyright (c) 2021 Bart Bilos
# For conditions of distribution and use, see LICENSE file

# Version: 20230101

# current makefile base dir relative to Makefile
PROJ_DIR := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))

# project settings
NAME := LPC844M201BD64_C_tests
BOARD := nuclone_LPC844M201BD64_tests
$(NAME)_TARGET := MCU
$(NAME)_MCU := LPC844M201BD64
$(NAME)_DEFINES += -DMINUNIT_MAX_TESTS=100 -DMINUNIT_REPORT_DISABLE # saves load of flash space
$(NAME)_LIBS := libMcuLL squantorLibC squantorLibEmbedded squantorMinUnit
$(NAME)_FILES := $(PROJ_DIR)/src/main.cpp \
$(PROJ_DIR)/src/$(BOARD).cpp \
$(PROJ_DIR)/src/LPC844M201_teardown.cpp \
$(PROJ_DIR)/src/test_LPC844M201_teardown.cpp \
$(PROJ_DIR)/src/test_LPC844M201_gpio.cpp \
$(PROJ_DIR)/src/test_LPC844M201_iocon.cpp \
$(PROJ_DIR)/src/test_LPC844M201_usart_polled.cpp \
$(PROJ_DIR)/src/test_LPC844M201_spi_polled.cpp \
$(PROJ_DIR)/src/test_LPC844M201_i2c_polled.cpp \
$(PROJ_DIR)/src/test_LPC844M201_sct.cpp
$(NAME)_INCLUDES := -I$(PROJ_DIR)/inc -Icommon/inc

# --- nothing user definable below ---
PROJECTS += $(NAME)
$(NAME)_PROJECT_ROOT := $(PROJ_DIR)
$(NAME)_BOARD := $(BOARD)

