

/*


serial removed

Sketch uses 2334 bytes (7%) of program storage space. Maximum is 30720 bytes.
Global variables use 26 bytes (1%) of dynamic memory, leaving 2022 bytes for local variables. Maximum is 2048 bytes.

*/

#include <SX126x.h>
//#include <Ra01S.h>

//#define SSPIN PIN_PA0  // SPI select
//#define RESET PIN_PA7 // Reset
//#define BUSY  PIN_PA6  //  Busy
//#define DIO1  PIN_PA3 //  Interrupt DIO1 

#define SSPIN 10  // SPI select
#define RESET 9 // Reset
#define BUSY  4  //  Busy
//#define DIO1  6 //  Interrupt DIO1 



#define RF_FREQUENCY                                433000000 // Hz  center frequency
#define TX_OUTPUT_POWER                             22        // dBm tx output power
#define LORA_BANDWIDTH                              4         // bandwidth=125khz  0:250kHZ,1:125kHZ,2:62kHZ,3:20kHZ.... look for radio line 392                                                               
#define LORA_SPREADING_FACTOR                       7        // spreading factor=11 [SF5..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]

#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false     // variable data payload
#define LORA_IQ_INVERSION_ON                        false
#define LORA_PAYLOADLENGTH                          0         // 0: variable receive length 
                                                               // 1..255 payloadlength

                                                               
//SX126x  lora(PD5,               //Port-Pin Output: SPI select
//             PD6,               //Port-Pin Output: Reset 
//             PD7,               //Port-Pin Input:  Busy
//             PB0                //Port-Pin Input:  Interrupt DIO1 
//             );

//SX126x  lora(PB2,               //Port-Pin Output: SPI select
//             PB1,               //Port-Pin Output: Reset 
//             PD4,               //Port-Pin Input:  Busy
//             PD6                //Port-Pin Input:  Interrupt DIO1 
//             );
             
//SX126x  lora(10,               //Port-Pin Output: SPI select
//             9,               //Port-Pin Output: Reset 
//             4                //Port-Pin Input:  Busy
//             );
             

SX126x  lora(SSPIN,RESET,BUSY);

void setup() 
{
  //Serial.begin(115200);
  //Serial.println("START");

             
    //Serial.println("Disable TCXO");
  int16_t ret = lora.begin(RF_FREQUENCY,             //frequency in Hz                     
                           TX_OUTPUT_POWER);          //tx power in dBm
  if (ret != ERR_NONE) while(1) {delay(1);}
  
  lora.LoRaConfig(LORA_SPREADING_FACTOR, 
                    LORA_BANDWIDTH, 
                    LORA_CODINGRATE, 
                    LORA_PREAMBLE_LENGTH, 
                    LORA_PAYLOADLENGTH, 
                    false,              //crcOn  
                    false);             //invertIrq
   

}

//uint8_t i;

void loop() 
{
//  Serial.println("Ping");
//  lora.Send(&i, 1,  SX126x_TXMODE_SYNC);
//  i++;
//  delay(5000);

  uint8_t txData[255];
  sprintf((char *)txData, "Hello World %lu", millis());
  uint8_t len = strlen((char *)txData);

  // Wait for transmission to complete
  if (lora.Send(txData, len, SX126x_TXMODE_SYNC)) {
    //Serial.println("Send success");
  } else {
    //Serial.println("Send fail");
  }
  delay(5000);

  // Do not wait for the transmission to be completed
  //lora.Send(txData, len, SX126x_TXMODE_ASYNC );
  //delay(5000);
}