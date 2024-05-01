# Ra-01S_SX1268_LoRa_433

Using Ra-01S_SX1268_LoRa_433

https://github.com/nopnop2002/Arduino-LoRa-Ra01S

https://github.com/nopnop2002/Arduino-LoRa-Ra01S/blob/main/example/Ra01S-TX/Ra01S-TX.ino

Works well with the **Ra-01S SX1268 433MHz Lora Module**.

```C++
/*
LORA

https://github.com/sandeepmistry/arduino-LoRa/tree/master

https://github.com/sandeepmistry/arduino-LoRa/blob/master/API.md

https://gitlab.com/iot-lab-org/ATtiny84_low_power_LoRa_node_OOP

https://forum.arduino.cc/t/arduino-lora-sending-0x00/673191/3

https://stackoverflow.com/questions/69037253/i-use-a-lora-sx1278-on-attiny44-a-and-my-spi-connection-is-unstable-sometimes

https://arduino.stackexchange.com/questions/86586/lora-recieves-only-2-4-packets-then-stops-working

https://www.thethingsnetwork.org/forum/t/attiny85-rfm95-temperature-sensor/11211

https://www.mobilefish.com/developer/lorawan/lorawan_quickguide_build_lora_node_rfm95_arduino_pro_mini.html

https://github.com/hallard/Mini-LoRa

https://forum.arduino.cc/t/lora-ra-01-question/562206/16

https://github.com/nopnop2002/Arduino-LoRa-Ra01S

SPI pins on the ATMEGA are 9(sck) 10(mosi) and 11(miso) physically and digiatal pins 14(miso) 15(sck) and 16(mosi)
when using an Arduino Nano these are Digital pins 11(Mosi) 12(Miso) and 13(SCK).

WORKS

https://github.com/chandrawi/LoRaRF-Arduino

https://github.com/nopnop2002/Arduino-LoRa-Ra01S


WITH SERIAL

Sketch uses 7719 bytes (23%) of program storage space. Maximum is 32768 bytes.
Global variables use 182 bytes (5%) of dynamic memory, leaving 2890 bytes for local variables. Maximum is 3072 bytes.

SERIAL REMOVED
Sketch uses 5846 bytes (17%) of program storage space. Maximum is 32768 bytes.
Global variables use 182 bytes (5%) of dynamic memory, leaving 2890 bytes for local variables. Maximum is 3072 bytes.

pinMode and digitalWrite replaced
Sketch uses 5642 bytes (17%) of program storage space. Maximum is 32768 bytes.
Global variables use 182 bytes (5%) of dynamic memory, leaving 2890 bytes for local variables. Maximum is 3072 bytes.

*/

#include <Ra01S.h>

#define RF_FREQUENCY                                433000000 // Hz  center frequency
//#define RF_FREQUENCY                                866000000 // Hz  center frequency
//#define RF_FREQUENCY                                915000000 // Hz  center frequency
#define TX_OUTPUT_POWER                             17        // dBm tx output power
#define LORA_BANDWIDTH                              4         // bandwidth
                                                              // 2: 31.25Khz
                                                              // 3: 62.5Khz
                                                              // 4: 125Khz
                                                              // 5: 250KHZ
                                                              // 6: 500Khz                                                               
#define LORA_SPREADING_FACTOR                       7         // spreading factor [SF5..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]

#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_PAYLOADLENGTH                          0         // 0: Variable length packet (explicit header)
                                                              // 1..255  Fixed length packet (implicit header)

//#define USE_EBYTE

/*
 * for ATmega328/2560
 * VCC    3V3/3V3
 * GND    GND/GND
 * SCK    13/52
 * MISO   12/50
 * MOSI   11/51
 * NSS     5/5
 * RST     6/6
 * BUSY    7/7
 * TXEN    8/8 for EBYTE
 * RXEN    9/9 for EBYTE
 */
 
SX126x  lora(10,               //Port-Pin Output: SPI select
             9,               //Port-Pin Output: Reset 
             4                //Port-Pin Input:  Busy
             );

void setup() 
{
  delay(1000);
  Serial.begin(115200);

  //lora.DebugPrint(true);

#ifdef USE_EBYTE
  Serial.println("Enable TCXO");
  int16_t ret = lora.begin(RF_FREQUENCY,              //frequency in Hz
                           TX_OUTPUT_POWER,           //tx power in dBm
                           3.3,                       //use TCXO
                           true);                     //use TCXO
  if (ret != ERR_NONE) while(1) {delay(1);}
#else
  Serial.println("Disable TCXO");
  int16_t ret = lora.begin(RF_FREQUENCY,              //frequency in Hz
                           TX_OUTPUT_POWER);          //tx power in dBm
  if (ret != ERR_NONE) while(1) {delay(1);}
#endif // USE_EBYTE

  lora.LoRaConfig(LORA_SPREADING_FACTOR, 
                  LORA_BANDWIDTH, 
                  LORA_CODINGRATE, 
                  LORA_PREAMBLE_LENGTH, 
                  LORA_PAYLOADLENGTH, 
                  true,               //crcOn  
                  false);             //invertIrq

Serial.println("##### SETUP DONE #####");
}

void loop() 
{
  uint8_t txData[255];
  sprintf((char *)txData, "Hello World %lu", millis());
  uint8_t len = strlen((char *)txData);

  // Wait for transmission to complete
  if (lora.Send(txData, len, SX126x_TXMODE_SYNC)) 
  {
    Serial.println("Send success");
  } else {
    Serial.println("Send fail");
  }

  // Do not wait for the transmission to be completed
  //lora.Send(txData, len, SX126x_TXMODE_ASYNC );

  delay(5000);
}
```
