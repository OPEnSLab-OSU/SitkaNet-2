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
//#include "SitkaNetJSON.h"
//#include "FeatherFault.h"

// Include configuration
const char* json_config = 
#include "config.h"
;

// Set enabled modules
LoomFactory<
	Enable::Internet::Ethernet,
	Enable::Sensors::Disabled,
	Enable::Radios::Enabled,
	Enable::Actuators::Disabled,
	Enable::Max::Disabled
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };

void setup() 
{ /*
	if (FeatherFault::DidFault()) {
        // perform cleanup here
  } 
  Serial.begin(9600);
//  while(!Serial)
  FeatherFault::PrintFault(Serial);
  Serial.flush();
  FeatherFault::StartWDT(FeatherFault::WDTTimeout::WDT_8S);
  MARK;
  */

  // for Hypnos with ethernet stacked on sensor rail
  //pinMode(5, OUTPUT);    // Enable control of 3.3V rail
  //pinMode(6, OUTPUT);   // Enable control of 5V rail
  //digitalWrite(5, LOW); // Enable 3.3V rail
  //digitalWrite(6, HIGH);  // Enable 5V rail
  
	Loom.begin_serial();
	 // MARK;
	Loom.parse_config(json_config);
	 // MARK;
	Loom.print_config();
	 // MARK;

	LPrintln("\n ** Setup Complete ** ");
	 // MARK;
}

void loop() 
{ 
	if (Loom.LoRa().receive_blocking(10000)) {
  // MARK;
		Loom.display_data();
/*  if(Loom.SDCARD().get_active()){
          Loom.SDCARD().log();
    }
    else{
          Serial.println("NO SD CARD FOUND");
    }*/
	//	MARK;
    if(!Loom.GoogleSheets().publish()) {
          Serial.println("Failed to print to Gsheets");
   // MARK;
    }
  }
}
