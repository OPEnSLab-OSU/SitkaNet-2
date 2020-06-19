#include "SitkaNetJSON.h"

static bool get_data_point_from_contents(const JsonArrayConst& contents, const char* module_name, const char* data_key, float& out) {
	for (const JsonVariantConst& module_data : contents) {
		// if the module name matches, check the data key
		const char* name = module_data["module"];
		if (name == nullptr)
			continue;
		if (strncmp(name, module_name, 15) != 0)
			continue;
		const JsonObjectConst data_obj = module_data["data"];
		if (data_obj.isNull())
			continue;
		// find the data we need
		const JsonVariantConst data_value = data_obj[data_key];
		if (!data_value.isNull() && data_value.is<float>()) {
			// write to out
			out = data_value.as<float>();
			// done!
			return true;
		}
	}
	// return an emptey (null) varient
	out = NAN;
	return false;
}

static bool get_data_point_from_contents_int(const JsonArrayConst& contents, const char* module_name, const char* data_key, int& out) {
	for (const JsonVariantConst& module_data : contents) {
		// if the module name matches, check the data key
		const char* name = module_data["module"];
		if (name == nullptr)
			continue;
		if (strncmp(name, module_name, 15) != 0)
			continue;
		const JsonObjectConst data_obj = module_data["data"];
		if (data_obj.isNull())
			continue;
		// find the data we need
		const JsonVariantConst data_value = data_obj[data_key];
		if (!data_value.isNull() && data_value.is<int>()) {
			// write to out
			out = data_value.as<int>();
			// done!
			return true;
		}
	}
	// return an emptey (null) varient
	out = -1;
	return false;
}

void json_to_struct(const JsonObjectConst& data, SitkaNet_t& out) {
	// name, instance
	const JsonObjectConst id = data["id"];
	if (!id.isNull()) {
		const char* name = id["name"];
		if (name != nullptr)
			strncpy(out.data.name, name, 15);
		else
			out.data.name[0] = 0;
		out.data.instance = id["instance"] | -1;
	}
	// timestamp
	const JsonObjectConst stamp = data["timestamp"];
	if (!stamp.isNull()) {
		const char* date = stamp["date"].as<const char*>();
		if (date != nullptr)
			strncpy(out.data.datestamp, date, 15);
		else
			out.data.datestamp[0] = 0;
		const char* time = stamp["time"].as<const char*>();
		if (time != nullptr)
			strncpy(out.data.timestamp, time, 15);
		else
			out.data.timestamp[0] = 0;
	}
	// get a reference to the "contents" object
	const JsonArrayConst contents = data["contents"];
	if (contents.isNull())
		return;
	// find every data point we care about, and populate the struct with it
	// packet #
	get_data_point_from_contents_int(contents, "Packet", "Number", out.data.pktnumber);
	// vbatt
	get_data_point_from_contents(contents, "Analog", "Vbat", out.data.vbatt);
	// STEMMA moisture
	const char* stemma_names[] = { "STEMMA_0", "STEMMA_1", "STEMMA_2" };
	for (uint8_t i = 0; i < 3; i++)
		get_data_point_from_contents(contents, stemma_names[i], "capactive", out.data.stemmaMoisture[i]);
	// STEMMA temp
	for (uint8_t i = 0; i < 3; i++)
		get_data_point_from_contents(contents, stemma_names[i], "temperature", out.data.stemmaTemp[i]);
	// Terros moisture
	const char* teros_moisture_names[] = { "T_AM", "T_BM", "T_CM" };
	for (uint8_t i = 0; i < 3; i++)
		get_data_point_from_contents(contents, teros_moisture_names[i], "M", out.data.terrosMoisture[i]);
	// Teros temperature
	const char* teros_temp_names[] = { "T_AT", "T_BT", "T_CT" };
	for (uint8_t i = 0; i < 3; i++)
		get_data_point_from_contents(contents, teros_temp_names[i], "T", out.data.terrosTemp[i]);
	// MS5803 Pressure
	const char* ms5803_names[] = { "MS5803_4", "MS5803_5" };
	for (uint8_t i = 0; i < 2; i++)
		get_data_point_from_contents(contents, ms5803_names[i], "pressure", out.data.ms5803Pressure[i]);
	// MS5803 temperature
	for (uint8_t i = 0; i < 2; i++)
		get_data_point_from_contents(contents, ms5803_names[i], "temp", out.data.ms5803Temp[i]);
	// SHT31D humidity
	get_data_point_from_contents(contents, "SHT31D_7", "humid", out.data.sht31dHumidity);
	// SHT31D temperature
	get_data_point_from_contents(contents, "SHT31D_7", "temp", out.data.sht31dTemp);
	// MMA Int
	get_data_point_from_contents_int(contents, "Accel", "Count", out.data.mmaInt);
	// Tipping Bucket
	get_data_point_from_contents_int(contents, "Tip", "Count", out.data.tipBucket);
	// last RSSI
	get_data_point_from_contents(contents, "rssi", "value", out.data.lastRssi);
}


