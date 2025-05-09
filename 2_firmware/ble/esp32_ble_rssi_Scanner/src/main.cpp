#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

int scanTime = 5; // 스캔 시간(초)
BLEScan* pBLEScan;

// 타겟 장치 이름 또는 MAC 주소
const std::string targetName = "ESP32_A";
// const std::string targetMac = "XX:XX:XX:XX:XX:XX"; // MAC으로도 가능

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getName() == targetName) {
      int rssi = advertisedDevice.getRSSI();
      Serial.println("🎯 ESP32_A 발견!");
      Serial.print("📶 RSSI: ");
      Serial.print(rssi);
      Serial.print(" dBm");

      // 거리 추정 (간단한 공식)
      float txPower = -59; // 1m 기준 RSSI (환경에 따라 보정 필요)
      float n = 2.0;        // 감쇠 지수
      float distance = pow(10.0, ((txPower - rssi) / (10 * n)));
      Serial.print(" 📏 추정 거리: ");
      Serial.print(distance, 2);
      Serial.println(" m");
    }
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("🔍 BLE 스캔 시작...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); 
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); 
}

void loop() {
  pBLEScan->start(scanTime, false);
  delay(2000);
}
