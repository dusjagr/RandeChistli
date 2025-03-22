#include <OneWire.h>
#include <Adafruit_NeoPixel.h>
#include "pitches.h"

/* ======== RANDE_CHISTLI - TEMPERATURE MONITORING SYSTEM ========
 * 
 * Project developed for RandeLab - Forest-Geeking and Residency Place
 * Author: Marc Dusseiller (dusjagr)
 * 
 * HARDWARE REQUIREMENTS:
 * - ATtiny85 microcontroller
 * - DS18B20/DS18S20 OneWire temperature sensor (connected to PB4)
 * - NeoPixel/WS2812 RGB LED strip (8 LEDs, connected to pin 0)
 * - Speaker/Buzzer (connected to pin 1)
 * - 4.7K pull-up resistor for OneWire sensor
 * 
 * FUNCTIONALITY:
 * This device monitors ambient temperature and provides visual and audio
 * feedback through RGB LEDs and a speaker. It warns when temperatures
 * drop below a certain threshold, which is useful for environmental 
 * monitoring in forest/cabin settings.
 * 
 * VISUAL INDICATORS:
 * - LED colors change based on temperature
 * - Rainbow animation on startup
 * - Binary patterns for status display
 * 
 * AUDIO INDICATORS:
 * - Melody plays on startup
 * - Warning beeps when temperature falls below warning threshold
 * 
 * TEMPERATURE THRESHOLDS:
 * - Low temperature: 4°C
 * - Warning temperature: 5°C
 * - Maximum temperature range: 30°C
 */

// OneWire DS18S20, DS18B20, DS1822 Temperature Example
//
// http://www.pjrc.com/teensy/td_libs_OneWire.html
//
// The DallasTemperature library can do all this work for you!
// http://milesburton.com/Dallas_Temperature_Control_Library


#define ONEWIREPIN   PB4
#define NEOPIXELPIN    0
#define NUMPIXELS      8
#define SPEAKERPIN     1

#define hell          25 // Brightness
#define lowTemp        4
#define maxTemp       30
#define warnTemp      5
#define warnAlarm     2 // How often it beeps when below warnTemp

  int updateSpeed = 1000; // maybe 750ms is enough, maybe not
  uint8_t warnCount = 0;
  uint8_t statusLED = (maxTemp-10) * 3;
  int tempColor = 0;
  int showPixel = 0;

  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius;
  int16_t raw;

/* HARDWARE CONNECTIONS:
 * - DS18B20/DS18S20 temperature sensor: Connect to ONEWIREPIN (PB4)
 *   - VCC: Connect to 3.3V or 5V
 *   - GND: Connect to ground
 *   - DATA: Connect to ONEWIREPIN (PB4) with 4.7K pull-up resistor to VCC
 * 
 * - NeoPixel LED strip: Connect to NEOPIXELPIN (pin 0)
 *   - VCC: Connect to 5V
 *   - GND: Connect to ground
 *   - DATA: Connect to NEOPIXELPIN (pin 0)
 * 
 * - Speaker/Buzzer: Connect to SPEAKERPIN (pin 1)
 *   - One terminal to SPEAKERPIN (pin 1)
 *   - Other terminal to ground
 */

OneWire  ds(ONEWIREPIN);  // on pin 10 (a 4.7K resistor is necessary)

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIXELPIN, NEO_GRB + NEO_KHZ800);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Specific functions of the NEO-Pixel Library
 _____ _____ _____        _         _ 
|   | |   __|     |   ___|_|_ _ ___| |
| | | |   __|  |  |  | . | |_'_| -_| |
|_|___|_____|_____|  |  _|_|_,_|___|_|
                     |_|              
========================================================================================================================
   _________    
  | NEO(x)  |   void setWhiteAllPixel(uint32_t color)                   -> Sets all the pixels to the white level
  |  o___o  |   void displayBinrayValue(uint16_t value, uint32_t color) -> displays binary number
  |__/_____|   uint32_t Wheel(byte WheelPos)                           -> Input a value 0 to 255 to get a color value.   
                                                                        The colours are a transition r - g - b - back to r. 
                                
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/* FUNCTION: setWhiteAllPixel
 * Sets all pixels to the same white brightness level
 * 
 * Parameters:
 * - color: Brightness value (0-255) for white
 */
