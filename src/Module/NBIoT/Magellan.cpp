/*
Magellan v2.3.9 NB-IoT Playgroud Platform .
support Quectel BC95 
NB-IoT with AT command

supported board and hardware competible
  Arduino UNO please use software serial on pin8=RX pin9=TX and pin4=RST
  Arduino MEGA please use software serial on pin48=RX pin46=TX and pin4=RST 
  NUCLEO_L476RG please use Hardware serial on pin2=RX pin8=TX and pin4=RST

Develop with coap protocol 
reference with 
https://tools.ietf.org/html/rfc7252

support post and get method
only Magellan IoT Platform 
https://www.aismagellan.io

*** the payload limit up on dynamic memory of your board   
*** if you use arduino uno it limit 100
    character for payload string 
 
    example
      payload = {"temperature":25.5}
      the payload is contain 20 character

Author:   Phisanupong Meepragob
Create Date:     1 May 2017. 
Modified: 21 June 2018.

(*bug fix can not get response form buffer but can send data to the server)
(*v1.58.1 bug fix get method retransmit message)
(*v2.1.0 bug fix multiple definition variable with AIS_NB_BC95)
(*v2.1.1 bug fix get method retransmit)
(*v2.1.2 bug fix REBOOT CAUSE by hardware (BC95 hardware reset))
(*v2.2.2 bug fix check hardware connection with NB-Shield module)
(*v2.3.0 add feature get data from dashboard)
(*v2.3.1 bug fix disable print state when server send repeat response)
(*v2.3.2 bug fix init test AT command and include hardware check information)
(*v2.3.3 add IMSI information)
(*v2.3.4 bug fix display matching with msg id )
(*v2.3.5 bug fix repeat scan if hardware connection error! )
(*v2.3.6 bug fix lost communication with NB-IoT Module! )
(*v2.3.7 bug fix time retransmit and count timeout 3 time to reset)
(*v2.3.8 add feature get rssi)
(*v2.3.9 add example code and get rssi+imsi before connect)
Released for private use.
*/

#include "Magellan.h"
Magellan nbiot;



const char serverIP[] = "103.20.205.85";

Magellan::Magellan()
{
}
/*------------------------------
  Connect to NB-IoT Network
              &
  Initial Magellan
  ------------------------------
*/
bool Magellan::begin(uint8_t slot,char *auth)
{

  bool created=false;
  
  uint8_t nbPowerPin = getPinIO[slot];
  Tony.pinMode(nbPowerPin, OUTPUT);
  delay(10);
  Tony.digitalWrite(nbPowerPin, HIGH);
  
  ser_nbiot = Tony.SerialBegin(slot,9600);
  _Serial = &ser_nbiot;
  Serial.println();
  Serial.println(F("               AIS BC95 NB-IoT Magellan V2.3.9"));

  /*---------------------------------------
      Initial BC95 Module 
    ---------------------------------------
  */
  previous_check=millis();

  if(LastError!=""){
    Serial.print(F("LastError"));
    Serial.println(LastError);    
  }
  check_module_ready();

  if (!hw_connected)
  {
    en_post=false;
    en_get=false;
    return false;
  }
  reboot_module();
  setup_module();
 
  auth_len=0;
  pathauth=auth;
  cnt_auth_len(pathauth);

  if(auth_len>13){
    path_hex_len=2;
  }
  else{
    path_hex_len=1;
  }
  
  previous=millis();
  created=true;
  previous_time_get=millis();


  return created;
}

void Magellan::reboot_module()
{

  Serial.println(F(">>Rebooting "));
  _Serial->println(F("AT+NRB"));
  vTaskDelay(100);

  while(1){
    if(_Serial->available()){
      data_input = _Serial->readStringUntil('\n');
      if(data_input.indexOf(F("OK"))!=-1){
        Serial.println(data_input);
        break;
      }
      else{
        if (data_input.indexOf(F("REBOOT_"))!=-1)
        {
          Serial.println(data_input);
        }
        else{
          Serial.print(F("."));
        }
      }
    }
	vTaskDelay(1);
  }
  vTaskDelay(10000);

}

