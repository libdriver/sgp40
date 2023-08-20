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
 * @file      driver_sgp40_register_test.c
 * @brief     driver sgp40 register test source file
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

#include "driver_sgp40_register_test.h"
#include <stdlib.h>

static sgp40_handle_t gs_handle;        /**< sgp40 handle */

/**
 * @brief  register test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t sgp40_register_test(void)
{
    uint8_t res;
    uint16_t reg;
    uint16_t result;
    uint16_t id[3];
    float rh;
    float temp;
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
    
    /* start register test */
    sgp40_interface_debug_print("sgp40: start register test.\n");
    
    /* sgp40 init */
    res = sgp40_init(&gs_handle);
    if (res != 0)
    {
        sgp40_interface_debug_print("sgp40: init failed.\n");
    
        return 1;
    }
    
    /* sgp40 soft reset test */
    sgp40_interface_debug_print("sgp40: sgp40_soft_reset test.\n");
    
    /* soft reset */
    res = sgp40_soft_reset(&gs_handle);
    if (res != 0)
    {
        sgp40_interface_debug_print("sgp40: soft failed.\n");
        (void)sgp40_deinit(&gs_handle);
        
        return 1;
    }
    sgp40_interface_debug_print("sgp40: check soft reset %s.\n", res == 0 ? "ok" : "error");
    
    /* sgp40_get_measure_test test */
    sgp40_interface_debug_print("sgp40: sgp40_get_measure_test test.\n");
    
    /* get measure test */
    res = sgp40_get_measure_test(&gs_handle, &result);
    if (res != 0)
    {
        sgp40_interface_debug_print("sgp40: get measure test failed.\n");
        (void)sgp40_deinit(&gs_handle);
        
        return 1;
    }
    
    /* output */
    sgp40_interface_debug_print("sgp40: test result is 0x%04X.\n", result);
    
    /* sgp40_get_serial_id test */
    sgp40_interface_debug_print("sgp40: sgp40_get_serial_id test.\n");
    
    /* get serial id */
    res = sgp40_get_serial_id(&gs_handle, id);
    if (res != 0)
    {
        sgp40_interface_debug_print("sgp40: get serial id failed.\n");
        (void)sgp40_deinit(&gs_handle);
        
        return 1;
    }
    sgp40_interface_debug_print("sgp40: check get serial id %s.\n", res == 0 ? "ok" : "error");
    sgp40_interface_debug_print("sgp40: serial id 0x%04X 0x%04X 0x%04X.\n", (uint16_t)(id[0]), (uint16_t)(id[1]), (uint16_t)(id[2]));
    
    /* sgp40_turn_heater_off test */
    sgp40_interface_debug_print("sgp40: sgp40_turn_heater_off test.\n");
    
    /* turn heater off */
    res = sgp40_turn_heater_off(&gs_handle);
    if (res != 0)
    {
        sgp40_interface_debug_print("sgp40: turn heater off failed.\n");
        (void)sgp40_deinit(&gs_handle);
        
        return 1;
    }
    sgp40_interface_debug_print("sgp40: check turn heater off %s.\n", res == 0 ? "ok" : "error");
    
    /* sgp40_humidity_convert_to_register test */
    sgp40_interface_debug_print("sgp40: sgp40_humidity_convert_to_register test.\n");
    
    /* humidity convert to register */
    rh = (float)(rand() % 1000) / 10.0f;
    res = sgp40_humidity_convert_to_register(&gs_handle, rh, &reg);
    if (res != 0)
    {
        sgp40_interface_debug_print("sgp40: humidity convert to register failed.\n");
        (void)sgp40_deinit(&gs_handle);
        
        return 1;
    }
    sgp40_interface_debug_print("sgp40: %0.2f%% humidity convert to register 0x%04X.\n", rh, reg);
    
    /* sgp40_temperature_convert_to_register test */
    sgp40_interface_debug_print("sgp40: sgp40_temperature_convert_to_register test.\n");
    
    /* temperature convert to register */
    temp = (float)(rand() % 500) / 10.0f;
    res = sgp40_temperature_convert_to_register(&gs_handle, temp, &reg);
    if (res != 0)
    {
        sgp40_interface_debug_print("sgp40: temperature convert to register failed.\n");
        (void)sgp40_deinit(&gs_handle);
        
        return 1;
    }
    sgp40_interface_debug_print("sgp40: %0.2fC temperature convert to register 0x%04X.\n", temp, reg);
    
    /* finish register test */
    sgp40_interface_debug_print("sgp40: finish register test.\n");
    (void)sgp40_deinit(&gs_handle);
    
    return 0;
}
