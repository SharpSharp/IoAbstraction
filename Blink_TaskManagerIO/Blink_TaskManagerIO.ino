/*
 * Blink_TaskManagerIO
 * 
 * A sketch to demonstrate how to modify Blink to use the IoAbstraction
 * library to control the MCP23017 and PCF8574 Digital Expanders. 
 * 
 * TaskManagerIO is implemented to enable the removal of delay().
 * This results in the loop() being near empty, with the code moved 
 * to an invert LED fucntion
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
  // switchOnLED is scheduled to run every 2000 milliseconds.
  taskManager.scheduleFixedRate(2000,switchOnLED);
}
// a call back method that gets called once every 2 seconds from the schedule
void switchOnLED() {
  multiIo.digitalWrite(LED_BUILTIN, HIGH);
  multiIo.digitalWrite(MCP23017_LED, HIGH);
  multiIo.digitalWrite(PCF8574_LED, HIGH);
  multiIo.sync();
  taskManager.scheduleOnce(1000,switchOffLED);
}
// a call back method that gets called one second after switchOnLED
void switchOffLED(){
  multiIo.digitalWrite(LED_BUILTIN, LOW);
  multiIo.digitalWrite(MCP23017_LED, LOW);
  multiIo.digitalWrite(PCF8574_LED, LOW);
  multiIo.sync();
}

void loop() {
  taskManager.runLoop();
}
