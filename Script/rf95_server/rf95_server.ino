#include <RH_RF95.h>
#include <SoftwareSerial.h>
    
SoftwareSerial SSerial(10, 11); // RX, TX

#define COMSerial SSerial
#define ShowSerial Serial

RH_RF95<SoftwareSerial> rf95(COMSerial);

int led = 13;

///////////////////////
#define packTimeout 5
uint8_t buf2[512];
uint16_t i2 = 0;
//////////////////////

void setup() {
    ShowSerial.begin(19200, SERIAL_8N1);
    while(!Serial) { }
    ShowSerial.println("RF95 - Server test.");

    pinMode(led, OUTPUT);

    if (!rf95.init()) {
        ShowSerial.println("RF95 - init failed");
        while (1);
    }

    rf95.setFrequency(868.0);
}

void loop() {
   if (rf95.available()) {
    while(rf95.available()) {
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);
      if (rf95.recv(buf, &len)) 
      {
        ShowSerial.write(buf, len);
        len = 0;
      }
    }
  }
  
  if (ShowSerial.available()) {
    while(1) {
      if(ShowSerial.available()) {
        buf2[i2] = (char)ShowSerial.read();
        if(i2<512-1) i2++;
      } else {
        delay(packTimeout);
        if(!ShowSerial.available()) {
          break;
        }
      }
    }
    rf95.send(buf2, i2);
    i2 = 0;
    
    rf95.waitPacketSent();
  }
}
