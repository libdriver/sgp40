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
 * @file      driver_sgp40_basic.c
 * @brief     driver sgp40 basic source file
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

#include "driver_sgp40_basic.h"
#include "driver_sgp40_algorithm.h"

static sgp40_handle_t gs_handle;                         /**< sgp40 handle */
static sgp40_gas_index_algorithm_t gs_voc_handle;        /**< voc handle */

/**
 * @brief  basic example init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t sgp40_basic_init(void)
{
    uint8_t res;

    /* link functions */
    DRIVER_SGP40_LINK_INIT(&gs_handle, sgp40_handle_t);
    DRIVER_SGP40_LINK_IIC_INIT(&gs_handle, sgp40_interface_iic_init);
    DRIVER_SGP40_LINK_IIC_DEINIT(&gs_handle, sgp40_interface_iic_deinit);
    DRIVER_SGP40_LINK_IIC_WRITE_COMMAND(&gs_handle, sgp40_interface_iic_write_cmd);
    DRIVER_SGP40_LINK_IIC_READ_COMMAND(&gs_handle, sgp40_interface_iic_read_cmd);
    DRIVER_SGP40_LINK_DELAY_MS(&gs_handle, sgp40_interface_delay_ms);
    DRIVER_SGP40_LINK_DEBUG_PRINT(&gs_handle, sgp40_interface_debug_print);

    /* sgp40 init */
    res = sgp40_init(&gs_handle);
    if (res != 0)
    {
        sgp40_interface_debug_print("sgp40: init failed.\n");

        return 1;
    }

    /* soft reset */
    res = sgp40_soft_reset(&gs_handle);
    if (res != 0)
    {
        sgp40_interface_debug_print("sgp40: soft failed.\n");
        (void)sgp40_deinit(&gs_handle);

        return 1;
    }

    /* voc algorithm init */
    sgp40_algorithm_init(&gs_voc_handle, SGP40_ALGORITHM_TYPE_VOC);

    return 0;
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t sgp40_basic_deinit(void)
{
    /* close sgp40 */
    if (sgp40_deinit(&gs_handle) != 0)
    {
        return 1;
    }

    return 0;
}

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
uint8_t sgp40_basic_read(float temperature, float humidity, int32_t *voc_gas_index)
{
    uint8_t res;
    uint16_t raw_humidity;
    uint16_t raw_temperature;
    uint16_t sraw_voc;

    /* humidity convert to register */
    res = sgp40_humidity_convert_to_register(&gs_handle, humidity, &raw_humidity);
    if (res != 0)
    {
        sgp40_interface_debug_print("sgp40: humidity convert to register failed.\n");

        return 1;
    }

    /* temperature convert to register */
    res = sgp40_temperature_convert_to_register(&gs_handle, temperature, &raw_temperature);
    if (res != 0)
    {
        sgp40_interface_debug_print("sgp40: temperature convert to register failed.\n");

        return 1;
    }

    /* get measure raw */
    res = sgp40_get_measure_raw(&gs_handle, raw_humidity, raw_temperature, &sraw_voc);
    if (res != 0)
    {
        sgp40_interface_debug_print("sgp40: get measure raw failed.\n");

        return 1;
    }

    /* algorithm process */
    sgp40_algorithm_process(&gs_voc_handle, sraw_voc, voc_gas_index);
    
    return 0;
}

/**
 * @brief      basic example read without compensation
 * @param[out] *voc_gas_index points to a voc gas index buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t sgp40_basic_read_without_compensation(int32_t *voc_gas_index)
{
    uint8_t res;
    uint16_t sraw_voc;

    /* get measure raw */
    res = sgp40_get_measure_raw_without_compensation(&gs_handle, &sraw_voc);
    if (res != 0)
    {
        sgp40_interface_debug_print("sgp40: get measure raw without compensation failed.\n");

        return 1;
    }

    /* algorithm process */
    sgp40_algorithm_process(&gs_voc_handle, sraw_voc, voc_gas_index);
    
    return 0;
}

/**
 * @brief      basic example get serial id
 * @param[out] *id points to an id buffer
 * @return     status code
 *             - 0 success
 *             - 1 get serial id failed
 * @note       none
 */
uint8_t sgp40_basic_get_serial_id(uint16_t id[3])
{
    uint8_t res;

    /* get serial id */
    res = sgp40_get_serial_id(&gs_handle, id);
    if (res != 0)
    {
        sgp40_interface_debug_print("sgp40: get serial id failed.\n");

        return 1;
    }

    return 0;
}
