#include <BufferedInput.h>
#include <BufferedOutput.h>
#include <loopTimer.h>
#include <millisDelay.h>
#include <PinFlasher.h>
#include <SafeString.h>
#include <SafeStringNameSpace.h>
#include <SafeStringNameSpaceEnd.h>
#include <SafeStringNameSpaceStart.h>
#include <SafeStringReader.h>
#include <SafeStringStream.h>
#include <SerialComs.h>

#include <TEA5767N.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

TEA5767N radio = TEA5767N();
LiquidCrystal_I2C lcd(0x27, 20, 4);

char MODE = 'N';  // 'R' = FM RADIO | 'B' = BLUETOOTH | 'N' = NONE
int analogPin = 0;
int val = 0;
int frequencyInt = 0;
float frequency = 101.1;
float previousFrequency = 0;
int signalStrength = 0;
#define BTN_01_PIN = 0
#define BTN_2_PIN = 0
String SelectionStage;  //None = None, Startup = Audio source selection, Switch = Switching Source. RemoteConnectWait = Waiting for Remote Control Connection
String OldStageSelection;
//createSafeString(SelectionStage, 8);
//createSafeString(OldStageSelection, 30);
int SelectionStageNum = 0;
String debug_command;
String SelectedMode = "None";
String BluetoothAudioPortOpened = "No";
String RadioAudioPortOpened = "No";
bool BluetoothAlreadyStarted = false;
String BluetoothStatus = " ";
SoftwareSerial btSerial(2, 3);  //RX TX


void setup() {
  Wire.begin();
  Serial.begin(9600);
  btSerial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Starting Up...");
  Serial.println("PRE-BOOT: OK");
  SelectionStage = "";
  delay(2000);
  //command("AT+CM08\r\n");
  BluetoothAlreadyStarted = true;
  MuteAll();
  radio.selectFrequency(frequency);
  radio.setStereoNoiseCancellingOn();
  SelectionStageNum = 0;
  SelectionStage = "Startup";
  Serial.print("Startup OK.");
  SelectionStage = String("RemoteConnectWait");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Waiting for");
  lcd.setCursor(0, 1);
  lcd.print("Remote Connection");
}



void UpdateRadioInfo() {

  if (MODE == 'R') {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RADIO:");
    lcd.setCursor(0, 1);
    String frequencyString = String(frequency);
    lcd.print(frequencyString);
  }
}


void UpdateBluetoothInfo() {
  if (MODE == 'B') {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("BLUETOOTH AUDIO:");
    lcd.setCursor(0, 1);
    lcd.print(BluetoothStatus);
  }
}


void CheckBTStatus() {
}


void StartRadio() {
  radio.turnTheSoundBackOn();
  radio.selectFrequency(frequency);
  MODE = 'R';

  UpdateRadioInfo();
}


void StartBluetooth() {
  if (BluetoothAlreadyStarted == false) {
    command("AT+CM01");
  }

  MODE = 'B';
  UpdateBluetoothInfo();
}





void MuteAll() {
  // Mute all Audio
  radio.setStereoNoiseCancellingOn();
  radio.mute();
}

