/*
  Board: ESP32 Dev Module

  Linux Troubleshoot:
    $ sudo adduser <username> dialout # only one time
    $ sudo chmod a+rw /dev/ttyUSB0 # every time

*/

#define POWER_LED  17
#define SIGNAL_PIN 4
#define STOP_LED 16
#define PLUSE_LED 5

void setup() {
  
  Serial.begin(38400);
  Serial.println("Start");

  pinMode(POWER_LED, OUTPUT);
  pinMode(SIGNAL_PIN, INPUT);
  pinMode(STOP_LED, OUTPUT);
  pinMode(PLUSE_LED, OUTPUT);

  digitalWrite(POWER_LED, HIGH);

}

_Bool preSignal = LOW;

int counter = 0;

void loop() {

  _Bool curSignal = !digitalRead(SIGNAL_PIN);
  digitalWrite(PLUSE_LED, curSignal);

  if (curSignal != preSignal) {
  
    if (curSignal == HIGH) {

      Serial.printf("On: %d\n", ++counter);
    
    }
  
  }

  preSignal = curSignal;

}
