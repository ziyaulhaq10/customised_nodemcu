#include <EEPROM.h>

// EEPROM size
#define EEPROM_SIZE 512

String jsonData = "";

// Read from EEPROM
void loadJSONFromEEPROM() {
  Serial.println("Reading from EEPROM...");
  jsonData = "";
  for (int i = 0; i < EEPROM_SIZE; i++) {
    char c = EEPROM.read(i);
    if (c == '\0') break;
    jsonData += c;
  }
  Serial.println("Loaded JSON: " + jsonData);
}

// Save to EEPROM
void saveJSONToEEPROM(const String& data) {
  Serial.println("Saving to EEPROM: " + data);
  for (int i = 0; i < EEPROM_SIZE; i++) {
    if (i < data.length()) {
      EEPROM.write(i, data[i]);
    } else {
      EEPROM.write(i, 0); // Null terminate
    }
  }
  EEPROM.commit();
}
