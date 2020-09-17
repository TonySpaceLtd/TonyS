#include "TonyS_X1.h"
#include "TonyS_X1_ExternalModule.h"

#define HOST "YOUR_SERVER"
#define PORT "YOUR_PORT"
#define USER "YOUR_USERNAME"
#define PASS "YOUR_PASSWORD"
#define PATH "/"              // operate path

UC20_FTP client;
UC20_file f;


void setup() {
  Serial.begin(115200);
  Tony.begin();  //----  begin Library
  delay(10);
  gsm.initial(SLOT2);

  Serial.println("########## UC20 FTP ##########");
  Serial.print("Power on module...");
  while(!gsm.powerOn()) 
    Serial.print(".");
  Serial.println(" READY");
  Serial.print  ("Network registration...");
  Serial.println(gsm.getNetworkStatus()? " READY":" NOT READY");
  Serial.println("IMEI     : " + gsm.getIMEI());
  Serial.println("IMSI     : " + gsm.getIMSI());
  Serial.println("Operator : " + gsm.getOperator());
  Serial.print  ("RSSI     : ");
  Serial.println(gsm.getSignal());
  Serial.println();

  // create file
  Serial.println("+++++ local +++++++++++++++++++++++++++++++++++++++++++++++++++++++");
  Serial.println("# create new file (local)");
  f.open("RAM:testLocal.txt",FMODE_OVERWRITE);
  f.beginWrite();
  gsm.print("ftp example file");
  f.endWrite();
  f.close();
  Serial.println("# list local file");
  f.list("RAM:*");
  Serial.println();

  // ftp connect
  Serial.println("+++++ remote ++++++++++++++++++++++++++++++++++++++++++++++++++++++");
  client.initConfig();
  client.account(USER,PASS);
  client.fileType(BINARY);
  int error = client.login(HOST,PORT);
  if(error != 0) {
    Serial.print("# login fail ");
    Serial.println(error);
    Serial.println("\n...");
    return;
  }
  Serial.println("# login");
  client.setPath(PATH);
  Serial.println("# path");
  Serial.println(client.getPath());
  Serial.println("# list remote file");
  client.list();

  // ftp put
  Serial.println("# put");
  client.put("testRemote.txt","RAM:testLocal.txt");
  client.wait_putdone();
  Serial.println("# list remote file");
  client.list();

  // ftp get
  Serial.println("# get");
  client.get("testRemote.txt","RAM:testLocal2.txt");

  // ftp disconnect
//  client.del("testRemote.txt");     // uncomment to delete file on remote
  Serial.println("# logout");
  client.logout();
  Serial.println();

  // read get file (local)
  Serial.println("+++++ local +++++++++++++++++++++++++++++++++++++++++++++++++++++++");
  Serial.println("# list local file");
  f.list("RAM:*");
  Serial.println("# read file");
  f.open("RAM:testLocal2.txt",FMODE_READONLY);
  long len = f.filesize("RAM:testLocal2.txt");
  for(int i=0; i<len; i++) {
    Serial.write(f.read());
  }
  Serial.println();
  f.close();
//  f.del("RAM:testLocal.txt");       // uncomment to delete local file
//  f.del("RAM:testLocal2.txt");      // uncomment to delete local file
  Serial.println("\n...");
}

void loop() {
  if(Serial.available() > 0) {
    gsm.write(Serial.read());
  }
  if(gsm.available() > 0) {
    Serial.write(gsm.read());
  }
}
