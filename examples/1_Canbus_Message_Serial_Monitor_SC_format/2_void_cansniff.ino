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
