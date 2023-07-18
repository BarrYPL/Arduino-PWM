#include <FastLED.h>

#define NUM_LEDS 8
#define LED_PIN 6
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
#define BRIGHTNESS 255
CRGB led[NUM_LEDS];

static int order[4] = {0, 0, 0 };
int pwmPins[] = { 5, 10, 9 };
char inChar;
char inData[40];
char argFt[5];
char argSc[5];
char argTr[5];
int flag = 0;
byte incommingOrderFlag = 0;

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(led, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  pinMode(pwmPins[1], OUTPUT);
  pinMode(pwmPins[2], OUTPUT);
  pinMode(pwmPins[3], OUTPUT);
  Serial.begin(115200);
  LEDReset();
  setTimers();
  sei();
  analogWrite(pwmPins[1], 0);
  analogWrite(pwmPins[2], 0);
  analogWrite(pwmPins[3], 0);
}

void loop() {
  for (int idx = 0; idx < 255; idx++) {
    checkForSerial();
    //Serial.println(idx);
    for (int ledIdx = 0; ledIdx < 3; ledIdx++) {
      analogWrite(pwmPins[ledIdx],idx);
    }
    delay(5);
  }
  for (int idx = 255; idx > 0; idx--) {
    checkForSerial();
    //Serial.println(idx);
    for (int ledIdx = 0; ledIdx < 3; ledIdx++) {
      analogWrite(pwmPins[ledIdx],idx);
    }
    delay(5);
  }
}

void checkForSerial() {
  int index1 = 0;
  int index2 = 0;
  int index3 = 0;
  while (Serial.available() > 0) {
    inChar = Serial.read();
    if (inChar == ',') {
      flag++;
    }
    if (flag == 0) {
      argFt[index1] = inChar;
      index1++;
      argFt[index1] = '\0';
    }
    if ((flag == 1) && (index1 == 0)) {
      if (inChar != ',') {
        argSc[index2] = inChar;
        index2++;
        argSc[index2] = '\0';
      }
    }
    if ((flag == 2) && (index2 == 0)) {
      if (inChar != ',') {
        argTr[index3] = inChar;
        index3++;
        argTr[index3] = '\0';
      }
    }
    if (flag == 1) {
      index1 = 0;
    }
    if (flag == 2) {
      index2 = 0;
    }
    if (flag == 3) {
      index3 = 0;
    }
    incommingOrderFlag = 1;
  }
  if (flag == 2) {
    order[0] = atoi(argFt);
    order[1] = atoi(argSc);
    order[2] = atoi(argTr);
    flag = 0;
  }
  if (incommingOrderFlag == 1) {
    applyChanges();
    incommingOrderFlag = 0;
    order[0] = -1;
    order[1] = -1;
    order[2] = -1;
  }
}

int pwmPin(int k) {
  int pin = 0;
  if (k > 0 && k < 4) {
    k = k - 1;
    pin = pwmPins[k];
  } else {
    pin = 0;
  }
  return pin;
}

void LEDReset() {
  for (int i = 0; i < NUM_LEDS; i++) {
    led[i] = CRGB(0, 0, 0);
  }
  FastLED.show();
}

void applyChanges() {
  //  Serial.print("order[0] = ");
  //  Serial.print(order[0]);
  //  Serial.println("");
  //  Serial.print("order[1] = ");
  //  Serial.print(order[1]);
  //  Serial.println("");
  //  Serial.print("order[2] = ");
  //  Serial.print(order[2]);
  //  Serial.println("");
  for (byte i = 0; i < (sizeof(order) / sizeof(order[0])) - 1; i++) {
    if (order[i] >= 0 && order[i] <= 255) {
      //analogWrite(pwmPins[i], order[i]);
      setLEDS(i, order[i]);
    }
  }
  FastLED.show();
}

void setLEDS(int ledNum, int val) {
  ledNum = 2 - ledNum;
  switch (ledNum) {
    case 0:
      led[ledNum] = CRGB(0, 0, val);
      break;
    case 1:
      led[ledNum] = CRGB(0, val, 0);
      break;
    case 2:
      led[ledNum] = CRGB(val, 0, 0);
      break;
    default:
      break;
  }
}

void setTimers() {
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR3A = 0;

  TCCR3A |= (1 << COM3A1);
  TCCR1A |= (1 << COM1A1) | (1 << COM1B1);
  OCR1B = 0;
  OCR1A = 0;
  OCR3A = 0;
  TCCR3A |= (1 << WGM10);
  TCCR3A |= (1 << WGM12) | (1 << CS12);
  TCCR1A |= (1 << WGM10);
  TCCR1B |= (1 << WGM12) | (1 << CS12);
}
