/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_sgp40_basic.h
 * @brief     driver sgp40 basic header file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2023-08-30
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2023/08/30  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef DRIVER_SGP40_BASIC_H
#define DRIVER_SGP40_BASIC_H

#include "driver_sgp40_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup sgp40_example_driver sgp40 example driver function
 * @brief    sgp40 example driver modules
 * @ingroup  sgp40_driver
 * @{
 */

/**
 * @brief  basic example init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t sgp40_basic_init(void);

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t sgp40_basic_deinit(void);

/**
 * @brief      basic example read
 * @param[in]  temperature is the current temperature in C
 * @param[in]  humidity is the current humidity in %
 * @param[out] *voc_gas_index points to a voc gas index buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t sgp40_basic_read(float temperature, float humidity, int32_t *voc_gas_index);

/**
 * @brief      basic example read without compensation
 * @param[out] *voc_gas_index points to a voc gas index buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t sgp40_basic_read_without_compensation(int32_t *voc_gas_index);

/**
 * @brief      basic example get serial id
 * @param[out] *id points to an id buffer
 * @return     status code
 *             - 0 success
 *             - 1 get serial id failed
 * @note       none
 */
uint8_t sgp40_basic_get_serial_id(uint16_t id[3]);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
