#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"

//Declaremos los pines CE y el CSN
#define CE_PIN 9
#define CSN_PIN 10

//Variable con la dirección del canal por donde se va a transmitir
byte direccion[5] ={'c','a','n','a','l'};

//creamos el objeto radio (NRF24L01)
RF24 radio(CE_PIN, CSN_PIN);

const int ID_machine = 101;

//vector con los datos a enviar
int datos[5] = {ID_machine, 0, 0, 0, 0};

int value_btn_mantto = 0;
int value_btn_calidad = 0;
int value_btn_operador = 0;

int tiempo_inicio = 0;
int tiempo_final = 0;
int total = 0;

bool band_btn_mantto = false;
bool band_btn_calidad = false;
bool band_btn_operador = false;
bool ok = false;

bool band_connection = false;


int estado_act_btn_mantto;
int estado_ant_btn_mantto;
unsigned long temporizador_mantto;

int estado_act_btn_calidad;
int estado_ant_btn_calidad;
unsigned long temporizador_calidad;

int estado_act_btn_operador;
int estado_ant_btn_operador;
unsigned long temporizador_operador;

unsigned long tiemporebote = 50;

int torreta_mantto = 5;
int torreta_calidad = 6;
int torreta_operador = 7;

void setup() {
  printf_begin();
  //inicializamos el NRF24L01 
  radio.begin();
  
//Abrimos un canal de escritura
 radio.openWritingPipe(direccion);
 
  Serial.begin(9600);   //iniciar puerto serie
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  
  pinMode(torreta_mantto, OUTPUT);
  pinMode(torreta_calidad, OUTPUT);
  pinMode(torreta_operador, OUTPUT);

  estado_act_btn_mantto = HIGH;
  estado_ant_btn_mantto = HIGH;

  estado_act_btn_calidad = HIGH;
  estado_ant_btn_calidad = HIGH;

  estado_act_btn_operador = HIGH;
  estado_ant_btn_operador = HIGH;
}

void loop(){
  if (band_connection == false)
  {
    for (int i=0; i<10; i++)
    {
      datos[4] = 1;
      Serial.println("ID: " + (String)datos[0] + " Mantto: " + (String)datos[1] + " Calidad: " + (String)datos[2] + " Operador: " +(String)datos[3] + " New: " + (String)datos[4]);
      radio.write(datos, sizeof(datos));
      delay(100);
    }
    band_connection = true;
  }
  
  value_btn_mantto = digitalRead(2);
  value_btn_calidad = digitalRead(3);
  value_btn_operador = digitalRead(4);

  
 /* 
 + BOTON MANTENIMIENTO 
 */ 
  if ( estado_act_btn_mantto == value_btn_mantto ) 
  {
    temporizador_mantto = 0;
  }
  else 
  {
    if ( temporizador_mantto == 0 ) {
      temporizador_mantto = millis();
    }
    else 
    {
      if ( millis()-temporizador_mantto > tiemporebote ) {
          estado_act_btn_mantto = !estado_act_btn_mantto;
        }
     }
  
  if ( estado_ant_btn_mantto == HIGH && estado_act_btn_mantto == LOW )
  {
    band_btn_mantto = true;
    if(datos[1] == 1)
      {
        datos[1] = 0;
        digitalWrite(torreta_mantto, LOW);
      }
      else{ 
        datos[1] = 1;
        digitalWrite(torreta_mantto, HIGH);

        datos[2] = 0;
        datos[3] = 0;
        digitalWrite(torreta_calidad, LOW);
        digitalWrite(torreta_operador, LOW);
      } 
  }
  estado_ant_btn_mantto = estado_act_btn_mantto;
}

/* 
 + BOTON CALIDAD 
 */
if ( estado_act_btn_calidad == value_btn_calidad ) 
  {
    temporizador_calidad = 0;
  }
  else 
  {
    if ( temporizador_calidad == 0 ) {
      temporizador_calidad = millis();
    }
    else 
    {
      if ( millis()-temporizador_calidad > tiemporebote ) {
          estado_act_btn_calidad = !estado_act_btn_calidad;
        }
     }
  
  if ( estado_ant_btn_calidad == HIGH && estado_act_btn_calidad == LOW )
  {
    band_btn_calidad = true;
    if(datos[2] == 1)
      {
        datos[2] = 0;
        digitalWrite(torreta_calidad, LOW);
      }
      else{ 
        datos[2] = 1;
        digitalWrite(torreta_calidad, HIGH);

        datos[1] = 0;
        datos[3] = 0;
        digitalWrite(torreta_mantto, LOW);
        digitalWrite(torreta_operador, LOW);
      } 
  }
  estado_ant_btn_calidad = estado_act_btn_calidad;
}

/* 
 + BOTON OPERADOR 
 */
  if ( estado_act_btn_operador == value_btn_operador ) 
  {
    temporizador_operador = 0;
  }
  else 
  {
    if ( temporizador_operador == 0 ) {
      temporizador_operador = millis();
    }
    else 
    {
      if ( millis()-temporizador_operador > tiemporebote ) {
          estado_act_btn_operador = !estado_act_btn_operador;
        }
     }
  
  if ( estado_ant_btn_operador == HIGH && estado_act_btn_operador == LOW )
  {
    band_btn_operador = true;
    if(datos[3] == 1)
      {
        datos[3] = 0;
        digitalWrite(torreta_operador, LOW);
      }
      else{ 
        datos[3] = 1;
        digitalWrite(torreta_operador, HIGH);

        datos[1] = 0;
        datos[2] = 0;
        digitalWrite(torreta_calidad, LOW);
        digitalWrite(torreta_mantto, LOW);
      } 
  }
  estado_ant_btn_operador = estado_act_btn_operador;
}

  if (band_btn_mantto == true || band_btn_calidad == true || band_btn_operador == true)
  {
    while(ok == false)
    {
      datos[4] = 0;
      ok = radio.write(datos, sizeof(datos));
      if(ok == true)
      {
        Serial.println((String)datos[0] + " mantto: " + (String)datos[1] + " calidad: " + (String)datos[2] + " Operador: " + (String)datos[3] + " New: " + (String)datos[4]);
      }
      delay(10);
    }

    
    ok = false;
    band_btn_mantto = false;
    band_btn_calidad = false;
    band_btn_operador = false;
    value_btn_mantto = 0;
    value_btn_calidad = 0;
    value_btn_operador = 0;
  }
}
