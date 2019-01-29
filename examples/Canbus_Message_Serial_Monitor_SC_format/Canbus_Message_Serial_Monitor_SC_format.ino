/* EXAMPLE SKETCH FOR TEENSY 3.6 + Add-on Transceivers Backpack
 * INPUT CANBUS DATA, OUTPUT SERIAL MONITOR FORMAT
 * <HH:MM:SS.SSS> CAN<#> <Arbitration ID> <"RTR" or Data in hex format>
 * https://github.com/tonton81/IFCT for library information
 * https://www.tindie.com/products/Fusion/dual-can-bus-adapter-for-teensy-35-36/ for Add-on Tranceivers Backpack 
 */

#include <IFCT.h>
const int can0_enable = 28;  // defines can0 enable pin, pin 28, as can0_enable
const int can1_enable = 35;  // defines can1 enable pin, pin 28, as can1_enable
elapsedMillis timer1;
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
   *          Write TX Functions Here: example created below, define variable start values globally
   *          it's a good idea to write high frequency messages first...
   */
   
  CAN_message_t msg;
  if (timer1 >= 10){
    timer1 = 0;
    msg.bus = 0;
    msg.flags.extended = 0;
    msg.id = 0x123;
    msg.len = 4;
    msg.buf[0] = 0x01;
    msg.buf[1] = 0x02;
    msg.buf[2] = 0x03;
    msg.buf[3] = 0x04;
    msg.seq = 1;
    Can0.write(msg);
  }
}

void canSniff0(const CAN_message_t &msg) { // global callback
//  CAN_message_t current_msg = msg; //prerequesite for next line
//  current_msg.bus==1  ? current_msg.seq=1, Can0.write(current_msg) : current_msg.seq=1, Can1.write(current_msg); //ternary function to bridge canbus comms
  unsigned long runSeconds = ((millis()/1000)%10000000000);
  unsigned long runMicros = micros()%1000000;
  char buf[512];
  char *pbuf = buf;
  uint16_t text_length = 0;
  sprintf(pbuf,"(%010lu.%06lu) can%01i",runSeconds,runMicros,msg.bus);
  pbuf += 24, text_length += 24;
  sprintf(pbuf,  ((!msg.flags.extended) ? "      %03lX" : " %08lX"), msg.id);
  pbuf+=9, text_length +=9;
  if (msg.rtr == 0){
    for ( uint8_t i = 0; i < msg.len; i++ ) {
      sprintf(pbuf," %02X",msg.buf[i]);
      pbuf += 3, text_length +=3;
    }
  }
  else{
    sprintf(pbuf," RTR");
    pbuf += 4, text_length +=4;
  }
  sprintf(pbuf,"\r\n");
  pbuf += 2, text_length +=2;
  Serial.write(buf,text_length);
}

void canSniff1(const CAN_message_t &msg) { // global callback
//  CAN_message_t current_msg = msg; //prerequesite for next line.
//  current_msg.bus==1  ? current_msg.seq=1, Can0.write(current_msg) : current_msg.seq=1, Can1.write(current_msg); //ternary function to bridge canbus comms
  unsigned long runSeconds = ((millis()/1000)%10000000000);
  unsigned long runMicros = micros()%1000000;
  char buf[512];
  char *pbuf = buf;
  uint16_t text_length = 0;
  sprintf(pbuf,"(%010lu.%06lu) can%01i",runSeconds,runMicros,msg.bus);
  pbuf += 24, text_length += 24;
  sprintf(pbuf,  ((!msg.flags.extended) ? "      %03lX" : " %08lX"), msg.id);
  pbuf+=9, text_length +=9;
  if (msg.rtr == 0){
    for ( uint8_t i = 0; i < msg.len; i++ ) {
      sprintf(pbuf," %02X",msg.buf[i]);
      pbuf += 3, text_length +=3;
    }
  }
  else{
    sprintf(pbuf," RTR");
    pbuf += 4, text_length +=4;
  }
  sprintf(pbuf,"\r\n");
  pbuf += 2, text_length +=2;
  Serial.write(buf,text_length);
}