void Magellan::setup_module()
{
  data_input="";
  Serial.print(F(">>Setting"));
  _Serial->println(F("AT+CMEE=1"));
  delay(500);
  Serial.print(F("."));

  /*--------------------------------------
      Config module parameter
    --------------------------------------
  */
  _Serial->println(F("AT+CFUN=1"));
  vTaskDelay(6000);
  Serial.print(F("."));
  _Serial->println(F("AT+NCONFIG=AUTOCONNECT,true"));
  vTaskDelay(500);
  Serial.println(F("OK"));

  vTaskDelay(4000);
  Serial.print(F(">>RSSI "));
  Serial.print(rssi());
  Serial.println(F(" dBm"));

  _serial_flush(); 
  data_input="";
  Serial.print(F(">>IMSI "));
  _Serial->println(F("AT+CIMI"));
  while(1){
    
    if(_Serial->available()){
      data_input = _Serial->readStringUntil('\n');
      if(data_input.indexOf(F("OK"))!=-1){
        break;
      }
      else{
        Serial.print(data_input);
      }
    }
	vTaskDelay(1);
  }
  Serial.println(); 

  _Serial->println(F("AT+CGATT=1"));
  delay(500);
  Serial.print(F(">>Connecting"));
  _serial_flush();
  /*--------------------------------------
      Check network connection
    --------------------------------------
  */

  while(1){
    if(_Serial->available()){
      data_input = _Serial->readStringUntil('\n');
      if(debug) Serial.println(data_input);
      if(data_input.indexOf(F("+CGATT:1"))!=-1){
        break;
      }
	  vTaskDelay(1);
    }
    _Serial->println(F("AT+CGATT?"));
    Serial.print(F("."));
    vTaskDelay(2000);
  }
  data_input="";
  /*-------------------------------------
      Create network socket
    -------------------------------------
  */
  Serial.println(F("OK"));
  _Serial->println(F("AT+NSOCR=DGRAM,17,5684,1"));
  vTaskDelay(500); 
  _serial_flush(); 
}

void Magellan::check_module_ready()
{
  _Serial->println(F("AT"));
  delay(100);
  _Serial->println(F("AT"));
  while(1){
    if(_Serial->available()){
      data_input = _Serial->readStringUntil('\n');
      if(data_input.indexOf(F("OK"))!=-1){
        hw_connected=true;
        break;
      }
    }
    else{
      unsigned int current_check=millis();
      if (current_check-previous_check>5000)
      {
        Serial.println(F("Error to connect NB Module try reset or check your hardware"));
        previous_check=current_check;
        hw_connected=false;
        //break;
      }
      else{
        _Serial->println(F("AT"));
        delay(100);
      }
    }
  }

  _serial_flush();
}

