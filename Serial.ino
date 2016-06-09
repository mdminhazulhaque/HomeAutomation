#include <SoftwareSerial.h>

SoftwareSerial GSM_Module(6, 7); // RX, TX

void setup() {
  Serial.begin(9600);
  Serial.println("GSM Module!");
  GSM_Module.begin(9600);
}

void loop()
{
  if (GSM_Module.available())
  {
    Serial.write(GSM_Module.read());
  }
  if (Serial.available())
  {
    GSM_Module.write(Serial.read());
  }
}
