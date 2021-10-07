// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License
//
// ++choi test parsing JSON for aws-iot shadow messages
//
// This example shows how to deserialize a JSON document with ArduinoJson.
//
// https://arduinojson.org/v6/example/parser/

#include <ArduinoJson.h>

void setup() {
  // Initialize serial port
  Serial.begin(115200);
//  while (!Serial) continue;

  // Allocate the JSON document
  //
  // Inside the brackets, 200 is the capacity of the memory pool in bytes.
  // Don't forget to change this value to match your JSON document.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<200> doc;

  // StaticJsonDocument<N> allocates memory on the stack, it can be
  // replaced by DynamicJsonDocument which allocates in the heap.
  //
  // DynamicJsonDocument doc(200);

  // JSON input string.
  //
  // Using a char[], as shown here, enables the "zero-copy" mode. This mode uses
  // the minimal amount of memory because the JsonDocument stores pointers to
  // the input buffer.
  // If you use another type of input, ArduinoJson must copy the strings from
  // the input to the JsonDocument, so you need to increase the capacity of the
  // JsonDocument.
  char json[] =
//      "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
        "{\"state\":{\"reported\":{\"temp\":24,\"humid\":60,\"led\":true}}}";
          
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json);
//  JsonObject obj = doc.as<JsonObject>();

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  // display Json
//  obj.printTo(Serial);
  Serial.print(doc.as<JsonObject>());     //?? why not working
  Serial.println("");
  
  // Fetch values.
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do doc["time"].as<long>();
  JsonObject state = doc["state"];
  JsonObject reported = state["reported"];
  int temp = reported["temp"];
  int humid = reported["humid"];
  boolean led = reported["led"];
//  const char* sensor = doc["sensor"];
//  long time = doc["time"];
//  double latitude = doc["data"][0];
//  double longitude = doc["data"][1];

  // Print values.
  Serial.println("temp: "+String(temp));
  Serial.println("humid: "+String(humid));
  Serial.println("LED: "+String((led?"ON":"OFF")));
}

void loop() {
  // not used in this example
}