void Magellan::_serial_flush()
{
  while(1){
    if(_Serial->available()){
      data_input=_Serial->readStringUntil('\n');
    }
    else{
      break;
    }
  }
}
/*------------------------------
    CoAP Message menagement 
  ------------------------------
*/
void Magellan::printHEX(char *str)
{
  char *hstr;
  hstr=str;
  char out[3]="";
  int i=0;
  bool flag=false;
  while(*hstr)
  {
    flag=itoa((int)*hstr,out,16);
    
    if(flag)
    {
      _Serial->print(out);

      if(debug)
      {
        Serial.print(out);
      }
      
    }
    hstr++;
  }
}
void Magellan::printmsgID(unsigned int messageID)
{
  char Msg_ID[3];
  
  utoa(highByte(messageID),Msg_ID,16);
  if(highByte(messageID)<16)
  {
    if(debug) Serial.print(F("0"));
    _Serial->print(F("0"));
    if(debug) Serial.print(Msg_ID);
    _Serial->print(Msg_ID);
  }
  else
  {
    _Serial->print(Msg_ID);
    if(debug)  Serial.print(Msg_ID);
  }

  utoa(lowByte(messageID),Msg_ID,16);
  if(lowByte(messageID)<16)
  {
    if(debug)  Serial.print(F("0"));
    _Serial->print(F("0"));
    if(debug)  Serial.print(Msg_ID);
    _Serial->print(Msg_ID);
  }
  else
  {
    _Serial->print(Msg_ID);
    if(debug)  Serial.print(Msg_ID);
  }
} 
void Magellan::print_pathlen(unsigned int path_len,char *init_str)
{   
    unsigned int extend_len=0;

    if(path_len>13){
      extend_len=path_len-13;

      char extend_L[3];
      itoa(lowByte(extend_len),extend_L,16);
      _Serial->print(init_str);
      _Serial->print(F("d"));

      if(debug) Serial.print(init_str);
      if(debug) Serial.print(F("d"));

      if(extend_len<=15){
        _Serial->print(F("0"));
        _Serial->print(extend_L);

        if(debug) Serial.print(F("0"));
        if(debug) Serial.print(extend_L);
      }
      else{
        _Serial->print(extend_L);
        if(debug) Serial.print(extend_L);
      }
      

    }
    else{
      if(path_len<=9)
      {
        char hexpath_len[2]="";   
        sprintf(hexpath_len,"%i",path_len);
        _Serial->print(init_str);
        _Serial->print(hexpath_len);
        if(debug) Serial.print(init_str);
        if(debug) Serial.print(hexpath_len);

      }
      else
      {
        if(path_len==10) 
        {
          _Serial->print(init_str);
          _Serial->print(F("a"));
          if(debug) Serial.print(init_str);
          if(debug) Serial.print(F("a"));
        }
        if(path_len==11)
        {
          _Serial->print(init_str);
          _Serial->print(F("b"));
          if(debug) Serial.print(init_str);
          if(debug) Serial.print(F("b"));
        } 
        if(path_len==12)
        {
          _Serial->print(init_str);
          _Serial->print(F("c"));
          if(debug) Serial.print(init_str);
          if(debug) Serial.print(F("c"));
        } 
        if(path_len==13)
        {
          _Serial->print(init_str);
          _Serial->print(F("d"));
          if(debug) Serial.print(init_str);
          if(debug) Serial.print(F("d"));
        } 
        
      }
   }  
}
void Magellan::cnt_auth_len(char *auth)
{
  char *hstr;
  hstr=auth;
  char out[3]="";
  int i=0;
  bool flag=false;
  while(*hstr)
  {
    auth_len++;
    
    
    hstr++;
  }
}

unsigned int Magellan::option_len(unsigned int path_len)
{
  unsigned int len=0;
    if (path_len>0)
      {
        if (path_len>13)
        {
          len=2;
        }
        else
        {
          len=1;
        }
      }
  return len+path_len;
}

void Magellan::print_uripath(String uripath)
{
    unsigned int uripathlen=uripath.length();
    if (uripathlen>0)
      {
          print_pathlen(uripathlen,"0");
          
          char data[uripath.length()+1]="";
          uripath.toCharArray(data,uripath.length()+1);

          printHEX(data);
      }
}
/*----------------------------- 
    CoAP Method POST
  -----------------------------
*/
String Magellan::post(String payload)
{
  #if defined(ARDUINO_AVR_PRO) || (ARDUINO_AVR_UNO)
    if(payload.length()>100){
      Serial.println("Warning payload size exceed the limit of memory");
    }
    else{
      return post_data(payload,"","","");
    }
  #else
    return post_data(payload,"","","");
  #endif
  
}
String Magellan::linenotify(String token,String msg)
{
  return post_data(msg,"Linenotify",token,"");
}
String Magellan::post_data(String payload,String option1,String option2,String option3)
{
  //unsigned int timeout[5]={12000,14000,18000,26000,42000};
  unsigned int timeout[5]={2000,2000,2000,18000,18000};
  unsigned int prev_send=millis();

  if(!get_process && en_post){
    data_buffer="";
    previous_send=millis();
    send_ACK=false;
    ACK=false;
    success=false;
    token=random(0,32767);
    post_token=token;
    if(debug) Serial.println(F("Load new payload"));
    Msg_ID++;
    for (byte i = 0; i <= 4; ++i)
    {
      post_process=true;
      en_chk_buff=false;
      post_ID=Msg_ID;
      Msgsend(payload,option1,option2,option3);

      vTaskDelay(1);
      while(true)
      {
        en_chk_buff=true;
        waitResponse();
        unsigned int currenttime=millis();
        if (currenttime-previous_send>timeout[i] || success)
        {
          if(debug) Serial.println(currenttime-previous_send);
          previous_send=currenttime;
          en_post=true;
          en_get=true;
          post_process=false;
          break;
        }
		vTaskDelay(1);
      }
      if (success)
      {
        break;
      }
      else{
            if(i+1<5){
              if(printstate) Serial.print(F(">> Retransmit"));
              if(printstate) Serial.println(i+1);
              if(printstate) Serial.println(timeout[i+1]-10000);
            }

      }

     }
     if (!success)
     {
       	Serial.print(F("timeout : "));
        data_input="";
        count_timeout++;
        Serial.println(count_timeout);
   		if(printstate) Serial.println();

	       if(count_timeout>=3)
	       {
	       	count_timeout = 0;
		        do{
		            check_module_ready();
		        }
		        while(!hw_connected);
		        reboot_module();
		        setup_module();
	       }
        
     }
    }

    

  post_process=false;
  _Serial->flush();
  return data_buffer;
}