static JsonObject make_module_object(const JsonArray& contents, const char* name) {
	const JsonObject data = contents.createNestedObject();
	data["module"] = name;
	return data.createNestedObject("data");
}

void struct_to_json(const SitkaNet_t& in, const JsonObject& out) {
	// start adding object to it!
	out["type"] = "data";
	// id block
	const JsonObject id = out.createNestedObject("id");
	id["name"] = in.data.name;
	id["instance"] = in.data.instance;
	// timestamp block
	const JsonObject timestamp = out.createNestedObject("timestamp");
	timestamp["date"] = in.data.datestamp;
	timestamp["time"] = in.data.timestamp;
	// contents array
	const JsonArray contents = out.createNestedArray("contents");
	// add objects for each sensor!
	// vbatt
	{
		const JsonObject data = make_module_object(contents, "Analog");
		data["Vbat"] = in.data.vbatt;
	}
	// pkt number
	{
		const JsonObject data = make_module_object(contents, "Packet");
		data["Number"] = in.data.pktnumber;
	}
	// stemma
	const char* stemma_names[] = { "STEMMA_0", "STEMMA_1", "STEMMA_2" };
	for (uint8_t i = 0; i < 3; i++) {
		const JsonObject data = make_module_object(contents, stemma_names[i]);
		data["capacitive"] = in.data.stemmaMoisture[i];
		data["temperature"] = in.data.stemmaTemp[i];
	}
	// Teros moisture
	const char* teros_temp_names[] = { "T_AT", "T_BT", "T_CM" };
	for (uint8_t i = 0; i < 3; i++) {
		const JsonObject data = make_module_object(contents, teros_temp_names[i]);
		data["T"] = in.data.terrosTemp[i];
	}
	// Teros tempurature
	const char* teros_moisture_names[] = { "T_AM", "T_BM", "T_CM" };
	for (uint8_t i = 0; i < 3; i++) {
		const JsonObject data = make_module_object(contents, teros_moisture_names[i]);
		data["M"] = in.data.terrosMoisture[i];
	}
	// ms5803
	const char* ms5803_names[] = { "MS5803_4", "MS5803_5" };
	for (uint8_t i = 0; i < 2; i++) {
		const JsonObject data = make_module_object(contents, ms5803_names[i]);
		data["pressure"] = in.data.ms5803Pressure[i];
		data["temp"] = in.data.ms5803Temp[i];
	}
	// sht31d
	{
		const JsonObject data = make_module_object(contents, "SHT31D_7");
		data["humid"] = in.data.sht31dHumidity;
		data["temp"] = in.data.sht31dTemp;
	}
	// tip bucket
	{
		const JsonObject data = make_module_object(contents, "Tip");
		data["Count"] = in.data.tipBucket;
	}
	// mma int
	{
		const JsonObject data = make_module_object(contents, "Accel");
		data["Count"] = in.data.mmaInt;
	}
	// last RSSI
	{
		const JsonObject data = make_module_object(contents, "rssi");
		data["value"] = in.data.lastRssi;
	}
}
