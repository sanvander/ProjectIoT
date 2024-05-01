/***************************************************
  This is our GFX example for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/


#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "SD.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10
#define TFT_RST 8
#define SD_CS 6

const int buttonPin = 2;
int buttonState;         
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
const int interval = 50; // Interval in milliseconden (1 seconde)
unsigned long previousMillis = 0;

int button_clicks = 0;

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
// If using the breakout, change pins as desired
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

void setup() {
   pinMode(2, INPUT);
  
  tft.begin();
  tft.setRotation(2); // Aanpasbaar naar scherm orientatie
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println(button_clicks);
  
    if (!SD.begin(SD_CS)) {
    tft.println("SD card initialization failed!");
    return;
    
  }
  tft.println("SD card contents:");
  printDirectory(SD.open("/"), 0);
  
}

void loop() {
  int reading = digitalRead(buttonPin);
  unsigned long currentMillis = millis();

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        button_clicks = button_clicks + 1;
        tft.setCursor(0, 0);
        clearLine(0);
        tft.println(button_clicks);
        
      }
    }
  }
  lastButtonState = reading;


  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
  }
  



}

void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i=0; i<numTabs; i++) {
      tft.print('\t');
    }
    tft.println(entry.name());
    if (entry.isDirectory()) {
      printDirectory(entry, numTabs+1);
    }
    entry.close();
  }
}

void clearLine(int y) {
  tft.fillRect(0, y, tft.width(), 20, ILI9341_BLACK);
  // tft.textHeight() geeft de hoogte van de huidige tekstgrootte
}

