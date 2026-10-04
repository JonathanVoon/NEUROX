#include <Wire.h>
#include <MPU6050.h>
#include <WiFi.h>
#include <esp_now.h>
#include <FirebaseESP32.h>
#include <addons/RTDBHelper.h>

const char* ssid = "Ken's iPhone";
const char* password = "hihihoho";

#define FIREBASE_HOST "neurox-app-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "52WJ3w6szb6vnLtw6xNowrA9EX39xLDMLn9dvy7D"

uint8_t mainNodeMac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

#define SDA_PIN 4
#define SCL_PIN 5

#define motor1 6
#define motor2 7

MPU6050 mpu1(0x68);
MPU6050 mpu2(0x69);

#define STILL_TIME 2000
#define GYRO_STILL_THRESHOLD 500
#define GYRO_SECOND 1000
#define CALIBRATION_SAMPLES 100

int16_t mpu1GXOffset = 0;
int16_t mpu1GYOffset = 0;
int16_t mpu1GZOffset = 0;

int16_t mpu2GXOffset = 0;
int16_t mpu2GYOffset = 0;
int16_t mpu2GZOffset = 0;

unsigned long mpu1StillStart = 0;
unsigned long mpu2StillStart = 0;

bool motor1On = false;
bool motor2On = false;

// NEW: waits for the main node's "start" signal before doing anything
volatile bool sessionStarted = false;

typedef struct struct_message {
  uint8_t command;
} struct_message;

struct_message outgoingData;
struct_message incomingData; // NEW: holds whatever the main node sends us
esp_now_peer_info_t peerInfo;

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void calibrateGyro() {
  long gx1Total = 0;
  long gy1Total = 0;
  long gz1Total = 0;

  long gx2Total = 0;
  long gy2Total = 0;
  long gz2Total = 0;

  Serial.println("Starting gyro calibration...");
  Serial.println("KEEP BOTH MPUs STILL");

  for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
    int16_t ax1, ay1, az1, gx1, gy1, gz1;
    int16_t ax2, ay2, az2, gx2, gy2, gz2;

    mpu1.getMotion6(&ax1, &ay1, &az1, &gx1, &gy1, &gz1);
    mpu2.getMotion6(&ax2, &ay2, &az2, &gx2, &gy2, &gz2);

    gx1Total += gx1;
    gy1Total += gy1;
    gz1Total += gz1;

    gx2Total += gx2;
    gy2Total += gy2;
    gz2Total += gz2;

    delay(10);
  }

  mpu1GXOffset = gx1Total / CALIBRATION_SAMPLES;
  mpu1GYOffset = gy1Total / CALIBRATION_SAMPLES;
  mpu1GZOffset = gz1Total / CALIBRATION_SAMPLES;

  mpu2GXOffset = gx2Total / CALIBRATION_SAMPLES;
  mpu2GYOffset = gy2Total / CALIBRATION_SAMPLES;
  mpu2GZOffset = gz2Total / CALIBRATION_SAMPLES;

  Serial.println("Calibration complete");

  Serial.print("MPU1 Offset: ");
  Serial.print(mpu1GXOffset);
  Serial.print(", ");
  Serial.print(mpu1GYOffset);
  Serial.print(", ");
  Serial.println(mpu1GZOffset);

  Serial.print("MPU2 Offset: ");
  Serial.print(mpu2GXOffset);
  Serial.print(", ");
  Serial.print(mpu2GYOffset);
  Serial.print(", ");
  Serial.println(mpu2GZOffset);

  Serial.println("-----------------------------");
}

void sendEspNowCommand(uint8_t cmd) {
  outgoingData.command = cmd;
  esp_now_send(mainNodeMac, (uint8_t *) &outgoingData, sizeof(outgoingData));
}

void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingDataBytes, int len) {
  if (len < sizeof(struct_message)) return;
  memcpy(&incomingData, incomingDataBytes, sizeof(incomingData));

  if (incomingData.command == 3) {
    sessionStarted = true;
    Serial.println("Received liao, start command from box");
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  digitalWrite(motor1, LOW);
  digitalWrite(motor2, LOW);

  Wire.begin(SDA_PIN, SCL_PIN);
  delay(500);

  mpu1.initialize();
  mpu2.initialize();

  Serial.println("MPU6050 Test");

  Serial.print("MPU1 connection: ");
  Serial.println(mpu1.testConnection() ? "OK" : "FAILED");

  Serial.print("MPU2 connection: ");
  Serial.println(mpu2.testConnection() ? "OK" : "FAILED");

  calibrateGyro();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());

  config.database_url = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.reconnectNetwork(true);
  Firebase.begin(&config, &auth);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  memcpy(peerInfo.peer_addr, mainNodeMac, 6);
  peerInfo.channel = WiFi.channel();
  peerInfo.encrypt = false;

  esp_now_add_peer(&peerInfo);

  // NEW: start listening for the main node's "start" command
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("Waiting for START command from main node...");
}

