#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Servo.h>

// Inisialisasi Pin
#define SS_PIN 10
#define RST_PIN 9
#define led_merah 2
#define led_biru 3
#define buzzer 7

MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd (0x27,16,2);
Servo myServo;

// init array
String data[2][2] = {
  {"Ahmad Zuhril", "2081215337"}, //37 - whiteCard
  {"Nando Septian", "12812813747"}, //47 - blueCard
}; 

String UID;
bool check;
bool off = LOW;
bool on = HIGH;

void setup() {
  lcd.init();
  lcd.begin(16, 2);
  lcd.backlight();
  myServo.attach(8);
  myServo.write(90);
  pinMode(led_merah, OUTPUT);
  digitalWrite(led_merah, off);
  pinMode(led_biru, OUTPUT);
  digitalWrite(led_biru, off);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, off);
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  lcd.setCursor(0,0);
  lcd.println("Tempelkan Kartu!!");
}

void loop() {
  // cek kartu ada/tidak
  if (!rfid.PICC_IsNewCardPresent() || rfid.PICC_ReadCardSerial()) return;

  // Verifikasi / menerima 1x input
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  // gabung hasil string jadi satu
  for(int i = 0; i <= 3; i++) {
    UID += String(rfid.uid.uidByte[i]);
  } 

  // cek data / tampilkan 
  tampilkanData();

  // Set UID jadi kosong
  UID = "";
}

void(*reset)(void) = 0; //reset arduino

void tampilkanData() {
  for(int i = 0; i < 1; i++) {
    if(data[i][1] == UID) {
      Serial.println("Data Valid");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("  Data Sudah  ");
      lcd.setCursor(0,1);
      lcd.print("  Terdaftar!!  ");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(" Selamat Datang ");
      lcd.setCursor(0, 1);
      lcd.print(" " + data[i][0] + " "); //show nama id card yang terdaftar

      // led biru nyala, servo & buzzer
      digitalWrite(led_biru, on);
      myServo.write(0);
      delay(1000);
      digitalWrite(buzzer, on);
      delay(300);
      digitalWrite(led_biru, off);
      digitalWrite(buzzer, off);
      delay(1500);
      lcd.clear();
      myServo.write(90);
      check = true;
      break;
    }
    check = false;
  }

  if(!check) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("  Data Belum  ");
    lcd.setCursor(0,1);
    lcd.print("  Terdaftar!!  ");

    // led Merah Nyala
    digitalWrite(led_merah, on);
    digitalWrite(buzzer, on);
    delay(600);
    digitalWrite(led_merah, off);
    digitalWrite(buzzer, off);
    delay(500);

    // Show lcd gagal
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Hubungi Admin");
    lcd.setCursor(0,1);
    lcd.print("Agar Didaftarkan!");
    delay(2500);
    reset();
  }
}



