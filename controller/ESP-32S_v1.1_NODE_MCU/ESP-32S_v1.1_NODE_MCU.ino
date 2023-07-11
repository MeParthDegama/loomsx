/*

  Board: ESP32 Dev Module

  Linux Troubleshoot:
    $ sudo adduser <username> dialout # only one time
    $ sudo chmod a+rw /dev/ttyUSB0 # every time

*/

#include "ArduinoWebsockets.h"
#include "WiFi.h"
#include "SSD1306Wire.h"

#define POWER_LED  17
#define SIGNAL_PIN 4
#define STOP_LED 16
#define PLUSE_LED 5

#define OFF_TIME_DELAY 2000

// server
const char* ssid = "LOOMX-PROTO";
const char* password = "!@#$%12345";
const char* websockets_server_host = "serverip_or_name";
const uint16_t websockets_server_port = 8080;

using namespace websockets;
WebsocketsClient client;

//OLED related variables
#define OLED_ADDR   0x3c
#define OLED_SDA    21
#define OLED_SCL    22
#define OLED_RST    16

SSD1306Wire  display(OLED_ADDR, OLED_SDA, OLED_SCL); 

void setup() {
  
  Serial.begin(38400);
  Serial.println("Start");

  WiFi.begin(ssid, password);

  bool connected = client.connect(websockets_server_host, websockets_server_port, "/");
  if(connected) {
      Serial.println("Connected!");
  } else {
      Serial.println("Not Connected!");
  }

  pinMode(POWER_LED, OUTPUT);
  pinMode(SIGNAL_PIN, INPUT);
  pinMode(STOP_LED, OUTPUT);
  pinMode(PLUSE_LED, OUTPUT);

  digitalWrite(POWER_LED, HIGH);

  initOLED();

}

struct timeval tv;

int sutter_counter = 0;
_Bool pre_signal = LOW;
uint64_t prev_time = 0;

_Bool pre_mstatus = false;

String display_line_1 = "hello";
String display_line_2 = "hello";
String display_line_3 = "hello";

void loop() {

  if (WiFi.status() == WL_NO_SSID_AVAIL) {
    display_line_1 = "WiFi Not Found";
    showOLEDMessage();
  } else {
    if (WiFi.status() != WL_CONNECTED) {
      display_line_1 = "Connecting...";
      showOLEDMessage(); 
    } else {
      display_line_1 = "Connected";
      showOLEDMessage();
    }
  }

	gettimeofday(&tv, NULL);
	uint64_t curr_time = tv.tv_sec * 1000LL + (tv.tv_usec / 1000LL);

  _Bool cur_signal = digitalRead(SIGNAL_PIN);
  //Serial.println(pre_signal);
  digitalWrite(PLUSE_LED, cur_signal);

  if (cur_signal != pre_signal) {
  
    if (cur_signal == HIGH) {

      if (!pre_mstatus) {
        digitalWrite(STOP_LED, LOW);
      }

    	prev_time = curr_time;
      Serial.printf("Count: %d\n", ++sutter_counter);
//      showOLEDMessage("Count:", String(sutter_counter), "");

    display_line_2 = "Count: " + String(sutter_counter);
    showOLEDMessage();


      pre_mstatus = true;
    
    }
  
  }

  if ((curr_time - prev_time) > OFF_TIME_DELAY) {

    if (pre_mstatus) {
      
      digitalWrite(STOP_LED, HIGH);
      display_line_2 = "Stoped";
      showOLEDMessage();
      sutter_counter = 0;

      pre_mstatus = false;
    
    }

  }
  
  pre_signal = cur_signal;
  
}

void initOLED() {
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);
  display.init();   
}

void showOLEDMessage() {
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, display_line_1);       //  adds to buffer
  display.drawString(0, 20, display_line_2);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 40, display_line_3);
  display.display();                     // displays content in buffer
}
