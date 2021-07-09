#include <Loom.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "SDI12.h"
#include "wiring_private.h" // pinPeripheral() function
#include "FeatherFault.h"

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

//////////////////////////////////////////////////////////
#define DELAY_IN_MINUTES 15
#define DELAY_IN_SECONDS 0
#define HUB_LORA_ADDRESS 3						  
#define SDI_PIN 11
#define RTC_INT_PIN 12
#define TIP_INT_PIN 0

//////////////////////////////////////////////////////////
/* function declarations */ 
void clear_alarms();
byte charToDec(char i);
void printInfo(char i);
void printBufferToScreen();
void takeMeasurement(char i);
boolean checkActive(char i);
boolean isTaken(byte i);
boolean setTaken(byte i);
volatile bool rtc_flag = false;

//////////////////////////////////////////////////////////
/* global variable declarations */ 

SDI12 mySDI12(SDI_PIN);
String buffer = "";
int terosCounter = 1;

volatile bool tipFlag = false;

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

//Tipping Bucket ISR
void wakeUpTip()
{
  detachInterrupt(TIP_INT_PIN);
  interruptTime = millis();
  if (interruptTime - lastInterruptTime > 200)
  {
    tipFlag = true;
    tipCount++;
    lastInterruptTime = interruptTime;
  }
}

//RTC Wake Interrupt
void wakeUpRTC()
{
  detachInterrupt(RTC_INT_PIN);
  rtc_flag = true;
}
//////////////////////////////////////////////////////////

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RTC_INT_PIN, INPUT_PULLUP);
  pinMode(TIP_INT_PIN, INPUT_PULLUP);
  MARK;
  // Needs to be done for Hypnos Board
  pinMode(5, OUTPUT);   // Enable control of 3.3V rail 
  pinMode(6, OUTPUT);   // Enable control of 5V rail 
  MARK;
  //See Above
  digitalWrite(5, LOW); // Enable 3.3V rail
  digitalWrite(6, HIGH); // Enable 5V rail
  MARK;
  Loom.begin_serial(true);
  Loom.parse_config(json_config);
  Loom.print_config();
  MARK;
	mySDI12.begin();			  
  delay(500); // allow things to settle
  MARK;
  Serial.println("Scanning all addresses, please wait...");
  //Quickly Scan the Address Space
  MARK;
  for(byte i = 'A'; i <= 'D'; i++) if(checkActive(i)) setTaken(i);   // scan address space A-Z
  MARK;
  //See if there are any active sensors
  boolean found = false;
  MARK;
  for(byte i = 0; i < 10; i++){
    if(isTaken(i)){
      found = true;
      break;
    }
  }
  MARK;
  Loom.InterruptManager().register_ISR(TIP_INT_PIN, wakeUpTip, LOW, ISR_Type::IMMEDIATE);
  Loom.InterruptManager().register_ISR(RTC_INT_PIN, wakeUpRTC, LOW, ISR_Type::IMMEDIATE);
  MARK;
  LPrintln("\n ** Setup Complete ** ");
  MARK;
}

void loop() 
{  
  if (tipFlag)
  {
    tipFlag = false;
    MARK;
    Loom.InterruptManager().reconnect_interrupt(TIP_INT_PIN);
    MARK;
  }
  else 
  { 
    MARK;        
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(5, LOW); // Enable 3.3V rail
    digitalWrite(6, HIGH);
    
    pinMode(10, OUTPUT); //Enable SD card pins
    pinMode(23, OUTPUT);
    pinMode(24, OUTPUT);
    MARK;
    Loom.DS3231().clear_alarms();
    MARK;
    Loom.power_up();
    MARK;
    Loom.measure();
    Loom.package();
    delay(100);
    MARK;
     // scan address space 0-9
    for(char i = 'A'; i <= 'D'; i++) if(isTaken(i)){
      printInfo(i);
      Serial.print("\t");
      takeMeasurement(i);
      Serial.println();
    }
    MARK;
    Loom.add_data("Tip", "Count", tipCount);
    Loom.add_data("rssi", "value", Loom.LoRa().get_signal_strength());
    Loom.display_data();
    // Log using default filename as provided in configuration
	if(Loom.SDCARD().get_active()){					  
		Loom.SDCARD().log();
	}
		else{
          Serial.println("NO SD CARD FOUND");
    } 
    MARK;

    // Send to hub lora address HUB_LORA_ADDRESS
    Loom.LoRa().send(HUB_LORA_ADDRESS);
    MARK;
  }
														   
  //Go to sleep if accelerometer is not triggered
  MARK;
  Loom.InterruptManager().RTC_alarm_duration(TimeSpan(0, 0, DELAY_IN_MINUTES, DELAY_IN_SECONDS));
  MARK;
  Loom.InterruptManager().reconnect_interrupt(RTC_INT_PIN);
  MARK;
  Loom.InterruptManager().reconnect_interrupt(TIP_INT_PIN);

  Loom.power_down();
  
  digitalWrite(LED_BUILTIN, LOW);
  
  rtc_flag = false;
  
  pinMode(23, INPUT); //Disable SD card pins to prevent current leak
  pinMode(24, INPUT);
  pinMode(10, INPUT);

  digitalWrite(5, HIGH); // Turn off 3.3V rail
  digitalWrite(6, LOW);

    Loom.SleepManager().sleep();
    while (!rtc_flag);
  MARK;
}
//////////////////////////////////////////////////////////
// comment/uncomment these to enable functionality described
/* Transient detection donfiguration for mma accelerometer, use this format and Adafruit_MMA8451::writeRegister8_public to configure registers */

// converts allowable address characters '0'-'9', 'a'-'z', 'A'-'Z',
// to a decimal number between 0 and 61 (inclusive) to cover the 62 possible addresses
byte charToDec(char i){
  if((i >= '0') && (i <= '9')) return i - '0';
  if((i >= 'a') && (i <= 'z')) return i - 'a' + 10;
  if((i >= 'A') && (i <= 'Z')) return i - 'A' + 37;
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
  
  char * strtokIndx; // this is used by strtok() as an index
  float moisture; //Teros soil moisture
  float temp; //Teros soil temperature 
  char buf[16];
  if (terosCounter == 1) 
  {
    buffer.toCharArray(buf, 16);
    strtokIndx = strtok(buf,",");      // get the first part - the string
    moisture = atof(strtokIndx);     // convert this part to a float
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    temp = atof(strtokIndx);     // convert this part to an integer

    Loom.add_data("T_AM", "M", moisture);
    Loom.add_data("T_AT", "T", temp);    
//    Loom.add_data("T_A", "M", buffer);
    terosCounter = 2;
  }
  else if (terosCounter == 2) 
  {
    buffer.toCharArray(buf, 16);
    strtokIndx = strtok(buf,",");      // get the first part - the string
    moisture = atof(strtokIndx);     // convert this part to a float
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    temp = atof(strtokIndx);     // convert this part to an integer

    Loom.add_data("T_BM", "M", moisture);
    Loom.add_data("T_BT", "T", temp); 
//    Loom.add_data("T_B", "M", buffer);
    terosCounter = 3;
  }
  else if (terosCounter == 3) 
  {
    buffer.toCharArray(buf, 16);
    strtokIndx = strtok(buf,",");      // get the first part - the string
    moisture = atof(strtokIndx);     // convert this part to a float
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    temp = atof(strtokIndx);     // convert this part to an integer

    Loom.add_data("T_CM", "M", moisture);
    Loom.add_data("T_CT", "T", temp); 
//    Loom.add_data("T_B", "M", buffer);
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
