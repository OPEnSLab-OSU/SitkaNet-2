///////////////////////////////////////////////////////////////////////////////

// This is a basic example that demonstrates usage of the Hypnos board.

// The Hypnos board includes
// - SD
// - DS3231 RTC
// - Ability to power of peripherals

// Further details about the Hypnos board can be found here:
// https://github.com/OPEnSLab-OSU/OPEnS-Lab-Home/wiki/Hypnos

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#include "SDI12.h"
//#include <EnableInterrupt.h>
#include "wiring_private.h" // pinPeripheral() function

// Include configuration
const char* json_config = 
#include "config.h"
;

// Set enabled modules
LoomFactory<
	Enable::Internet::Disabled,
	Enable::Sensors::Enabled,
	Enable::Radios::Enabled,
	Enable::Actuators::Enabled,
	Enable::Max::Enabled
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };

#define SDI_PIN 11
#define ACCEL_INT_PIN 18
#define TIP_INT_PIN 19

/* function declarations */ 
void configInterrupts(Adafruit_MMA8451 device);
void mmaPrintIntSRC(uint8_t dataRead);
void mmaSetupSlideSentinel();

byte charToDec(char i);
void printInfo(char i);
void printBufferToScreen();
void takeMeasurement(char i);
boolean checkActive(char i);
boolean isTaken(byte i);
boolean setTaken(byte i);

/* global variable declarations */ 
//////////////////////////////////////////////////////////
Adafruit_MMA8451 mma = Adafruit_MMA8451();
bool accelFlag = false;
sensors_event_t event; 

SDI12 mySDI12(SDI_PIN);
String buffer = "";
int terosCounter = 1;

// keeps track of active addresses
// each bit represents an address:
// 1 is active (taken), 0 is inactive (available)
// setTaken('A') will set the proper bit for sensor 'A'
byte addressRegister[8] = {
  0B00000000,
  0B00000000,
  0B00000000,
  0B00000000,
  0B00000000,
  0B00000000,
  0B00000000,
  0B00000000
};

int tipCount = 0;

unsigned long lastInterruptTime = 0;
unsigned long interruptTime = 0;

//////////////////////////////////////////////////////////
//MMA Accelerometer ISR
void wakeUpAccel()
{
  detachInterrupt(digitalPinToInterrupt(ACCEL_INT_PIN));
  accelFlag = true;
}

//Tipping Bucket ISR
void wakeUpTip()
{
  //detachInterrupt(digitalPinToInterrupt(ACCEL_INT_PIN));
  interruptTime = millis();
  if (interruptTime - lastInterruptTime > 200)
  {
    tipCount++;
  }
  lastInterruptTime = interruptTime;
}

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
	// Needs to be done for Hypno Board
	pinMode(5, OUTPUT);		// Enable control of 3.3V rail 

	//See Above
	digitalWrite(5, LOW);	// Enable 3.3V rail

	Loom.begin_serial(true);
	Loom.parse_config(json_config);
	Loom.print_config();
  
  mySDI12.begin();
  delay(500); // allow things to settle

  Serial.println("Scanning all addresses, please wait...");
  //Quickly Scan the Address Space

  for(byte i = '0'; i <= '9'; i++) if(checkActive(i)) setTaken(i);   // scan address space 0-9

  //See if there are any active sensors
  boolean found = false;

  for(byte i = 0; i < 10; i++){
    if(isTaken(i)){
      found = true;
      break;
    }
  }

  mmaSetupSlideSentinel();
  
  accelFlag = false;
  configInterrupts(mma);

  digitalWrite(ACCEL_INT_PIN, INPUT_PULLUP); // pullup interrupt
  attachInterrupt(digitalPinToInterrupt(ACCEL_INT_PIN), wakeUpAccel, LOW);
  Serial.println("MMA Interrupt attached");

  digitalWrite(TIP_INT_PIN, INPUT_PULLUP); // pullup interrupt
  attachInterrupt(digitalPinToInterrupt(TIP_INT_PIN), wakeUpTip, FALLING);
  Serial.println("Tip Interrupt attached");  
  
  mma.readRegister8(MMA8451_REG_TRANSIENT_SRC); //clear the interrupt register

	LPrintln("\n ** Setup Complete ** ");
}


