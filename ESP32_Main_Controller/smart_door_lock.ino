#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "HUSKYLENS.h"
#include <HardwareSerial.h>
#include <ESP32Servo.h>
#include "time.h"
#include <LiquidCrystal_I2C.h>

// ------------------- OBJECTS --------------------
HUSKYLENS huskylens;
HardwareSerial mySerial(2); // RX=16, TX=17 (HuskyLens)
HardwareSerial camSerial(1); // UART1 for ESP32-CAM (RX=GPIO26, TX=GPIO25)
Servo sweepServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ------------------- WIFI -----------------------
const char* ssid = "yours ssid";
const char* password = "yours password";

// Use your actual web app URL
String GOOGLE_SCRIPT_URL = "yours google script url her";

// ------------------- PINS -----------------------
const int RED_LED = 2;
const int GREEN_LED = 4;
const int BUZZER_PIN = 15;
const int SERVO_PIN = 14;

// ------------------- Attendance timing -----------------------
const unsigned long ATTENDANCE_DURATION = 30000; // 30 sec (ms) - Optimized
const unsigned long INTER_PHASE_DELAY = 1000;     // 1 second between phases

// ------------------- DETECTION ARRAYS -----------------------
bool detectedIN[8]  = { false }; // index 1..7
bool detectedOUT[8] = { false }; // index 1..7

// ------------------- PERIODS --------------------
struct Period { int hour; int minute; };
Period periods[7] = {
  {14,41}, {14,45}, {14,50}, {14,55}, {15,00}, {14,30}, {14,35}
};

// ------------------- STUDENTS -------------------
struct Student { String name; String hall; };

Student getStudentFromID(int id){
  switch(id){
    case 1: return {"KALPANA","MEMBER"};
    case 2: return {"HEMANTH","MEMBER"};
    case 3: return {"SRAVYA","MEMBER"};
    case 4: return {"CHANDINI","MEMBER"};
    case 5: return {"CHANDU","MEMBER"};
    case 6: return {"CHANDU","MEMBER"};
    case 7: return {"AJAY","MEMBER"};
    default: return {"Unknown","Unknown"};
  }
}

// -------------------------------------------------
// Door Control
// -------------------------------------------------
void unlockDoor() {
  sweepServo.attach(SERVO_PIN);
  sweepServo.write(90); // Unlock position
  delay(3000);          // Wait for user to pass
  sweepServo.write(0);  // Lock position
  delay(500);
  sweepServo.detach();  // Save power/prevent jitter
}

// -------------------------------------------------
// LCD Helpers
// -------------------------------------------------
void lcdShowCounts(int inCount, int outCount){
  char buf[17];
  sprintf(buf, "IN:%02d OUT:%02d", inCount, outCount);
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print(buf);
}

void lcdTypewriter(String text){
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  for(int i=0;i<text.length() && i<16;i++){
    lcd.print(text[i]);
    delay(40);
  }
}

void lcdScrollTextOnce(String text, unsigned long displayMillis){
  if(text.length() <= 16){
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print(text);
    delay(displayMillis);
    return;
  }
  String scroll = text + "   ";
  unsigned long start = millis();
  int pos = 0;
  int len = scroll.length();
  while(millis() - start < displayMillis){
    String out = scroll.substring(pos, pos + 16);
    if(out.length() < 16){
      out += scroll.substring(0, 16 - out.length());
    }
    lcd.setCursor(0,1);
    lcd.print(out);
    pos++;
    if(pos >= len) pos = 0;
    delay(150);
  }
}

void lcdShowClock(){
  time_t now = time(nullptr);
  struct tm* t = localtime(&now);
  char buff[17];
  sprintf(buff, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);
  lcd.setCursor(0,0);
  lcd.print(buff);
}

// -------------------------------------------------
// NETWORK / GOOGLE SHEETS
// -------------------------------------------------
void sendToGoogleSheetsRaw(String name, String hallTicket, int id, int period, String statusIN, String statusOUT){
  if(WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    return;
  }
  
  String url = GOOGLE_SCRIPT_URL +
    "?name=" + name +
    "&hallTicket=" + hallTicket +
    "&id=" + String(id) +
    "&period=" + String(period) +
    "&statusIN=" + statusIN +
    "&statusOUT=" + statusOUT;

  url.replace(" ", "%20");

  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient http;
  if (http.begin(client, url)) {
    int code = http.GET();
    Serial.print("[HTTP] GET: "); Serial.println(code);
    http.end();
  }
}

void sendCaptureCommandToCam() {
  camSerial.println("CAPTURE");
  Serial.println("[ESP32] Sent CAPTURE to ESP32-CAM");
}

