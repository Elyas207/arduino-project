#include <DFRobot_BT401.h>
#include <TEA5767N.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

TEA5767N radio = TEA5767N();
LiquidCrystal_I2C lcd(0x27, 20, 4);

char MODE = 'N'; // 'R' = FM RADIO | 'B' = BLUETOOTH | 'N' = NONE
int analogPin = 0;
int val = 0;
int frequencyInt = 0;
float frequency = 101.1;
float previousFrequency = 0;
int signalStrength = 0;
#define BTN_01_PIN = 0
#define BTN_2_PIN = 0
String SelectionStage = ""; //None = None, Startup = Audio source selection, Switch = Switching Source
int SelectionStageNum = 0;
String debug_command;
String SelectedMode = "None";
String BluetoothAudioPortOpened = "No";
String RadioAudioPortOpened = "No";
bool BluetoothAlreadyStarted = false;
SoftwareSerial btSerial(3, 2);  //RX TX
DFRobot_BT401 bt;


void setup() {
  Wire.begin();
  Serial.begin(115200);
  btSerial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Starting Up...");
  Serial.println("PRE-BOOT: OK");
  delay(2000);
  while (!bt.begin(btSerial)) {
    Serial.println("Init failed, please check wire connection!");
    delay(1000);
    Serial.print("Error with Bluetooth Module");
  }

  bt.switchFunction(bt.eBluetooth);
  BluetoothAlreadyStarted = true;
  MuteAll();
  radio.selectFrequency(101.1);
  radio.setStereoNoiseCancellingOn();
  SelectionStageNum = 0;
  SelectionStage = "Startup";
  Serial.print("Startup OK.");

}



void UpdateRadioInfo()
{

  if (MODE == 'R') {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RADIO:");
    lcd.setCursor(0, 1);
    String frequencyString = String(frequency);
    lcd.print(frequencyString);

  }



}


void UpdateBluetoothInfo()
{
  if (MODE == 'B') {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("BLUETOOTH AUDIO:");
    lcd.setCursor(0, 1);
    lcd.print("Connected");
    bt.switchFunction(bt.eBluetooth);
    Serial.println(bt.getBtStatus());
    Serial.println(bt.getBtStatus());
    Serial.println(bt.getBtStatus());
    Serial.println(bt.getBtStatus());
    
    if (bt.getBtStatus()== bt.eError) 
    {
      Serial.println("eError!");
    }

  }

}



void StartRadio() {
  radio.turnTheSoundBackOn();
  radio.selectFrequency(frequency);
  MODE = 'R';

  UpdateRadioInfo();



}


void StartBluetooth() {
  if (BluetoothAlreadyStarted == false) {
    // btSerial.begin(115200);
    /*Delay 2s for the BT401 to start*/
    delay(2000);
    while (!bt.begin(btSerial)) {
      Serial.println("Init failed, please check wire connection!");
      delay(1000);
    }


  }

  MODE = 'B';
  UpdateBluetoothInfo();

}





void MuteAll() {
  // Mute all Audio
  radio.setStereoNoiseCancellingOn();
  radio.mute();
  //bt.switchFunction(bt.eIdle);
  //bt.setVOl(0);

}

void loop()
{
  if (SelectionStage == "Startup")
  {
    lcd.setCursor(0, 0);
    lcd.print("Choose a mode:");
    if (SelectionStageNum == 1)
    {
      lcd.setCursor(0, 1); //counting starts at 0
      lcd.print("- Bluetooth Audio");
      lcd.setCursor(0, 2); //counting starts at 0
      lcd.print("Radio");
    }

    if (SelectionStageNum == 0)
    {
      lcd.setCursor(0, 1); //counting starts at 0
      lcd.print("Bluetooth Audio");
      lcd.setCursor(0, 2); //counting starts at 0
      lcd.print("- Radio");

    }

  }



  if (SelectionStage == "None")

  {
    if (SelectedMode == "Radio" || SelectedMode == "Bluetooth")
    {
      if (SelectedMode == "Radio")
      {
        if (RadioAudioPortOpened == "No")
        {
          StartRadio();

        }
      }

      if (SelectedMode == "Bluetooth")
      {
        if (BluetoothAudioPortOpened == "No")
        {
          StartBluetooth();

        }
      }


    }

  }

  if (Serial.available())
  {
    debug_command = Serial.readStringUntil('\n');
    if (debug_command.equals("up"))
    {
      if (SelectionStage == "Startup")
      {
        if (SelectionStageNum == 0)
        {
          SelectionStageNum = 1;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Choose a mode:");

        }
      }
    }

    if (debug_command.equals("down"))
    {
      if (SelectionStage == "Startup")
      {
        if (SelectionStageNum == 1)
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Choose a mode:");
          SelectionStageNum = 0;

        }
      }

    }


    if (debug_command.equals("select"))
    {
      if (SelectionStage == "Startup")
      {
        if (SelectionStageNum == 0)
        {

          SelectedMode = "Radio";
          SelectionStage = "None";
          BluetoothAudioPortOpened = "No";
          RadioAudioPortOpened = "Yes";
          MODE = 'R';
          StartRadio();

        }

        if (SelectionStageNum == 1)
        {
          SelectedMode = "Bluetooth";
          SelectionStage = "None";
          RadioAudioPortOpened = "No";
          BluetoothAudioPortOpened = "Yes";
          MODE = 'B';
          StartBluetooth();

        }


      }

    }



    if (debug_command.equals("switch_bluetooth"))
    {
      MuteAll();
      Serial.print("Switching Over to Bluetooth Audio");
      SwitchAudioSource("Bluetooth");



    }


    if (debug_command.equals("switch_radio"))
    {
      MuteAll();
      Serial.print("Switching Over to FM Radio");
      SwitchAudioSource("Radio");



    }





  }
}

void SwitchAudioSource(String toSwitch)
{
  if (toSwitch == "Bluetooth")
  {

    if (MODE == 'R')
    {
      MuteAll();
      Serial.print("Switch Function active... Posting to BT Audio Module");
      SelectionStage = "Switching";
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Switching to:");
      lcd.setCursor(0, 1);
      lcd.print("Bluetooth Audio");
      StartBluetooth();



    }

  }


  if (toSwitch == "Radio")
  {
    if (MODE == 'B')
    {
      MuteAll();
      Serial.print("Switch Function active... Posting to FM Radio Module");
      SelectionStage = "Switching";
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Switching to:");
      lcd.setCursor(0, 1);
      lcd.print("Radio");
      StartRadio();

    }

  }
}
