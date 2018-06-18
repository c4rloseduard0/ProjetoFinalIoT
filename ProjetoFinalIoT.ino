#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Ultrasonic.h>
#include <PubSubClient.h>

//Declaração das API keys blynk, thinkspeak, cloudmqtt)
char blynkAuth[] = "db573e62b4594161acec725b43b10332";
String keyThignkSpeak = "MK8HO6U55RJB3FVM";

//Credenciais para fazer login na rede
char ssid[] = "AP 303";
char pass[] = "81463544";

//Servidores
char server[] = "api.thingspeak.com";
const char* mqttServer = "m14.cloudmqtt.com";

int pinValue; //Variavel que pega o valor do botão virtual do blynk

void mqtt_callback(char* topic, byte* payload, unsigned int length);

//Definição dos pinos
#define BLYNK_PRINT Serial
#define SERVO_PIN 32
#define TRIGGER 4
#define ECHO 2
#define LM35 A0
#define LDR1 34
#define LDR2 35

//Variaveis de controle
int istrueLdr, istrueTemp, istrueSom, istrueTodos;

//Variaveis usadas para inicialização do servo
int freq = 50;
int channel = 0;
int resolution = 8;
int dutyCycle = 15;

//Variáveis dos sensores
float cmMsec;
float temperatura;
int ldr1, ldr2;
float medLdr;

//Variaveis do cloudMQTT
const char* mqttUser = "ilnczjao";
const int mqttPort = 15336;
const char* mqttPassword = "ZpofjvUKX0NS";

WiFiClient tsclient;
WiFiClient client; //Objeto da classe WifiClient
Ultrasonic ultrasonic(TRIGGER, ECHO); //Inicia o sensor ultrassônico
PubSubClient mqttClient(client); //Objeto da classe PubsubClient

BLYNK_WRITE(V1){ //Retorna o valor do botão virtual 
  pinValue = param.asInt(); 
}

BLYNK_WRITE(V5){ //Retorna o valor do botão virtual 
  istrueLdr = param.asInt(); 
}

BLYNK_WRITE(V6){ //Retorna o valor do botão virtual 
  istrueTemp = param.asInt(); 
}

BLYNK_WRITE(V7){ //Retorna o valor do botão virtual 
  istrueSom = param.asInt(); 
}

void setup(){
  Serial.begin(115200);  //Inicia o terminal Serial
  Blynk.begin(blynkAuth, ssid, pass); //conecta com o servidor do blynk

  //Inicialização do Servo motor
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(SERVO_PIN, channel);
  ledcWrite(channel, dutyCycle);

  //Conecta à rede
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(callback);

  while (!mqttClient.connected()) {
    Serial.println("Connecting to MQTT...");
     if (mqttClient.connect("ESP32Client", mqttUser, mqttPassword )) {
      Serial.println("connected");   
     }else {
      Serial.print("failed with state ");
      Serial.print(mqttClient.state());
      delay(2000);
    }
  }

  mqttClient.publish("liga","desliga");
  mqttClient.subscribe("ldr");
  mqttClient.subscribe("sonic");
  mqttClient.subscribe("temp");
  
  
}

//Função que move o servo -> abre a porta para liberar comida
void move_porta() {
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

//Função que envia os dados dos sensores ao ThinkSpeak
void enviaValores(float cmMsec, float temperatura, float ldr){
      
  if (tsclient.connect(server,80)){  
    String postStr = keyThignkSpeak;
    postStr +="&field1="; 
    postStr += String(cmMsec);
    postStr +="&field2="; 
    postStr += String(temperatura);
    postStr +="&field3="; 
    postStr += String(ldr);
    tsclient.print("POST /update HTTP/1.1\n");
    tsclient.print("Host: api.thingspeak.com\n");
    tsclient.print("Connection: close\n");
    tsclient.print("X-THINGSPEAKAPIKEY: "+keyThignkSpeak+"\n");
    tsclient.print("Content-Type: application/x-www-form-urlencoded\n");
    tsclient.print("Content-Length: ");
    tsclient.print(postStr.length());
    tsclient.print("\n\n");
    tsclient.print(postStr);
  }
  tsclient.stop(); 
  Serial.print("Distância: ");
  Serial.println(cmMsec);
  Serial.print("Temperatura: ");
  Serial.println(temperatura);
  Serial.print("LDR: ");
  Serial.println(ldr);
  Serial.println("----------------------");
  Serial.println("Waiting...");
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida no tópico: ");
  Serial.println(topic);
 
  Serial.print("Mensagem:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    String msg;
    
    //obtem a string do payload recebido
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
    }

    if (msg.equals("LU")) {
      istrueSom = 1;
    }

    if (msg.equals("DU")) {
      istrueSom = 0;
    }

    if (msg.equals("LT")) {
      istrueTemp = 1;
    }

    if (msg.equals("DT")) {
      istrueTemp = 0;
    }

    if (msg.equals("LL")) {
      istrueLdr = 1;
      Serial.println("Passou por aqui");
    }

    if(msg.equals("DL")) {
      istrueLdr = 0;
    }

    if (msg.equals("LA")){
      istrueLdr =  1;
      istrueTemp = 1;
      istrueSom = 1;
    }

    if (msg.equals("DA")){
      istrueLdr =  0;
      istrueTemp = 0;
      istrueSom = 0;
    }
  }
}

void enviaBlynk(float cmMsec, float temperatura, float ldr){
  Blynk.virtualWrite(V2, ldr);         //Envia os dados do sensor de luminosidade para a porta Vitual 2
  Blynk.virtualWrite(V3, temperatura); //Envia os dados do sensor de temperatura para a porta Vitual 3
  Blynk.virtualWrite(V4, cmMsec);      //Envia os dados do sensor ultrassonico para a porta Vitual 4
}

long previousMillis = 0;
void loop(){
  unsigned long currentMillis = millis();
  Blynk.run();
  mqttClient.loop();
  if ( pinValue == 1) {
    move_porta();      
  }

  if(istrueLdr){
    ldr1 = analogRead(LDR1);
    ldr2 = analogRead(LDR2);
    ldr1 = map(ldr1, 0, 4095, 0, 1023);
    ldr2 = map(ldr2, 0, 4095, 0, 1023);
    medLdr = (ldr1 + ldr2) / 2;
  }else{
    ldr1 = 0;
    ldr2 = 0;
    medLdr = 0;
  }

  if(istrueSom){
    cmMsec = ultrasonic.convert(ultrasonic.timing(), Ultrasonic::CM);
  }else{
    cmMsec = 0;
  }

  if(istrueTemp){  
    temperatura = analogRead(LM35)/ 2048.0 * 3300*0.1;
  }else{
    temperatura = 0;
  }
  Serial.print("Distância: ");
  Serial.println(cmMsec);
  Serial.print("Temperatura: ");
  Serial.println(temperatura);
  Serial.print("LDR: ");
  Serial.println(medLdr);
  if (currentMillis - previousMillis > 15000) {
    enviaValores(cmMsec, temperatura, medLdr);
    enviaBlynk(cmMsec, temperatura, medLdr);
    previousMillis = currentMillis;
  }
}
