#include "esp_camera.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "base64.h"

// ---------------- CONFIG ----------------
const char* WIFI_SSID = "BIT-SIH";
const char* WIFI_PASS = "bitsih@2025";

// Apps Script URL for Image Upload
const String GOOGLE_SCRIPT_URL = "https://script.google.com/macros/s/AKfycbxYcI92rESL0iVDXBcCE-7426s09A5sv53NakncCgDTrqYHQ-pqyX6gakPxL8zwkyQsww/exec";
const String SHARED_SECRET = "Uday@9440100517"; 

// ---------------- CAMERA PIN DEFINES (AI-Thinker ESP32-CAM) ----------------
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

const framesize_t CAMERA_FRAME_SIZE = FRAMESIZE_QVGA; 
const int CAMERA_JPEG_QUALITY = 10; 

bool startCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer   = LEDC_TIMER_0;
  config.pin_d0       = Y2_GPIO_NUM;
  config.pin_d1       = Y3_GPIO_NUM;
  config.pin_d2       = Y4_GPIO_NUM;
  config.pin_d3       = Y5_GPIO_NUM;
  config.pin_d4       = Y6_GPIO_NUM;
  config.pin_d5       = Y7_GPIO_NUM;
  config.pin_d6       = Y8_GPIO_NUM;
  config.pin_d7       = Y9_GPIO_NUM;
  config.pin_xclk     = XCLK_GPIO_NUM;
  config.pin_pclk     = PCLK_GPIO_NUM;
  config.pin_vsync    = VSYNC_GPIO_NUM;
  config.pin_href     = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn     = PWDN_GPIO_NUM;
  config.pin_reset    = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = CAMERA_FRAME_SIZE;
  config.jpeg_quality = CAMERA_JPEG_QUALITY;
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);
  return (err == ESP_OK);
}

void setup() {
  Serial.begin(115200);
  if (!startCamera()) {
    while(true){ delay(1000); }
  }

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void handleCaptureCommand() {
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) return;
  
  String b64 = base64::encode(fb->buf, fb->len);
  esp_camera_fb_return(fb);

  if (WiFi.status() != WL_CONNECTED) return;

  String payload = "{\"secret\":\"" + SHARED_SECRET + "\",\"name\":\"Unknown\",\"status\":\"Intruder\",\"image\":\"" + b64 + "\"}";

  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient https;

  if (https.begin(client, GOOGLE_SCRIPT_URL)) {
    https.addHeader("Content-Type", "application/json");
    int httpCode = https.POST(payload);
    https.end();
  }
}

void loop() {
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    line.trim();
    if (line.equals("CAPTURE")) {
      handleCaptureCommand();
    }
  }
}
