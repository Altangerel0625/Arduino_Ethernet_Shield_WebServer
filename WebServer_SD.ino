#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

EthernetServer server(80);

File webPage;

void setup() {
Ethernet.begin(mac);
server.begin();
Serial.begin(9600);

Serial.println(Ethernet.localIP());
Serial.println("Initializing SD card...");
if (!SD.begin(4)) {
  Serial.println("Error - SD card initializing failed!");
  return;
  }
  Serial.println("SUCCESS -SD card initialized.");
  if (!SD.exists("index.html")) {
    Serial.println("ERROR - Can't find index.html file!");
    }
    Serial.println("SUCCESS - Found index.html file.");
}

void loop() {
  EthernetClient client = server.available();
  if(client) {
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();

          webPage = SD.open("index.html");
          if (webPage) {
             while(webPage.available()) {
              client.write(webPage.read());              
              }
              webPage.close();
              }
              break;
          }
          if (c == '\n'){
            currentLineIsBlank = true;
            }
            else if (c != '\r'){
            currentLineIsBlank = false;  
          }
        }
      }
      delay(1);
      client.stop();
    }
  
  }
