# Skysis-Room-Availability-System
It is a room availability project developed by the SkySis team.


<details>
  <summary>Table of Contents</summary>
  
  - [About The Project](#about-the-project)
  - [Getting Started](#getting-started)
  - [Installation](#installation)
  - [Usage](#usage)
  - [Contact](#contact)
  - [Acknowledgements](#acknowledgements)
</details>

## About The Project

This project aims to send data to a web server over Wi-Fi using an ESP32 and an RFID reader.

## Getting Started

To get a local copy up and running follow these simple steps.

### Prerequisites

You need to have Arduino IDE installed on your machine.

### Installation

1. Clone the repo:
    ```sh
    https://github.com/skylab-kulubu/Skysis-Room-Availability-System.git
    cd project_name
    ```

2. Follow the steps below to set up the development environment.

### Step 1: Install Arduino IDE

1. Download and install [Arduino IDE](https://www.arduino.cc/en/software).
2. Open Arduino IDE and navigate to `File -> Preferences`.
3. In the "Additional Boards Manager URLs" field, add `https://dl.espressif.com/dl/package_esp32_index.json` and click "OK".
4. Go to `Tools -> Board -> Boards Manager`, search for `esp32`, and install the ESP32 boards.
5. Select your board as `ESP32 Wrover Module`: `Tools -> Board -> ESP32 Wrover Module`.

### Step 2: Install Required Libraries

You need to install the following libraries:
- WiFi
- HTTPClient
- SPI
- MFRC522
- ArduinoJson

To install these libraries, open Arduino IDE and navigate to `Sketch -> Include Library -> Manage Libraries`. Search for each library and click the "Install" button.

### Step 3: Opening the Project in Arduino IDE

1. Open Arduino IDE.
2. Navigate to `File -> Open` and select the `.ino` file in the project directory.
3. After opening the project in Arduino IDE, make the necessary settings (e.g., selecting the correct board and port).

### Step 4: Uploading the Code

Connect the ESP32 to your computer and select the correct port from `Tools -> Port`. Then, upload the code to the ESP32 by navigating to `Sketch -> Upload` (or click the "Upload" button in the top left corner).

### Usage

Here's the code used in this project:

```cpp
#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ArduinoJson.h>
#include <iostream>
#include <stdio.h>
#include <string.h>

// Wi-Fi settings
const char* ssid = "sky-lab";
const char* password = "sky-labpassword";

// RFID RC522 settings
#define SS_PIN 15
#define RST_PIN 22
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();

  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(500);
    return;
  }

  // Read RFID UID
  String rfid_uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    rfid_uid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "") + String(mfrc522.uid.uidByte[i], HEX);
  }

  rfid_uid.toUpperCase();
  Serial.print("RFID UID: ");
  Serial.println(rfid_uid);
  const char* c = rfid_uid.c_str();

  // Web server URL
  const char* server = "https://oda.yildizskylab.com/api/room/passCard/";
  char serverName[100];
  strcpy(serverName, server);    
  strcat(serverName, c);  

  // Send HTTP request if Wi-Fi is connected
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);

    // Create JSON data
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["room"] = rfid_uid;

    String jsonData;
    serializeJson(jsonDoc, jsonData);

    // Send HTTP POST request
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.print("HTTP Response Code: ");
      Serial.println(httpResponseCode);
      Serial.print("Response: ");
      Serial.println(payload);
    } 
    http.end();
  }

  delay(10000); 
}
```


### Step 5: Circuit Diagram and Required Materials

Below is the list of required components for this project along with links to purchase them:

- **ESP32 Development Board**: [Buy here](https://www.robotistan.com/esp32-esp-32s-wifi-bluetooth-dual-mode-gelistirme-karti?language=tr&h=453f8cc6&_sgm_campaign=product&_sgm_source=7369&_sgm_action=search&_sgm_term=esp32&_sgm_pinned=false)
- **MFRC522 RFID Reader**: [Buy here](https://www.robotistan.com/rc522-rfid-nfc-kiti-rc522-rfid-nfc-modulu-kart-ve-anahtarlik-kiti-1356)
- **Jumper Wires**: [Buy here](https://www.robotistan.com/500-parca-jumper-kablo-seti?language=tr&h=ca082375&_sgm_campaign=product&_sgm_source=9148&_sgm_action=search&_sgm_term=jumper&_sgm_pinned=false)
- **Breadboard**: [Buy here](https://www.robotistan.com/breadboard-2?language=tr&h=1107c55c&_sgm_campaign=product&_sgm_source=1617&_sgm_action=search&_sgm_term=breadb&_sgm_pinned=false)
- **USB Cable for ESP32**: [Buy here](https://www.robotistan.com/mikro-usb-kablo-1?language=tr&h=ab5e2833&_sgm_campaign=product&_sgm_source=799&_sgm_action=search&_sgm_term=micro+usb&_sgm_pinned=false)
- **Gepro UM-0255 5V 2A Adapter**:[Buy here](https://www.robotistan.com/gepro-um-0255-5v-2a-adaptor?language=tr&h=a00968d7&_sgm_campaign=product&_sgm_source=11219&_sgm_action=search&_sgm_term=adapt%C3%B6r&_sgm_pinned=false)

1. Connect the MFRC522 RFID Reader to the ESP32 as follows:
   - **RST** to **GPIO 22**
   - **SDA** to **GPIO 15**
   - **MOSI** to **GPIO 23**
   - **MISO** to **GPIO 19**
   - **SCK** to **GPIO 18**
   - **GND** to **GND**
   - **3.3V** to **3.3V**

For detailed wiring and setup, refer to the [ESP32 and MFRC522 Wiring Guide](https://randomnerdtutorials.com/esp32-rfid-reader-mfrc522-arduino/).

2. After completing the connections, upload the code to your ESP32 using the steps mentioned earlier.

3. Open the Serial Monitor in Arduino IDE (`Tools -> Serial Monitor`) and set the baud rate to `115200`.

4. Verify that the ESP32 connects to Wi-Fi and reads the RFID card's UID.

5. Each time an RFID card is read, its UID will be sent to the web server, and the response code will be displayed in the Serial Monitor.


## Contact

SKYLAB

- Website: [https://yildizskylab.com](https://yildizskylab.com/)
- Email: [contact@skysis.com](mailto:contact@skysis.com)
- GitHub: [https://github.com/skylab-kulubu](https://github.com/skylab-kulubu)
- X: [@SKYLAB](https://x.com/SkyLabKulubu)
- YouTube: [@SKYLAB](https://www.youtube.com/channel/UCF_qBKpUnM3X_C3L-gLEO4A)
- Instagram: [@SKYLAB](https://www.instagram.com/ytuskylab?utm_source=ig_web_button_share_sheet&igsh=ZDNlZDc0MzIxNw==)
- Linkedn: [@SKYLAB](https://www.linkedin.com/company/ytuskylab/)
- Discord: [@SKYLAB](https://discord.com/invite/6jFBjH8y63)

  *"TO KNOW MORE ABOUT STUDENT BRANCH"*
  
  **SKYLAB**
  
## Acknowledgements

- [Arduino](https://www.arduino.cc/)
- [ESP32](https://www.espressif.com/en/products/socs/esp32)
- [GitHub](https://github.com/)
- [WiFi Library](https://www.arduino.cc/en/Reference/WiFi)
- [HTTPClient Library](https://www.arduino.cc/en/Reference/HTTPClient)
- [SPI Library](https://www.arduino.cc/en/Reference/SPI)
- [MFRC522 Library](https://github.com/miguelbalboa/rfid)
- [ArduinoJson Library](https://arduinojson.org/)


