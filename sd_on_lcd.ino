#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "SD.h"


#define TFT_DC 9
#define TFT_CS 10
#define TFT_RST 8
#define SD_CS 6

const int buttonPin = 5; 
int buttonState;         
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;


// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
// If using the breakout, change pins as desired
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

void setup() {
  pinMode(5, INPUT);
  tft.begin();
  tft.setRotation(2); // Aanpasbaar naar scherm orientatie
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  
    if (!SD.begin(SD_CS)) {
    tft.println("SD card initialization failed!");
    return;
  }
  tft.println("Druk op de knop om SD-inhoud te zien");
}

void loop() {
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        while (digitalRead(5) == LOW) {
          delay(50);  
        }   
  
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(0, 0);
        tft.println("SD card contents:");
        printDirectory(SD.open("/"), 0);
        
      }
    }
  }
  lastButtonState = reading;
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