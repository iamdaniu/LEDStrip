#include <Arduino.h>

const int GREEN_PIN = 3;
const int RED_PIN = 5;
const int BLUE_PIN = 6;
int allPins[3] = { RED_PIN, GREEN_PIN, BLUE_PIN };

const int BUTTON_PIN = 2;

void phaseLED(int);
void blinkLED(int);
void blinkWhite(int);
void buttonPressed();

const int maxMode = 3;
typedef void (*led_handler) (int);
led_handler handlers[maxMode] = { blinkLED, phaseLED, blinkWhite };

volatile int mode = 0;
volatile bool modeChange = false;

void setup() {
  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonPressed, FALLING);

  for (int i = 0; i < 3; i++) {
      pinMode(allPins[i], OUTPUT);
      digitalWrite(allPins[i], LOW);
  }
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.println("setup finished");
}

void loop() {
  for (int i = 0; i < 3; i++)
  {
    led_handler handlerFunction = handlers[mode];
    handlerFunction(allPins[i]);
  }
}

void buttonPressed() {
  mode++;
  if (mode > maxMode) {
    mode = 0;
  }
  modeChange = true;
}

void phaseLED(int ledPin) {
  Serial.println("phasing");
  for (int i = 0; i < 512; i++)
  {
    int setValue = 256 - abs(i - 256);
    analogWrite(ledPin, setValue);
    delay(10);
    if (modeChange) {
      modeChange = false;
      break;
    }
  }
  analogWrite(ledPin, 0);
}

void blinkLED(int ledPin) {
  Serial.println("blinking");
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
}


void blinkWhite(int ledPin) {
  Serial.println("blinking");
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(allPins[i], HIGH);
  }
  
  delay(500);
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(allPins[i], LOW);
  }
  delay(500);
}

