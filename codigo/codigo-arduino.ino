
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h> 

#define pinLedVerde 2
#define pinLedRojo 3
#define pinBuzzer 4
#define pinRele 5
#define pinSCL 8             //PIN SCL DE TTP229
#define pinSDO 9             //PIN SDO DE TTP229
#define pinSDA 10            //PIN SDA DE RFID
#define pinRST 7             //PIN RST DE RFID




LiquidCrystal_I2C lcd(0x27, 16, 2);         //MAPEADO LCD
 
MFRC522 mfrc522(pinSDA, pinRST);            //MAPEADO RFID

byte Tecla;                                   //DECLARO VARIABLE CON NOMBRE TECLA DE TIPO BYTE PARA ALMACENAR VALORES DE 0 A 255 
char claveIngresada[5];                         //DECLARO VARIABLE CON UN ARRAY DE 5 DIGITOS MAXIMO
const char claveCorrecta[] = "1234";           //DECLARO CARACTER CONSTANTE DE LA CLAVE CORRECTA REPRESENTADO EN EL ARRAY POR 1234
bool AccesoRfid = false;                    //DECLARO VALOR BOOLEANO EN FALSE PARA CAMBIARLO DEPENDIENDO DE LA VERIFICACION 

void setup() {

  Serial.begin(9600);                      //HABILITA COMUNICACION SERIE DE 9600 BAUDIOS BITS POR SEGUNDO
  pinMode(pinSCL, OUTPUT);                 //DECLARO MODOS DE PINES
  pinMode(pinSDO, INPUT_PULLUP);
   pinMode(pinLedVerde, OUTPUT);
  pinMode(pinLedRojo, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinRele, OUTPUT);

  digitalWrite (pinRele, LOW);            //EN INICIO EL RELE ESTA EN BAJO PARA QUE LA CERRADURA PERMANEZCA CERRADA

  lcd.begin(16, 2);                        //INICIO DE DISPLAY LCD I2C
  lcd.backlight();
  lcd.setCursor(2, 0);
  lcd.print("BIENVENIDO");
  lcd.setCursor(1, 1);
  lcd.print("SI DESEA ENTRAR");
  delay(1500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("INGRESE LA CLAVE");

  SPI.begin();                             //INICIO COMUNICACION SPI
  mfrc522.PCD_Init();                      //INICIO RFID 
}





void loop() {



  Tecla= Lectura_TTP229();                              //TECLA ALMACENA LOS DATOS RECIBIDOS DE LA FUNCION LECTURA_TTP229

  if (Tecla)                                         //CONDICIONES DE TECLAS
  {  digitalWrite(pinBuzzer, HIGH);
     delay (50);
     digitalWrite (pinBuzzer, LOW);
    if (Tecla == 15)                                 // TECLA 15 PARA VERIFICAR CLAVE  
    {
      claveIngresada[4] = '\0';                                               //EN EL ARRAY DE CLAVE INGRESADA SE LE ASIGNA AL VALOR 4 VALOR NULO          
      if (AccesoRfid || (strcmp(claveIngresada, claveCorrecta) == 0))             //SI EL ACCESORFID ES VALIDO O LA COMPARACION DE CLAVE INGRESADA ES IGUAL A CLAVE CORRECTA IGUALAR EL VALOR NULO A 0  
      {
        lcd.clear();                                               //LIMPIA LA PANTALLA LCD
        lcd.print("ACCESO CONCEDIDO");                             //IMPRIME ACCESO CONCEDIDO EN CASO DE CLAVE CORRECTA 
        digitalWrite (pinLedVerde, HIGH);                          //ENCIENDE EL LED VERDE
        digitalWrite (pinRele, HIGH);                              //ABRE LA PUERTA 
        delay (2500);                                             //DURACION DE 2,5 SEG
        digitalWrite (pinLedVerde, LOW);                          //LUEGO DE 2,5 SEG APAGA LED VERDE
        digitalWrite (pinRele, LOW);                             //CIERRA LA PUERTA
                                                                    
        AccesoRfid = false;                                    // DESPUES DE CONCEDER ACCESO REESTABLECE ACCESO RFID
      }
      else                                                     //SI NO ES CORRECTO
      {
        lcd.clear();                                            //LIMPIAR PANTALLA DEL LCD
        lcd.print("ACCESO DENEGADO");                           //IMPRIMIR EN LA PANTALLA ACCESO DENEGADO
        digitalWrite (pinLedRojo, HIGH);                        //ENCIENDE EL LED ROJO
        tone(pinBuzzer, 1000);                                  //ENCIENDE LA ALARMA O BUZZER
        delay (2500);                                           //SE MANTIENE POR 2,5 SEG Y LUEGO....
        digitalWrite (pinLedRojo, LOW);                         //APAGA LED ROJO
        noTone(pinBuzzer);                                      //APAGA LA ALARMA O BUZZER
      }

      delay(2000);                                             //LUEGO DE CUALQUIER CASO ESPERAR 2 SEG Y....
      lcd.clear();                                             //LIMPIAR LA PANTALLA 
      lcd.print("INGRESE LA CLAVE");                           //IMPRIMIR DEVUELTA INGRESE LA CLAVE
      memset(claveIngresada, 0, sizeof(claveIngresada));       //REESTABLECE EL ARRAY CLAVE INGRESADA LLENANDOLO DE 0s BORRANDO TODA LA CLAVE Y PERMITIENDO LLENAR OTRA VEZ EL ARRAY CLAVE INGRESADA CON VALORES TIPO BYTE 
    }
    else if (Tecla == 12)                                      //OTRA CONDICION SI LA TECLA 12 ES PRESIONADA REALIZAR LO SIGUIENTE
    {
      lcd.setCursor(strlen(claveIngresada) - 1, 1);            //EN EL CURSOR DEL LCD PRIMERA FILA(ABAJO), RETROCEDE EN EL CURSOR EN UN DIGITO (-1)
      lcd.print(" ");                                          //IMPRIME UN VACIO COMO FORMA DE BORRAR EL ULTIMO DIGITO INGRESADO EN FORMA DE ASTERISCO
      claveIngresada[strlen(claveIngresada) - 1] = '\0';       //EN EL ARRAY DEL TTP229 TAMBIEN LE RESTA UN DIGITO HACIENDOLO NULO PARA VOLVER A UN DIGITO ANTERIOR
    } 
      else if (Tecla == 16) {
      lcd.clear();
      memset(claveIngresada, 0, sizeof(claveIngresada));
      lcd.print ("INGRESE LA CLAVE");
    } else                                                       //SI NO COINCIDE CON LA CONDICION ANTERIOR
      {
      if (strlen(claveIngresada) < 4)                          //SI LA LONGITUD DEL ARRAY EN CLAVE INGRESADA ES MENOR A 4
      {
        claveIngresada[strlen(claveIngresada)] = Tecla + '0';       //EN EL PUNTERO DE LA CLAVE INGRESADA PERMITE AGREGAR CARACTERES HASTA COMPLETAR EL ARRAY
        lcd.setCursor(strlen(claveIngresada) - 1, 1);               //EN EL CURSOR DEL LCD SE SIGUE AGREGANDO CARACTERES DESPUES DEL ULTIMO
        lcd.print('*');                                             //Y SE IMPRIMEN EN FORMA DE * 
      }
    }
  }





  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial())      //SI SE PRESENTA UNA NUEVA RFID SE LEE SU SERIALCARD(UID)
  {
    String uidString = "";                                                  //DECLARO VARIABLE TIPO STRING LLAMADA UIDSTRING Y QUE SU VALOR INICIAL SEA VACIO
    for (byte i = 0; i < mfrc522.uid.size; i++)                         //SE INCREMENTA EL VALOR  DE BYTE HASTA TENER EL UID SIZE(TAMAÑO)
    {
      uidString += String(mfrc522.uid.uidByte[i], HEX);                  //LA UIDSTRING SE VA RELLENANDO CON LA VARIABLE STRING DEPENDIENDO LOS BYTES OBTENIDOS DEL UID EN HEXADECIMAL
    }

    if (RfidAutorizado(uidString))                                 //SI SE CUMPLE LA FUNCION RFID AUTORIZADO
    {
       AccesoRfid = true;                                       // ESTABLECE EL ACCESORFID A TRUE SI ES VERDADERO
        lcd.clear();                                            //LIMPIA LA PANTALLA LCD 
        lcd.print("RFID CORRECTO");                             //IMPRIME EN LA PANTALLA RFID CORRECTO 
        digitalWrite (pinLedVerde, HIGH);                      //ENCIENDE LED VERDE
        digitalWrite (pinRele, HIGH);                           //ABRE LA PUERTA 
        delay (2500);                                          //ESPERA 2,5 SEG Y...
        digitalWrite (pinLedVerde, LOW);                       //APAGA LED VERDE
        digitalWrite (pinRele, LOW);                          //CIERRA LA PUERTA
        lcd.clear();                                           //LIMPIA LA PANTALLA      
        lcd.print("INGRESE LA CLAVE");                         //IMPRIME EN LA PANTALLA INGRESE LA CLAVE
    }
    else                                                       //SI NO SE CUMPLE LA ANTERIOR CONDICION
    {
     AccesoRfid = false;                                       //ESTABLECER EL ACCESORFID COMO FALSO PARA INCORRECTO
        lcd.clear();                                           //LIMPIA LA PANTALLA
        lcd.print("RFID INCORRECTO");                          //IMPRIME RFID INCORRECTO
        digitalWrite (pinLedRojo, HIGH);                       //ENCIENDE LED ROJO
        tone(pinBuzzer, 1000);                                 //ENCIENDE BUZZER TIPO ALARMA
        delay (2500);                                          //SE MANTIENE DURANTE 2,5 SEG Y...
        digitalWrite (pinLedRojo, LOW);                        //SE APAGA EL LED ROJO    
        noTone(pinBuzzer);                                     //SE APAGA EL BUZZER                                      
       lcd.clear();                                            //LIMPIA LA PANTALLA
       lcd.print("INGRESE LA CLAVE");                          //IMPRIME INGRESE LA CLAVE
    }

    mfrc522.PICC_HaltA();                                      //TERMINA DE RECIBIR INFORMACION DE LA TARJETA RFID
  }

  delay(200);                                                 //TIEMPO PARA VOLVER A SCANEAR DE 200 MILISEGUNDOS
}



