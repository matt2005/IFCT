void canSniff0(const CAN_message_t &msg) { // global callback

  CAN_message_t current_msg = msg;
  current_msg.bus==1  ? current_msg.seq=1, Can0.write(current_msg) : current_msg.seq=1, Can1.write(current_msg); 

  unsigned long runMillis= millis();
  unsigned long allSeconds=millis()/1000;
  uint16_t runHours= allSeconds/3600;
  uint8_t secsRemaining=allSeconds%3600;
  uint8_t runMinutes=secsRemaining/60;
  uint8_t remainder_sec=secsRemaining%60;
  uint16_t remainder_ms=runMillis%1000;
  char buf[512];
  char *pbuf = buf;
  uint16_t text_length = 0;
  sprintf(pbuf,"%02d:%02d:%02d.%03d CAN%01d",runHours,runMinutes,remainder_sec,remainder_ms,msg.bus);
  pbuf += 17, text_length +=17;
  
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

  CAN_message_t current_msg = msg;
  current_msg.bus==1  ? current_msg.seq=1, Can0.write(current_msg) : current_msg.seq=1, Can1.write(current_msg); 

  unsigned long runMillis= millis();
  unsigned long allSeconds=millis()/1000;
  uint16_t runHours= allSeconds/3600;
  uint8_t secsRemaining=allSeconds%3600;
  uint8_t runMinutes=secsRemaining/60;
  uint8_t remainder_sec=secsRemaining%60;
  uint16_t remainder_ms=runMillis%1000;
  char buf[512];
  char *pbuf = buf;
  uint16_t text_length = 0;
  sprintf(pbuf,"%02d:%02d:%02d.%03d CAN%01d",runHours,runMinutes,remainder_sec,remainder_ms,msg.bus);
  pbuf += 17, text_length +=17;
  
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