void loop() 
{
  // perform any bigger interrupt related actions here, this will just print some info to show what interrupted the accel
  if(accelFlag){
      Serial.println("Interrupt triggered");   
      uint8_t dataRead = mma.readRegister8(MMA8451_REG_TRANSIENT_SRC); //clear the interrupt register
      mmaPrintIntSRC(dataRead);

      digitalWrite(ACCEL_INT_PIN, INPUT_PULLUP);
      accelFlag = false; // reset flag, clear the interrupt
      // reattach the interrupt, can be done anywhere in code, but only after the interrupt has triggered and detached
      //EIC->INTFLAG.reg = 1 << ACCEL_INT_PIN; // clear interrupt flag pending
      EIC->INTFLAG.reg = 0x01ff; // clear interrupt flag pending
      attachInterrupt(digitalPinToInterrupt(ACCEL_INT_PIN), wakeUpAccel, LOW);
  }
  
  digitalWrite(LED_BUILTIN, HIGH);

	Loom.measure();
	Loom.package();
   // scan address space 0-9
  for(char i = '0'; i <= '9'; i++) if(isTaken(i)){
    printInfo(i);
    Serial.print("\t");
    takeMeasurement(i);
    Serial.println();
  }
  Loom.add_tip_count(tipCount);
	Loom.display_data();
	// Log using default filename as provided in configuration
	// in this case, 'datafile.csv'
	Loom.SDCARD().log();

  // Send to address 1
  //Loom.LoRa().send(1);

  digitalWrite(LED_BUILTIN, LOW);
  delay(300000);
	Loom.pause();	 
}

// comment/uncomment these to enable functionality described
/* Transient detection donfiguration for mma accelerometer, use this format and Adafruit_MMA8451::writeRegister8_public to configure registers */
void configInterrupts(Adafruit_MMA8451 device){
    uint8_t dataToWrite = 0;

    // MMA8451_REG_CTRL_REG3
    // Interrupt control register

    //dataToWrite |= 0x80;    // FIFO gate option for wake/sleep transition, default 0
    dataToWrite |= 0x40;    // Wake from transient interrupt enable
    //dataToWrite |= 0x20;    // Wake from orientation interrupt enable
    //dataToWrite |= 0x10;    // Wake from Pulse function enable
    //dataToWrite |= 0x08;    // Wake from freefall/motion decect interrupt
    //dataToWrite |= 0x02;    // Interrupt polarity 1 = active high
    dataToWrite |= 0x01;    // (0) Push/pull or (1) open drain interrupt, determines whether bus is driven by device, or left to hang

    device.writeRegister8_public(MMA8451_REG_CTRL_REG3, dataToWrite);

    dataToWrite = 0;

    // MMA8451_REG_CTRL_REG4
    // Interrupt enable register, enables interrupts that are not commented

    //dataToWrite |= 0x80;    // Auto sleep/wake interrupt
    //dataToWrite |= 0x40;    // FIFO interrupt
    dataToWrite |= 0x20;    // Transient interrupt - enabled
    //dataToWrite |= 0x10;    // orientation
    //dataToWrite |= 0x08;    // Pulse interrupt
    //dataToWrite |= 0x04;    // Freefall interrupt
    dataToWrite |= 0x01;    // data ready interrupt, MUST BE ENABLED FOR USE WITH ARDUINO
    device.writeRegister8_public(MMA8451_REG_CTRL_REG4, dataToWrite | 0x01);
    
    dataToWrite = 0;

    // MMA8451_REG_CTRL_REG5
    // Interrupt pin configuration register, select interrupt on pin 0 or pin 1
    // see datasheet for register's description, ths int routed to pin 2

    device.writeRegister8_public(MMA8451_REG_CTRL_REG5, dataToWrite | 0x01);

    dataToWrite = 0;
    
    // MMA8451_REG_TRANSIENT_CFG
    dataToWrite |= 0x10;  // Latch enable to capture accel values when interrupt occurs
    dataToWrite |= 0x08;    // Z transient interrupt enable
    dataToWrite |= 0x04;    // Y transient interrupt enable
    dataToWrite |= 0x02;    // X transient interrupt enable
    //dataToWrite |= 0x01;    // High-pass filter bypass
    device.writeRegister8_public(MMA8451_REG_TRANSIENT_CFG, dataToWrite);

    Serial.print("MMA8451_REG_TRANSIENT_CFG: ");
    Serial.println(device.readRegister8(MMA8451_REG_TRANSIENT_CFG), HEX);
    
    dataToWrite = 0;

    // MMA8451_REG_TRANSIENT_THS
    // Transient interrupt threshold in units of .06g
    //Acceptable range is 1-127
    dataToWrite = 0x20; //2g threshold
    device.writeRegister8_public(MMA8451_REG_TRANSIENT_THS, dataToWrite);

    dataToWrite = 0;

    // MMA8451_REG_TRANSIENT_CT  0x20
    dataToWrite = 0; // value is 0-255 for numer of counts to debounce for, depends on ODR
    device.writeRegister8_public(MMA8451_REG_TRANSIENT_CT, dataToWrite);

    dataToWrite = 0;    
}

