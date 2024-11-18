[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver SGP40

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/sgp40/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

SGP40是一款數位氣體感測器，設計用於輕鬆集成到空氣淨化器或按需控制的通風系統中。 Sensionon的CMOSens ® 科技在單個晶片上提供了一個完整、易於使用的感測器系統，該系統具有數位I2C介面和溫度控制的微熱板，可提供基於VOC的濕度補償室內空氣品質訊號。 Sensionon强大的氣體指數算灋可以直接處理輸出信號，將原始訊號轉換為VOC指數，作為室內空氣品質的可靠衡量標準。 氣體指數算灋可自動適應感測器所處的環境。 傳感元件和氣體指數算灋對現實應用中存在的污染氣體具有無與倫比的魯棒性，能够實現獨特的長期穩定性以及低漂移和設備間的變化。 非常小的2.44 x 2.44 x 0.85毫米3 DFN封裝可在有限的空間內應用。 Sensionon最先進的生產工藝保證了高再現性和可靠性。 膠帶和卷軸包裝以及對標準SMD組裝工藝的適用性使SGP40註定適用於大批量應用。

LibDriver SGP40是LibDriver推出的SGP40全功能驅動，該驅動提供VOC讀取、溫濕度矯正等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
    - [LibDriver](#LibDriver)
    - [Gas Index Algorithm](#Gas-Index-Algorithm)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver SGP40的源文件。

/interface目錄包含了LibDriver SGP40與平台無關的IIC總線模板。

/test目錄包含了LibDriver SGP40驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver SGP40編程範例。

/doc目錄包含了LibDriver SGP40離線文檔。

/datasheet目錄包含了SGP40數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的IIC總線模板，完成指定平台的IIC總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

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

### 文檔

在線文檔: [https://www.libdriver.com/docs/sgp40/index.html](https://www.libdriver.com/docs/sgp40/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

#### LibDriver

此存儲庫中的所有原始驅動程序原始程式碼版權所有（c）2015年至今的LibDriver。 此原始程式碼根據MIT許可證（MIT）獲得許可，如下所示。

版權（c）2015 -現在LibDriver版權所有

MIT許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔檔案（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發佈、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。 在任何情况下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自契约、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

#### Gas Index Algorithm

此存儲庫中的氣體指數算灋原始程式碼（driver_sgp40_algorithm.c和driver_sgp40_algoritm.h）版權所有（c）2022，Sensirion AG。 此原始程式碼根據BSD 3條款許可證授權，如下所示。

Sensirion AG版權所有（c）2022。

BSD 3-條款許可

以原始程式碼和二進位形式重新分發和使用，有或沒有

如果滿足以下條件，則允許進行修改：

1.重新分發原始程式碼必須保留上述版權聲明，本

條件清單和以下免責聲明。

2.二進位形式的再分發必須複製上述版權聲明，

檔案中的條件清單和以下免責聲明

和/或分發時提供的其他資料。

3.版權持有人的姓名或名稱

貢獻者可能被用來背書或推廣來自以下來源的產品

本軟件未經事先明確書面許可。

本軟件由版權所有者和貢獻者“按原樣”提供

以及任何明示或暗示的保證，包括但不限於

對適銷性和特定用途適用性的默示保證

否認。 在任何情况下，版權持有人或貢獻者均不承擔責任

適用於任何直接、間接、附帶、特殊、懲戒性或後果性

損害賠償（包括但不限於採購替代貨物或

服務； 使用、數據或利潤損失； 或業務中斷）然而

根據任何責任理論，無論是契约、嚴格責任，

或因使用而以任何管道產生的侵權行為（包括疏忽或其他）

即使被告知此類損壞的可能性。

### 聯繫我們

請聯繫lishifenging@outlook.com。