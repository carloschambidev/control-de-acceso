# Control de Acceso con Arduino UNO

Proyecto electrónico de control de acceso basado en Arduino UNO. El sistema permite autorizar entradas de forma segura usando dos métodos: RFID y clave numérica mediante teclado táctil. El estado se muestra en un display LCD con módulo I2C.

## Descripción del proyecto

Este proyecto consiste en un control de acceso electrónico diseñado para una puerta o dispositivo de seguridad. Combina:

- Arduino UNO R3
- Lector RFID MFRC522
- Teclado táctil TTP229
- Display LCD I2C de 12 segmentos (mostrar mensajes y el ingreso de clave)
- Módulo relé para activar la cerradura magnética
- LED rojo y verde para indicar estado
- Buzzer para señales sonoras

El objetivo es validar usuarios de forma rápida y confiable, ofreciendo dos formas de autenticación:

1. Acceso por tarjeta RFID
2. Acceso por clave numérica con teclado táctil

## Componentes principales

- Arduino UNO R3
- Módulo lector RFID MFRC522
- Teclado táctil TTP229
- Display LCD con módulo I2C
- Módulo relé
- Cerradura magnética
- LED rojo y LED verde con resistencias de 220 Ω
- Buzzer pasivo

## Funcionamiento

### Acceso por RFID

El lector RFID escanea la tarjeta al acercarla. Cada tarjeta contiene un identificador único (UID). El código en Arduino compara el UID con la lista de autorizados y:

- Si es válido, activa el relé y permite el acceso.
- Si es inválido, muestra un mensaje de rechazo y no activa la cerradura.

### Acceso por teclado táctil

El teclado táctil TTP229 permite ingresar una clave numérica. Mientras se escribe, el display muestra asteriscos para proteger la contraseña. El sistema verifica la clave:

- Si la clave es correcta, se activa el relé y se concede el acceso.
- Si la clave es incorrecta, se muestra un mensaje de error.

## Conexiones principales

Conexiones realizadas de la siguiente forma:

- Arduino UNO GND → relé, I2C, RFID, teclado TTP229, buzzer
- Arduino UNO 5 V → relé VCC, I2C VCC
- Arduino UNO 3.3 V → RFID VCC, teclado TTP229 VCC
- Arduino UNO A4 (SDA) → display I2C SDA
- Arduino UNO A5 (SCL) → display I2C SCL
- Arduino UNO 2 → LED verde
- Arduino UNO 3 → LED rojo
- Arduino UNO 4 → buzzer
- Arduino UNO 5 → relé IN
- Arduino UNO 8 → teclado TTP229 SCL
- Arduino UNO 9 → teclado TTP229 SDO
- Arduino UNO 10 → RFID SDA
- Arduino UNO 11 → RFID MOSI
- Arduino UNO 12 → RFID MISO
- Arduino UNO 13 → RFID SCK

También se utilizan resistencias de 220 Ω para los LEDs y la protección de los circuitos.

## Estructura del repositorio

- `Readme.md`: documentación del proyecto
- `codigo/`: código fuente y recursos de Arduino
- `Informe-del-TP/`: informe técnico del proyecto en PDF
- `Licencia/`: licencia del proyecto

## Ventajas del diseño

- Autenticación dual: RFID y clave numérica
- Indicadores visuales y sonoros para el usuario
- Display LCD I2C con mensajes de estado
- Control de cerradura mediante relé

## Notas adicionales

- El display LCD ofrece una interfaz clara para el usuario: ingreso de clave, validación exitosa o denegada.
- El sistema es escalable: se puede ampliar añadiendo más tarjetas RFID autorizadas o cambiando la clave desde el código.
- El informe técnico también incluye un cálculo de costos y un diseño de gabinete estimado.

## Cómo usar

1. Conectar todos los módulos según el esquema de conexiones.
2. Cargar el código en `codigo/codigo-arduino.ino` al Arduino UNO.
3. Alimentar el sistema.
4. Probar el acceso con una tarjeta RFID autorizada o ingresando la clave correcta en el teclado táctil.

## Autores

- Aquino, Thomas 
- Borcard, Jonathan 
- Chambi, Carlos 
- Morales, Leonel 

## Docente

- Linares Diego