/*----------------------------- 
    CoAP Method GET
  -----------------------------
*/
String Magellan::get(String Resource)
{
  return get_data(Resource,"");
}
String Magellan::get_dashboard(String Resource)
{
  return get_data("dashboard."+Resource,"");
}
String Magellan::get_api(String Resource,String Proxy)
{
  #if defined(ARDUINO_AVR_PRO) || (ARDUINO_AVR_UNO)
    return "Warning this module has not enough memory";
  #else
    return get_data(Resource,Proxy);
  #endif

  
}
String Magellan::get_data(String Resource,String Proxy)
{

  int timeout[5]={8000,16000,32000,64000,128000};
  rcvdata="";
  data_buffer="";
  if(!post_process && en_get){
    do{
      token=random(0,32767);
      get_token=token;
      success=false;
      for (int i = 0; i <=4; ++i)
        {
          get_process=true;
          data_buffer="";
          Msgget(Resource,Proxy);          
          //}
          en_chk_buff=false;
          while(true)
          {
            en_chk_buff=true;
            unsigned int current_time=millis();
            

            if(current_time-previous_time_get>timeout[i] || success || (ACK && more_flag) || NOTFOUND){
              previous_time_get=current_time;
              if(i==4){
                if(printstate) Serial.println(F(">> Get timeout"));
                get_process=false;
              }
              if(debug) Serial.println(F("break"));
              break;
            }
            waitResponse();
            
          }


          if((rcvdata.length()>0 && GETCONTENT && !more_flag) || success || NOTFOUND){
            get_process=false;
            break;
          }
          else{
            if (!more_flag)
            {
              if(printstate) Serial.print(F(">> Retransmit"));
              if(printstate) Serial.println(i+1);
              if(printstate) Serial.println(timeout[i]);
            }

          }
        }
      }while(more_flag);
  }
  return rcvdata;
}

/*-----------------------------
    Massage Package
  -----------------------------
*/
void Magellan::Msgsend(String payload,String option1,String option2,String option3)
{

  send_buffer=payload;
  char data[send_buffer.length()+1]="";
  send_buffer.toCharArray(data,send_buffer.length()+1);
  
  if(en_post){

      if(printstate) Serial.print(F(">> post: Msg_ID "));
      if(printstate) Serial.print(Msg_ID);
      if(printstate) Serial.print(F(" "));
      if(printstate) Serial.print(send_buffer);

      _Serial->print(F("AT+NSOST=1,"));
      _Serial->print(serverIP);
      _Serial->print(F(",5683,"));

      unsigned int buff_len=auth_len+11+2+path_hex_len+send_buffer.length();
      unsigned int option1_len=option1.length();
      unsigned int option2_len=option2.length();
      unsigned int option3_len=option3.length();

      buff_len+=option_len(option1_len);
      buff_len+=option_len(option2_len);
      buff_len+=option_len(option3_len);

      _Serial->print(buff_len);
      if(debug) Serial.print(F(",4202"));

      _Serial->print(F(",4202"));
      printmsgID(Msg_ID);
      printmsgID(post_token);                     //print token
      _Serial->print(F("b54e42496f54"));
      if(debug) Serial.print(F("b54e42496f54"));
      print_pathlen(auth_len,"0");

      printHEX(pathauth);

      print_uripath(option1);
      print_uripath(option2);
      print_uripath(option3);

      //_Serial->print(F("1132"));                 //content-type json
      _Serial->print(F("ff")); 
      if(debug) Serial.print(F("ff"));
      printHEX(data);  
      _Serial->println();

      if(printstate) Serial.println();
      
  }   
}