void loop() {
  if (SelectionStage == "Startup") {
    lcd.setCursor(0, 0);
    lcd.print("Choose a mode:");
    if (SelectionStageNum == 1) {
      lcd.setCursor(0, 1);  //counting starts at 0
      lcd.print("- Bluetooth Audio");
      lcd.setCursor(0, 2);  //counting starts at 0
      lcd.print("Radio");
    }

    if (SelectionStageNum == 0) {
      lcd.setCursor(0, 1);  //counting starts at 0
      lcd.print("Bluetooth Audio");
      lcd.setCursor(0, 2);  //counting starts at 0
      lcd.print("- Radio");
    }
  }



  if (SelectionStage == "None")

  {
    if (SelectedMode == "Radio" || SelectedMode == "Bluetooth") {
      if (SelectedMode == "Radio") {
        if (RadioAudioPortOpened == "No") {
          StartRadio();
        }
      }

      if (SelectedMode == "Bluetooth") {
        if (BluetoothAudioPortOpened == "No") {
          StartBluetooth();
        }
      }
    }
  }



  if (btSerial.available()) {
    debug_command = btSerial.readStringUntil('\n');
    Serial.println(debug_command);

    if (debug_command.equals("REMOTE_COMMAND=HOME")) {
      if (SelectionStage == "None") {
        MuteAll();
        lcd.clear();
        SelectionStageNum = 0;
        SelectionStage = "Startup";
      }
    }

    if (debug_command.equals("REMOTE_COMMAND=CONNECTED")) {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Connected to the");
      lcd.setCursor(0, 2);
      lcd.print("Control Website!");
      delay(1500);
      lcd.clear();
      if (SelectionStage.equals("RemoteConnectWait")) {
        if (SelectedMode == "Radio" || SelectedMode == "Bluetooth") {
          if (SelectedMode == "Radio") {
            SelectionStage = "None";
            UpdateRadioInfo();
            btSerial.write("MODE_SET=RADIO");
          }

          if (SelectedMode == "Bluetooth") {
            SelectionStage = "None";
            UpdateBluetoothInfo();
            btSerial.write("MODE_SET=BT");
          }

        }

        else {
          SelectionStage = "Startup";
          btSerial.write("STARTUP=TRUE");
        }
      }
    }


    if (debug_command.equals("REMOTE_COMMAND=DISCONNECTED") || debug_command.equals("TS+04")) {
      OldStageSelection = SelectionStage;
      SelectionStage = "RemoteConnectWait";
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Disconnected from");
      lcd.setCursor(0, 2);
      lcd.print("the Control Website");
      delay(1500);
      lcd.clear();
      SelectionStage = OldStageSelection;
      if (SelectionStage.equals("Startup") || OldStageSelection.equals("Startup") || SelectionStage.equals("None") || OldStageSelection.equals("None")) {
        SelectionStage = "RemoteConnectWait";
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Waiting for");
        lcd.setCursor(0, 1);
        lcd.print("Remote Connection");
      }
    }



    if (debug_command.equals("REMOTE_COMMAND=UP")) {
      Serial.println(SelectionStage);
      Serial.println("Debug Command is: Up");
      if (SelectionStage == "Startup") {

        Serial.println(SelectionStage);

        if (SelectionStageNum == 0) {
          SelectionStageNum = 1;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Choose a mode:");
        }
      }
    }


    if (debug_command.equals("REMOTE_COMMAND=DOWN")) {
      Serial.println("Remote Command is: Down");
      if (SelectionStage == "Startup") {
        if (SelectionStageNum == 1) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Choose a mode:");
          SelectionStageNum = 0;
        }
      }
    }



    if (debug_command.equals("REMOTE_COMMAND=SELECT")) {
      Serial.println("Remote Command is: Select");
      if (SelectionStage == "Startup") {
        if (SelectionStageNum == 0) {
            
          SelectedMode = "Radio";
          SelectionStage = "None";
          BluetoothAudioPortOpened = "No";
          RadioAudioPortOpened = "Yes";
          MODE = 'R';
          StartRadio();
          btSerial.write("MODE_SET=RADIO");
        }

        if (SelectionStageNum == 1) {
          SelectedMode = "Bluetooth";
          SelectionStage = "None";
          RadioAudioPortOpened = "No";
          BluetoothAudioPortOpened = "Yes";
          MODE = 'B';
          StartBluetooth();
          btSerial.write("MODE_SET=BT");
        }
      }
    }
  }
}

void SwitchAudioSource(String toSwitch) {
  if (toSwitch == "Bluetooth") {

    if (MODE == 'R') {
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


  if (toSwitch == "Radio") {
    if (MODE == 'B') {
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





String command(const char *toSend) {
  String result;
  Serial.print("Sending: ");
  Serial.println(toSend);
  btSerial.println(toSend);
}



// if (Serial.available()) {
//    debug_command = Serial.readStringUntil('\n');
//    Serial.println(debug_command);
//    if (debug_command.equals("up")) {
//
//      Serial.println(SelectionStage);
//      Serial.println("Debug Command is: Up");
//      if (SelectionStage == "Startup") {
//
//        Serial.println(SelectionStage);
//
//        if (SelectionStageNum == 0) {
//          SelectionStageNum = 1;
//          lcd.clear();
//          lcd.setCursor(0, 0);
//          lcd.print("Choose a mode:");
//        }
//      }
//    }
//
//    if (debug_command.equals("down")) {
//      Serial.println("Debug Command is: Down");
//      if (SelectionStage == "Startup") {
//        if (SelectionStageNum == 1) {
//          lcd.clear();
//          lcd.setCursor(0, 0);
//          lcd.print("Choose a mode:");
//          SelectionStageNum = 0;
//        }
//      }
//    }
//
//
//    if (debug_command.equals("select")) {
//      Serial.println("Debug Command is: Select");
//      if (SelectionStage == "Startup") {
//        if (SelectionStageNum == 0) {
//
//          SelectedMode = "Radio";
//          SelectionStage = "None";
//          BluetoothAudioPortOpened = "No";
//          RadioAudioPortOpened = "Yes";
//          MODE = 'R';
//          StartRadio();
//        }
//
//        if (SelectionStageNum == 1) {
//          SelectedMode = "Bluetooth";
//          SelectionStage = "None";
//          RadioAudioPortOpened = "No";
//          BluetoothAudioPortOpened = "Yes";
//          MODE = 'B';
//          StartBluetooth();
//        }
//      }
//    }
//
//
//
//    if (debug_command.equals("switch_bluetooth")) {
//      MuteAll();
//      Serial.print("Switching Over to Bluetooth Audio");
//      SwitchAudioSource("Bluetooth");
//    }
//
//
//    if (debug_command.equals("switch_radio")) {
//      MuteAll();
//      Serial.print("Switching Over to FM Radio");
//      SwitchAudioSource("Radio");
//    }
//  }