/* Setup for mma use with Slide Sentinel, other use cases will be pretty similar */
void mmaSetupSlideSentinel(){
  if (! mma.begin()) {
    Serial.println("Couldnt start");
    //while (1); 
  }
  
  Serial.println("MMA8451 found!");

  // library configurations
  mma.setRange(MMA8451_RANGE_2_G);
  mma.setDataRate(MMA8451_DATARATE_6_25HZ);
  Serial.print("Range = "); Serial.print(2 << mma.getRange()); Serial.println("G");

  while (mma.readRegister8(MMA8451_REG_CTRL_REG2) & 0x40);
}


void mmaPrintIntSRC(uint8_t dataRead){
    if(dataRead & 0x40) Serial.println("Event Active");
    if(dataRead & 0x20){
        Serial.println("\tZ event");
        if(dataRead & 0x10) Serial.println("\t\tZ Negative g");
        else Serial.println("\t\tZ Positive g");
    }
    if(dataRead & 0x08){
        Serial.println("\tY event");
        if(dataRead & 0x04) Serial.println("\t\tY Negative g");
        else Serial.println("\t\tY Positive g");
    }
    if(dataRead & 0x02){
        Serial.println("\tX event");
        if(dataRead & 0x01) Serial.println("\t\tX Negative g");
        else Serial.println("\t\tX Positive g");
    }
}

// converts allowable address characters '0'-'9', 'a'-'z', 'A'-'Z',
// to a decimal number between 0 and 61 (inclusive) to cover the 62 possible addresses
byte charToDec(char i){
  if((i >= '0') && (i <= '9')) return i - '0';
//  if((i >= 'a') && (i <= 'z')) return i - 'a' + 10;
//  if((i >= 'A') && (i <= 'Z')) return i - 'A' + 37;
  else return i;
}

// gets identification information from a sensor, and prints it to the serial port
// expects a character between '0'-'9', 'a'-'z', or 'A'-'Z'.
void printInfo(char i){
  int j;
  String command = "";
  command += (char) i;
  command += "I!";
  for(j = 0; j < 1; j++){
    mySDI12.sendCommand(command);
    delay(30);
    if(mySDI12.available()>1) break;
    if(mySDI12.available()) mySDI12.read();
  }

  while(mySDI12.available()){
    char c = mySDI12.read();
    if((c!='\n') && (c!='\r')) Serial.write(c);
    delay(5);
  }
}

