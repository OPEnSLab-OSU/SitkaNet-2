#pragma once

#include <stdint.h>
#include <ArduinoJson.h>

struct SitkaNet_Base_t {
	char name[16];
	int instance;
	char datestamp[16];
	char timestamp[16];
	int pktnumber;
	float vbatt;
	float stemmaMoisture[4];
	float stemmaTemp[4];
	float terrosMoisture[4];
	float terrosTemp[4];
	float ms5803Pressure[2];
	float ms5803Temp[2];
	float sht31dHumidity;
	float sht31dTemp;
	int tipBucket;
	int mmaInt;
	float lastRssi;
};


typedef union {
	SitkaNet_Base_t data;
	uint8_t raw[sizeof(SitkaNet_Base_t)];
} SitkaNet_t;

/**
 * Convert a JSON output from Loom into a SitkaNet_t struct for transmission over LoRa.
 * JSON MUST be of the format outputted by Loom.
 * @param[in] data JsonObject pointing to the root object of the Loom data bundle
 * @param[in] tipBucket Number representing the tipping bucket count
 * @param[in] mmaInt Number representing the accelerometer interrupt count
 * @param[in] lastRssi The last signal strength of the LoRa packet sentry
 * @param[out] out SiktaNet_t struct to fill with values.
 */
void json_to_struct(const JsonObjectConst& data, const int tipBucket, const int mmaInt, const float lastRssi, SitkaNet_t& out);

/**
 * Convert a SitkaNet_t struct into a JSON document of the format outputted by Loom.
 * All values in the SiktaNet_t struct MUST be initialized correctly, NAN is allowed.
 * Will output a JSON document of the following format:
 * ```JSON
 * {
 *   "type": "data",
 *   "id": {
 *     "name": "Device",
 *     "instance": 1
 *   },
 *   "timestamp": {
 *     "date": "2020/2/13",
 *     "time": "11:0:0"
 *   },
 *   "contents": [
 *     {
 *       "module": "Analog",
 *       "data": {
 *         "Vbat": 4.229735851
 *       }
 *     },
 *     {
 *       "module": "Packet",
 *       "data": {
 *         "Number": 1
 *       }
 *     },
 *     {
 *       "module": "STEMMA_0",
 *       "data": {
 *         "capacitive": 12.30000019,
 *         "temperature": 43.79999924
 *       }
 *     },
 *     {
 *       "module": "STEMMA_1",
 *       "data": {
 *         "capacitive": 12.19999981,
 *         "temperature": 43.38000107
 *       }
 *     },
 *     {
 *       "module": "STEMMA_2",
 *       "data": {
 *         "capacitive": 1233,
 *         "temperature": 4321.799805
 *       }
 *     },
 *     {
 *       "module": "STEMMA_3",
 *       "data": {
 *         "capacitive": 12.34300041,
 *         "temperature": 432.7999878
 *       }
 *     },
 *     {
 *       "module": "Teros_0",
 *       "data": {
 *         "moisture": 3434.300049,
 *         "temperature": 54.90000153
 *       }
 *     },
 *     {
 *       "module": "Teros_1",
 *       "data": {
 *         "moisture": 343.2999878,
 *         "temperature": 5.900000095
 *       }
 *     },
 *     {
 *       "module": "Teros_2",
 *       "data": {
 *         "moisture": 344.2999878,
 *         "temperature": 524.9000244
 *       }
 *     },
 *     {
 *       "module": "Teros_3",
 *       "data": {
 *         "moisture": 434.2999878,
 *         "temperature": 54.70000076
 *       }
 *     },
 *     {
 *       "module": "MS5803_0",
 *       "data": {
 *         "pressure": 434.3443909,
 *         "temperature": 56.79999924
 *       }
 *     },
 *     {
 *       "module": "MS5803_1",
 *       "data": {
 *         "pressure": 34.3443985,
 *         "temperature": 6.800000191
 *       }
 *     },
 *     {
 *       "module": "SHT31D_0",
 *       "data": {
 *         "humidity": 98.59999847,
 *         "tempurature": 64.80000305
 *       }
 *     },
 *     {
 *       "module": "MISC_0",
 *       "data": {
 *         "tipBucket": 10,
 *         "mmaInt": 11,
 *         "lastRssi": 21.39999962
 *       }
 *     }
 *   ]
 * }
 *```
 * @param[in] in SiktaNet_t struct to translate
 * @param[out] out JsonObject to populate with the above JSON (must have enough memory available!)
 */
void struct_to_json(const SitkaNet_t& in, const JsonObject& out);