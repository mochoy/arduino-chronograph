#include "U8glib.h"

#define IR_REC_ONE_PIN 0;
#define IR_REC_TWO_PIN 1;
#define GATE_DISPLACEMENT 4;    //in inches
#define IR_GATE_TRIP_VAL 95;

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);   

double firstTripTime, secondTripTime;
boolean hasFirstTripped = false;

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
  u8g.drawStr( 0, 22, "Hello Worlds!");
}

void chrono () {
    if (hasFirstTripped == false && map(analogRead(IR_REC_ONE_PIN), 0, 1024, 0, 100) > IR_GATE_TRIP_VAL)  {
        firstTripTime = micros();
        hasFirstTripped = true;
    }
    if (hasFirstTripped == true && (map(analogRead(IR_REC_TWO_PIN), 0, 1024, 0, 100) > IR_GATE_TRIP_VAL) ) {
        secondTripTime = micros();
        hasFirstTripped = false;
        calculateChronoReadings();
    }
}

double calculateChronoReadings () {
    
}


