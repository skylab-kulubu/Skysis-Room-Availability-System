#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ArduinoJson.h>
#include <iostream>
#include <stdio.h>
#include <string.h>

// Wi-Fi Ayarları
const char* ssid = "sky-lab";
const char* password = "sky-labharika";


// RFID RC522 Ayarları
#define SS_PIN 15
#define RST_PIN 22
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();

  Serial.println("Wi-Fi'ye bağlanılıyor...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi'ye bağlandı.");
  Serial.print("IP Adresi: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(500);
    return;
  }

  // RFID UID'sini oku
  String rfid_uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    rfid_uid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "") + String(mfrc522.uid.uidByte[i], HEX);
  }

  rfid_uid.toUpperCase();
  Serial.print("RFID UID: ");
  Serial.println(rfid_uid);
  const char* c = rfid_uid.c_str();

  // Web Sunucusu URL'si
  const char*server="https://oda.yildizskylab.com/api/room/passCard/";
  char serverName[100];
  // serverName stringlerini ekleme
  strcpy(serverName, server);    
  strcat(serverName, c);  

  // Wi-Fi bağlıysa HTTP isteği gönder
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);

    // JSON veri oluştur
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["room"] = rfid_uid;

    String jsonData;
    serializeJson(jsonDoc, jsonData);

    // HTTP isteği gönder
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.print("HTTP Yanıt Kodu: ");
      Serial.println(httpResponseCode);
      Serial.print("Yanıt: ");
      Serial.println(payload);
    } 
    http.end();
  }

  delay(10000); 
}
