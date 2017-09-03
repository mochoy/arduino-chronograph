#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define IR_REC_ONE_PIN 0
#define IR_REC_TWO_PIN 1
#define GATE_DISPLACEMENT 4    //in inches
#define IR_GATE_TRIP_VAL 95

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

double firstTripTime, secondTripTime;
boolean hasFirstTripped = false;
char chronoValsToPrint[4];

void setup () {
  u8g2.begin();
}

void loop () {
  // chrono();
  display();
}

void display () {
  u8g2.firstPage();
    do {
      sprintf(chronoValsToPrint,"%04d", analogRead(IR_REC_ONE_PIN));
      u8g2.setFont(u8g2_font_ncenB10_tr);
      u8g2.drawUTF8(0, 24, chronoValsToPrint);
    } while ( u8g2.nextPage() );
  delay(1000);
}


void chrono () {
  if (hasFirstTripped == false && (map(analogRead(IR_REC_ONE_PIN), 0, 1024, 0, 100) > IR_GATE_TRIP_VAL) ) {
    firstTripTime = micros();
    hasFirstTripped = true;
  }
  if (hasFirstTripped == true && (map(analogRead(IR_REC_TWO_PIN), 0, 1024, 0, 100) > IR_GATE_TRIP_VAL) ) {
    secondTripTime = micros();
    hasFirstTripped = false;
    // chronoValsToPrint = (String)calculateChronoReadings();
  }
}

double calculateChronoReadings () {
  double timeElapsed = (secondTripTime - firstTripTime) * 1000000;
  double displacement = GATE_DISPLACEMENT / 12;

  return displacement/timeElapsed;
}


