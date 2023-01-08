/*
 * Blink_Io
 * 
 * A sketch to demonstrate how to use the IoAbstraction and TaskManagerIO
 * libraries to control the MCP23017 and PCF8574 Digital Expanders.
 * This version uses multi IO Abstraction so the pin numbers are unique.
 * TaskManagerIO handles the input switches which are controlling the
 * output switches
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
#define MCP23017_PIN 103
#define PCF8574_PIN 123

// create a multi Io that allocates the first 100 pins to arduino pins
MultiIoAbstraction multiIo(EXPANDER);

// create an IO abstractions for the MCP23017 and the PCF8574
MCP23017IoAbstraction mcp23017(0x20);
PCF8574IoAbstraction  pcf8574(0x21, IO_PIN_NOT_DEFINED);

// when the switch is pressed then this function will be called.
void onSwitchPress(uint8_t key, bool held) {
  // toggle the state of the built in LED and LEDs on the expanders.
  uint8_t ledState = multiIo.digitalRead(LED_BUILTIN);

  multiIo.digitalWrite(LED_BUILTIN,  !ledState);
  
  if (key == MCP23017_PIN) {
    ledState = multiIo.digitalRead(MCP23017_LED);
    multiIo.digitalWrite(MCP23017_LED, !ledState);
  }
  if (key == PCF8574_PIN) {
    ledState = multiIo.digitalRead(PCF8574_LED);
    multiIo.digitalWrite(PCF8574_LED,  !ledState);
  }

  Serial.print("Switch ");
  Serial.print(key);
  Serial.println(held ? " Held down" : " Pressed");
}

void setup() {
  // Initialise wire to enable the I2C connection
  Wire.begin();
 
  // add the mcp23017 allocating 20 more pins, from 100..119
  // then add the pcf8574 allocating 10 more pins, from 120..129
  // more expanders can be added when further pins are required
  multiIo.addIoDevice(mcp23017, 20);
  multiIo.addIoDevice(pcf8574, 10);

  // set up the led pins as outputs on each device
  multiIo.pinMode(LED_BUILTIN, OUTPUT);
  multiIo.pinMode(MCP23017_LED, OUTPUT);
  multiIo.pinMode(PCF8574_LED, OUTPUT);

  // set up the button the expander. we choose poll everything but you could
  // also SWITCHES_POLL_KEYS_ONLY and SWITCHES_NO_POLLING for interrupt mode.
  switches.init(asIoRef(multiIo), SWITCHES_POLL_EVERYTHING, true);
  switches.addSwitch(MCP23017_PIN, onSwitchPress);
  switches.addSwitch(PCF8574_PIN, onSwitchPress);

  Serial.begin(115200);
}

void loop() {
  taskManager.runLoop();
}
