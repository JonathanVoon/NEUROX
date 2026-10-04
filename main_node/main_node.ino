#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <esp_now.h>
#include <FirebaseESP32.h>
#include <addons/RTDBHelper.h>

const char* ssid = "Lin";
const char* password = "Tai.8181";

#define FIREBASE_HOST "neurox-app-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "52WJ3w6szb6vnLtw6xNowrA9EX39xLDMLn9dvy7D"

#define RELAY_AIR_PUMP 4
#define RELAY_HOTPUMP 19
#define RELAY_COLDPUMP 21
#define RELAY_VALVE 18
#define RELAY_HEATER 22

#define WARM_SENSOR_PIN 17
#define COLD_SENSOR_PIN 16

OneWire warmOneWire(WARM_SENSOR_PIN);
OneWire coldOneWire(COLD_SENSOR_PIN);

DallasTemperature warmSensor(&warmOneWire);
DallasTemperature coldSensor(&coldOneWire);

const float TARGET_TEMP = 38.0;

float warmTemp = 0.0;
float coldTemp = 0.0;

// Pneumatic cycle timing: fill 3s, release (vent) 5s
const unsigned long FILL_TIME = 3000;
const unsigned long RELEASE_TIME = 5000;
const unsigned long CYCLE_TIME = FILL_TIME + RELEASE_TIME;
const unsigned long SEQUENCE_DURATION = 30000;

const int NUM_CYCLES = (SEQUENCE_DURATION + CYCLE_TIME - 1) / CYCLE_TIME; // = 4 cycles (32s total)

typedef struct struct_message {
  uint8_t command;
} struct_message;

struct_message incomingData;
struct_message outgoingData;

volatile bool coldWaterRequested = false;
volatile bool massageRequested = false;

bool initialSequenceDone = false;

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;


void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingDataBytes, int len) {
  if (len < sizeof(struct_message)) {
    return;
  }
  memcpy(&incomingData, incomingDataBytes, sizeof(incomingData));
  if (!initialSequenceDone) {
    return;
  }
  if (incomingData.command == 1) {
    coldWaterRequested = true;
  }
  else if (incomingData.command == 2) {
    massageRequested = true;
  }
}

void sendStartCommand() {
  outgoingData.command = 3;
  uint8_t broadcastAddress[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
  };
  esp_now_send(
    broadcastAddress,
    (uint8_t *)&outgoingData,
    sizeof(outgoingData)
  );
}

void pneumaticCycle() {
  digitalWrite(RELAY_VALVE, LOW);
  digitalWrite(RELAY_AIR_PUMP, LOW);
  delay(FILL_TIME);

  digitalWrite(RELAY_AIR_PUMP, HIGH);
  digitalWrite(RELAY_VALVE, HIGH);
  delay(RELEASE_TIME);
}

void runPneumaticCycles(int cycles) {
  for (int i = 0; i < cycles; i++) {
    pneumaticCycle();
  }
}

void preparationPhase() {

  Serial.println("Preparation phase started");

  digitalWrite(RELAY_HOTPUMP, LOW);

  runPneumaticCycles(NUM_CYCLES);

  digitalWrite(RELAY_HOTPUMP, HIGH);

  Serial.println("Preparation phase complete");
}

void recoveryColdWater() {
  Serial.println("Recovery: cold water started");
  digitalWrite(RELAY_COLDPUMP, LOW);
  delay(5000);
  digitalWrite(RELAY_COLDPUMP, HIGH);
  Serial.println("Recovery: cold water complete");
}

void recoveryMassage() {
  Serial.println("Recovery: massage started");
  runPneumaticCycles(NUM_CYCLES);
  Serial.println("Recovery: massage complete");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(RELAY_AIR_PUMP, OUTPUT);
  pinMode(RELAY_HOTPUMP, OUTPUT);
  pinMode(RELAY_COLDPUMP, OUTPUT);
  pinMode(RELAY_VALVE, OUTPUT);
  pinMode(RELAY_HEATER, OUTPUT);

  digitalWrite(RELAY_AIR_PUMP, HIGH);
  digitalWrite(RELAY_HOTPUMP, HIGH);
  digitalWrite(RELAY_COLDPUMP, HIGH);
  digitalWrite(RELAY_VALVE, HIGH);
  digitalWrite(RELAY_HEATER, HIGH);

  warmSensor.begin();
  coldSensor.begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  config.database_url = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  Firebase.reconnectNetwork(true);
  Firebase.begin(&config, &auth);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = WiFi.channel();
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);

  Serial.println("System ready");
}

void loop() {
  warmSensor.requestTemperatures();
  coldSensor.requestTemperatures();
  warmTemp = warmSensor.getTempCByIndex(0);
  coldTemp = coldSensor.getTempCByIndex(0);

  bool warmSensorOk = (warmTemp != DEVICE_DISCONNECTED_C);
  bool coldSensorOk = (coldTemp != DEVICE_DISCONNECTED_C);

  if (!warmSensorOk || !coldSensorOk) {
    Serial.println("Temperature sensor disconnected - continuing anyway (testing mode)");
  }
  if (warmSensorOk) {
    Firebase.setFloat(fbdo, "/neurox/main_node/warm_temperature", warmTemp);
  }
  if (coldSensorOk) {
    Firebase.setFloat(fbdo, "/neurox/main_node/cold_temperature", coldTemp);
  }
  Serial.print("Warm Water: ");
  if (warmSensorOk) Serial.print(warmTemp); else Serial.print("-- (disconnected)");
  Serial.print(" C | Cold Water: ");
  if (coldSensorOk) Serial.print(coldTemp); else Serial.print("-- (disconnected)");
  Serial.println(" C");
  if (warmSensorOk && warmTemp < TARGET_TEMP) {
    digitalWrite(RELAY_HEATER, LOW);
    Serial.println("Heating ON");
  }
  else {
    digitalWrite(RELAY_HEATER, HIGH);
    if (!initialSequenceDone) {
      preparationPhase();
      initialSequenceDone = true;
      delay(500);
      sendStartCommand();
      Serial.println("Start command sent to secondary node");
    }
  }

  if (!initialSequenceDone) {
    delay(500);
    return;
  }

  if (coldWaterRequested) {
    coldWaterRequested = false;
    recoveryColdWater();
  }

  if (massageRequested) {
    massageRequested = false;
    recoveryMassage();
  }

  delay(500);
}
