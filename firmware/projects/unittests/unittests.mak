# SPDX-License-Identifier: MIT
# Copyright (c) 2021 Bart Bilos
# For conditions of distribution and use, see LICENSE file

# Version: 20240223

# current makefile base dir relative to Makefile
PROJ_DIR := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))

# project settings
NAME := unittests
$(NAME)_TARGET := PC
$(NAME)_DEFINES := -DMINUNIT_MAX_TESTS=1000
$(NAME)_LIBS := squantorMinUnit libMcu
$(NAME)_FILES := $(PROJ_DIR)/src/main.cpp \
$(PROJ_DIR)/src/test_LPC84X_i2c_hal.cpp \
$(PROJ_DIR)/src/test_ringbuf.cpp \
$(PROJ_DIR)/src/test_fifoallocator.cpp \
$(PROJ_DIR)/src/test_finoallocator.cpp \
$(PROJ_DIR)/src/test.cpp \
$(PROJ_DIR)/src/test_rmw.cpp \
$(PROJ_DIR)/src/test_bitblit_eq.cpp \
$(PROJ_DIR)/src/test_bitblit_gr.cpp \
$(PROJ_DIR)/src/test_bitblit_sm.cpp \
$(PROJ_DIR)/src/test_bitmap.cpp \
$(PROJ_DIR)/src/test_blit.cpp \
$(PROJ_DIR)/src/utilities.cpp
# $(PROJ_DIR)/src/test_bitblit2d.cpp
# $(PROJ_DIR)/src/test_elementpack.cpp
$(NAME)_INCLUDES := -I$(PROJ_DIR)/inc -Icommon/inc
$(NAME)_PREBUILD_SCRIPT := $(PROJ_DIR)/prebuild.sh
$(NAME)_POSTBUILD_SCRIPT := $(PROJ_DIR)/postbuild.sh

# --- nothing user definable below ---
PROJECTS += $(NAME)
$(NAME)_PROJECT_ROOT := $(PROJ_DIR)
$(NAME)_DEFINES +=