byte Lectura_TTP229(void)                                     //DECLARA FUNCION TIPO BYTE PARA LECTURA DEL TECLADO TTP229
{
  byte Contador;                                            //VARIABLE TIPO BYTE LLAMADA CONTADOR PARA DECLARAR LOS PARAMETROS QUE TIENE EL TECLADO
  byte Estado_Tecla = 0;                                    //VARIABLE TIPO BYTE PARA DECLARAR EL ESTADO DE LA TECLA 

  for (Contador = 1; Contador <= 16; Contador++)            //MIENTRAS EL CONTADOR INCREMENTE HASTA 16 NUMEROS ENTEROS(MIENTRAS RECORRA LAS TECLAS DEL TTP229)
  {
    digitalWrite(pinSCL, LOW);                              //EL PIN SCL DEL TTP229 SE ESTABLECE EN BAJO PARA PREPARAR LA LECTURA DE LAS TECLAS PRESIONADAS
    delayMicroseconds(5);                                   //UN TIEMPO DE ESPERA PARA EVITAR CONFUNSIONES DE TECLAS PRESIONADAS
    if (!digitalRead(pinSDO))                               //CONDICION INVERTIDA DE LA LECTURA DIGITAL DEL PIN SDO DEL TECLADO TTP229(SI SE PRESIONA UNA ------->
                                                            // TECLA EL PIN SDO SE ESTABLECE EN BAJO Y EN EL PROGRAMA SE INVIERTE OSEA RECONOCE LA TECLA PRESIONADA)
      Estado_Tecla = Contador;                              //EL ESTADO DE LA TECLA VA A SER IGUAL A LA DEL CONTADOR HASTA LAS 16 TECLAS DEL TTP229(TECLA PRESIONADA) 
    digitalWrite(pinSCL, HIGH);                             //Y EL PIN SCL SE ESTABLECE EN ESTADO ALTO PARA COMPLETAR UN CICLO DE RELOJ EN EL TTP229
    delayMicroseconds(5);                                   //TIEMPO DE ESPERA PARA EVITAR CONFUSIONES DE TECLAS PRESIONADAS DURANTE 5 MICROSEGUNDOS
  }
  return Estado_Tecla;                                     //DEVUELVE ESTADO DE LA TECLA EN VALOR A LA TECLA PRESIONADA LUEGO DE LA CONDICION FOR SI NO SE HA PRESIONADO NINGUNA SE MANTIENE EN 0
}




