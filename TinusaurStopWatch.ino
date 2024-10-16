#include "LedControl.h"
#include <debounce.h>

#define BTN     4
#define MOSI    1
#define SCK     2
#define CS      0

#define CLEARED 0
#define RUNNING 1
#define STOPPED 2

LedControl lc = LedControl(MOSI,SCK,CS,1);
int runState = CLEARED;
bool showingState = false;
int t = 0;
unsigned long currentMillis;
unsigned long startMillis;
int period = 100;

static void buttonHandler(uint8_t btnId, uint8_t btnState) {
  if (btnState != BTN_PRESSED) return;
  runState++;
  if (runState > STOPPED) runState = CLEARED;
}

static Button stateButton(BTN, buttonHandler);

void printTime() {
  int d0 =  t % 10;
  int d1 =  (t / 10) % 10;
  int d2 =  (t - (t % 100)) / 100;
  lc.clearDisplay(0);
  delayMicroseconds(10);
  lc.setDigit(0,2,d2,false);
  delayMicroseconds(10);
  lc.setDigit(0,1,d1,true);
  delayMicroseconds(10);
  lc.setDigit(0,0,d0,false);
}

void setup() {
 // Disable watchdog timer to prevent unexpected resets
  MCUSR &= ~(1<<WDRF);
  WDTCR |= (1<<WDCE) | (1<<WDE);
  WDTCR = 0x00;

  pinMode(BTN, INPUT_PULLUP);
  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);
  delayMicroseconds(100);
}

void loop() { 
  stateButton.update(digitalRead(BTN));
  currentMillis = millis();
  if (currentMillis - startMillis >= period) {
    if (runState == CLEARED) {
      t = 0;
    } else if (runState == RUNNING) {
      t++;
      if (t == 999) runState = STOPPED;
    }
    printTime();
    startMillis = currentMillis;  
  }
}



