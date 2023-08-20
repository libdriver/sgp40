### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

IIC Pin: SCL/SDA PB8/PB9.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. SGP40

#### 3.1 Command Instruction

1. Show sgp40 chip and driver information.

   ```shell
   sgp40 (-i | --information)
   ```

2. Show sgp40 help.

   ```shell
   sgp40 (-h | --help)
   ```

3. Show sgp40 pin connections of the current board.

   ```shell
   sgp40 (-p | --port)
   ```

4. Run sgp40 register test.

   ```shell
   sgp40 (-t reg | --test=reg)
   ```

5. Run sgp40 read test, num means test times.

   ```shell
   sgp40 (-t read | --test=read) [--times=<num>]
   ```

6. Run sgp40 read without compensation function, num means read times.

   ```shell
   sgp40 (-e read-without-compensation | --example=read-without-compensation) [--times=<num>]
   ```

7. Run sgp40 read function, num means read times, temp means current temperature and rh means current relative humidity.

   ```shell
   sgp40 (-e read | --example=read) [--times=<num>] [--temperature=<temp>] [--humidity=<rh>]
   ```

8. Get sgp40 serial id.

   ```shell
   sgp40 (-e serial-id | --example=serial-id)
   ```

#### 3.2 Command Example

```shell
sgp40 -i

sgp40: chip is Sensirion SGP40.
sgp40: manufacturer is Sensirion.
sgp40: interface is IIC.
sgp40: driver version is 1.0.
sgp40: min supply voltage is 1.7V.
sgp40: max supply voltage is 3.6V.
sgp40: max current is 4.00mA.
sgp40: max temperature is 55.0C.
sgp40: min temperature is -20.0C.
```

```shell
sgp40 -p

sgp40: SCL connected to GPIOB PIN8.
sgp40: SDA connected to GPIOB PIN9.
```

```shell
sgp40 -t reg

sgp40: chip is Sensirion SGP40.
sgp40: manufacturer is Sensirion.
sgp40: interface is IIC.
sgp40: driver version is 1.0.
sgp40: min supply voltage is 1.7V.
sgp40: max supply voltage is 3.6V.
sgp40: max current is 4.00mA.
sgp40: max temperature is 55.0C.
sgp40: min temperature is -20.0C.
sgp40: start register test.
sgp40: sgp40_soft_reset test.
sgp40: check soft reset ok.
sgp40: sgp40_get_measure_test test.
sgp40: test result is 0xD400.
sgp40: sgp40_get_serial_id test.
sgp40: check get serial id ok.
sgp40: serial id 0x0000 0x0343 0x9669.
sgp40: sgp40_turn_heater_off test.
sgp40: check turn heater off ok.
sgp40: sgp40_humidity_convert_to_register test.
sgp40: 76.80% humidity convert to register 0xC49A.
sgp40: sgp40_temperature_convert_to_register test.
sgp40: 16.70C temperature convert to register 0x5A41.
sgp40: finish register test.
```

```shell
sgp40 -t read --times=3

sgp40: chip is Sensirion SGP40.
sgp40: manufacturer is Sensirion.
sgp40: interface is IIC.
sgp40: driver version is 1.0.
sgp40: min supply voltage is 1.7V.
sgp40: max supply voltage is 3.6V.
sgp40: max current is 4.00mA.
sgp40: max temperature is 55.0C.
sgp40: min temperature is -20.0C.
sgp40: start read test.
sgp40: measure raw test.
sgp40: raw voc is 0x0002 and voc gas index is 0.
sgp40: raw voc is 0x5A63 and voc gas index is 0.
sgp40: raw voc is 0x5CF7 and voc gas index is 0.
sgp40: measure raw without compensation test.
sgp40: raw voc is 0x5ECE and voc gas index is 0.
sgp40: raw voc is 0x6063 and voc gas index is 0.
sgp40: raw voc is 0x617E and voc gas index is 0.
sgp40: finish read test.
```

```shell
sgp40 -e read --temperature=25.0 --humidity=50.0 --times=3

sgp40: 1/3.
sgp40: voc gas index is 0.
sgp40: 2/3.
sgp40: voc gas index is 0.
sgp40: 3/3.
sgp40: voc gas index is 0.
```

```shell
sgp40 -e read-without-compensation --times=3

sgp40: 1/3.
sgp40: voc gas index is 0.
sgp40: 2/3.
sgp40: voc gas index is 0.
sgp40: 3/3.
sgp40: voc gas index is 0.
```

```shell
sgp40 -e serial-id

sgp40: serial id 0x0000 0x0343 0x9669.
```

```shell
sgp40 -h

Usage:
  sgp40 (-i | --information)
  sgp40 (-h | --help)
  sgp40 (-p | --port)
  sgp40 (-t reg | --test=reg)
  sgp40 (-t read | --test=read) [--times=<num>]
  sgp40 (-e read | --example=read) [--times=<num>] [--temperature=<temp>] [--humidity=<rh>]
  sgp40 (-e read-without-compensation | --example=read-without-compensation) [--times=<num>]
  sgp40 (-e serial-id | --example=serial-id)

Options:
  -e <read | read-without-compensation | serial-id>, --example=<read | read-without-compensation | serial-id>
                                          Run the driver example.
  -h, --help                              Show the help.
      --humidity=<rh>                     Set the humidity.([default: 50.0f])
      --temperature=<temp>                Set the temperature.([default: 25.0f])
  -i, --information                       Show the chip information.
  -p, --port                              Display the pin connections of the current board.
  -t <reg | read>, --test=<reg | read>    Run the driver test.
      --times=<num>                       Set the running times.([default: 3])
```

