/*
 * Blink_Io
 * 
 * A skectch to demonstrate how to modify Blink to use the IoAbstraction
 * library to control the MCP23017 and PCF8574 Digital Expanders.
 * 
 * Note that the expanders require seperate I2C addresses. On the PCF8574, 
 * change the A0 jumper from J2 to J4 to set the address to 0x21.
 * 
 * https://github.com/davetcc/IoAbstraction
 */

#include <IoAbstraction.h>
#include <IoAbstractionWire.h>
//#include <Wire.h>

#define LED_PIN 4

BasicIoAbstraction ioBoard = internalDigitalDevice();
MCP23017IoAbstraction ioExpander0(0x20, IO_PIN_NOT_DEFINED, &Wire);
PCF8574IoAbstraction  ioExpander1(0x21, IO_PIN_NOT_DEFINED, &Wire);

void setup() {
  Wire.begin();
  ioBoard.pinMode(LED_BUILTIN, OUTPUT);
  ioExpander0.pinMode(LED_PIN, OUTPUT);
  ioExpander1.pinMode(LED_PIN, OUTPUT);
}

void loop() {
  ioBoard.digitalWrite(LED_BUILTIN, HIGH);  
  ioExpander0.digitalWrite(LED_PIN, HIGH);  
  ioExpander1.digitalWrite(LED_PIN, HIGH);
  ioExpander0.sync();
  ioExpander1.sync();
  delay(1000);
  
  ioBoard.digitalWrite(LED_BUILTIN, LOW);
  ioExpander0.digitalWrite(LED_PIN, LOW); 
  ioExpander1.digitalWrite(LED_PIN, LOW);
  ioExpander0.sync();
  ioExpander1.sync();
  delay(1000); 
}