void Magellan::Msgget(String Resource,String Proxy)
{
  
  char data[Resource.length()+1]="";
  Resource.toCharArray(data,Resource.length()+1); 
  GETCONTENT=false;
  ACK=false;
  Msg_ID++;
  get_ID=Msg_ID;
  if(printstate) Serial.print(F(">> GET data : Msg_ID "));
  if(printstate) Serial.print(Msg_ID);
  if(printstate) Serial.print(" ");
  if(printstate) Serial.print(Resource);
  
  _Serial->print(F("AT+NSOST=1,"));
  _Serial->print(serverIP);
  _Serial->print(F(",5683,"));

  if(debug) Serial.print(F("AT+NSOST=1,")); 
  if(debug) Serial.print(serverIP);
  if(debug) Serial.print(F(",5683,")); 

  unsigned int path_len=Resource.length();
  unsigned int buff_len=0;
  if(path_len>13){
    buff_len=52+path_len;
  }
  else{
    buff_len=51+path_len;
  }

  if(Resource.indexOf(F("API"))!=-1){
    buff_len=buff_len+2+Proxy.length();
    if(debug) Serial.print(buff_len);
  }

  _Serial->print(buff_len+2);
  if(debug) Serial.print(F(",4201"));
  _Serial->print(F(",4201"));
  printmsgID(Msg_ID);
  printmsgID(get_token);                     //print token
  _Serial->print(F("b54e42496f54"));
  if(debug) Serial.print(F("b54e42496f54"));
  _Serial->print(F("0d17"));
  if(debug) Serial.print(F("0d17"));
  printHEX(pathauth);

  print_pathlen(path_len,"4");

  printHEX(data);

  _Serial->print(F("8104"));      //Block size 256 recommended for BC95 with CoAP Header
  if(debug) Serial.print(F("8104"));

  //_Serial->print(F("8105"));      //Block size 512 recommended for BC95
  //if(debug) Serial.print(F("8105"));
  //_Serial->print(F("8106"));    //Block size 1024
  //if(debug) Serial.print(F("8106"));
  
  /* ##########################
      Proxy Uri additional
     ########################## 
  */ 
  if(Resource.indexOf(F("API"))!=-1){

      char data[Proxy.length()+1]="";
      Proxy.toCharArray(data,Proxy.length()+1); 

      //if(debug) Serial.print(F("420184"));    //Block size 256 recommended for BC95 with CoAP Header
      //_Serial->print(F("420184")); 
      //if(debug) Serial.print(F("420185"));  //Block size 512 recommended for BC95
      //_Serial->print(F("420185"));   
      //print_pathlen(Proxy.length(),"8");
      print_pathlen(Proxy.length(),"c");
      printHEX(data);
  }



  _Serial->println();

  _Serial->flush();

  if(printstate) Serial.println();
  sendget=true;
}