// -------------------------------------------------
// SETUP
// -------------------------------------------------
void setup(){
  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, 16, 17); // HuskyLens
  camSerial.begin(115200, SERIAL_8N1, 26, 25);

  lcd.init();
  lcd.backlight();
  lcdTypewriter("System Booting");

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  sweepServo.attach(SERVO_PIN);
  sweepServo.write(0);
  delay(500);
  sweepServo.detach();

  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    lcdShowClock();
    delay(300);
  }
  lcdTypewriter("WiFi Connected");

  while(!huskylens.begin(mySerial)){
    Serial.println("HuskyLens error!");
    delay(500);
  }
  lcdTypewriter("HuskyLens Ready");

  configTime(5*3600 + 1800, 0, "pool.ntp.org", "time.nist.gov");
  
  for(int i=1;i<=7;i++){ detectedIN[i] = false; detectedOUT[i] = false; }
  lcdShowCounts(0,0);
}

// -------------------------------------------------
// MAIN LOOP
// -------------------------------------------------
void loop(){
  if(WiFi.status() != WL_CONNECTED) WiFi.begin(ssid, password);
  
  lcdShowClock();
  time_t now = time(nullptr);
  struct tm* current = localtime(&now);

  static bool periodDone[7] = {false};

  for(int p=0; p<7; p++){
    if(current->tm_hour == periods[p].hour && current->tm_min == periods[p].minute && !periodDone[p]){
      periodDone[p] = true;
      int PER = p + 1;
      
      lcdTypewriter("Period " + String(PER) + " Start");
      for(int i=1;i<=7;i++){ detectedIN[i] = false; detectedOUT[i] = false; }

      // PHASE 1: IN
      lcdTypewriter("IN Attendance");
      unsigned long startIN = millis();
      while(millis() - startIN < ATTENDANCE_DURATION){
        lcdShowClock();
        if(!huskylens.request()) continue;
        while(huskylens.available()){
          HUSKYLENSResult result = huskylens.read();
          int id = result.ID;
          if(id > 0 && id <= 7){
            if(!detectedIN[id]){
              detectedIN[id] = true;
              Student s = getStudentFromID(id);
              sendToGoogleSheetsRaw(s.name, s.hall, id, PER, "Present", "NA");
              lcdScrollTextOnce("Welcome " + s.name, 2000);
              digitalWrite(GREEN_LED, HIGH);
              unlockDoor();
              digitalWrite(GREEN_LED, LOW);
            }
          } else {
            digitalWrite(RED_LED, HIGH);
            tone(BUZZER_PIN, 2000, 200);
            sendToGoogleSheetsRaw("Unknown","Intruder",0,PER,"Intruder","NA");
            sendCaptureCommandToCam();
            lcdTypewriter("INTRUDER!");
            delay(500);
            digitalWrite(RED_LED, LOW);
          }
        }
      }

      delay(INTER_PHASE_DELAY);

      // PHASE 2: OUT
      lcdTypewriter("OUT Attendance");
      unsigned long startOUT = millis();
      while(millis() - startOUT < ATTENDANCE_DURATION){
        lcdShowClock();
        if(!huskylens.request()) continue;
        while(huskylens.available()){
          HUSKYLENSResult result = huskylens.read();
          int id = result.ID;
          if(id > 0 && id <= 7){
            if(!detectedOUT[id]){
              detectedOUT[id] = true;
              Student s = getStudentFromID(id);
              String inS = detectedIN[id] ? "Present" : "NA";
              sendToGoogleSheetsRaw(s.name, s.hall, id, PER, inS, "Present");
              lcdScrollTextOnce("Goodbye " + s.name, 2000);
              digitalWrite(GREEN_LED, HIGH);
              unlockDoor();
              digitalWrite(GREEN_LED, LOW);
            }
          } else {
            digitalWrite(RED_LED, HIGH);
            tone(BUZZER_PIN, 2000, 200);
            sendToGoogleSheetsRaw("Unknown","Intruder",0,PER,"NA","Intruder");
            sendCaptureCommandToCam();
            lcdTypewriter("INTRUDER!");
            delay(500);
            digitalWrite(RED_LED, LOW);
          }
        }
      }

      // FINAL SYNC
      lcdTypewriter("Syncing Cloud...");
      for(int i=1;i<=7;i++){
        Student s = getStudentFromID(i);
        sendToGoogleSheetsRaw(s.name, s.hall, i, PER, detectedIN[i]?"Present":"Absent", detectedOUT[i]?"Present":"Absent");
      }
      lcdTypewriter("Done!");
      delay(1000);
    }
    if(current->tm_min != periods[p].minute) periodDone[p] = false;
  }
  
  lcdShowCounts(countArrayPresent(detectedIN), countArrayPresent(detectedOUT));
  delay(100); 
}

int countArrayPresent(bool arr[]){
  int c = 0;
  for(int i=1;i<=7;i++) if(arr[i]) c++;
  return c;
}
