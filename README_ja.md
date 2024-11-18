[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver SGP40

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/sgp40/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

SGP40 は、空気清浄機またはデマンド制御換気システムに簡単に統合できるように設計されたデジタル ガス センサーです。 Sensirion の CMOSens ® テクノロジーは、デジタル I 2 C インターフェイスと温度制御されたマイクロ ホットプレートを備えたシングル チップ上で完全で使いやすいセンサー システムを提供し、湿度補償された VOC ベースの室内空気質信号を提供します。 出力信号は Sensirion の強力なガス指数アルゴリズムによって直接処理され、生の信号をドア内の空気品質の堅牢な尺度として VOC 指数に変換できます。 ガス インデックス アルゴリズムは、センサーがさらされる環境に自動的に適応します。 センシング要素とガスインデックスアルゴリズムの両方は、現実世界のアプリケーションに存在する汚染ガスに対する比類のない堅牢性を特徴としており、独自の長期安定性と低いドリフトおよびデバイス間のばらつきを実現します。 非常に小型の 2.44 x 2.44 x 0.85 mm 3 DFN パッケージにより、限られたスペースでのアプリケーションが可能になります。 Sensirion の最先端の製造プロセスは、高い再現性と信頼性を保証します。 テープ アンド リール パッケージと標準 SMD アセンブリ プロセスへの適合性により、SGP40 は大量生産用途向けに設計されています。

LibDriver SGP40 は、LibDriver によって起動された SGP40 のフル機能ドライバーです。VOC 読み取り、温湿度補正などの機能を提供します。 LibDriver は MISRA に準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
    - [LibDriver](#LibDriver)
    - [Gas Index Algorithm](#Gas-Index-Algorithm)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver SGP40のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver SGP40用のプラットフォームに依存しないIICバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver SGP40ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver SGP40プログラミング例が含まれています。

/ docディレクトリには、LibDriver SGP40オフラインドキュメントが含まれています。

/ datasheetディレクトリには、SGP40データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないIICバステンプレートを参照して、指定したプラットフォームのIICバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/sgp40/index.html](https://www.libdriver.com/docs/sgp40/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

#### LibDriver

このリポジトリ内のすべてのオリジナル ドライバー ソース コードは、Copyright (c) 2015 - present LibDriver です。このソース コードは、以下のように MIT ライセンス (MIT) に基づいてライセンスされます。

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

#### Gas Index Algorithm

このリポジトリのガスインデックスアルゴリズムのソースコード（driver_sgp40_algorithm.c および driver_sgp40_algorithm.h）は、Copyright (c) 2022、Sensirion AG です。このソースコードは、以下のように BSD 3 条項ライセンスに基づいてライセンスされています。

Copyright (c) 2022、Sensirion AG 無断転載禁止。

BSD 3 条項ライセンス

ソースおよびバイナリ形式での再配布および使用は、変更の有無にかかわらず、以下の条件が満たされる場合に許可されます:

1. ソースコードの再配布には、上記の著作権表示、この条件リスト、および以下の免責事項が保持される必要があります。

2. バイナリ形式での再配布には、上記の著作権表示、この条件リスト、および以下の免責事項を、配布に付属するドキュメントおよび/またはその他の資料に再現する必要があります。

3. 著作権所有者の名前またはその貢献者の名前は、事前に書面による許可を得ることなく、このソフトウェアから派生した製品の推奨または宣伝に使用できません。

このソフトウェアは、著作権所有者および貢献者によって「現状のまま」提供されます。

また、明示的または黙示的な保証（商品性および特定目的への適合性に関する黙示的な保証を含みますが、これに限定されません）は、

否認されます。いかなる場合も、著作権所有者または貢献者は、契約、厳格責任、不法行為（過失またはその他を含む）を問わず、このソフトウェアの使用から何らかの形で生じた直接的、間接的、偶発的、特別、懲罰的、または結果的な損害（代替品またはサービスの調達、使用、データ、または利益の喪失、または事業中断を含むがこれらに限定されない）に対して、たとえそのような損害の可能性について知らされていたとしても、いかなる責任理論に基づいても、一切の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。