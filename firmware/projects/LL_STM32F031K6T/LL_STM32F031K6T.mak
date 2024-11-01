# SPDX-License-Identifier: MIT
# Copyright (c) 2021 Bart Bilos
# For conditions of distribution and use, see LICENSE file

# Version: 20230101

# current makefile base dir relative to Makefile
PROJ_DIR := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))

# project settings
NAME := test_LL_STM32F031K6T
BOARD := nuclone_STM32F031K6T_LL
$(NAME)_TARGET := MCU
$(NAME)_MCU := STM32F031K6
$(NAME)_DEFINES += -DMINUNIT_MAX_TESTS=100 -DMINUNIT_REPORT_DISABLE
$(NAME)_LIBS := libMcu squantorLibC squantorLibEmbedded squantorMinUnit
$(NAME)_FILES := $(PROJ_DIR)/src/main.cpp \
common/src/$(BOARD).cpp \
common/src/STM32F031K6T_teardown.cpp
$(NAME)_INCLUDES := -I$(PROJ_DIR)/inc -Icommon/inc

# --- nothing user definable below ---
PROJECTS += $(NAME)
$(NAME)_PROJECT_ROOT := $(PROJ_DIR)
$(NAME)_BOARD := $(BOARD)
$(NAME)_DEFINES += -D$(BOARD)
