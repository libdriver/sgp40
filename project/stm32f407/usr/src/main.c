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
 * @file      main.c
 * @brief     main source file
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
#include "driver_sgp40_register_test.h"
#include "driver_sgp40_read_test.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "uart.h"
#include "getopt.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];        /**< uart buffer */
volatile uint16_t g_len;   /**< uart buffer length */

/**
 * @brief     sgp40 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t sgp40(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"humidity", required_argument, NULL, 1},
        {"temperature", required_argument, NULL, 2},
        {"times", required_argument, NULL, 3},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint32_t times = 3;
    float rh = 50.0f;
    float temp = 25.0f;

    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* rh */
            case 1 :
            {
                rh = atof(optarg);
                
                break;
            }
             
            /* temperature */
            case 2 :
            {
                temp = atof(optarg);
                
                break;
            }
            
            /* running times */
            case 3 :
            {
                /* set the times */
                times = atol(optarg);
                
                break;
            } 
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        /* run reg test */
        if (sgp40_register_test() != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_read", type) == 0)
    {
        /* run read test */
        if (sgp40_read_test(times) != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("e_read", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        int32_t voc_gas_index;
        
        /* init */
        res = sgp40_basic_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            /* delay 1000ms */
            sgp40_interface_delay_ms(1000);
            
            /* read data */
            res = sgp40_basic_read(temp, rh, &voc_gas_index);
            if (res != 0)
            {
                (void)sgp40_basic_deinit();
                
                return 1;
            }
            
            /* output */
            sgp40_interface_debug_print("sgp40: %d/%d.\n", (uint32_t)(i + 1), (uint32_t)times);
            sgp40_interface_debug_print("sgp40: voc gas index is %d.\n", voc_gas_index);
        }
        
        /* deinit */
        (void)sgp40_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_read-without-compensation", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        int32_t voc_gas_index;
        
        /* init */
        res = sgp40_basic_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            /* delay 1000ms */
            sgp40_interface_delay_ms(1000);
            
            /* read data */
            res = sgp40_basic_read_without_compensation(&voc_gas_index);
            if (res != 0)
            {
                (void)sgp40_basic_deinit();
                
                return 1;
            }
            
            /* output */
            sgp40_interface_debug_print("sgp40: %d/%d.\n", (uint32_t)(i + 1), (uint32_t)times);
            sgp40_interface_debug_print("sgp40: voc gas index is %d.\n", voc_gas_index);
        }
        
        /* deinit */
        (void)sgp40_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_serial-id", type) == 0)
    {
        uint8_t res;
        uint16_t id[3];
        
        /* init */
        res = sgp40_basic_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* get serial id */
        res = sgp40_basic_get_serial_id(id);
        if (res != 0)
        {
            (void)sgp40_basic_deinit();
            
            return 1;
        }
        
        /* output */
        sgp40_interface_debug_print("sgp40: serial id 0x%04X 0x%04X 0x%04X.\n", (uint16_t)(id[0]), (uint16_t)(id[1]), (uint16_t)(id[2]));
        
        /* deinit */
        (void)sgp40_basic_deinit();
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        sgp40_interface_debug_print("Usage:\n");
        sgp40_interface_debug_print("  sgp40 (-i | --information)\n");
        sgp40_interface_debug_print("  sgp40 (-h | --help)\n");
        sgp40_interface_debug_print("  sgp40 (-p | --port)\n");
        sgp40_interface_debug_print("  sgp40 (-t reg | --test=reg)\n");
        sgp40_interface_debug_print("  sgp40 (-t read | --test=read) [--times=<num>]\n");
        sgp40_interface_debug_print("  sgp40 (-e read | --example=read) [--times=<num>] [--temperature=<temp>] [--humidity=<rh>]\n");
        sgp40_interface_debug_print("  sgp40 (-e read-without-compensation | --example=read-without-compensation) [--times=<num>]\n");
        sgp40_interface_debug_print("  sgp40 (-e serial-id | --example=serial-id)\n");
        sgp40_interface_debug_print("\n");
        sgp40_interface_debug_print("Options:\n");
        sgp40_interface_debug_print("  -e <read | read-without-compensation | serial-id>, --example=<read | read-without-compensation | serial-id>\n");
        sgp40_interface_debug_print("                                          Run the driver example.\n");
        sgp40_interface_debug_print("  -h, --help                              Show the help.\n");
        sgp40_interface_debug_print("      --humidity=<rh>                     Set the humidity.([default: 50.0f])\n");
        sgp40_interface_debug_print("      --temperature=<temp>                Set the temperature.([default: 25.0f])\n");
        sgp40_interface_debug_print("  -i, --information                       Show the chip information.\n");
        sgp40_interface_debug_print("  -p, --port                              Display the pin connections of the current board.\n");
        sgp40_interface_debug_print("  -t <reg | read>, --test=<reg | read>    Run the driver test.\n");
        sgp40_interface_debug_print("      --times=<num>                       Set the running times.([default: 3])\n");
        
        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        sgp40_info_t info;
        
        /* print sgp40 info */
        sgp40_info(&info);
        sgp40_interface_debug_print("sgp40: chip is %s.\n", info.chip_name);
        sgp40_interface_debug_print("sgp40: manufacturer is %s.\n", info.manufacturer_name);
        sgp40_interface_debug_print("sgp40: interface is %s.\n", info.interface);
        sgp40_interface_debug_print("sgp40: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        sgp40_interface_debug_print("sgp40: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        sgp40_interface_debug_print("sgp40: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        sgp40_interface_debug_print("sgp40: max current is %0.2fmA.\n", info.max_current_ma);
        sgp40_interface_debug_print("sgp40: max temperature is %0.1fC.\n", info.temperature_max);
        sgp40_interface_debug_print("sgp40: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        sgp40_interface_debug_print("sgp40: SCL connected to GPIOB PIN8.\n");
        sgp40_interface_debug_print("sgp40: SDA connected to GPIOB PIN9.\n");
        
        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;
    
    /* stm32f407 clock init and hal init */
    clock_init();
    
    /* delay init */
    delay_init();
    
    /* uart init */
    uart_init(115200);
    
    /* shell init && register sgp40 function */
    shell_init();
    shell_register("sgp40", sgp40);
    uart_print("sgp40: welcome to libdriver sgp40.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart_print("sgp40: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("sgp40: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("sgp40: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("sgp40: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("sgp40: param is invalid.\n");
            }
            else
            {
                uart_print("sgp40: unknown status code.\n");
            }
            uart_flush();
        }
        delay_ms(100);
    }
}
