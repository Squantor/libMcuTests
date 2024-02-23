# SPDX-License-Identifier: MIT
# Copyright (c) 2022 Bart Bilos
# For conditions of distribution and use, see LICENSE file

# Version: 20230101

# current makefile base dir relative to Makefile
BASE_DIR := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))

# include all projects
include $(BASE_DIR)/projects/PC/PC.mak
include $(BASE_DIR)/projects/tests_C_LPC844M201BD64/tests_C_LPC844M201BD64.mak
include $(BASE_DIR)/projects/tests_CPP_LPC844M201BD64/tests_CPP_LPC844M201BD64.mak
include $(BASE_DIR)/projects/tests_template/tests_template.mak
include $(BASE_DIR)/projects/tests_CPP_LPC812M101DH20_first/tests_CPP_LPC812M101DH20_first.mak
include $(BASE_DIR)/projects/tests_CPP_LPC812M101DH20_second/tests_CPP_LPC812M101DH20_second.mak
include $(BASE_DIR)/projects/tests_CPP_LPC812M101DH20_third/tests_CPP_LPC812M101DH20_third.mak
include ${BASE_DIR}/projects/tests_CPP_CortexM0plus/tests_CPP_CortexM0plus.mak

