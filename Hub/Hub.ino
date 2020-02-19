///////////////////////////////////////////////////////////////////////////////

// This is a basic example of receiving data via LoRa. 
// This particular example blocks while waiting for data
// While you can do a variety of things with the received data, this example
// simply prints it and logs it to an SD Card 

// The corresponding example is LoRa > Transmit

// See https://openslab-osu.github.io/Loom/html/class_loom___lo_ra.html
// for details of LoRa config options

// There is a similar receiving example that, unlike this one, will not
// block / wait for a packet before continuing, that example is 
// LoRa > Receive

// The maximum time to wait for a packet is provided in milliseconds, 
// 5000 in this case

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>
#include "SitkaNetJSON.h"

// Include configuration
const char* json_config = 
#include "config.h"
;

// Set enabled modules
LoomFactory<
	Enable::Internet::All,
	Enable::Sensors::Enabled,
	Enable::Radios::Enabled,
	Enable::Actuators::Disabled,
	Enable::Max::Disabled
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };



void setup() 
{ 
	Loom.begin_serial();
	Loom.parse_config(json_config);
	Loom.print_config();

	LPrintln("\n ** Setup Complete ** ");
}

void loop() 
{
  SitkaNet_t in_data;
	if (Loom.LoRa().receive_blocking_raw(in_data.raw, sizeof(in_data.raw), 5000)) {
    JsonObject internal_json = Loom.internal_json(true);
    struct_to_json(in_data, internal_json);
		Loom.display_data();
		Loom.GoogleSheets().publish();
	}
}
