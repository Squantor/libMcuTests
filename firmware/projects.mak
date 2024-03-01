# SPDX-License-Identifier: MIT
# Copyright (c) 2022 Bart Bilos
# For conditions of distribution and use, see LICENSE file

# Version: 20230101

# current makefile base dir relative to Makefile
BASE_DIR := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))

# include all projects
include $(BASE_DIR)/projects/PC/PC.mak
include $(BASE_DIR)/projects/tests_template/tests_template.mak
include ${BASE_DIR}/projects/LL_CortexM0plus/LL_CortexM0plus.mak
include $(BASE_DIR)/projects/LL_LPC812DH20_1/LL_LPC812DH20_1.mak
include $(BASE_DIR)/projects/LL_LPC812DH20_2/LL_LPC812DH20_2.mak
include $(BASE_DIR)/projects/LL_LPC812DH20_3/LL_LPC812DH20_3.mak
include $(BASE_DIR)/projects/LL_LPC844M201/LL_LPC844M201.mak


