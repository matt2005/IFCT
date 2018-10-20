
/* EXAMPLE SKETCH FOR TEENSY 3.6 + TINDIE DUAL CANBUS BACKPACK
 * INPUT CANBUS DATA, OUTPUT SERIAL MONITOR FORMAT
 * <HH:MM:SS.SSS> CAN<#> <Arbitration ID> <"RTR" or Data in hex format>
 * https://github.com/tonton81/IFCT for library information
 */

#include <IFCT.h>
const int can0_enable = 28;  // defines can0 enable pin, pin 28, as can0_enable
const int can1_enable = 35;  // defines can1 enable pin, pin 28, as can1_enable

void setup() {
  Serial.begin(115200);
  Serial.println("Time: Can: ID: Data:");
  pinMode(can0_enable, OUTPUT);  // for the can0 transceiver enable pin
  digitalWrite(can0_enable, LOW);  // can0 enable pin needs to be low to receive messages
  pinMode(can1_enable, OUTPUT);  // for the can1 transceiver enable pin
  digitalWrite(can1_enable, LOW);  // can1 enable pin needs to be low to receive messages
  Can0.setRX(ALT);  // sets Rx alternate pin for fusion 360 dual can backpack
  Can0.setTX(ALT);  // sets Tx alternate pin for fusion 360 dual can backpack
  Can0.setBaudRate(500000); // Canbus 0 Bitrate in bits per second.
  Can0.enableFIFO();
  Can0.enableFIFOInterrupt();
  Can0.onReceive(canSniff0);
  Can0.intervalTimer(); // enable queue system and run callback in background.
  
  Can1.setRX();  // sets Rx alternate pin for fusion 360 dual can backpack
  Can1.setTX();  // sets Tx alternate pin for fusion 360 dual can backpack
  Can1.setBaudRate(500000); // Canbus 1 Bitrate in bits per second.
  Can1.enableFIFO(1);
  Can1.enableFIFOInterrupt(1);
  Can1.onReceive(canSniff1);
  Can1.intervalTimer(); // enable queue system and run callback in background.


}
  /* Canbus methods/classes options.
   *  msg.mb              returns mailbox,
   *  msg.bus             returns 0 for CAN0, 1 for CAN1
   *  msg.len             returns DLC,
   *  msg.flags.extended  returns 0 if Standard ID, 1 if extended ID,
   *  msg.rtr             returns 0 if no RTR flag, 1 if RTR flag,
   *  msg.timestamp       returns 16b value of timestamp in millis,
   *  msg.buf[x]          returns data in element [x]
   */
void loop() {
  /* 
   *          Write TX Functions Here:
   */
}

void canSniff0(const CAN_message_t &msg) { // global callback

  /*      ternary function for bridging can0 : can 1 
   *     ( msg.bus ) ? Can0.write(msg) : Can1.write(msg); 
   */
  
  unsigned long runMillis= millis();
  unsigned long allSeconds=millis()/1000;
  uint16_t runHours= allSeconds/3600;
  uint8_t secsRemaining=allSeconds%3600;
  uint8_t runMinutes=secsRemaining/60;
  uint8_t remainder_sec=secsRemaining%60;
  uint16_t remainder_ms=runMillis%1000;
  char buf[512]; char *pbuf = buf; uint16_t text_length = 0;
  
  sprintf(pbuf,"%02d:%02d:%02d.%03d CAN%01d",runHours,runMinutes,remainder_sec,remainder_ms,msg.bus);
  pbuf += 17; text_length +=17;
  switch (msg.flags.extended){
    case 0: // if MESSAGE ID is 11 Bit
      sprintf(pbuf,"      %003x",msg.id);
    break;
    case 1: // if MESSAGE ID is 29 Bit
      sprintf(pbuf," %00000008x",msg.id);
    break;
  }
  pbuf+=9; text_length +=9;
  switch(msg.rtr){
    case 0: // if MESSAGE is Normal Frame
      for ( uint8_t i = 0; i < msg.len; i++ ) {
        sprintf(pbuf," %02X",msg.buf[i]);
        pbuf += 3; text_length +=3;
      }
    break;
    case 1: // if MESSAGE is RTR Frame
      sprintf(pbuf," RTR");
      pbuf += 4; text_length +=4;
    break;
  }
  sprintf(pbuf,"\r\n");
  pbuf += 2; text_length +=2;
  Serial.write(buf,text_length);
}

void canSniff1(const CAN_message_t &msg) { // global callback

  /*      ternary function for bridging can0 : can 1 
   *     ( msg.bus ) ? Can0.write(msg) : Can1.write(msg); 
   */
  
  unsigned long runMillis= millis();
  unsigned long allSeconds=millis()/1000;
  uint16_t runHours= allSeconds/3600;
  uint8_t secsRemaining=allSeconds%3600;
  uint8_t runMinutes=secsRemaining/60;
  uint8_t remainder_sec=secsRemaining%60;
  uint16_t remainder_ms=runMillis%1000;
  char buf[512]; char *pbuf = buf; uint16_t text_length = 0;
  
  sprintf(pbuf,"%02d:%02d:%02d.%03d CAN%01d",runHours,runMinutes,remainder_sec,remainder_ms,msg.bus);
  pbuf += 17; text_length +=17;
  switch (msg.flags.extended){
    case 0: // if MESSAGE ID is 11 Bit
      sprintf(pbuf,"      %003x",msg.id);
    break;
    case 1: // if MESSAGE ID is 29 Bit
      sprintf(pbuf," %00000008x",msg.id);
    break;
  }
  pbuf+=9; text_length +=9;
  switch(msg.rtr){
    case 0: // if MESSAGE is Normal Frame
      for ( uint8_t i = 0; i < msg.len; i++ ) {
        sprintf(pbuf," %02X",msg.buf[i]);
        pbuf += 3; text_length +=3;
      }
    break;
    case 1: // if MESSAGE is RTR Frame
      sprintf(pbuf," RTR");
      pbuf += 4; text_length +=4;
    break;
  }
  sprintf(pbuf,"\r\n");
  pbuf += 2; text_length +=2;
  Serial.write(buf,text_length);
}
