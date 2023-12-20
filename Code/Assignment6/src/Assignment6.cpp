// Assignment 6
// Assignment6.cpp
// Srinath Dittakavi & Karim Sasa (Group 8)
#include "Particle.h"

SerialLogHandler logHandler;
SYSTEM_THREAD(ENABLED);


int bluePin = D2;
int greenPin = D3;
int redPin = D4;
const unsigned long UPDATE_INTERVAL = 5000;
unsigned long lastUpdate = 0;
unsigned long currentMillis = millis();
void onDataReceived(const uint8_t *data, size_t len, const BlePeerDevice &peer, void *context);

BleUuid customServiceUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
BleCharacteristic turnOnOffCharacteristic("turnOnOff", BleCharacteristicProperty::WRITE_WO_RSP, "6E400003-B5A3-F393-E0A9-E50E24DCCA9E", customServiceUuid, onDataReceived, NULL);
BleCharacteristic selectColorCharacteristic("selectColor", BleCharacteristicProperty::WRITE_WO_RSP, "6E400002-B5A3-F393-E0A9-E50E24DCCA9E", customServiceUuid, onDataReceived, NULL);
BleCharacteristic notifyStatusCharacteristic("notifyStatus", BleCharacteristicProperty::NOTIFY, "6E400004-B5A3-F393-E0A9-E50E24DCCA9E", customServiceUuid);

void loop();
void configureBLE();
void updateLedStatus();
void onTurnOnOffWrite(const uint8_t *data, size_t len, const BlePeerDevice &peer, void *context);
void onSelectColorWrite(const uint8_t *data, size_t len, const BlePeerDevice &peer, void *context);

bool isLedOn = false;
String selectedColor = "NONE";

// make sure to send as UTF8 to appear on the serial port !!
void onDataReceived(const uint8_t *data, size_t len, const BlePeerDevice &peer, void *context)
{
    Log.info("onDataReceived!!");
    Log.info("Received data from: %02X:%02X:%02X:%02X:%02X:%02X:", peer.address()[0], peer.address()[1], peer.address()[2], peer.address()[3], peer.address()[4], peer.address()[5]);
    
    String receivedData = String((char *)data, len);
    
    
    if (receivedData == "ON" || receivedData == "OFF")
    {
        onTurnOnOffWrite(data, len, peer, context); 
    }

    if (receivedData == "RED" || receivedData == "GREEN" || receivedData == "BLUE")
    {
        onSelectColorWrite(data, len, peer, context); 
    }

	  
 
  
}


void setup() {
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);

  configureBLE();
}

void loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - lastUpdate >= UPDATE_INTERVAL) {
        lastUpdate = currentMillis;

        if (BLE.connected()) {
            String ledStatus;
            String colorStatus;

            if (isLedOn) {
                ledStatus = "ON";
                colorStatus = selectedColor;
            } else {
                ledStatus = "OFF";
                colorStatus = selectedColor;
            }

            String statusStr = ledStatus + ", " + colorStatus;

            uint8_t readData[statusStr.length() + 1]; 
            statusStr.toCharArray((char*)readData, statusStr.length() + 1);

            notifyStatusCharacteristic.setValue(readData, statusStr.length());
            Log.info("Updated Status and Color : %s", statusStr.c_str());
        }
    }
}

void configureBLE() {
  BLE.on();
  BLE.addCharacteristic(turnOnOffCharacteristic);
  BLE.addCharacteristic(selectColorCharacteristic);
  BLE.addCharacteristic(notifyStatusCharacteristic);



  BleAdvertisingData advData;
  advData.appendServiceUUID(customServiceUuid);
  BLE.advertise(&advData);
}

void onTurnOnOffWrite(const uint8_t *data, size_t len, const BlePeerDevice &peer, void *context) {
  String dataStr = String((char *)data, len);

  if (dataStr == "ON") {
    isLedOn = true;
    Log.info("LED IS ON");
  } else if (dataStr == "OFF") {
    isLedOn = false;
    Log.info("LED IS OFF");
    updateLedStatus();
  }


}

void onSelectColorWrite(const uint8_t *data, size_t len, const BlePeerDevice &peer, void *context) {
  String dataStr = String((char *)data, len);

  Log.info("Received color: %s", dataStr.c_str());

  if (dataStr == "RED") {
    selectedColor = "RED";
  } else if (dataStr == "GREEN") {
    selectedColor = "GREEN";
  } else if (dataStr == "BLUE") {
    selectedColor = "BLUE";
  } else {
    selectedColor = "OFF";
  }

  updateLedStatus();
}

void updateLedStatus() {
  if (isLedOn) {
    if (selectedColor == "RED") {
      digitalWrite(redPin, HIGH);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, LOW);
    } else if (selectedColor == "GREEN") {
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, HIGH);
      digitalWrite(bluePin, LOW);
    } else if (selectedColor == "BLUE") {
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, HIGH);
    } 
  } else {
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
  }
}

