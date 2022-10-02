#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DFRobot_BT401.h>
#include <SoftwareSerial.h>
#include <TEA5767N.h>


LiquidCrystal_I2C lcd(0x27,20,4);
DFRobot_BT401 bt;
SoftwareSerial btSerial(2, 3);  //RX TX
TEA5767N radio = TEA5767N();




byte BT_ICON[8] =
{
0b00110,
0b10101,
0b10101,
0b01110,
0b01110,
0b10101,
0b10101,
0b00110
};


byte RADIO_ICON[8] = {
  0b01000,
  0b00100,
  0b00010,
  0b00001,
  0b11111,
  0b11001,
  0b11001,
  0b11111
};




int analogPin = 0;
int val = 0; 
int frequencyInt = 0;
float frequency = 0;
float previousFrequency = 0;
int signalStrength = 0;
char MODE = 'R';

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("BOOTING UP");
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Choose a Mode:");
  lcd.setCursor(1,1);
  lcd.print("BT | Radio");
  radio.setMonoReception();
  radio.setStereoNoiseCancellingOn();
}


void ShowSettingsMenu() {
  
}



void StartRadio(){


  
}




void StartBTaudio(){
  btSerial.begin(9600);
  delay(1200);
  
  while(!bt.begin(btSerial)){
   lcd.clear();
   Serial.println("Init failed, please check wire connection!");
   lcd.setCursor(0,1);
   lcd.print("ERROR: CHECK");
   lcd.setCursor(1,1);
   lcd.print("BT WIRE CONNECTION");
  }


  //while(bt.begin(btSerial)){
    //if 
    
    
    //}

  
  
  
   
   
}




void printFrequency(float frequency)
{
  String frequencyString = String(frequency,1);
  if(frequencyString.length() == 4)
  {
    lcd.print(frequencyString);
  }
  else
  {
    lcd.print(frequencyString,0,12);
  }
}


void printSignalStrength(){
  signalStrength = radio.getSignalLevel();
  String signalStrenthString = String(signalStrength);
  lcd.setCursor(1,1)
  
}
void loop() {
  if (MODE == 'R') {
    val = val/30;
    frequencyInt = map(val, 2, 1014, 8700, 10700); //Analog value to frequency from 87.0 MHz to 107.00 MHz 
    float frequency = frequencyInt/100.0f;

    if(frequency - previousFrequency >= 0.1f || previousFrequency - frequency >= 0.1f)
  {
    lcd.clear();
    radio.selectFrequency(frequency);
    printSignalStrength();
    printFrequency(frequency);
    previousFrequency = frequency;    
  }
    
    
    }

}