void loop() {
  // NEW: do nothing at all until the main node says go
  if (!sessionStarted) {
    delay(500);
    return;
  }

  int16_t ax1, ay1, az1, gx1, gy1, gz1;
  int16_t ax2, ay2, az2, gx2, gy2, gz2;

  mpu1.getMotion6(&ax1, &ay1, &az1, &gx1, &gy1, &gz1);
  mpu2.getMotion6(&ax2, &ay2, &az2, &gx2, &gy2, &gz2);

  int16_t correctedGy1 = gy1 - mpu1GYOffset;
  int16_t correctedGy2 = gy2 - mpu2GYOffset;

  bool mpu1Still = (correctedGy1 >= -GYRO_STILL_THRESHOLD && correctedGy1 <= GYRO_STILL_THRESHOLD);

  bool mpu2Still = (correctedGy2 >= -GYRO_SECOND && correctedGy2 <= GYRO_SECOND);

  Serial.print("MPU1 | Gyro: ");
  Serial.print(gx1);
  Serial.print(", ");
  Serial.print(gy1);
  Serial.print(", ");
  Serial.print(gz1);
  Serial.print(" | Corrected Y: ");
  Serial.print(correctedGy1);
  Serial.print(" | Still: ");
  Serial.print(mpu1Still ? "YES" : "NO");
  Serial.print(" | Motor: ");
  Serial.println(motor1On ? "ON" : "OFF");

  Serial.print("MPU2 | Gyro: ");
  Serial.print(gx2);
  Serial.print(", ");
  Serial.print(gy2);
  Serial.print(", ");
  Serial.print(gz2);
  Serial.print(" | Corrected Y: ");
  Serial.print(correctedGy2);
  Serial.print(" | Still: ");
  Serial.print(mpu2Still ? "YES" : "NO");
  Serial.print(" | Motor: ");
  Serial.println(motor2On ? "ON" : "OFF");

  if (mpu1Still) {
    if (mpu1StillStart == 0) mpu1StillStart = millis();

    if ((millis() - mpu1StillStart >= STILL_TIME) && !motor1On) {
      motor1On = true;
      digitalWrite(motor1, HIGH);
      sendEspNowCommand(1);

      Firebase.setBool(fbdo, "/neurox/secondary_node/mpu1_still", true);
      Firebase.setBool(fbdo, "/neurox/secondary_node/motor1", true);

      Serial.println("MPU1 STILL FOR 2 SECONDS -> MOTOR1 ON");
    }
  } else {
    mpu1StillStart = 0;

    if (motor1On) {
      motor1On = false;
      digitalWrite(motor1, LOW);

      Firebase.setBool(fbdo, "/neurox/secondary_node/mpu1_still", false);
      Firebase.setBool(fbdo, "/neurox/secondary_node/motor1", false);

      Serial.println("MPU1 MOVED -> MOTOR1 OFF");
    }
  }

  if (mpu2Still) {
    if (mpu2StillStart == 0) mpu2StillStart = millis();

    if ((millis() - mpu2StillStart >= STILL_TIME) && !motor2On) {
      motor2On = true;
      digitalWrite(motor2, HIGH);
      sendEspNowCommand(2);

      Firebase.setBool(fbdo, "/neurox/secondary_node/mpu2_still", true);
      Firebase.setBool(fbdo, "/neurox/secondary_node/motor2", true);

      Serial.println("MPU2 STILL FOR 2 SECONDS -> MOTOR2 ON");
    }
  } else {
    mpu2StillStart = 0;

    if (motor2On) {
      motor2On = false;
      digitalWrite(motor2, LOW);

      Firebase.setBool(fbdo, "/neurox/secondary_node/mpu2_still", false);
      Firebase.setBool(fbdo, "/neurox/secondary_node/motor2", false);

      Serial.println("MPU2 MOVED -> MOTOR2 OFF");
    }
  }

  Serial.println("-----------------------------");

  delay(500);
}