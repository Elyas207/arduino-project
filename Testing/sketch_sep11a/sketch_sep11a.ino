#include <DFRobot_BT401.h>
#include <SoftwareSerial.h>

DFRobot_BT401 bt;
SoftwareSerial btSerial(3, 2);

void setup(){
  Serial.begin(115200);
  btSerial.begin(115200);
  delay(2000);
  
  while(!bt.begin(btSerial)){
    Serial.println("Init failed, please check wire connection!");
    delay(1000);
  }
}

void loop() {
  Serial.println(Serial.read());
  Serial.println(bt.getBtStatus());
  if (bt.getBtStatus() == bt.eStandby)
  {
    Serial.println("eError");
    
  }

}
