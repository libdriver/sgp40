[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver SGP40

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/sgp40/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

SGP40是一款数字气体传感器，设计用于轻松集成到空气净化器或按需控制的通风系统中。Sensionon的CMOSens®技术在单个芯片上提供了一个完整、易于使用的传感器系统，该系统具有数字I2C接口和温度控制的微热板，可提供基于VOC的湿度补偿室内空气质量信号。Sensionon强大的气体指数算法可以直接处理输出信号，将原始信号转换为VOC指数，作为室内空气质量的可靠衡量标准。气体指数算法可自动适应传感器所处的环境。传感元件和气体指数算法对现实应用中存在的污染气体具有无与伦比的鲁棒性，能够实现独特的长期稳定性以及低漂移和设备间的变化。非常小的2.44 x 2.44 x 0.85毫米3 DFN封装可在有限的空间内应用。Sensionon最先进的生产工艺保证了高再现性和可靠性。胶带和卷轴包装以及对标准SMD组装工艺的适用性使SGP40注定适用于大批量应用。

LibDriver SGP40是LibDriver推出的SGP40全功能驱动，该驱动提供VOC读取、温湿度矫正等功能并且它符合MISRA标准。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic](#example-basic)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
    - [LibDriver](#LibDriver)
    - [Gas Index Algorithm](#Gas-Index-Algorithm)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver SGP40的源文件。

/interface目录包含了LibDriver SGP40与平台无关的IIC总线模板。

/test目录包含了LibDriver SGP40驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver SGP40编程范例。

/doc目录包含了LibDriver SGP40离线文档。

/datasheet目录包含了SGP40数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

/misra目录包含了LibDriver MISRA代码扫描结果。

### 安装

参考/interface目录下与平台无关的IIC总线模板，完成指定平台的IIC总线驱动。

将/src目录，您使用平台的接口驱动和您开发的驱动加入工程，如果您想要使用默认的范例驱动，可以将/example目录加入您的工程。

### 使用

您可以参考/example目录下的编程范例完成适合您的驱动，如果您想要使用默认的编程范例，以下是它们的使用方法。

#### example basic

```C
#include "driver_sgp40_basic.h"

uint8_t res;
uint16_t id[3];
uint32_t i;
uint32_t times = 3;
float rh = 50.0f;
float temp = 25.0f;
int32_t voc_gas_index;

/* init */
res = sgp40_basic_init();
if (res != 0)
{
    return 1;
}

...
    
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
    
    ...
}

...
    
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
    
    ...
}

...

/* get serial id */
res = sgp40_basic_get_serial_id(id);
if (res != 0)
{
    (void)sgp40_basic_deinit();

    return 1;
}

/* output */
sgp40_interface_debug_print("sgp40: serial id 0x%04X 0x%04X 0x%04X.\n", (uint16_t)(id[0]), (uint16_t)(id[1]), (uint16_t)(id[2]));  

...
    
/* deinit */
(void)sgp40_basic_deinit();

return 0;
```

### 文档

在线文档: [https://www.libdriver.com/docs/sgp40/index.html](https://www.libdriver.com/docs/sgp40/index.html)。

离线文档: /doc/html/index.html。

### 贡献

请参考CONTRIBUTING.md。

### 版权

#### LibDriver

此存储库中的所有原始驱动程序源代码版权所有（c）2015年至今的LibDriver。此源代码根据MIT许可证（MIT）获得许可，如下所示。

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

#### Gas Index Algorithm

此存储库中的气体指数算法源代码（driver_sgp40_algorithm.c和driver_sgp40_algoritm.h）版权所有（c）2022，Sensirion AG。此源代码根据BSD 3条款许可证授权，如下所示。

Sensirion AG版权所有（c）2022。

BSD 3-条款许可

以源代码和二进制形式重新分发和使用，有或没有

如果满足以下条件，则允许进行修改：

1.重新分发源代码必须保留上述版权声明，本

条件列表和以下免责声明。

2.二进制形式的再分发必须复制上述版权声明，

文档中的条件列表和以下免责声明

和/或分发时提供的其他材料。

3.版权持有人的姓名或名称

贡献者可能被用来背书或推广来自以下来源的产品

本软件未经事先明确书面许可。

本软件由版权所有者和贡献者“按原样”提供

以及任何明示或暗示的保证，包括但不限于

对适销性和特定用途适用性的默示保证

否认。在任何情况下，版权持有人或贡献者均不承担责任

适用于任何直接、间接、附带、特殊、惩戒性或后果性

损害赔偿（包括但不限于采购替代货物或

服务；使用、数据或利润损失；或业务中断）然而

根据任何责任理论，无论是合同、严格责任，

或因使用而以任何方式产生的侵权行为（包括疏忽或其他）

即使被告知此类损坏的可能性。

### 联系我们

请联系lishifenging@outlook.com。