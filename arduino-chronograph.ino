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

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);   

double firstTripTime, secondTripTime;
boolean hasFirstTripped = false;
String chronoValsToPrint = "";

void setup () {
  setupDisplay();
}

void loop () {
  loopDisplay();
  chrono();
}

void setupDisplay () {
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);    
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);  
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
}

void loopDisplay () {
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() ); 
}

void draw () {
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr( 0, 22, chronoValsToPrint);
}

void chrono () {
    if (hasFirstTripped == false && (map(analogRead(IR_REC_ONE_PIN), 0, 1024, 0, 100) > IR_GATE_TRIP_VAL) ) {
        firstTripTime = micros();
        hasFirstTripped = true;
    }
    if (hasFirstTripped == true && (map(analogRead(IR_REC_TWO_PIN), 0, 1024, 0, 100) > IR_GATE_TRIP_VAL) ) {
        secondTripTime = micros();
        hasFirstTripped = false;
        chronoValsToPrint = (String)calculateChronoReadings();
    }
}

double calculateChronoReadings () {
    double timeElapsed = (secondTripTime - firstTripTime) * 1000000;
    double displacement = GATE_DISPLACEMENT / 12;

    return displacement/timeElapsed;
}


