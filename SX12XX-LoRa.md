# Ra-01S_SX1268_LoRa_433


https://github.com/StuartsProjects/SX12XX-LoRa/blob/master/examples/SX126x_examples/LowMemory/8_LoRa_LowMemory_TX/8_LoRa_LowMemory_TX.ino

https://github.com/StuartsProjects/SX12XX-LoRa/tree/master


Reliable Packets with LoRa - Part 1

https://stuartsprojects.github.io/2021/08/25/Reliable-Packets-Part1.html

```C++
/*******************************************************************************************************
  Programs for Arduino - Copyright of the author Stuart Robinson - 08/02/20

  This program is supplied as is, it is up to the user of the program to decide if the program is
  suitable for the intended purpose and free from errors.
*******************************************************************************************************/

/*******************************************************************************************************
  Program Operation - The program transmits a packet without using a processor buffer, the LoRa device
  internal buffer is filled direct with variables. The program is a simulation of the type of packet
  that might be sent from a GPS tracker. Note that in this example a buffer of text is part of the
  transmitted packet, this does need a processor buffer which is used to fill the LoRa device internal
  buffer, if you don't need to transmit text then the uint8_t trackerID[] = "Tracker1"; definition
  can be ommited.

  The matching receiving program '9_LoRa_LowMemory_RX' can be used to receive and display the packet,
  though the program  '15_LoRa_RX_Structure' should receive it as well, since the packet contents are
  the same.

  The contents of the packet received, and printed to serial monitor, should be;

  "tracker1" (buffer)      - trackerID
  1+             (uint32_t)    - packet count
  51.23456       (float)       - latitude
  -3.12345       (float)       - longitude
  199            (uint16_t)    - altitude
  8              (uint8_t)     - number of satellites
  3999           (uint16_t)    - battery voltage
  -9             (int8_t)      - temperature

  Serial monitor baud rate is set at 9600.













Sketch uses 4987 bytes (15%) of program storage space. Maximum is 32768 bytes.
Global variables use 233 bytes (7%) of dynamic memory, leaving 2839 bytes for local variables. Maximum is 3072 bytes.


Sketch uses 4622 bytes (14%) of program storage space. Maximum is 32768 bytes.
Global variables use 233 bytes (7%) of dynamic memory, leaving 2839 bytes for local variables. Maximum is 3072 bytes.



*******************************************************************************************************/

#include <SPI.h>
#include <SX126XLT.h>
#include "Settings.h"

SX126XLT LT;

uint32_t TXpacketCount = 0;
uint8_t TXPacketL;
uint32_t startmS, endmS;

void loop()
{
  TXpacketCount++;

  if (Send_Test_Packet())
  {
    //Serial.print(TXpacketCount);
   // Serial.print(F(" "));
    //Serial.print(TXPacketL);
    //Serial.print(F(" Bytes Sent"));
    //Serial.print(F(" "));
    //Serial.print(endmS - startmS);
    //Serial.print(F("mS"));
  }
  else
  {
    //Serial.print(F("Send Error - IRQreg,"));
    //Serial.print(LT.readIrqStatus(), HEX);
  }

  //Serial.println();
  //delay(packet_delay);
}


uint8_t Send_Test_Packet()
{
  //The SX12XX buffer is filled with variables of a known type and order. Make sure the receiver
  //uses the same variable type and order to read variables out of the receive buffer.

  //float latitude, longitude;
  //uint16_t altitude, voltage;
  //uint8_t satellites;
  //int16_t temperature;
  uint8_t len;

  //test data
  //uint8_t trackerID[] = "tracker1";
  //latitude = 51.23456;
  //longitude = -3.12345;
  //altitude = 199;
  //satellites = 9;
  //voltage = 3999;
  //temperature = -9;

  LT.startWriteSXBuffer(0);                         //start the write at location 0
  //LT.writeBuffer(trackerID, sizeof(trackerID));     //= 13 bytes (12 characters plus null (0) at end)
  LT.writeUint32(TXpacketCount);                    //+4 = 17 bytes
  //LT.writeFloat(latitude);                          //+4 = 21 bytes
  //LT.writeFloat(longitude);                         //+4 = 25 bytes
  //LT.writeUint16(altitude);                         //+2 = 27 bytes
  //LT.writeUint8(satellites);                        //+1 = 28 bytes
  //LT.writeUint16(voltage);                          //+2 = 30 bytes
  //LT.writeInt8(temperature);                        //+1 = 31 bytes total to send
  len = LT.endWriteSXBuffer();

  //digitalWrite(LED1, HIGH);
  startmS = millis();

  TXPacketL = LT.transmitSXBuffer(0, len, 5000, TXpower, WAIT_TX);   //set a TX timeout of 5000mS

  endmS = millis();

  //digitalWrite(LED1, LOW);

  return TXPacketL;
}


void led_Flash(uint16_t flashes, uint16_t delaymS)
{
  uint16_t index;
  for (index = 1; index <= flashes; index++)
  {
    //digitalWrite(LED1, HIGH);
    //delay(delaymS);
    //digitalWrite(LED1, LOW);
    //delay(delaymS);
  }
}


void setup()
{
  //pinMode(LED1, OUTPUT);
  led_Flash(2, 125);

  //Serial.begin(9600);

  SPI.begin();

  if (LT.begin(NSS, NRESET, RFBUSY, DIO1, LORA_DEVICE))
  {
    led_Flash(2, 125);
  }
  else
  {
    //Serial.println(F("Device error"));
    while (1)
    {
      led_Flash(50, 50);                                            //long fast speed flash indicates device error
    }
  }

  LT.setupLoRa(Frequency, Offset, SpreadingFactor, Bandwidth, CodeRate, Optimisation);

  //Serial.println(F("Transmitter ready"));
  //Serial.println();
}
```

# SETTINGS.H

```C++
/*******************************************************************************************************
  Programs for Arduino - Copyright of the author Stuart Robinson - 06/02/20

  This program is supplied as is, it is up to the user of the program to decide if the program is
  suitable for the intended purpose and free from errors.
*******************************************************************************************************/

//*******  Setup hardware pin definitions here ! ***************

//These are the pin definitions for one of my own boards, the Easy Pro Mini,
//be sure to change the definitions to match your own setup.

#define NSS 10                                  //select pin on LoRa device
#define NRESET 9                                //reset pin on LoRa device
#define LED1 8                                  //on board LED, high for on
#define RFBUSY 7                                //SX126X busy pin 
#define DIO1 3                                  //DIO1 pin on LoRa device, used for RX and TX done 

#define LORA_DEVICE DEVICE_SX1268               //we need to define the device we are using


//*******  Setup LoRa Parameters Here ! ***************

//LoRa Modem Parameters
const uint32_t Frequency = 434000000;           //frequency of transmissions in hertz
const uint32_t Offset = 0;                      //offset frequency for calibration purposes

const uint8_t Bandwidth = LORA_BW_125;          //LoRa bandwidth
const uint8_t SpreadingFactor = LORA_SF7;       //LoRa spreading factor
const uint8_t CodeRate = LORA_CR_4_5;           //LoRa coding rate
const uint8_t Optimisation = LDRO_AUTO;         //low data rate optimisation setting, normally set to auto

const int8_t TXpower = 10;                      //LoRa transmit power in dBm

const uint16_t packet_delay = 1000;             //mS delay between packets
```