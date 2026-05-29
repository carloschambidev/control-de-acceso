#include <SoftwareSerial.h>

SoftwareSerial espSerial(0, 1); // RX, TX (cambia los pines según tu configuración)
const char* ssid = "nombre_de_tu_wifi";
const char* password = "contraseña_de_tu_wifi";
const char* serverIP = "dirección_ip_de_tu_servidor"; // Cambia esto por la IP de tu servidor
const int serverPort = 8080; // Cambia esto por el puerto en el que está corriendo tu servidor

void setup() {
  Serial.begin(115200);
  espSerial.begin(115200);
  
  delay(1000);
  
  sendCommand("AT", 1000);
  sendCommand("AT+CWMODE=1", 1000); // Modo cliente WiFi
  sendCommand("AT+CWJAP=\"" + String(ssid) + "\",\"" + String(password) + "\"", 10000); // Conexión a WiFi
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    String data = "Hola desde Arduino!";
    String url = "http://" + String(serverIP) + ":" + String(serverPort) + "/data";
    
    sendCommand("AT+CIPSTART=\"TCP\",\"" + String(serverIP) + "\"," + String(serverPort), 5000); // Establecer conexión TCP
    sendCommand("AT+CIPSEND=" + String(data.length() + 2), 1000); // Enviar tamaño de datos
    espSerial.println(data);
    delay(1000);
    sendCommand("AT+CIPCLOSE", 1000); // Cerrar conexión
    
    delay(5000); // Espera 5 segundos antes de enviar más datos
  }
}

void sendCommand(String command, int timeout) {
  espSerial.println(command);
  delay(timeout);
  while (espSerial.available()) {
    Serial.write(espSerial.read());
  }
}
