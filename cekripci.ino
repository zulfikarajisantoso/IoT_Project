
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <Wire.h>
#include "RTClib.h"
##include "CTBot.h" //Pendeklarasian Library
#define DHTPIN 13 // DHT11
#define DHTTYpe DHT11
DHT dht(DHTPIN, DHTTYpe);

CTBot myBot;
TBMessage msg;

RTC_DS3231 rtc;
//DS3231 rtc(SDA,SCL);
char ssid[] = "ketombegatal";              // Enter your WIFI name
char pass[] = "ciumdulu";        // Enter your WIFI password

  
//var token dan id tele
String token = "6111511620:AAHeZkpQ9XlZeSS8Ydq_zMEaGYoIKJ_ltec";
const int id = 1196474960;

String statussuhu;
String statuskelembapan;
String statuskipas;
String statuslampu;

void setup() 
{
    Serial.begin(115200); 
    Wire.begin(); // Start the I2C
    rtc.begin();  // Init RTC
    

// koneksi ke wifi
   mybot.wifiConnect(ssid, pass);
  
//  set token tele
  mybot.setTelegramToken(token);
  
//  cek koneksi wifi my bot
  if(mybot.testConnection())
    Serial.println("Koneksi Berhasil");
  else
    Serial.println("koneksi gagal");
//    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
//   rtc.adjust(DateTime(2023, 4, 8, 10, 29, 0));
  dht.begin(); 
}

void Kondisi(){
 int h = dht.readHumidity();
 int t = dht.readTemperature(); 

   if(t > 33) {     
      statussuhu = "Panas";
      statuskipas = "Nyala";
    }
      else if (t < 10) {
      statuskelembapan = "Dingin";
       statuskipas = "Nyala";
      }
    else{
      statussuhu = "Aman";
       statuskipas = "Mati";
    }
     if(h > 65) {     
      statuskelembapan = "Tinggi";
       statuslampu = "Nyala";
    }
    else{
      statuskelembapan = "Aman";
      statuslampu = "Mati";
    }
}

void jam(){
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

void pesan(){
  
  int h = dht.readHumidity();
  int t = dht.readTemperature(); 
  
  }

void loop() {
//  int h = dht.readHumidity();
//  int t = dht.readTemperature();
//  DateTime now = rtc.now(); 

  TBMessage msg;
  
  if(mybot.getNewMessage(msg))
  { 
    String pesan = msg.text;

      if(pesan == "/start")
      {
        String reply = "";
        reply += "Hi Aji Anti Air \n";
        reply += "Pilih Menu Dibawah : \n";
        reply += "Lihat Status Suhu : /suhu \n";
        reply += "Lihat Status Kelembapan : /kelembapan \n";
        reply += "Lihat Status Alat : /alat \n";
        reply += "Lihat Update Camera : /camera \n"; 
        mybot.sendMessage(id, reply);
      }
      
//      if(pesan == "/suhu")
//      {     
//        String temp = "Suhu : ";
//          temp += "Udara "+ statussuhu + " di angka ";
//          temp += int(t);
//          temp += " *C\n"; 
//          temp += "Kipas : "+statuskipas ;       
//       mybot.sendMessage(id, temp);
//      }
//      
//      if( pesan == "/kelembapan")
//      {      
//        String temp = "Suhu : ";        
//          temp += "Kelembapan udara "+ statuskelembapan + " di angka ";
//          temp += int(h);
//          temp += " %\n";
//          temp += "Lampu : " + statuslampu;                 
//       mybot.sendMessage(id, temp);
//      }
   }
delay(2000);

//BOt 
//  if(now.minute() == 43  && now.second() < 10 ) {       
//      String temp = "Update Suhu dan Kelembapan Gudang : \n";    
//      temp += "Suhu ";temp += int(t);temp += "  *C\n";
//      temp += "Kelembapan ";temp += int(h);temp += "  %";
//      mybot.sendMessage(id, temp);   
//  } 
}   
