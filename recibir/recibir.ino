#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
 
//Declaremos los pines CE y el CSN
#define CE_PIN 9
#define CSN_PIN 10
 
//Variable con la dirección del canal que se va a leer
byte direccion[5] ={'c','a','n','a','l'}; 

//creamos el objeto radio (NRF24L01)
RF24 radio(CE_PIN, CSN_PIN);

//vector para los datos recibidos
int datos[5];

void setup()
{
 //inicializamos el NRF24L01 
  radio.begin();
  //inicializamos el puerto serie
  Serial.begin(9600); 
  //Abrimos el canal de Lectura
  radio.openReadingPipe(1, direccion);
  //empezamos a escuchar por el canal
  radio.startListening();
 
}
 
void loop() {
 if ( radio.available() )
 {    
     //Leemos los datos y los guardamos en la variable datos[]
     radio.read(datos,sizeof(datos));
     //reportamos por el puerto serial los datos recibidos
     String data = "";
     data = "ID:" + (String)datos[0] + 
     "-Mantto:" + (String)datos[1] + 
     "-Calidad:" + (String)datos[2] + 
     "-Operador:" + (String)datos[3] +
     "-New:" + (String)datos[4];
     Serial.println(data); 
 }
 delay(1000);
}
