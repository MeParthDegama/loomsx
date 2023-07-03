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

#define OFF_TIME_DELAY 2000

void setup() {
  
  Serial.begin(38400);
  Serial.println("Start");

  pinMode(POWER_LED, OUTPUT);
  pinMode(SIGNAL_PIN, INPUT);
  pinMode(STOP_LED, OUTPUT);
  pinMode(PLUSE_LED, OUTPUT);

  digitalWrite(POWER_LED, HIGH);

}

struct timeval tv;

int sutter_counter = 0;
_Bool pre_signal = LOW;
uint64_t prev_time = 0;

_Bool pre_mstatus = false;

void loop() {

	gettimeofday(&tv, NULL);
	uint64_t curr_time = tv.tv_sec * 1000LL + (tv.tv_usec / 1000LL);

  _Bool cur_signal = !digitalRead(SIGNAL_PIN);
  //digitalWrite(PLUSE_LED, cur_signal);

  if (cur_signal != pre_signal) {
  
    if (cur_signal == HIGH) {

      if (!pre_mstatus) {
        digitalWrite(STOP_LED, LOW);
      }

    	prev_time = curr_time;
      Serial.printf("Count: %d\n", ++sutter_counter);

      pre_mstatus = true;
    
    }
  
  }

  if ((curr_time - prev_time) > OFF_TIME_DELAY) {

    if (pre_mstatus) {
      
      digitalWrite(STOP_LED, HIGH);
      sutter_counter = 0;

      pre_mstatus = false;
    
    }

  }
  

  pre_signal = cur_signal;

}