bool RfidAutorizado(String uidString)                   //FUNCION TIPO BOOLEANO PARA RFID AUTORIZADOS, CON LOS PARAMETROS DE VARIABEL STRING LLAMADA UID STRING
{
  const String UIDsAutorizados[] = {                   //CONSTANTE DE TIPO STRING LLAMADA UIDsAUTORIZADOS CON UN ARRAY INDEFINIDO IGUAL A LOS SIGUIENTES USUARIOS REGISTRADOS O UIDs
      "e311a69a",                                     // UID DE USUARIO 1
      "731cae9a"                                      // UID DE USUARIO 2 .....
  };

  for (byte i = 0; i < sizeof(UIDsAutorizados) / sizeof(UIDsAutorizados[0]); i++)  //DECLARA VARIABLE TIPO BYTE INICIADA EN 0, MIENTRAS i SEA MENOR QUE EL NUMERO TOTAL DE ELEMENTOS EN EL ARREGLO, Y SE VA INCREMENTANDO
  {
    if (uidString.equals(UIDsAutorizados[i]))                   //SI EL STRING ES IGUAL A LOS UIDS AUTORIZADOS CON ARRAY TIPO BYTE i CUMPLE LA CONDICION Y REALIZA LO SIGUIENTE
    {
      return true;                                              //DEVUELVE VALOR TRUE 
    }
  }
  return false;                                               //SINO DEVUELVE VALOR FALSE
}
