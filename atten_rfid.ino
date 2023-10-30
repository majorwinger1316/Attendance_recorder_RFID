#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define Buzzer 2
#define LED1 3
#define LED2 4

MFRC522 mfrc522(SS_PIN, RST_PIN);

byte att1 = 0;
byte att2 = 0;

unsigned long startTime = 0;
bool timerRunning = false;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  
  Serial.println("RFID Attendance System");
  Serial.println("Scan your RFID card to start the timer.");
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(Buzzer, OUTPUT);
}

void loop() {
  digitalWrite(LED1, HIGH);
  if ( ! mfrc522.PICC_IsNewCardPresent())
    return;
  if ( ! mfrc522.PICC_ReadCardSerial())
    return;
  
  String ID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    ID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    ID.concat(String(mfrc522.uid.uidByte[i], HEX));
    delay(100);
  }
  Serial.println();
  ID.toUpperCase();
  if (ID.substring(1) == "83 25 E7 FA" && att1 == 0 ) {
        startTimer();
        tone(Buzzer, 12000);
        delay(200);
        noTone(Buzzer);
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, HIGH);
        delay(500);
        digitalWrite(LED2, LOW);
        att1 = 1;
  } else if (ID.substring(1) == "83 25 E7 FA" && att1 == 1 ) {
        stopTimer();
        tone(Buzzer, 12000);
        delay(400);
        noTone(Buzzer);
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, HIGH);
        delay(800);
        digitalWrite(LED2, LOW);
        att1 = 0;
  } if (ID.substring(1) == "E3 B9 4F F4" && att2 == 0 ) {
        startTimer();
        tone(Buzzer, 12000);
        delay(200);
        noTone(Buzzer);
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, HIGH);
        delay(500);
        digitalWrite(LED2, LOW);
        att2 = 1;
  } else if (ID.substring(1) == "E3 B9 4F F4" && att2 == 1 ) {
        stopTimer();
        tone(Buzzer, 12000);
        delay(400);
        noTone(Buzzer);
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, HIGH);
        delay(800);
        digitalWrite(LED2, LOW);
        att2 = 0;
  }
}

void startTimer() {
  startTime = millis();
  timerRunning = true;
  Serial.println("Timer started.");
}

void stopTimer() {
  unsigned long endTime = millis();
  unsigned long elapsedTime = endTime - startTime;
  timerRunning = false;
  Serial.print("Timer stopped.\nElapsed time: ");
  Serial.print(elapsedTime/1000);
  Serial.print(" Seconds\n");
  if (elapsedTime >= 10000) { 
    markPresent();
  } else {
    markAbsent();
  }
}

void markPresent() {
  Serial.println("Person marked as present.");
}

void markAbsent() {
  Serial.println("Person marked as absent.");
}
