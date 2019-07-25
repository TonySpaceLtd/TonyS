#define  Power   2   //  Pin 2 for control ON-OFF power
String Recv;
const int RX1 = 34;  // RX & TX Channels 1
const int TX1 = 32;  // RX & TX Channels 1
const int RX2 = 35;  // RX & TX Channels 2
const int TX2 = 33;  // RX & TX Channels 2

void setup() 
{
  Serial2.begin(115200, SERIAL_8N1, RX1, TX1);  //----- Set up Serial Port
  pinMode(Power, OUTPUT); // ------- Set to OUTPUT
  digitalWrite(Power, HIGH);  //---- HIGH for ON Board's power
}

void loop() 
{
  //---------- Test Read & Write RS232 ----------//
  Serial2.print("Random : ");
  Serial2.println(random(0, 255));

  while (Serial2.available()) {
    Recv = Recv+char(Serial2.read());
  }
  Serial.print("Get data : ");
  Serial.println(Recv);
  Recv = "";
  delay(100);
  //---------------------------------------------//
}