///////////////////////////////////////////////////////////////////////////////

// This is a basic example of sending data via LoRa. 

// The corresponding example is LoRa > Receive (or Receive_Blocking)

// These two examples are the Loom equivalent of the basic RX / TX LoRa 
// examples

// See https://openslab-osu.github.io/Loom/html/class_loom___lo_ra.html
// for details of LoRa config options

// While you can send whatever data you collect, for the purposes of this 
// example, only analog data is being collected

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Adafruit_seesaw.h"

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

Adafruit_seesaw ss;

// Include configuration
const char* json_config = 
#include "config.h"
;

// Set enabled modules
LoomFactory<
	Enable::Internet::Disabled,
	Enable::Sensors::Enabled,
	Enable::Radios::Enabled,
	Enable::Actuators::Disabled,
	Enable::Max::Disabled
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };

bool ssFound = false;

void setup() 
{ 
	Loom.begin_serial(false);
	Loom.parse_config(json_config);
	Loom.print_config();

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  // text display tests
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  if (ss.begin(0x36)) {
    ssFound = true;
  }
	LPrintln("\n ** Setup Complete ** ");
}

void loop() 
{
	Loom.measure();
	Loom.package();
	Loom.display_data();

	// Send to address 1
	Loom.LoRa().send(3);

  display.clearDisplay();
  display.println(Loom.LoRa().get_rssi());
  display.print(Loom.LoRa().get_drop_rate());
  display.print(";");
  display.print(Loom.LoRa().get_last_ten_drop_rate());
  display.setCursor(0,0);
  display.display();

  Loom.pause();

  if (ssFound) {
    display.clearDisplay();
    display.print("T: ");
    display.print(ss.getTemp());
    display.println("*C");
    display.print("C: ");
    display.print(ss.touchRead(0));
    display.setCursor(0,0);
    display.display();
  }

	Loom.pause();	// Delay between interations set with 'interval' in config
}
