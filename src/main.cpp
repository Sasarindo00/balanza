#include <Wire.h>
#include <TimeLib.h>
#include <Arduino.h>
#include "HX711.h"
#include <LiquidCrystal.h>

#define DEBUG_HX711

// Parámetro para calibrar el peso y el sensor
#define CALIBRACION 474500.5//487525.5 //487000.0

// Pin de datos y de reloj
byte pinData = 7;
byte pinClk = 8;
int tara = A2;
int modo = A1;
int tim = A0;
int k = 0;
float valor= 0;
int mi=0, se=0, t=0;
int start;
int intervaloTemp = 1000; // leeremos cada 1 segundos
long tAnterior = 0; // almacena la ultima vez que se lanzo nuestro tiempo

// led
const int rs = 12, en = 11, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


//anti rebote de botones

/**
 * @brief
 *
 * @param boton
 */
void anti_debounce(byte boton){
		delay(100);
		while (digitalRead(boton));
		delay(100);

}
// medir el tiempo
void tiempo (){
		lcd.setCursor(11, 0);
		lcd.print("mi");
		lcd.setCursor(12, 1);
		lcd.print(mi);
		lcd.setCursor(13, 0);
		lcd.print(":");
		lcd.setCursor(13, 1);
		lcd.print(":");
		lcd.setCursor(14, 0);
		lcd.print("se");
		lcd.setCursor(14, 1);
		lcd.print(se);
		Serial.print(mi);
		Serial.print(":");
		Serial.print(se);
		se++;
		if(se>59) {
				se=0;
				lcd.clear();
				mi++;
		}

}

// Objeto HX711
HX711 bascula;
void setup() {

		pinMode(tara, INPUT);
		pinMode(modo, INPUT);
		pinMode(tim, INPUT);
// inicia elled en numerode columnas y filas
		lcd.begin(16, 2);

		Serial.begin(9600);
#ifdef DEBUG_HX711

		// Iniciar comunicación serie

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

		if (k==0) {
#ifdef DEBUG_HX711
				valor= bascula.get_units();
				lcd.setCursor(0, 0);
				lcd.print("Peso: ");
				lcd.setCursor(0, 1);
				lcd.print(valor);
				lcd.setCursor(5, 0);
				lcd.print(" kg");
				Serial.print("[HX7] Leyendo: ");
				Serial.print(valor, 3);
				Serial.print(" Kg     ");
#endif
		}
		if (k==1) {
#ifdef DEBUG_HX711
				valor= bascula.get_units();
				lcd.setCursor(0, 0);
				lcd.print("Peso: ");
				lcd.setCursor(0, 1);
				lcd.print(valor*1000,0);
				lcd.setCursor(5, 0);
				lcd.print(" gr");
				Serial.print("[HX7] Leyendo: ");
				Serial.print(valor*1000,1);
				Serial.print(" gr     ");
#endif
		}
		if (digitalRead(modo) && k==0) {
				anti_debounce(modo);
				Serial.println("mod1");
				k=1;
				lcd.clear();
		}
		if (digitalRead(modo) && k==1) {
				anti_debounce(modo);
				Serial.println("mod2");
				k=0;
				lcd.clear();
		}
		if (digitalRead(tara)) {
				anti_debounce(tara);
				Serial.println("tara");
				bascula.tare();

		}
		if (digitalRead(tim) && t==0) {
				anti_debounce(tim);
				t=1;

		}
		if (digitalRead(tim) && t==1) {
				anti_debounce(tim);
				t=0;
				se=0; mi=0;
				lcd.clear();
		}
		long tActual = millis();
		if( t ==1) {
				if (tActual - tAnterior >= intervaloTemp) {
						// Han pasado los 1 segundos => lanzamos el evento
						// IMPRIMIR POR CONSOLA ESTE VALOR
						tiempo();
						// Se actualiza el temporizador para el siguiente 1 segundos
						tAnterior = tActual;
						//delay(1000);
				}
		}
		Serial.println();
}