/*-------------------------------------
    Response Message management

    Example Message
    Type: ACK
    MID: 000001
    Code: Created
    Payload:200
  -------------------------------------
*/
void Magellan::print_rsp_header(String Msgstr)
{

  if(debug) Serial.println(Msgstr);

  resp_msgID = (unsigned int) strtol( &Msgstr.substring(4,8)[0], NULL, 16);
  print_rsp_Type(Msgstr.substring(0,2),resp_msgID);

  bool en_print=(post_process && resp_msgID==post_ID) || (get_process && resp_msgID==get_ID);

  switch((int) strtol(&Msgstr.substring(2,4)[0], NULL, 16))
   {
      case EMPTY:
                    EMP=true;
                    Msgstr.remove(0, 8);
                    break;
      case CREATED: 
                    EMP=false;
                    NOTFOUND=false;
                    GETCONTENT=false;
                    RCVRSP=true;

                    if (Msgstr.length()/2>4)
                    {
                      rsptoken=(unsigned int) strtol( &Msgstr.substring(8,12)[0], NULL, 16);
                      if (post_process && post_token==rsptoken)
                      {
                        if(debug) Serial.println(F("match token"));
                        if(debug) Serial.print(rsptoken);
                        success=true;
                        
                      }

                      Msgstr.remove(0, 12);
                    }
                    else{
                      Msgstr.remove(0, 8);
                    }

                    if(printstate && en_print) Serial.println(F("2.01 CREATED")); 
                    break;
      case DELETED: //if(printstate && en_print) Serial.println(F("2.02 DELETED")); 
                    break;
      case VALID: //if(printstate && en_print) Serial.println(F("2.03 VALID"));
                  break;
      case CHANGED: //if(printstate && en_print) Serial.println(F("2.04 CHANGED"));
                  break;
      case CONTENT: 
                    EMP=false;
                    NOTFOUND=false;
                    GETCONTENT=true;
                    RCVRSP=false;
                    if(get_process){
                      String blocksize=Msgstr.substring(20,22);
                      if(blocksize.indexOf(F("0C"))!=-1)
                        {
                          more_flag=true;
                        } 
                      if(blocksize.indexOf(F("04"))!=-1){ 
                          more_flag=false;
                        }

                      if (Msgstr.length()/2>4)
                      {
                        rsptoken=(unsigned int) strtol( &Msgstr.substring(8,12)[0], NULL, 16);
                        if (get_process && get_token==rsptoken)
                        {
                          if(debug) Serial.println(F("match token"));
                          if(debug) Serial.print(rsptoken);
                          if(!more_flag) success=true;
                          
                        }
                      }
                    }
                    Msgstr.remove(0, 8);
                    if(printstate && en_print) Serial.println(F("2.05 CONTENT")); 
                    break;
      case CONTINUE: //if(printstate && en_print) Serial.println(F("2.31 CONTINUE"));
                    Msgstr.remove(0, 8); 
                    break;
      case BAD_REQUEST: if(printstate && en_print) Serial.println(F("4.00 BAD_REQUEST"));
                    Msgstr.remove(0, 8); 
                    break;
      case FORBIDDEN: if(printstate && en_print) Serial.println(F("4.03 FORBIDDEN"));
                    Msgstr.remove(0, 8); 
                    break;
      case NOT_FOUND: 
                    if(printstate && en_print) Serial.println(F("4.04 NOT_FOUND"));
                    GETCONTENT=false; 
                    NOTFOUND=true;
                    RCVRSP=false;
                    break;
      case METHOD_NOT_ALLOWED: 
                    RCVRSP=false;
                    if(printstate && en_print) Serial.println(F("4.05 METHOD_NOT_ALLOWED")); 
                    break;
      case NOT_ACCEPTABLE: if(printstate && en_print) Serial.println(F("4.06 NOT_ACCEPTABLE")); 
                    break;
      case REQUEST_ENTITY_INCOMPLETE: //if(printstate && en_print) Serial.println(F("4.08 REQUEST_ENTITY_INCOMPLETE")); 
                    break;
      case PRECONDITION_FAILED: //if(printstate && en_print) Serial.println(F("4.12 PRECONDITION_FAILED")); 
                    break;
      case REQUEST_ENTITY_TOO_LARGE: //if(printstate && en_print) Serial.println(F("4.13 REQUEST_ENTITY_TOO_LARGE")); 
                    break;
      case UNSUPPORTED_CONTENT_FORMAT: if(printstate && en_print) Serial.println(F("4.15 UNSUPPORTED_CONTENT_FORMAT")); 
                    break;
      case INTERNAL_SERVER_ERROR: if(printstate && en_print) Serial.println(F("5.00 INTERNAL_SERVER_ERROR")); 
                    break;
      case NOT_IMPLEMENTED: //if(printstate && en_print) Serial.println(F("5.01 NOT_IMPLEMENTED")); 
                    break;
      case BAD_GATEWAY: if(printstate && en_print) Serial.println(F("5.02 BAD_GATEWAY")); 
                    break;
      case SERVICE_UNAVAILABLE: if(printstate && en_print) Serial.println(F("5.03 SERVICE_UNAVAILABLE")); 
                    break;
      case GATEWAY_TIMEOUT: if(printstate && en_print) Serial.println(F("5.04 GATEWAY_TIMEOUT")); 
                    break;
      case PROXY_NOT_SUPPORTED: if(printstate && en_print) Serial.println(F("5.05 PROXY_NOT_SUPPORTED")); 
                    break;

      default : //Optional
                GETCONTENT=false;
   }

   if(printstate && en_print) Serial.print(F("   Msg_ID "));

   
   if(printstate && en_print) Serial.println(resp_msgID);
}

