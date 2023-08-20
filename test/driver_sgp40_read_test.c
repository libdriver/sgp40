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
 * @file      driver_sgp40_read_test.c
 * @brief     driver sgp40 read test source file
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

#include "driver_sgp40_read_test.h"
#include "driver_sgp40_algorithm.h"

static sgp40_handle_t gs_handle;                         /**< sgp40 handle */
static sgp40_gas_index_algorithm_t gs_voc_handle;        /**< voc handle */

/**
 * @brief     read test
 * @param[in] times is the test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t sgp40_read_test(uint32_t times)
{
    uint8_t res;
    uint32_t i;
    sgp40_info_t info;
    
    /* link functions */
    DRIVER_SGP40_LINK_INIT(&gs_handle, sgp40_handle_t);
    DRIVER_SGP40_LINK_IIC_INIT(&gs_handle, sgp40_interface_iic_init);
    DRIVER_SGP40_LINK_IIC_DEINIT(&gs_handle, sgp40_interface_iic_deinit);
    DRIVER_SGP40_LINK_IIC_WRITE_COMMAND(&gs_handle, sgp40_interface_iic_write_cmd);
    DRIVER_SGP40_LINK_IIC_READ_COMMAND(&gs_handle, sgp40_interface_iic_read_cmd);
    DRIVER_SGP40_LINK_DELAY_MS(&gs_handle, sgp40_interface_delay_ms);
    DRIVER_SGP40_LINK_DEBUG_PRINT(&gs_handle, sgp40_interface_debug_print);
    
    /* sgp40 info */
    res = sgp40_info(&info);
    if (res != 0)
    {
        sgp40_interface_debug_print("sgp40: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip information */
        sgp40_interface_debug_print("sgp40: chip is %s.\n", info.chip_name);
        sgp40_interface_debug_print("sgp40: manufacturer is %s.\n", info.manufacturer_name);
        sgp40_interface_debug_print("sgp40: interface is %s.\n", info.interface);
        sgp40_interface_debug_print("sgp40: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        sgp40_interface_debug_print("sgp40: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        sgp40_interface_debug_print("sgp40: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        sgp40_interface_debug_print("sgp40: max current is %0.2fmA.\n", info.max_current_ma);
        sgp40_interface_debug_print("sgp40: max temperature is %0.1fC.\n", info.temperature_max);
        sgp40_interface_debug_print("sgp40: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start read test */
    sgp40_interface_debug_print("sgp40: start read test.\n");
    
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
    
    /* delay 1000ms */
    sgp40_interface_delay_ms(1000);
    
    /* measure raw test */
    sgp40_interface_debug_print("sgp40: measure raw test.\n");
    
    /* run times */
    for (i = 0; i < times; i++)
    {
        uint16_t raw_humidity;
        uint16_t raw_temperature;
        uint16_t sraw_voc;
        int32_t voc_gas_index;
        
        /* humidity convert to register */
        res = sgp40_humidity_convert_to_register(&gs_handle, 50.0f, &raw_humidity);
        if (res != 0)
        {
            sgp40_interface_debug_print("sgp40: humidity convert to register failed.\n");
            (void)sgp40_deinit(&gs_handle);
            
            return 1;
        }
        
        /* temperature convert to register */
        res = sgp40_temperature_convert_to_register(&gs_handle, 25.0f, &raw_temperature);
        if (res != 0)
        {
            sgp40_interface_debug_print("sgp40: temperature convert to register failed.\n");
            (void)sgp40_deinit(&gs_handle);
            
            return 1;
        }
        
        /* get measure raw */
        res = sgp40_get_measure_raw(&gs_handle, raw_humidity, raw_temperature, &sraw_voc);
        if (res != 0)
        {
            sgp40_interface_debug_print("sgp40: get measure raw failed.\n");
            (void)sgp40_deinit(&gs_handle);
            
            return 1;
        }
        
        /* algorithm process */
        sgp40_algorithm_process(&gs_voc_handle, sraw_voc, &voc_gas_index);

        /* output */
        sgp40_interface_debug_print("sgp40: raw voc is 0x%04X and voc gas index is %d.\n", sraw_voc, voc_gas_index);
        
        /* delay 1000ms */
        sgp40_interface_delay_ms(1000);
    }
    
    /* measure raw without compensation test */
    sgp40_interface_debug_print("sgp40: measure raw without compensation test.\n");
    
    /* run times */
    for (i = 0; i < times; i++)
    {
        uint16_t sraw_voc;
        int32_t voc_gas_index;
        
        /* get measure raw */
        res = sgp40_get_measure_raw_without_compensation(&gs_handle, &sraw_voc);
        if (res != 0)
        {
            sgp40_interface_debug_print("sgp40: get measure raw without compensation failed.\n");
            (void)sgp40_deinit(&gs_handle);
            
            return 1;
        }
        
        /* algorithm process */
        sgp40_algorithm_process(&gs_voc_handle, sraw_voc, &voc_gas_index);
        
        /* output */
        sgp40_interface_debug_print("sgp40: raw voc is 0x%04X and voc gas index is %d.\n", sraw_voc, voc_gas_index);
        
        /* delay 1000ms */
        sgp40_interface_delay_ms(1000);
    }

    /* finish read test */
    sgp40_interface_debug_print("sgp40: finish read test.\n");
    (void)sgp40_deinit(&gs_handle);
    
    return 0;
}
