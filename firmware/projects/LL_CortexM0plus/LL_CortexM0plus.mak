# SPDX-License-Identifier: MIT
# Copyright (c) 2021 Bart Bilos
# For conditions of distribution and use, see LICENSE file

# Version: 20230101

# current makefile base dir relative to Makefile
PROJ_DIR := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))

# project settings
NAME := CortexM0plus_tests
BOARD := nuclone_CortexM0plus_tests
$(NAME)_TARGET := MCU
$(NAME)_MCU := CortexM0plus_tests
$(NAME)_DEFINES += -DMINUNIT_MAX_TESTS=50 -DMINUNIT_REPORT_DISABLE
$(NAME)_LIBS := libMcuLL squantorLibC squantorLibEmbedded squantorMinUnit
$(NAME)_FILES := $(PROJ_DIR)/src/main.cpp \
common/src/$(BOARD).cpp \
common/src/CortexM0plus_teardown.cpp \
$(PROJ_DIR)/src/test_CortexM0plus_systick.cpp \
$(PROJ_DIR)/src/test_CortexM0plus_nvic.cpp \
$(PROJ_DIR)/src/test_CortexM0plus_scb.cpp

$(NAME)_INCLUDES := -I$(PROJ_DIR)/inc -Icommon/inc

# --- nothing user definable below ---
PROJECTS += $(NAME)
$(NAME)_PROJECT_ROOT := $(PROJ_DIR)
$(NAME)_BOARD := $(BOARD)

