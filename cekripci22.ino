
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <DHT.h>
#include <Wire.h>
#include "RTClib.h"

#define relay1 D3
#define relay2 D4

#define DHTPIN D7 // DHT11
#define DHTTYpe DHT11
DHT dht(DHTPIN, DHTTYpe);

RTC_DS3231 rtc;

// Wifi network station credentials
#define WIFI_SSID "wifilaptop"
#define WIFI_PASSWORD "password"

// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "5896750206:AAEsmhlFwnf7WCuxmeJ6sYOUO2otMaLNzn4"
String id = "5920715406";
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);

String statussuhu;
String statuskelembapan;
String statuskipas;
String statuslampu;

void handleNewMessages(int numNewMessages)
{
  for (int i = 0; i < numNewMessages; i++)
  {
    DateTime now = rtc.now(); 
    int h = dht.readHumidity();     
    int t = dht.readTemperature();
    String text = bot.messages[i].text;
    if(text == "/start")
    {
       String reply = "";
        reply += "Selamat Datang Pada Sistem Monitoring Keamanan, Suhu dan Kelembapan Gudang Beras \n";
        reply += "Pilih Menu Dibawah : \n";
        reply += "Lihat Status Suhu : /suhu \n";
        reply += "Lihat Status Kelembapan : /kelembapan \n";
        reply += "Lihat Situasi Gudang : /takefoto \n"; 
       bot.sendMessage(id, reply, "");
     }
    else if(text == "/suhu")
    {
       String reply = "";
          reply += "Suhu Udara "+ statussuhu + " di angka ";
          reply += int(t);
          reply += " *C\n"; 
          reply += "Kipas : "+ statuskipas ;   
       bot.sendMessage(id, reply, "");
     }
     else if(text == "/kelembapan")
     {      
        String reply = "";
          reply += "Kelembapan udara "+ statuskelembapan + " di angka ";
          reply += int(h);
          reply += " %\n"; 
          reply += "Lampu : "+ statuslampu ;   
       bot.sendMessage(id, reply, "");
      }
      else if(text == "/takefoto")    
     {      
        String reply = "";
         reply += "Untuk melihat situasi gudang secara langsung \n"; 
         reply += "Silahkan Beralih Ke Bot Telegram Khusus Galeri \n";
            reply += "Klik : @Galerigudangberas_bot \n";
        
         
         bot.sendMessage(id, reply, "");
      }
      else 
      {        
       String reply = "";
          reply += "Perintah Tidak Ada : Klik Perintah /start ";
          bot.sendMessage(id, reply, "");
      }     
  }
}


void setup()
{
  Serial.begin(115200);
  Wire.begin();  
  rtc.begin();  // Init RTC
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);

  
//  terhubung ke wifi
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  client.setTrustAnchors(&cert); // menambahkan root sertifikat untuk api.telegram.org
  
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);

// reset RTC
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // atur waktu otomatis
//  rtc.adjust(DateTime(2023, 4, 8, 10, 29, 0)); //atur waktu manual
  dht.begin(); 
}

void Kondisi(){
 int h = dht.readHumidity();
 int t = dht.readTemperature(); 
  //suhu
   if(t >= 31) {     
      statussuhu = "Panas";
      statuskipas = "Nyala";
      digitalWrite(relay1, HIGH);
    }
    else {
        statussuhu = "Aman";
       statuskipas = "Mati";
       digitalWrite(relay1, LOW);
     
    }
  // kelembapan
     if(h >= 66) {     
      statuskelembapan = "Tinggi";
      statuslampu = "Nyala";
      digitalWrite(relay2, HIGH);
    }
    else{
      statuskelembapan = "Aman";
      statuslampu = "Mati";
      digitalWrite(relay2, LOW);
    }
}

void Timer(){
  DateTime now = rtc.now(); 
   //Menampilkan RTC pada variable now
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  delay(4000);
}


void loop()
{
  Kondisi();
  Timer();
  DateTime now = rtc.now(); //waktu sekarang untuk rtc
  int h = dht.readHumidity();     
  int t = dht.readTemperature();
  
//RTC
 if (now.hour()== 9 && now.minute() == 10  && now.second() < 15){
      String temp = "Update Suhu dan Kelembapan Gudang : \n";    
      temp += "Suhu : ";temp += int(t);temp += " *C\n";
      temp += "Kelembapan : ";temp += int(h);temp += " %"; 
      bot.sendMessage(id, temp, ""); 
   }
    if (now.hour()== 15 && now.minute() == 10  && now.second() < 15){
      String temp = "Update Suhu dan Kelembapan Gudang : \n";    
      temp += "Suhu : ";temp += int(t);temp += " *C\n";
      temp += "Kelembapan : ";temp += int(h);temp += " %"; 
      bot.sendMessage(id, temp, ""); 
   }
    if (now.hour()== 21 && now.minute() == 10  && now.second() < 15){
      String temp = "Update Suhu dan Kelembapan Gudang : \n";    
      temp += "Suhu : ";temp += int(t);temp += " *C\n";
      temp += "Kelembapan : ";temp += int(h);temp += " %"; 
      bot.sendMessage(id, temp, ""); 
   }
// coba
//    if ( now.minute() == 47  && now.second() < 15){
//      String temp = "Update Suhu dan Kelembapan Gudang : \n";    
//      temp += "Suhu : ";temp += int(t);temp += " *C\n";
//      temp += "Kelembapan : ";temp += int(h);temp += " %"; 
//      bot.sendMessage(id, temp, ""); 
//   }

//BOT
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }    
}
