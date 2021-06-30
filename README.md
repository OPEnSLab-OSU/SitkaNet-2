# SitkaNet-2.1

Version 2.1 of SitkaNet

Project Wiki:
https://github.com/OPEnSLab-OSU/OPEnS-Lab-Home/wiki/SitkaNet

Built using LOOM:
https://github.com/OPEnSLab-OSU/Loom

Arduino SAMD 1.8.11
Adafruit SAMD 1.5.7
Arduino IDE 1.8.15
Loom SAMD 2.5.1 (with 2 files modified: LoRa.cpp, GoogleSheets.cpp)

To modify for each node (VER 1, VER 2, GAV 3, GAV 4, HAR 5, HAR 6)
1) Change Node/config.h lines 4 (name) and 5 (instance) --> sets Google Sheet sheet 
2) Change Node/config.h line 54 to match line 5 --> this is the LoRa address

To modify sampling time period
1) Change values in Node.ino line 201: Loom.InterruptManager().RTC_alarm_duration(TimeSpan(0,0,2,0));
	where (0,0,2,0) is 0 days, 0 hours, 2 minutes, 0 seconds