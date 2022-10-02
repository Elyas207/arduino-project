#include <SoftwareSerial.h>

SoftwareSerial btSerial(3, 2);
unsigned long previousMillis = 0;


void setup() {
  Serial.begin(9600);
  btSerial.begin(9600);
  delay(2000);
  command("AT+QT\r\n", 5000);

}

void loop() {


}



String command(const char *toSend, unsigned long milliseconds) {
  String result;
  Serial.print("Sending: ");
  Serial.println(toSend);
  btSerial.println(toSend);
  unsigned long startTime = millis();
  Serial.print("Received: ");
   while (millis() - startTime < milliseconds) {
    if (btSerial.available()) {
      char c = btSerial.read();
      result += c;  // append to the result string
    }
  }
  Serial.println(result);  // new line after timeout.
  return result;
}
