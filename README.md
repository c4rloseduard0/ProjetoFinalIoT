# Alimentador de Cães e Gatos

## Sumário

1. [Objetivo](#objetivo)
2. [Material Utilizado](#material-utilizado)
3. [Montagem](#montagem)  
3.1 [LDR](#ldr)  
3.2 [Servo](#servo)  
3.3 [Sensor de Temperatura](#sensor-de-temperatura)  
3.4 [Sensor Ultrasonico](#sensor-ultrassonico)  
3.5 [Projeto Final](#projeto-final)
4. [Funções](#funções)  
4.1 [setup()](#setup)  
4.2 [loop()](#loop)  
4.3 [move_porta()](#move_porta)  
4.4 [enviaValores()](#enviavalores-float-cmmsec-float-temperatura-float-ldr)  
4.5 [callback()](#callback-char-topic-byte-payload-unsigned-int-length)  
4.6 [enviaBlynk()](#enviablynk-float-cmmsec-float-temperatura-float-ldr)  

## Objetivo
Este projeto visa automatizar a alimentação de animais domésticos usando recursos online e bluetooth

## Material Utilizado

* 1 Servo Motor 9g
* 1 sensor ultrasônico HC-SR04
* 1 Sensor de temperatura LM35
* 2 Sensor de luminosidade LDR
* 1 ESP32
* 2 Resistores 10k Ω
* 1 Protoboard
* Alguns jumpers

## Montagem
OBS.: As imagens têm fim puramente didático, os sensores não ficaram juntos à protoboard, mas sim distribuídos por toda a maquete.
### LDR
![Ilustração dos LDR's](https://raw.githubusercontent.com/c4rloseduard0/ProjetoFinalIoT/master/img/mont_ldr.png)

### Servo
![Ilustração do Servo](https://raw.githubusercontent.com/c4rloseduard0/ProjetoFinalIoT/master/img/mont_servo.png)

### Sensor de Temperatura
![Ilustração do LM35](https://raw.githubusercontent.com/c4rloseduard0/ProjetoFinalIoT/master/img/mont_lm35.png)

### Sensor Ultrassonico
![Ilustração do hc-sr04](https://raw.githubusercontent.com/c4rloseduard0/ProjetoFinalIoT/master/img/mont_somsensor.png)

### Projeto Final
![Ilustração p.f.](https://raw.githubusercontent.com/c4rloseduard0/ProjetoFinalIoT/master/img/mont_geral.png)

## Funções
#### setup()
Esta função é padrão, nela são feitas a inicialização dos PIN's, do terminal serial, assim como as conexões com Wifi e servidores 

#### loop()
Assim como a setup, a função loop também é padrão, ela é a função principal do código, e fica executando constatemente como um laço infinito

#### move_porta()
Essa função é responsável por controlar o servo motor que abrirá a porta por onde a comida sairá, todas as vezes que ela é chamada o Servo vai se mover 90º, aguardar, e retornar para a posição inicial

#### enviaValores (float cmMsec, float temperatura, float ldr)
Essa função é responsável por enviar os valores lidos pelos sensores para o [ThinkSpeak](https://thingspeak.com/). O primeiro parâmetro é o valor lido pelo sensor ultrassônico, o segundo é o valor lido pelo sensor de temperatura e o terceiro é o valor lido pelo LDR. A função é chamada a cada 20 segundos, então ela se conecta ao servidor do [ThinkSpeak](https://thingspeak.com/), monta um header HTTP fazendo uma requisição do tipo POST, e então envia os valores lidos

#### callback (char\* topic, byte\* payload, unsigned int length)
A função callback é usada pelo do objeto da classe PubSubClient, para receber os valores enviados pelo [CloudMQTT](https://www.cloudmqtt.com/) e escolher a partir do tópico e da mensagem qual sensor deve ser ativado ou desativado

#### enviaBlynk (float cmMsec, float temperatura, float ldr)
A função enviaBlynk é similar a função enviaDados, ela recebe os mesmos parâmetros e os envia ao app [Blynk](https://www.blynk.cc/)
