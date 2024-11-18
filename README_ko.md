[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver SGP40

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/sgp40/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

SGP40은 공기 청정기 또는 수요 제어 환기 시스템에 쉽게 통합할 수 있도록 설계된 디지털 가스 센서입니다. Sensirion의 CMOSens ® 기술은 디지털 I 2 C 인터페이스와 온도 제어 마이크로 핫플레이트를 특징으로 하는 단일 칩에 완벽하고 사용하기 쉬운 센서 시스템을 제공하여 습도 보상 VOC 기반 실내 공기 품질 신호를 제공합니다. 출력 신호는 Sensirion의 강력한 Gas Index Algorithm에 의해 직접 처리되어 원시 신호를 VOC 지수로 변환하여 도어 공기 품질에 대한 강력한 측정값으로 사용할 수 있습니다. 가스 지수 알고리즘은 센서가 노출된 환경에 자동으로 적응합니다. 감지 요소와 가스 지수 알고리즘 모두 실제 응용 분야에 존재하는 오염 가스에 대한 타의 추종을 불허하는 견고성을 특징으로 하여 고유한 장기 안정성과 낮은 드리프트 및 장치 간 변동을 가능하게 합니다. 매우 작은 2.44 x 2.44 x 0.85 mm 3 DFN 패키지는 제한된 공간에서 애플리케이션을 가능하게 합니다. Sensirion의 최첨단 생산 공정은 높은 재현성과 신뢰성을 보장합니다. 표준 SMD 조립 공정에 대한 적합성과 함께 테이프 및 릴 패키징을 통해 SGP40은 대량 응용 분야에 적합합니다.

LibDriver SGP40은 LibDriver에서 출시한 SGP40의 전체 기능 드라이버입니다. VOC 판독, 온도 및 습도 보정 및 기타 기능을 제공합니다. LibDriver는 MISRA를 준수합니다.

### 콘텐츠

  - [설명](#설명)
  - [설치](#설치)
  - [사용](#사용)
    - [example basic](#example-basic)
  - [문서](#문서)
  - [기고](#기고)
  - [저작권](#저작권)
    - [LibDriver](#LibDriver)
    - [Gas Index Algorithm](#Gas-Index-Algorithm)
  - [문의하기](#문의하기)

### 설명

/src 디렉토리에는 LibDriver SGP40의 소스 파일이 포함되어 있습니다.

/interface 디렉토리에는 LibDriver SGP40용 플랫폼 독립적인 IIC버스 템플릿이 포함되어 있습니다.

/test 디렉토리에는 LibDriver SGP40드라이버 테스트 프로그램이 포함되어 있어 칩의 필요한 기능을 간단히 테스트할 수 있습니다.

/example 디렉토리에는 LibDriver SGP40프로그래밍 예제가 포함되어 있습니다.

/doc 디렉토리에는 LibDriver SGP40오프라인 문서가 포함되어 있습니다.

/datasheet 디렉토리에는 SGP40데이터시트가 있습니다.

/project 디렉토리에는 일반적으로 사용되는 Linux 및 마이크로컨트롤러 개발 보드의 프로젝트 샘플이 포함되어 있습니다. 모든 프로젝트는 디버깅 방법으로 셸 스크립트를 사용하며, 자세한 내용은 각 프로젝트의 README.md를 참조하십시오.

/misra 에는 LibDriver misra 코드 검색 결과가 포함됩니다.

### 설치

/interface 디렉토리에서 플랫폼 독립적인IIC 버스 템플릿을 참조하여 지정된 플랫폼에 대한 IIC버스 드라이버를 완성하십시오.

/src 디렉터리, 플랫폼용 인터페이스 드라이버 및 자체 드라이버를 프로젝트에 추가합니다. 기본 예제 드라이버를 사용하려면 /example 디렉터리를 프로젝트에 추가합니다.

### 사용

/example 디렉터리의 예제를 참조하여 자신만의 드라이버를 완성할 수 있습니다. 기본 프로그래밍 예제를 사용하려는 경우 사용 방법은 다음과 같습니다.

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

### 문서

온라인 문서: [https://www.libdriver.com/docs/sgp40/index.html](https://www.libdriver.com/docs/sgp40/index.html).

오프라인 문서: /doc/html/index.html.

### 기고

CONTRIBUTING.md 를 참조하십시오.

### 저작권

#### LibDriver

이 저장소의 모든 원본 드라이버 소스 코드는 저작권(c) 2015 - 현재 LibDriver입니다. 이 소스 코드는 아래와 같이 MIT 라이선스(MIT)에 따라 라이선스가 부여됩니다.

저작권 (c) 2015 - 지금 LibDriver 판권 소유

MIT 라이선스(MIT)

이 소프트웨어 및 관련 문서 파일("소프트웨어")의 사본을 얻은 모든 사람은 이에 따라 무제한 사용, 복제, 수정, 통합, 출판, 배포, 2차 라이선스를 포함하여 소프트웨어를 처분할 수 있는 권리가 부여됩니다. 소프트웨어의 사본에 대한 라이선스 및/또는 판매, 그리고 소프트웨어가 위와 같이 배포된 사람의 권리에 대한 2차 라이선스는 다음 조건에 따릅니다.

위의 저작권 표시 및 이 허가 표시는 이 소프트웨어의 모든 사본 또는 내용에 포함됩니다.

이 소프트웨어는 상품성, 특정 목적에의 적합성 및 비침해에 대한 보증을 포함하되 이에 국한되지 않는 어떠한 종류의 명시적 또는 묵시적 보증 없이 "있는 그대로" 제공됩니다. 어떤 경우에도 저자 또는 저작권 소유자는 계약, 불법 행위 또는 기타 방식에 관계없이 소프트웨어 및 기타 소프트웨어 사용으로 인해 발생하거나 이와 관련하여 발생하는 청구, 손해 또는 기타 책임에 대해 책임을 지지 않습니다.

#### Gas Index Algorithm

이 저장소의 가스 인덱스 알고리즘 소스 코드(driver_sgp40_algorithm.c 및 driver_sgp40_algorithm.h)는 Copyright (c) 2022, Sensirion AG입니다. 이 소스 코드는 아래와 같이 BSD 3-Choose License에 따라 라이센스가 부여됩니다.

저작권(c) 2022, Sensirion AG 모든 권리 보유.

BSD 3 - 조항 라이선스

다음 조건을 충족하는 경우 수정 여부와 관계없이 소스 및 바이너리 형태로 재배포 및 사용이 허용됩니다.

1. 소스 코드 재배포는 위의 저작권 고지, 이 조건 목록 및 다음 면책 조항을 유지해야 합니다.

2. 바이너리 형태로 재배포하는 경우 위의 저작권 고지, 이 조건 목록 및 다음 면책 조항을 배포와 함께 제공된 문서 및/또는 기타 자료에 복제해야 합니다.

3. 저작권 소유자의 이름이나 기여자의 이름은 구체적인 사전 서면 허가 없이는 이 소프트웨어에서 파생된 제품을 보증하거나 홍보하는 데 사용될 수 없습니다.

이 소프트웨어는 저작권 소유자 및 기여자에 의해 "있는 그대로" 제공됩니다.

그리고 상품성 및 특정 목적에의 적합성에 대한 묵시적 보증을 포함하되 이에 국한되지 않는 명시적 또는 묵시적 보증은

면책됩니다. 어떠한 경우에도 저작권 소유자 또는 기여자는

직접적, 간접적, 우발적, 특별, 모범적 또는 결과적

손해(대체 상품 또는 서비스 조달, 사용, 데이터 또는 이익 손실, 또는 사업 중단을 포함하되 이에 국한되지 않음)에 대해 책임을 지지 않습니다.

그러나

계약, 엄격책임,

불법행위(과실 또는 기타 포함)에 대한 책임 이론에 따라 발생하든, 이 소프트웨어의 사용으로 인해 어떤 식으로든 발생하든, 그러한 손해의 가능성을 통보받은 경우에도 마찬가지입니다.

### 문의하기

연락주세요lishifenging@outlook.com.