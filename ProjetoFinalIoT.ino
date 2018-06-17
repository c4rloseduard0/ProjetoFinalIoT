#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Ultrasonic.h>
/*
char blynkAuth[] = "db573e62b4594161acec725b43b10332";
String keyThignkSpeak = "MK8HO6U55RJB3FVM";

*/char ssid[] = "AP 303";
char pass[] = "81463544";

char server[] = "api.thingspeak.com";
/*int pinValue;

//#define BLYNK_PRINT Serial
#define SERVO_PIN 32
#define TRIGGER 4
#define ECHO 2
#define LM35 A0*/
#define LDR1 25
#define LDR2 26

/*int freq = 50;
int channel = 0;
int resolution = 8;
int dutyCycle = 15;
*/
//float cmMsec;
//float temperatura;
int ldr1, ldr2;

//WiFiClient client;
//Ultrasonic ultrasonic(TRIGGER, ECHO);

/*BLYNK_WRITE(V1)
{
  pinValue = param.asInt(); 
}*/

void setup(){
  Serial.begin(115200);
 // Blynk.begin(blynkAuth, ssid, pass);

  //ledcSetup(channel, freq, resolution);
  //ledcAttachPin(SERVO_PIN, channel);
  //ledcWrite(channel, dutyCycle);

  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin("AP 303", "81463544");
/*  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }*/
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop(){
  //Blynk.run();
  /*if ( pinValue == 1) {
    move_porta();      
  }*/
  
  //cmMsec = ultrasonic.convert(ultrasonic.timing(), Ultrasonic::CM);
  //temperatura = analogRead(LM35)/ 2048.0 * 3300*0.1;
  ldr1 = analogRead(25);
  //delay(100);
  ldr2 = analogRead(26);
  ldr1 = map(ldr1, 0, 4095, 0, 1023);
  ldr2 = map(ldr2, 0, 4095, 0, 1023);
  Serial.print("LDR1: ");
  Serial.println(ldr1);
  Serial.print("LDR2: ");
  Serial.println(ldr2);
  //float medLdr = (ldr1 + ldr2) / 2;
  //enviaValores(cmMsec, temperatura, ldr2);
   
}

/*void move_porta() {
  int pos;
  for ( pos = 15; pos <= 33; pos += 1) { 
    ledcWrite(channel, pos); 
    delay(30);
  }
  
  delay(2000);
  
  for (; pos >= 15; pos -= 1) { 
    ledcWrite(channel, pos);              
    delay(30);                    
  }
}
void enviaValores(float cmMsec, float temperatura, float ldr){
      
  if (client.connect(server,80)){  
    String postStr = keyThignkSpeak;
    postStr +="&field1="; 
    postStr += String(cmMsec);
    postStr +="&field2="; 
    postStr += String(temperatura);
    postStr +="&field3="; 
    postStr += String(ldr);
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+keyThignkSpeak+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
  }
  client.stop(); 
  Serial.print("Distância: ");
  Serial.println(cmMsec);
  Serial.print("Temperatura: ");
  Serial.println(temperatura);
  Serial.print("LDR: ");
  Serial.println(ldr);
  Serial.println("----------------------");
  Serial.println("Waiting...");
  delay(1000);
}*/
