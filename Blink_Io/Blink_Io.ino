/*
 * Blink_Io
 * 
 * A sketch to demonstrate how to modify Blink to use the IoAbstraction
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

#define MCP23017_LED 4
#define PCF8574_LED  4

// create an IO abstractions for the Arduino, MCP23017 and PCF8574
BasicIoAbstraction arduino = internalDigitalDevice();
MCP23017IoAbstraction mcp23017(0x20);
PCF8574IoAbstraction pcf8574(0x21, IO_PIN_NOT_DEFINED);

void setup() {
  // Initialise wire to enable the I2C connection
  Wire.begin();

  // set up the led pins as outputs on each device
  arduino.pinMode(LED_BUILTIN, OUTPUT);
  mcp23017.pinMode(MCP23017_LED, OUTPUT);
  pcf8574.pinMode(PCF8574_LED, OUTPUT);
}

void loop() {
  arduino.digitalWrite(LED_BUILTIN, HIGH);  
  mcp23017.digitalWrite(MCP23017_LED, HIGH);  
  pcf8574.digitalWrite(PCF8574_LED, HIGH);
  
  // sync the expanders after write and before delay
  mcp23017.sync();
  pcf8574.sync();
  delay(1000);
  
  arduino.digitalWrite(LED_BUILTIN, LOW);
  mcp23017.digitalWrite(MCP23017_LED, LOW); 
  pcf8574.digitalWrite(PCF8574_LED, LOW);

  // sync the expanders after write and before delay
  mcp23017.sync();
  pcf8574.sync();
  delay(1000); 
}
