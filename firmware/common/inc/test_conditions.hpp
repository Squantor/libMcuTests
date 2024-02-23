/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2021 Bart Bilos
 * For conditions of distribution and use, see LICENSE file
 */
#ifndef TEST_CONDITIONS_HPP
#define TEST_CONDITIONS_HPP

/**
 * \brief Executes a check and returns from function with false
 * 
 * @param[in]  test: condition to check
 * @return when the condition fails, will execute return false
 */
#define TESTANDRETURN(test) \
do {\
    if (!(test)) {\
        return false;\
    }\
} while(0)

#endif