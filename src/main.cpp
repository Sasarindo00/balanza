#include <Arduino.h>
#include "HX711.h"
 
#define DEBUG_HX711
 
// Parámetro para calibrar el peso y el sensor
#define CALIBRACION 487000.0
 
// Pin de datos y de reloj
byte pinData = 3;
byte pinClk = 4;
int tara = 7;
int modo = 8;
int time = 9;
int k=0;
float valor= 0;
void anti_debounce(byte boton){
  delay(100);
  while (digitalRead(boton));
  delay(100);
}


// Objeto HX711
HX711 bascula;
 
void setup() {
 
#ifdef DEBUG_HX711
  pinMode(tara, INPUT);
  pinMode(modo, INPUT);
  pinMode(time, INPUT);
  // Iniciar comunicación serie
  Serial.begin(9600);
  Serial.println("[HX7] Inicio del sensor HX711");
#endif
 
  // Iniciar sensor
  bascula.begin(pinData, pinClk);
  // Aplicar la calibración
  bascula.set_scale(CALIBRACION);
  // Iniciar la tara
  // No tiene que haber nada sobre el peso
  bascula.tare();
}
 
void loop() {
#ifdef DEBUG_HX711
valor= bascula.get_units();
if(digitalRead(modo) && k==0){
  Serial.print("[HX7] Leyendo: ");
  Serial.print(valor, 3);
  Serial.print(" Kg");
  Serial.println();
  k=1;
}else{
  if(digitalRead(modo) && k==1){
     Serial.print("[HX7] Leyendo: ");
  Serial.print(valor*1000, 0);
  Serial.print(" gr");
  Serial.println(); 
  k=0;
  }
}
#endif

if (digitalRead(tara)){
    anti_debounce(tara);
    Serial.println("tara");
    bascula.tare();
  }
}