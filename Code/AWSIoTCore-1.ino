#include "application.h"
#include "MQTT-TLS.h"
#include "keys.h"

SerialLogHandler logHandler;

void callback(char *topic, byte *payload, unsigned int length);

const char amazonIoTRootCaPem[] = AMAZON_IOT_ROOT_CA_PEM;
const char clientKeyCrtPem[] = CELINT_KEY_CRT_PEM;
const char clientKeyPem[] = CELINT_KEY_PEM;

MQTT client("a3ey7k1opdss62-ats.iot.us-east-1.amazonaws.com", 8883, callback);

const int solenoidPin = D7;
const int sensorWire = D6;

// receive message
void callback(char *topic, byte *payload, unsigned int length)
{
  Log.info("Received Data on topic %s", topic);
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String message(p);
  Log.info(p);
  Log.info("Success");
 

  if (message.equals("Open"))
  {
    Log.info("The Door is Open");
    digitalWrite(solenoidPin, HIGH);
   
  }
  else if (message.equals("Close")) {
    Log.info("The Door is Closed");
    digitalWrite(solenoidPin, LOW);
    
  }
}

#define ONE_DAY_MILLIS (24 * 60 * 60 * 1000)
unsigned long lastSync = millis();
int counter = 0;

void setup()
{
  if (millis() - lastSync > ONE_DAY_MILLIS)
  {
    Particle.syncTime();
    lastSync = millis();
  }

  RGB.control(true);

  // Enable TLS, set Root CA pem, private key file.
  client.enableTls(amazonIoTRootCaPem, sizeof(amazonIoTRootCaPem),
                   clientKeyCrtPem, sizeof(clientKeyCrtPem),
                   clientKeyPem, sizeof(clientKeyPem));
  Log.info("TLS enabled");

  // Connect to the server
  bool status = client.connect("aws_" + String(Time.now()));

  if (status)
  {
    Log.info("Connection is completed");
  }
  else
  {
    Log.info("Connection is not completed");
  }

  // Publish/subscribe
  if (client.isConnected())
  {
    Log.info("Client connected");
    client.publish("sensor", "hello world");
    client.subscribe("solonoid");
  }

  pinMode(solenoidPin, OUTPUT);
  digitalWrite(solenoidPin, LOW);
  pinMode(sensorWire, INPUT);
  digitalWrite(sensorWire, LOW);
  Serial.begin(9600);
}

void loop()
{
  if (client.isConnected())
  {
    client.loop();
  }
  delay(200);
  if (digitalRead(sensorWire) == LOW)
  {
    Serial.println("Wires Have Touched");
    client.publish("sensor", "Wires Touched");
    delay(2500);
  }
  else if (digitalRead(sensorWire) == HIGH)
  {
    Serial.println("Not Touched");
    client.publish("sensor", "Not Touched");
    delay(2500);
  }
}