void setWhiteAllPixel(uint32_t color)
{
  uint8_t n;
  for (n = 0; n < NUMPIXELS; n++)
  {
    pixels.setPixelColor(n, color, color, color);
  }
}

/* FUNCTION: setColorAllPixel
 * Sets all pixels to the same RGB color
 * 
 * Parameters:
 * - color: 32-bit color value (RGB format)
 */
void setColorAllPixel(uint32_t color)
{
  uint8_t n;
  for (n = 0; n < NUMPIXELS; n++)
  {
    pixels.setPixelColor(n, color);
  }
}

/* FUNCTION: displayBinrayValue
 * Displays a binary representation of a value using LEDs
 * Each bit that is set (1) will light up the corresponding LED
 * 
 * Parameters:
 * - value: The binary value to display (up to 16 bits)
 * - color: Color to use for lit pixels
 */
void displayBinrayValue(uint16_t value, uint32_t color)
{
  uint8_t n;
  for (n = 0; n < NUMPIXELS; n++)
  {
    if (value & (1 << n)) pixels.setPixelColor(n, color);
    //else pixels.setPixelColor(n,0); // off
  }
}

/* FUNCTION: rainbowCycle
 * Creates a smooth cycling rainbow pattern across multiple pixels
 * 
 * Parameters:
 * - wait: Delay time in milliseconds between updates
 * - rounds: Number of complete color cycles to perform
 * - rainbowPixels: Number of pixels to use for the rainbow effect
 */
