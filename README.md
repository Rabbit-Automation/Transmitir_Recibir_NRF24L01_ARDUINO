## Transmisión y recepción de datos con antena NRF24L01 ARDUINO

Este proyecto esta construido en Arduino con antenas NRF24L01. Se hace uso de la libreria [NRF24L01](https://github.com/nRF24/RF24).

## Funcionamiento

### Transmisión de mensajes

Variable con la dirección del canal por donde se va a transmitir: 
``` c
byte direccion[5] = {"c","a","n","a","l"}:
```

Creamos el objeto radio **NRF24L01**:
``` c
RF24 radio(CE_PIN, CSN_PIN);
```

Definicion del paquete de datos:
``` c
int datos[5] = {ID_machine, 0, 0, 0, 0}; 
```

Inicializamos el NRF24L01 y abrimos el canal de escritura/comunicación:
``` c
radio.begin();
radio.openWritingPipe(direccion);
```

Enviamos el mensaje y esperamos la respuesta del receptor:
``` c
ok = radio.write(datos, sizeof(datos)); 
```

### Recepción de mensajes

Variable con la dirección del canal por donde se va a recibir el mensaje: 
``` c
byte direccion[5] = {'c','a','n','a','l'};
```

Leemos los datos y los guardamos en la variable:
``` c
int datos[5];
radio.read(datos,sizeof(datos));
```

# Arquitectura

``` mermaid
flowchart LR
AR((Receptor))
AT1((Transmisor 1))
AT2((Transmisor 2))
AT3((Transmisor 3))

AT1 ---> AR 
AT2 ---> AR
AT3 ---> AR