void printBufferToScreen(){
  mySDI12.read(); // consume address
  mySDI12.read(); // consume address
  while(mySDI12.available()){
    char c = mySDI12.read();
    if(c == '+' || c == '-'){
      buffer += ',';
      if(c == '-') buffer += '-';
    }
    else if ((c != '\n') && (c != '\r')) {
      buffer += c;
    }
    delay(50);
  }
  
  if (terosCounter == 1) 
  {
    Loom.add_teros1(buffer);
    terosCounter = 2;
  }
  else if (terosCounter == 2) 
  {
    Loom.add_teros2(buffer);
    terosCounter = 1;
  }
  Serial.print(buffer);
}

void takeMeasurement(char i){
  String command = "";
  command += i;
  command += "M!"; // SDI-12 measurement command format  [address]['M'][!]
  mySDI12.sendCommand(command);
  // wait for acknowlegement with format [address][ttt (3 char, seconds)][number of measurments available, 0-9]
  String sdiResponse = "";
  delay(30);
  while (mySDI12.available())  // build response string
  {
    char c = mySDI12.read();
    if ((c != '\n') && (c != '\r'))
    {
      sdiResponse += c;
      delay(5);
    }
  }
  mySDI12.clearBuffer();

  // find out how long we have to wait (in seconds).
  unsigned int wait = 0;
  wait = sdiResponse.substring(1,4).toInt();

  // Set up the number of results to expect
  // int numMeasurements =  sdiResponse.substring(4,5).toInt();

  unsigned long timerStart = millis();
  while((millis() - timerStart) < (1000 * wait)){
    if(mySDI12.available())  // sensor can interrupt us to let us know it is done early
    {
      mySDI12.clearBuffer();
      break;
    }
  }
  // Wait for anything else and clear it out
  delay(30);
  mySDI12.clearBuffer();

  // in this example we will only take the 'DO' measurement
  command = "";
  command += i;
  command += "D0!"; // SDI-12 command to get data [address][D][dataOption][!]
  mySDI12.sendCommand(command);
  while(!mySDI12.available()>1); // wait for acknowlegement
  delay(300); // let the data transfer
  printBufferToScreen();
  mySDI12.clearBuffer(); 
  buffer = "";
}

// this checks for activity at a particular address
// expects a char, '0'-'9', 'a'-'z', or 'A'-'Z'
boolean checkActive(char i){

  String myCommand = "";
  myCommand = "";
  myCommand += (char) i;                 // sends basic 'acknowledge' command [address][!]
  myCommand += "!";

  for(int j = 0; j < 3; j++){            // goes through three rapid contact attempts
    mySDI12.sendCommand(myCommand);
    if(mySDI12.available()>1) break;
    delay(30);
  }
  if(mySDI12.available()>2){       // if it hears anything it assumes the address is occupied
    mySDI12.clearBuffer();
    return true;
  }
  else {   // otherwise it is vacant.
    mySDI12.clearBuffer();
  }
  return false;
}

// this quickly checks if the address has already been taken by an active sensor
boolean isTaken(byte i){
  i = charToDec(i); // e.g. convert '0' to 0, 'a' to 10, 'Z' to 61.
  byte j = i / 8;   // byte #
  byte k = i % 8;   // bit #
  return addressRegister[j] & (1<<k); // return bit status
}

// this sets the bit in the proper location within the addressRegister
// to record that the sensor is active and the address is taken.
boolean setTaken(byte i){
  boolean initStatus = isTaken(i);
  i = charToDec(i); // e.g. convert '0' to 0, 'a' to 10, 'Z' to 61.
  byte j = i / 8;   // byte #
  byte k = i % 8;   // bit #
  addressRegister[j] |= (1 << k);
  return !initStatus; // return false if already taken
}