void rainbowCycle(uint8_t wait, uint8_t rounds, uint8_t rainbowPixels) {
  uint16_t i, j;

  for (j = 0; j < 256 * rounds; j++) { 
    for (i = 0; i < rainbowPixels; i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / rainbowPixels) + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

/* FUNCTION: Wheel
 * Color conversion function that creates a color wheel effect
 * Maps a position (0-255) to a color value in the red-green-blue spectrum
 * 
 * Parameters:
 * - WheelPos: Position on the color wheel (0-255)
 * 
 * Returns:
 * - 32-bit color value
 */
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

/* AUDIO OUTPUT FUNCTIONS
 * The following code handles sound generation using direct pin manipulation 
 * for better performance on the ATtiny85
 */

// fast pin access
#define AUDIOPIN (1<<SPEAKERPIN)
#define PINLOW (PORTB&=~AUDIOPIN)
#define PINHIGH (PORTB|=AUDIOPIN)

/* FUNCTION: playSound
 * Generates a square wave tone at specified frequency and duration
 * Uses direct port manipulation for faster toggling
 * 
 * Parameters:
 * - freq_Hz: Frequency of the tone in Hertz
 * - duration_ms: Duration of the tone in milliseconds
 */
void playSound(long freq_Hz, long duration_ms)
{
  uint16_t n;
  uint32_t delayTime_us;
  uint32_t counts;

  delayTime_us = 1000000UL / freq_Hz / 2;
  counts = duration_ms * 1000 / delayTime_us;

  for (n = 0; n < counts; n++)
  {
    PINHIGH;
    delayMicroseconds(delayTime_us);
    PINLOW;
    delayMicroseconds(delayTime_us);
  }
}

/* MELODY DEFINITIONS
 * Musical notes for the startup melody
 * Notes are defined in pitches.h
 * PAUSE is used for silent intervals
 */
int c[] = {
  NOTE_B4, NOTE_G4, NOTE_D4, NOTE_G4, NOTE_A4, NOTE_D5, PAUSE, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_D4, NOTE_G4 
};

int pauseBetweenNotes = 80;
int noteDuration_ms = 150;

/* FUNCTION: playMart
 * Plays a melody defined in an array of notes
 * Also displays visual feedback on LEDs during playback
 * 
 * Parameters:
 * - notes: Array of note frequencies (including pauses)
 */
void playMart(int notes[])
{

  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote <= 11; thisNote++) {

    displayBinrayValue(notes[thisNote],pixels.Color(50,0,50));
    pixels.show(); // This sends the updated pixel color to the hardware.
    playSound( notes[thisNote], noteDuration_ms);
    setColorAllPixel(0); // pixels off
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    delay(pauseBetweenNotes);
        
  }
  
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* the setup routine runs once when you start the tape or press reset
========================================================================================================================
   _________            _                   
  | setup() |   ___ ___| |_ _ _ _ _ 
  |  o___o  |  |_ -| -_|  _| | | . |
  |__/_____|  |___|___|_| |___|  _|
                               |_|    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/* FUNCTION: setup
 * Initialization function that runs once at startup
 * 
 * - Configures pins
 * - Initializes NeoPixels
 * - Performs startup animation with rainbow effect
 * - Plays welcome melody
 */
void setup(void) {

  pinMode(SPEAKERPIN, OUTPUT);
  uint8_t brightness = hell;
  pixels.begin();
  pixels.setBrightness(brightness);
  
  rainbowCycle(5,2,16);
  
  for ( int i = brightness; i > 0; i--) {         
    pixels.setBrightness(i);
    pixels.show();
    delay (5);
  }
  delay(100);
  
  setColorAllPixel(0);
  pixels.setBrightness(brightness);
  pixels.show(); // Initialize all pixels to 'off'
    
  playMart(c);
  
  /* 
  for ( int i = 0; i < 1; i++) { 
    setColorAllPixel(Wheel(220));       // all pixels set to pink using the Wheel() function to choose a color from the spectrum 0-255
    
    for ( int i = 0; i < brightness; i++) {  
      for (int n = 0; n < NUMPIXELS; n++){       
        pixels.setPixelColor(n,i,0,i);
      }
    pixels.show();
    delay (3);
  }
  for ( int i = brightness; i > 0; i--) {         
    for (int n = 0; n < NUMPIXELS; n++){       
      pixels.setPixelColor(n,i,0,i);
    }
    pixels.show();
    delay (3);
  }
  delay(300);
  }
  delay(500);
  */
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* The main loop to put all your code
========================================================================================================================
   _________    _    
  | loop()  |  | |___ ___ ___ 
  |  o___o  |  | | . | . | . | 
  |__/___\__|  |_|___|___|  _| 
                         |_| 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

/* FUNCTION: loop
 * Main program loop that runs continuously
 * 
 * - Searches for DS18xxx temperature sensor
 * - Reads temperature values
 * - Updates LED display based on temperature
 * - Triggers audio alerts when temperature is below warning threshold
 * 
 * Temperature Color Mapping:
 * - Cold temperatures: Blue shades
 * - Medium temperatures: Green shades
 * - High temperatures: Red/Yellow shades
 * 
 * When temperature falls below warnTemp (5°C):
 * - LEDs will display current temperature
 * - Warning beeps will sound at intervals defined by warnAlarm
 */
void loop(void) {

  if ( !ds.search(addr)) {

    ds.reset_search();
    
    pixels.setPixelColor(0, Wheel(statusLED));
    pixels.show();
    delay(updateSpeed>>3);
    return;
  }

  /*
  if (OneWire::crc8(addr, 7) != addr[7]) {
      return;
  }
  */
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end

  delay(updateSpeed);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  pixels.setPixelColor(0, 0);
  pixels.show();
  delay(updateSpeed>>3);
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad
  
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }
  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  int16_t recalc =  raw / 16;
  
  tempColor = map(celsius, lowTemp, maxTemp, 160, 1);
  if (tempColor < 10) tempColor = 10;
  if (tempColor > 180) tempColor = 180;
  for (int n = 0; n <= 7; n++) {
      pixels.setPixelColor(n, Wheel(tempColor));
    }
  pixels.show();

  showPixel = map(recalc, lowTemp, maxTemp, 7, 1);
  if (showPixel < 1) showPixel = 1;
  if (showPixel > 7) showPixel = 7;
  pixels.setPixelColor(showPixel, 255,255,255);
  pixels.show();
  /*
  for ( int i = 0; i < blinkeSpeed; i++) { 
    pixels.setPixelColor(showPixel, 0,0,0);
    pixels.show();
    delay(updateSpeed/blinkeSpeed/2);
    pixels.setPixelColor(showPixel, 255,255,255);
    pixels.show();
    delay(updateSpeed/blinkeSpeed/2);
  }
  */
  
  if (celsius < warnTemp){
    warnCount++;
    if (warnCount % warnAlarm == 0){
      playSound( 400, 10);
    }
  }
  if (celsius < warnTemp + 5){
    warnCount++;
    if (warnCount % warnAlarm>>2 == 0){
      playSound( 1200, 100);
    }
  }

}