void Magellan::print_rsp_Type(String Msgstr,unsigned int msgID)
{
  bool en_print=(post_process && resp_msgID==post_ID) || (get_process && resp_msgID==get_ID);


  if(Msgstr.indexOf(ack)!=-1 || Msgstr.indexOf(acktk)!=-1)
  {
    
    if(printstate && en_print) Serial.print(F("<< ACK: "));
    if((resp_msgID==get_ID || resp_msgID==post_ID) && !EMP){
		        ACK=true;
	  }
    
    flag_rcv=true;
    en_post=true;
    en_get=true;
    
    send_ACK=false;
    cnt_cmdgetrsp=0;
  }
  if(Msgstr.indexOf(con)!=-1 || Msgstr.indexOf(contk)!=-1)
  {
    if(printstate && en_print) Serial.print(F("<< CON: "));
    //resp_cnt++;

    en_post=false;
    en_get=true;

    if(debug) Serial.println(F("Send ack"));
    _Serial->print(F("AT+NSOST=1,"));
    _Serial->print(serverIP);
    _Serial->print(F(",5683,"));
    _Serial->print(F("4"));
    _Serial->print(F(",6000"));
    printmsgID(msgID);
    _Serial->println();

    send_ACK=true;   

    ACK=false;
    cnt_cmdgetrsp=0;
  }
  if(Msgstr.indexOf(rst)!=-1)
  {
    if(printstate && en_print) Serial.print(F("<< RST: "));
    flag_rcv=true;
    ACK=false;
    cnt_cmdgetrsp=0;
  }
  if(Msgstr.indexOf(non_con)!=-1)
  {
    if(printstate && en_print) Serial.print(F("<< Non-Con: "));
    flag_rcv=true;
    ACK=false;
    cnt_cmdgetrsp=0;
  }
}

/*-----------------------------------
  Get response data from BC95 Buffer 
  -----------------------------------  
*/
void Magellan::miniresponse(String rx)
{
  
  print_rsp_header(rx);
  

  bool en_print=(post_process && resp_msgID==post_ID) || (get_process && resp_msgID==get_ID);

  String payload_rx=rx.substring(12,rx.length());
  String data_payload="";
  unsigned int indexff=0;

  
  indexff=payload_rx.indexOf(F("FF"));
  

  if(payload_rx.indexOf(F("FFF"))!=-1)
  {
      data_payload=payload_rx.substring(indexff+3,payload_rx.length());

      if(printstate && en_print) Serial.print(F("   RSP:"));
      //if(!more_flag) data_buffer="";                                          //clr buffer
      data_buffer="";
      for(unsigned int k=2;k<data_payload.length()+1;k+=2)
      {
        char str=(char) strtol(&data_payload.substring(k-2,k)[0], NULL, 16);
        if(printstate && en_print) Serial.print(str);

        if(GETCONTENT or RCVRSP){
            if (post_process && post_token==rsptoken || get_process && get_token==rsptoken){
              data_buffer+=str;
            }
          
        }
      }
      if(GETCONTENT)
      {
        rcvdata+=data_buffer;
        //if(!more_flag) data_buffer="";
        data_buffer="";
        getpayload=true;
      } 
      if(printstate && en_print) Serial.println(F(""));
  }
  else
  {
      data_payload=payload_rx.substring(indexff+2,payload_rx.length());
      if(printstate && en_print) Serial.print(F("   RSP:"));
      //if(!more_flag) data_buffer="";
      data_buffer="";                                       //clr buffer
      for(unsigned int k=2;k<data_payload.length()+1;k+=2)
      {
        char str=(char) strtol(&data_payload.substring(k-2,k)[0], NULL, 16);
        if(printstate && en_print) Serial.print(str);

        if(GETCONTENT or RCVRSP){
          if (post_process && post_token==rsptoken || get_process && get_token==rsptoken){
            data_buffer+=str;
          }
        }
      }
      if(GETCONTENT) {
        rcvdata+=data_buffer;
        //if(!more_flag) data_buffer="";
        data_buffer="";
        getpayload=true;
      } 
      if(printstate && en_print) Serial.println(F(""));     
  }

  if(success)
  { 
    if(printstate && en_print) Serial.println(F("------------ End ------------"));
    count_timeout = 0;
    /*
    while(1){
      if(_Serial->available()){
        data_input=_Serial->readStringUntil('\n');
      }
      else{
        break;
      }
    }
    */
  }
}

