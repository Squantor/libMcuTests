# SPDX-License-Identifier: MIT
# Copyright (c) 2022 Bart Bilos
# For conditions of distribution and use, see LICENSE file

# Version: 20230101

# current makefile base dir relative to Makefile
BASE_DIR := $(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))

# include all projects
include $(BASE_DIR)/projects/PC/PC.mak
include $(BASE_DIR)/projects/unittests/unittests.mak
include $(BASE_DIR)/projects/tests_template/tests_template.mak
include ${BASE_DIR}/projects/LL_CortexM0plus/LL_CortexM0plus.mak
include $(BASE_DIR)/projects/LL_LPC812DH20_1/LL_LPC812DH20_1.mak
include $(BASE_DIR)/projects/LL_LPC812DH20_2/LL_LPC812DH20_2.mak
include $(BASE_DIR)/projects/LL_LPC812DH20_3/LL_LPC812DH20_3.mak
include $(BASE_DIR)/projects/LL_LPC844BD64/LL_LPC844BD64.mak
include $(BASE_DIR)/projects/LL_LPC845BD48/LL_LPC845BD48.mak
include $(BASE_DIR)/projects/LL_RP2040/LL_RP2040.mak
include $(BASE_DIR)/projects/HAL_RP2040/HAL_RP2040.mak
include $(BASE_DIR)/projects/HAL_LPC845BD48/HAL_LPC845BD48.mak
include $(BASE_DIR)/projects/DRV_mux3to8_74hc595/DRV_mux3to8_74hc595.mak
