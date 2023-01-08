/*
 * Swicth_Press_Release
 * 
 * A sketch to demonstrate how to modify Blink to use the IoAbstraction
 * library to control the MCP23017 and PCF8574 Digital Expanders. 
 * 
 * TaskManagerIO is implemented to enable the use of push button switches.
 * This results in the loop() being near empty, with the code moved to fucntions
 * 
 * Note that the expanders require seperate I2C addresses. On the PCF8574, 
 * change the A0 jumper from J2 to J4 to set the address to 0x21.
 * 
 * https://github.com/davetcc/IoAbstraction
 */
 
#include <IoAbstraction.h>
#include <IoAbstractionWire.h>
#include <TaskManagerIO.h>
//#include <Wire.h>

#define EXPANDER 100

#define MCP23017_LED 104
#define PCF8574_LED 124

#define MCP23017_SWITCH 103
#define PCF8574_SWITCH  123

// create a multi Io that allocates the first 100 pins to arduino pins
MultiIoAbstraction multiIo(EXPANDER);

// create an IO abstractions for the MCP23017 and the PCF8574
MCP23017IoAbstraction mcp23017(0x20);
PCF8574IoAbstraction pcf8574(0x21, IO_PIN_NOT_DEFINED);

void setup() {
  // Initialise wire to enable the I2C connection
  Wire.begin();

  // add the mcp23017 allocating 20 more pins, from 100..119
  // then add the pcf8574 allocating 10 more pins, from 120..129
  multiIo.addIoDevice(mcp23017, 20);
  multiIo.addIoDevice(pcf8574, 10);

  // set up the led pins as outputs on each device
  multiIo.pinMode(LED_BUILTIN,  OUTPUT);
  multiIo.pinMode(MCP23017_LED, OUTPUT);
  multiIo.pinMode(PCF8574_LED,  OUTPUT);

  // set up the button the expander. we choose poll everything but you could
  // also SWITCHES_POLL_KEYS_ONLY and SWITCHES_NO_POLLING for interrupt mode.
  switches.init(asIoRef(multiIo), SWITCHES_POLL_EVERYTHING, true);
  
  switches.addSwitch(MCP23017_SWITCH, switchOnLED);
  switches.onRelease(MCP23017_SWITCH, switchOffLED);
  
  switches.addSwitch(PCF8574_SWITCH, switchOnLED);
  switches.onRelease(PCF8574_SWITCH, switchOffLED);
}

void switchOnLED(byte key, bool held) {
  multiIo.digitalWrite(LED_BUILTIN,  HIGH);
  multiIo.digitalWrite(MCP23017_LED, HIGH);
  multiIo.digitalWrite(PCF8574_LED,  HIGH);
}

void switchOffLED(byte key, bool held) {
  multiIo.digitalWrite(LED_BUILTIN,  LOW);
  multiIo.digitalWrite(MCP23017_LED, LOW);
  multiIo.digitalWrite(PCF8574_LED,  LOW);
} 

void loop() {
  taskManager.runLoop();
}