void Magellan:: waitResponse()
{ 
  unsigned long current=millis();
  if(en_chk_buff && (current-previous>=500) && !(_Serial->available()))
  {
      _Serial->println(F("AT+NSORF=1,512"));
      cnt_cmdgetrsp++;
      previous=current;
  }

  if(_Serial->available())
  {
    char data=(char)_Serial->read();
    if(data=='\n' || data=='\r')
    {
      if(k>2)
      {
        end=true;
        k=0;
      }
      k++;
    }
    else
    {
      data_input+=data;
    }
  }
  if(end){
      if(debug) Serial.println(data_input); 
      if(data_input.indexOf(F("+NSONMI:"))!=-1)
      {
          if(debug) Serial.print(F("send_NSOMI "));
          if(data_input.indexOf(F("+NSONMI:"))!=-1)
          {
			  delay(100);
            _Serial->println(F("AT+NSORF=1,512"));
            data_input=F("");
            send_NSOMI=true;
            if(printstate) Serial.println();
          }
          end=false;
      }
      else
        {


          end=false;
          
          if(data_input.indexOf(F("1,103.20.205.85"))!=-1)
          {
            int index1 = data_input.indexOf(F(","));
            if(index1!=-1) 
            {
              int index2 = data_input.indexOf(F(","),index1+1);
              index1 = data_input.indexOf(F(","),index2+1);
              index2 = data_input.indexOf(F(","),index1+1);
              index1 = data_input.indexOf(F(","),index2+1);

              String prev_buffer=data_input.substring(index2+1,index1);

              if(cnt_rcv_resp>=1 && prev_buffer.indexOf(F("FF"))!=-1 && more_flag){

                miniresponse(data_input.substring(index2+1,index1));
              }
              else{
                miniresponse(data_input.substring(index2+1,index1));
              }

              index2 = data_input.indexOf(F(","),index1+1);
              if(data_input.substring(index1+1,index2)!="0"){
                  if(debug) Serial.println(F("found buffer"));
                  _Serial->println(F("AT+NSORF=1,512"));
                  cnt_rcv_resp++;
              }
              else{
                cnt_rcv_resp=0;
              }
              
            }
          }
          else if((data_input.indexOf(F("ERROR"))!=-1))
          {
            if((data_input.indexOf(F("+CME ERROR")))!=-1)
            {
              int index1 = data_input.indexOf(F(":"));
              if(index1!=-1) {
                LastError=data_input.substring(index1+1,data_input.length());

                if((LastError.indexOf(F("159")))!=-1)
                {
                   Serial.print(F("Uplink busy"));
                   data_buffer="";				//Ver 1.54  clr buffer   
                }
              }
            }

          }
          else if ((data_input.indexOf(F("REBOOT")))!=-1)
          {
              breboot_flag=true;
          }

          else if(breboot_flag && data_input.indexOf(F("OK"))!=-1)
          {
                Serial.print(F(">>HW RST"));
                post_process=false;
                get_process=false;
                breboot_flag=false;
                setup_module();
          }

          send_NSOMI=false;
          data_input=F("");
          }       
        }
}
String Magellan:: rssi()
{
  delay(200);
  _serial_flush();
  int rssi = 0;
  String data_csq = "";
  data_input = "";
  _Serial->println(F("AT+CSQ"));
  delay(500);
  while(1)
  {    
    if(_Serial->available())
    {
      data_input = _Serial->readStringUntil('\n');
      //Serial.println(data_input);
      if(data_input.indexOf(F("OK"))!=-1)
      {
        break;
      }
      else
      {
      	if(data_input.indexOf(F("+CSQ"))!=-1)
      	{
	      	int start_index = data_input.indexOf(F(":"));
			    int stop_index  = data_input.indexOf(F(","));
			    data_csq = data_input.substring(start_index+1,stop_index);
			if (data_csq == "99")
			{
				data_csq = "N/A";
			}
			else
			{
				rssi = data_csq.toInt();
				rssi = (2*rssi)-113;
				data_csq = String(rssi);
			}
		}
      }
    }
  }
  return data_csq;
}



