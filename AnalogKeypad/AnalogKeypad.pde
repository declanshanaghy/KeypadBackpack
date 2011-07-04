#define DEBUG 1

#include <AnalogKeypad.h>

/***************************
  ANALOG PIN DEFINIDTIONS
****************************/
#define PIN_KEYS    A0

/*****************************
  GLOBAL VARS
******************************/
AnalogKeypad kpad = AnalogKeypad(PIN_KEYS);

void setup() {
  Serial.begin(115200);
  kpad.init();
}

void loop() {
  int k = kpad.readKey();
  switch (k) {
    case KEY_NONE:
      break;
    case KEY_STAR:
      Serial.println("*");
      break;
    case KEY_POUND:
      Serial.println("#");
      break;
    case KEY_0:
      Serial.println("0");
      break;
    case KEY_1:
      Serial.println("1");
      break;
    case KEY_2:
      Serial.println("2");
      break;
    case KEY_3:
      Serial.println("3");
      break;
    case KEY_4:
      Serial.println("4");
      break;
    case KEY_5:
      Serial.println("5");
      break;
    case KEY_6:
      Serial.println("6");
      break;
    case KEY_7:
      Serial.println("7");
      break;
    case KEY_8:
      Serial.println("8");
      break;
    case KEY_9:
      Serial.println("9");
      break;
  }
}
